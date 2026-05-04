#!/usr/bin/env bash
# Compare per-TU __TEXT,__text section bytes between two ssb64_game builds.
#
# Mismatches indicate semantic drift introduced by a refactor / reconcile /
# submodule move. The .text section contains compiled instruction bytes only;
# debug info and rodata strings (e.g. __FILE__) live elsewhere and don't
# affect this comparison, so paths and line numbers in debug output don't
# show up as false-positive diffs here. Note that __LINE__ baked into
# instructions (e.g. via PORT_RESOLVE → portRelocResolvePointerDebug) DOES
# show up — see tools/audit_text_diffs.sh to classify those out.
#
# Usage:
#     tools/compare_obj_text.sh <baseline-build-dir> <refactor-build-dir>
#
#     baseline-build-dir : path to <build>/CMakeFiles/ssb64_game.dir of the
#                          known-good build (e.g. main branch)
#     refactor-build-dir : path to <build>/CMakeFiles/ssb64_game.dir of the
#                          build under audit
#
# Both args may use either path style (with or without /decomp/ prefix in
# .o paths); the script normalises by stripping a leading decomp/ when
# matching refactor TUs to baseline TUs.
#
# macOS only — uses otool. For Linux, swap otool -t for objdump --section=.text -d.
set -uo pipefail

if [[ $# -ne 2 ]]; then
    echo "usage: $0 <baseline-build-dir> <refactor-build-dir>" >&2
    echo "  e.g. $0 /tmp/main-build/CMakeFiles/ssb64_game.dir build/CMakeFiles/ssb64_game.dir" >&2
    exit 2
fi

BASELINE="$1"
REFACTOR="$2"

[[ -d "$BASELINE" ]] || { echo "baseline dir not found: $BASELINE" >&2; exit 2; }
[[ -d "$REFACTOR" ]] || { echo "refactor dir not found: $REFACTOR" >&2; exit 2; }

# otool -t prints the file path on the first line, which embeds the build
# path and would be a false-positive diff. Strip it. Then drop the address
# column from each subsequent line (`<addr>  <hex words>`) so the only
# input to the hash is the actual byte stream.
text_hash() {
    otool -t "$1" 2>/dev/null | tail -n +2 | awk '{$1=""; print}' | shasum -a 256 | awk '{print $1}'
}

count_total=0
count_match=0
count_differ=0
count_missing_baseline=0
differ_files=()

while IFS= read -r ref_o; do
    count_total=$((count_total + 1))
    rel="${ref_o#${REFACTOR}/}"
    # Map decomp/src/foo.c.o → src/foo.c.o for baseline lookup if the
    # refactor moved sources into a decomp/ submodule.
    rel_baseline="${rel#decomp/}"
    base_o="${BASELINE}/${rel_baseline}"

    if [[ ! -f "$base_o" ]]; then
        count_missing_baseline=$((count_missing_baseline + 1))
        continue
    fi

    h_ref=$(text_hash "$ref_o")
    h_base=$(text_hash "$base_o")

    if [[ "$h_ref" == "$h_base" ]]; then
        count_match=$((count_match + 1))
    else
        count_differ=$((count_differ + 1))
        differ_files+=("$rel_baseline")
    fi
done < <(find "$REFACTOR" -name '*.o' | sort)

echo "=== Comparison summary ==="
printf '  total .o files in refactor build : %d\n' "$count_total"
printf '  identical __text section         : %d\n' "$count_match"
printf '  differing __text section         : %d\n' "$count_differ"
printf '  missing in baseline              : %d\n' "$count_missing_baseline"

if (( count_differ > 0 )); then
    echo
    echo "=== Files with differing __text (showing up to 30): ==="
    for f in "${differ_files[@]:0:30}"; do
        echo "  $f"
    done
    echo
    echo "Run tools/audit_text_diffs.sh with the same args to classify these"
    echo "(LINE_ONLY drift via __LINE__ vs STRUCTURAL semantic divergence)."
fi
