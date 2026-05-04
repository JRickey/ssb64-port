#!/usr/bin/env bash
# For each TU whose .text differs between two ssb64_game builds, classify
# the disassembly diff:
#
# - LINE_ONLY  : every changed line is `mov w2, #<imm>` — debug line-number
#                drift via __LINE__ baked into PORT_RESOLVE call sites
#                (which expand to portRelocResolvePointerDebug(token, FILE, LINE)).
#                These are semantics-preserving and expected after any change
#                that shifts line numbers in a TU that calls PORT_RESOLVE.
#
# - STRUCTURAL : at least one diff is something other than a w2 immediate move.
#                Indicates a real instruction-level change — a different
#                function body, register allocation shift, missed PORT guard
#                that changed compiled output, etc. Investigate each one.
#
# Run this AFTER tools/compare_obj_text.sh to deep-dive whatever it flagged
# as differing. Together they're how we proved Milestone 1 (decomp submodule
# migration) was provably semantics-preserving:
#   446/475 TUs byte-identical __text, 29 LINE_ONLY, 0 STRUCTURAL.
#
# Usage:
#     tools/audit_text_diffs.sh <baseline-build-dir> <refactor-build-dir>
#
# macOS / AArch64 only — the LINE_ONLY pattern is `mov w2, #<imm>` which
# is AArch64-specific. On x86 the equivalent would be `mov esi, <imm>` or
# `mov edx, <imm>` depending on calling convention; adjust the regex below
# if porting.
set -uo pipefail

if [[ $# -ne 2 ]]; then
    echo "usage: $0 <baseline-build-dir> <refactor-build-dir>" >&2
    exit 2
fi

BASELINE="$1"
REFACTOR="$2"

[[ -d "$BASELINE" ]] || { echo "baseline dir not found: $BASELINE" >&2; exit 2; }
[[ -d "$REFACTOR" ]] || { echo "refactor dir not found: $REFACTOR" >&2; exit 2; }

# Re-derive the divergent file list so this script is self-contained.
divergent=()
while IFS= read -r ref_o; do
    rel="${ref_o#${REFACTOR}/}"
    rel_baseline="${rel#decomp/}"
    base_o="${BASELINE}/${rel_baseline}"
    [[ -f "$base_o" ]] || continue
    h_ref=$(otool -t "$ref_o" | tail -n +2 | awk '{$1=""; print}' | shasum -a 256 | awk '{print $1}')
    h_base=$(otool -t "$base_o" | tail -n +2 | awk '{$1=""; print}' | shasum -a 256 | awk '{print $1}')
    [[ "$h_ref" != "$h_base" ]] && divergent+=("$rel_baseline")
done < <(find "$REFACTOR" -name '*.o' | sort)

line_only=0
structural=0
structural_files=()

for rel in "${divergent[@]}"; do
    ref_o="${REFACTOR}/decomp/${rel}"
    [[ -f "$ref_o" ]] || ref_o="${REFACTOR}/${rel}"
    base_o="${BASELINE}/${rel}"

    diff_out=$(diff \
        <(otool -tV "$base_o" 2>/dev/null | tail -n +2) \
        <(otool -tV "$ref_o" 2>/dev/null | tail -n +2) || true)

    # Lines from `diff` that represent actual differences start with < or >.
    # Filter to those, then drop any that match the AArch64 line-number
    # pattern (`mov w2, #0xNN` — w2 is the third arg register, where
    # __LINE__ lands in portRelocResolvePointerDebug). Anything left is
    # structural divergence.
    bad=$(printf '%s\n' "$diff_out" \
        | grep -E '^[<>]' \
        | grep -vE '^[<>]\s*$' \
        | grep -vE '^[<>]\s+[0-9a-f]+\s+mov\s+w2, #0x[0-9a-f]+$' \
        | wc -l \
        | tr -d ' ')

    if [[ "$bad" == "0" ]]; then
        line_only=$((line_only + 1))
    else
        structural=$((structural + 1))
        structural_files+=("$rel ($bad non-LINE diff lines)")
    fi
done

echo "=== Audit summary ==="
printf '  divergent TUs                 : %d\n' "${#divergent[@]}"
printf '  LINE_ONLY (mov w2, #imm)      : %d\n' "$line_only"
printf '  STRUCTURAL (real semantic)    : %d\n' "$structural"

if (( structural > 0 )); then
    echo
    echo "=== Files with structural divergence: ==="
    for f in "${structural_files[@]}"; do echo "  $f"; done
    exit 1
fi
