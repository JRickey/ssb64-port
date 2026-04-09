#!/usr/bin/env python3
"""
ub_scanner.py — Scan SSB64 decomp C code for IDO→MSVC undefined behavior.

Detects patterns that compile on IDO 7.1 but are UB or produce wrong results
on MSVC / modern C compilers. Each checker emits severity + explanation.

Usage:
    python tools/ub_scanner.py [--dir src/] [--exclude ovl8,db,libultra]
    python tools/ub_scanner.py --file src/lb/lbcommon.c
    python tools/ub_scanner.py --check signed-shift   # run one checker only
"""

import argparse
import os
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


@dataclass
class Finding:
    file: str
    line: int
    severity: str        # CRITICAL, HIGH, MEDIUM, LOW
    category: str        # short tag
    message: str         # what's wrong
    code: str            # the offending line
    suggestion: str = "" # how to fix


# ---------------------------------------------------------------------------
# Checkers
# ---------------------------------------------------------------------------

def check_signed_left_shift(filepath: str, lines: list[str]) -> list[Finding]:
    """Left-shifting signed values is UB in C99/C11 when result overflows
    or the value is negative. IDO treats it as a logical shift; MSVC may
    optimize assuming no UB."""
    findings = []
    # Pattern: (expr << N) where expr could be signed s32/int/s16/long
    # The COMBINE_FRACTIONAL macro does ((a) << 16) — dangerous with signed a.
    # FTOFIX32 returns (long)(...) which is signed.
    shift_re = re.compile(
        r'\b(e[12]|val|value|temp|result)\s*<<\s*(\d+)'
        r'|'
        r'\(\s*(e[12]|val)\s*<<\s*16\s*\)'
    )
    combine_frac_re = re.compile(r'COMBINE_FRACTIONAL')
    for i, line in enumerate(lines):
        stripped = line.lstrip()
        if stripped.startswith('//') or stripped.startswith('/*'):
            continue
        m = shift_re.search(line)
        if m:
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="signed-shift",
                message="Left-shift of potentially signed value (from FTOFIX32 or s32). "
                        "UB if value is negative.",
                code=line.rstrip(),
                suggestion="Cast to unsigned before shifting: ((u32)e1 << 16)"
            ))
        if combine_frac_re.search(line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="signed-shift",
                message="COMBINE_FRACTIONAL macro left-shifts arg by 16. "
                        "UB if first arg is signed and negative.",
                code=line.rstrip(),
                suggestion="Ensure first arg is cast to u32, or redefine macro "
                           "with (u32) cast."
            ))
    return findings


def check_type_pun_deref(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect *(Type*)&var patterns — strict aliasing violation.
    MSVC doesn't enforce strict aliasing by default, but it's still UB
    and can break under /O2 or with LTO."""
    findings = []
    pun_re = re.compile(
        r'\*\s*\(\s*'
        r'(u32|s32|f32|u16|s16|u8|s8|int|float|unsigned\s+int|long)'
        r'\s*\*\s*\)\s*[&(]'
    )
    # Also catch struct pun: *(StructName*)&var
    struct_pun_re = re.compile(
        r'\*\s*\(\s*([A-Z][A-Za-z0-9_]+)\s*\*\s*\)\s*&'
    )
    for i, line in enumerate(lines):
        stripped = line.lstrip()
        if stripped.startswith('//'):
            continue
        m = pun_re.search(line)
        if m:
            findings.append(Finding(
                file=filepath, line=i+1, severity="MEDIUM",
                category="strict-alias",
                message=f"Type-pun dereference *(({m.group(1)}*)&...). "
                        f"Strict aliasing violation.",
                code=line.rstrip(),
                suggestion="Use memcpy() or a union for type punning."
            ))
        m2 = struct_pun_re.search(line)
        if m2 and m2.group(1) not in ('PORT', 'void'):
            # Filter out common false positives
            if 'PORT_RESOLVE' not in line:
                findings.append(Finding(
                    file=filepath, line=i+1, severity="MEDIUM",
                    category="strict-alias",
                    message=f"Struct type-pun dereference *({m2.group(1)}*)&var. "
                            f"Reads memory as incompatible struct type.",
                    code=line.rstrip(),
                    suggestion="Use memcpy() or restructure to avoid aliasing."
                ))
    return findings


def check_ternary_side_effect(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect var = (cond) ? val : var++ — reads and modifies var
    without a sequence point. Clear UB."""
    findings = []
    # Pattern: varname++ or varname-- in ternary where same var is on LHS
    ternary_re = re.compile(
        r'(\w+)\s*=\s*.*\?\s*.*:\s*\1\s*(\+\+|--)'
        r'|'
        r'(\w+)\s*=\s*.*\?\s*\3\s*(\+\+|--).*:'
    )
    for i, line in enumerate(lines):
        m = ternary_re.search(line)
        if m:
            var = m.group(1) or m.group(3)
            findings.append(Finding(
                file=filepath, line=i+1, severity="CRITICAL",
                category="sequence-point",
                message=f"Variable '{var}' is read and modified in ternary "
                        f"expression without intervening sequence point. "
                        f"Undefined behavior.",
                code=line.rstrip(),
                suggestion=f"Split into: {var}++; if ({var} >= MAX) {var} = 0;"
            ))
    return findings


def check_union_type_punning(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect union { int i; float f; } patterns where .i is written
    and .f is read (or vice versa). UB in C99, implementation-defined
    in C11, but MSVC handles it. Still worth flagging."""
    findings = []
    # Look for union field writes followed by different field reads
    # This is hard to do with regex alone; focus on the known pattern:
    # data.i = (s32) param; ... = data.f;
    union_write_re = re.compile(r'(\w+)\.(i|integer|word|u)\s*=')
    for i, line in enumerate(lines):
        m = union_write_re.search(line)
        if m:
            var = m.group(1)
            written_field = m.group(2)
            # Look ahead for read of different field
            for j in range(i+1, min(i+5, len(lines))):
                if re.search(rf'{var}\.(f|float|flt)\b', lines[j]):
                    findings.append(Finding(
                        file=filepath, line=i+1, severity="MEDIUM",
                        category="union-pun",
                        message=f"Union '{var}' written as .{written_field}, "
                                f"read as float member. Type punning through "
                                f"union is implementation-defined.",
                        code=line.rstrip(),
                        suggestion="Use memcpy() for portable type punning."
                    ))
                    break
    return findings


def check_null_ptr_arithmetic(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect pointer arithmetic on N64 physical addresses (0x80xxxxxx)
    that would be invalid on PC."""
    findings = []
    phys_addr_re = re.compile(r'0x80[0-9a-fA-F]{5,6}')
    fb_write_re = re.compile(r'\*\s*fb\w*\+\+\s*=')
    uintptr_cmp_re = re.compile(r'\(uintptr_t\)\s*\w+\s*[<>]=?\s*0x80')
    for i, line in enumerate(lines):
        if uintptr_cmp_re.search(line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="phys-addr",
                message="Comparing pointer cast to N64 physical address "
                        "(0x80xxxxxx). Invalid on PC.",
                code=line.rstrip(),
                suggestion="Guard with #ifdef PORT or replace with "
                           "PC-compatible framebuffer access."
            ))
        elif fb_write_re.search(line) and phys_addr_re.search(line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="phys-addr",
                message="Direct framebuffer write to N64 physical address.",
                code=line.rstrip(),
                suggestion="Guard with #ifdef PORT."
            ))
    return findings


def check_macro_side_effects(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect calls to multi-eval macros (ABS, SQUARE, CUBE, BIQUAD)
    with side-effecting arguments (++, --, function calls)."""
    findings = []
    # These macros evaluate their arg 2-4 times
    multi_eval = re.compile(
        r'\b(ABS|ABSF|SQUARE|CUBE|BIQUAD|DISTANCE|TAKE_MAX|TAKE_MIN)\s*\('
    )
    for i, line in enumerate(lines):
        m = multi_eval.search(line)
        if m:
            macro = m.group(1)
            # Extract the argument (rough: from open paren to matching close)
            start = m.end()
            depth = 1
            j = start
            while j < len(line) and depth > 0:
                if line[j] == '(':
                    depth += 1
                elif line[j] == ')':
                    depth -= 1
                j += 1
            arg = line[start:j-1] if j <= len(line) else ""
            # Check for side effects in arg
            if re.search(r'\+\+|--|[a-zA-Z_]\w*\s*\(', arg):
                findings.append(Finding(
                    file=filepath, line=i+1, severity="HIGH",
                    category="macro-side-effect",
                    message=f"Macro {macro}() evaluates its argument multiple "
                            f"times. Argument '{arg.strip()}' has side effects.",
                    code=line.rstrip(),
                    suggestion=f"Assign argument to a temp variable before "
                               f"passing to {macro}()."
                ))
    return findings


def check_ftofix32_long(filepath: str, lines: list[str]) -> list[Finding]:
    """FTOFIX32 is defined as (long)(...). On LLP64 (Windows), long is
    32-bit; on LP64 (Linux/macOS), long is 64-bit. When stored in int
    or used in 32-bit bitwise ops, truncation may differ."""
    findings = []
    ftofix_re = re.compile(r'FTOFIX32\s*\(')
    for i, line in enumerate(lines):
        if ftofix_re.search(line):
            # Check if result is stored in int/s32/u32 (32-bit)
            if re.search(r'\b(int|s32|u32)\b.*=.*FTOFIX32', line):
                pass  # assignment is fine on Windows (long=32bit)
            # Flag if used directly in shift
            if re.search(r'FTOFIX32\s*\([^)]*\)\s*<<', line):
                findings.append(Finding(
                    file=filepath, line=i+1, severity="HIGH",
                    category="ftofix32",
                    message="FTOFIX32() result (signed long) shifted directly. "
                            "Signed left-shift is UB if negative. Also, "
                            "'long' width differs between platforms.",
                    code=line.rstrip(),
                    suggestion="Store in u32 before shifting."
                ))
    return findings


def check_attribute_undef(filepath: str, lines: list[str]) -> list[Finding]:
    """The macros.h file does #define __attribute__(x) which disables
    all GCC/Clang attributes globally. This can break alignas, packed,
    visibility, etc. in port code."""
    findings = []
    for i, line in enumerate(lines):
        if re.match(r'\s*#\s*define\s+__attribute__\s*\(\s*x\s*\)', line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="CRITICAL",
                category="attribute-undef",
                message="__attribute__ is globally undefined. This breaks "
                        "alignment, packing, and visibility attributes on "
                        "GCC/Clang. MSVC uses __declspec instead.",
                code=line.rstrip(),
                suggestion="Guard with #ifdef __sgi or move to an "
                           "IDO-specific header."
            ))
    return findings


def check_uninitialized_switch(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect variables declared before switch/if that are only assigned
    in some branches but used after. Simplified heuristic."""
    findings = []
    # Look for: type var; ... switch ... case: var = ...; break; ... use var
    # This is too complex for regex; just flag known dangerous patterns
    # where a pointer or index is declared without init before a switch
    decl_no_init = re.compile(
        r'^\s+(s32|u32|s16|u16|f32|void\s*\*|[A-Z]\w+\s*\*)\s+(\w+)\s*;'
    )
    for i, line in enumerate(lines):
        m = decl_no_init.search(line)
        if m:
            type_name = m.group(1).strip()
            var_name = m.group(2)
            # Only flag pointers and indices — highest risk
            if '*' in type_name or var_name in ('line_id', 'coll_type',
                                                  'index', 'idx', 'id'):
                # Look ahead for switch within 10 lines
                for j in range(i+1, min(i+10, len(lines))):
                    if re.search(r'\bswitch\b', lines[j]):
                        findings.append(Finding(
                            file=filepath, line=i+1, severity="MEDIUM",
                            category="uninit-var",
                            message=f"'{type_name} {var_name}' declared "
                                    f"without init before switch statement. "
                                    f"May be uninitialized on some paths. "
                                    f"MSVC debug fills with 0xCC.",
                            code=line.rstrip(),
                            suggestion=f"Initialize: {type_name} {var_name} = "
                                       f"{'NULL' if '*' in type_name else '0'};"
                        ))
                        break
    return findings


def check_s32_min_constants(filepath: str, lines: list[str]) -> list[Finding]:
    """S32_MIN is defined as 0x80000000 (unsigned in C). When compared
    against signed values or negated, behavior differs from INT_MIN."""
    findings = []
    for i, line in enumerate(lines):
        # -S32_MIN or -S8_MIN is UB (negation of most-negative)
        if re.search(r'-\s*S(8|16|32)_MIN\b', line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="int-negate-min",
                message="Negating S*_MIN (most-negative value) is signed "
                        "integer overflow — undefined behavior.",
                code=line.rstrip(),
                suggestion="Use unsigned arithmetic or special-case MIN."
            ))
        # S32_MIN is 0x80000000 which is unsigned int, not int
        # Comparing s32 < S32_MIN always false (promoted to unsigned)
        if re.search(r'\b\w+\s*[<>=!]+\s*S32_MIN\b', line):
            if not re.search(r'#define', line):
                findings.append(Finding(
                    file=filepath, line=i+1, severity="LOW",
                    category="min-unsigned",
                    message="S32_MIN (0x80000000) is unsigned int in C. "
                            "Comparison with signed value uses unsigned "
                            "semantics.",
                    code=line.rstrip(),
                    suggestion="Use ((s32)0x80000000) or INT32_MIN."
                ))
    return findings


def check_gc_bitfield_signed(filepath: str, lines: list[str]) -> list[Finding]:
    """GC_BITFIELD(n) does (1 << n). If n >= 31, this overflows signed
    int. Should be (1U << n) or (1UL << n)."""
    findings = []
    for i, line in enumerate(lines):
        if re.search(r'GC_BITFIELD\s*\(\s*3[12]\s*\)', line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="bitfield-overflow",
                message="GC_BITFIELD(31+) left-shifts signed 1 into or "
                        "past the sign bit. UB in C.",
                code=line.rstrip(),
                suggestion="Change macro to (1U << (n))."
            ))
    return findings


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------

ALL_CHECKERS = [
    ("signed-shift",      check_signed_left_shift),
    ("strict-alias",      check_type_pun_deref),
    ("sequence-point",    check_ternary_side_effect),
    ("union-pun",         check_union_type_punning),
    ("phys-addr",         check_null_ptr_arithmetic),
    ("macro-side-effect", check_macro_side_effects),
    ("ftofix32",          check_ftofix32_long),
    ("attribute-undef",   check_attribute_undef),
    ("uninit-var",        check_uninitialized_switch),
    ("min-constants",     check_s32_min_constants),
    ("bitfield-overflow", check_gc_bitfield_signed),
]


def scan_file(filepath: str, checkers: list) -> list[Finding]:
    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            lines = f.readlines()
    except (OSError, IOError):
        return []

    findings = []
    for _name, checker in checkers:
        findings.extend(checker(filepath, lines))
    return findings


def main():
    parser = argparse.ArgumentParser(
        description="Scan SSB64 decomp code for IDO→MSVC undefined behavior"
    )
    parser.add_argument('--dir', default='src/',
                        help="Directory to scan (default: src/)")
    parser.add_argument('--file', default=None,
                        help="Scan a single file")
    parser.add_argument('--exclude', default='ovl8,db,libultra',
                        help="Comma-separated subdirs to exclude")
    parser.add_argument('--include-all', action='store_true',
                        help="Don't exclude any directories")
    parser.add_argument('--check', default=None,
                        help="Run only this checker")
    parser.add_argument('--severity', default=None,
                        help="Only show this severity or above "
                             "(CRITICAL,HIGH,MEDIUM,LOW)")
    parser.add_argument('--summary', action='store_true',
                        help="Only show category counts")
    args = parser.parse_args()

    severity_order = {"CRITICAL": 0, "HIGH": 1, "MEDIUM": 2, "LOW": 3}
    min_severity = severity_order.get(args.severity, 3) if args.severity else 3

    # Select checkers
    if args.check:
        checkers = [(n, c) for n, c in ALL_CHECKERS if n == args.check]
        if not checkers:
            print(f"Unknown checker: {args.check}")
            print(f"Available: {', '.join(n for n, _ in ALL_CHECKERS)}")
            sys.exit(1)
    else:
        checkers = ALL_CHECKERS

    # Collect files
    if args.file:
        files = [args.file]
    else:
        exclude = set() if args.include_all else set(args.exclude.split(','))
        files = []
        for root, dirs, filenames in os.walk(args.dir):
            # Filter excluded subdirs
            rel = os.path.relpath(root, args.dir)
            parts = Path(rel).parts
            if parts and parts[0] in exclude:
                continue
            for fn in filenames:
                if fn.endswith(('.c', '.h')):
                    files.append(os.path.join(root, fn))

    # Also scan include/ for header issues
    if not args.file and os.path.isdir('include'):
        for root, dirs, filenames in os.walk('include'):
            for fn in filenames:
                if fn.endswith('.h'):
                    files.append(os.path.join(root, fn))

    # Scan
    all_findings: list[Finding] = []
    for f in sorted(files):
        all_findings.extend(scan_file(f, checkers))

    # Filter by severity
    all_findings = [f for f in all_findings
                    if severity_order.get(f.severity, 3) <= min_severity]

    # Sort: CRITICAL first, then HIGH, etc.
    all_findings.sort(key=lambda f: (severity_order.get(f.severity, 3),
                                      f.category, f.file, f.line))

    if args.summary:
        from collections import Counter
        by_cat = Counter()
        by_sev = Counter()
        for f in all_findings:
            by_cat[f.category] += 1
            by_sev[f.severity] += 1
        print(f"\n{'='*60}")
        print(f"UB Scanner Summary — {len(all_findings)} findings "
              f"in {len(files)} files")
        print(f"{'='*60}")
        print(f"\nBy severity:")
        for sev in ["CRITICAL", "HIGH", "MEDIUM", "LOW"]:
            if by_sev[sev]:
                print(f"  {sev:10s}: {by_sev[sev]}")
        print(f"\nBy category:")
        for cat, count in by_cat.most_common():
            print(f"  {cat:25s}: {count}")
        return

    # Full output
    print(f"\n{'='*70}")
    print(f"UB Scanner — {len(all_findings)} findings in {len(files)} files")
    print(f"{'='*70}")

    current_cat = None
    for f in all_findings:
        if f.category != current_cat:
            current_cat = f.category
            print(f"\n--- [{f.category}] ---\n")
        sev_marker = {"CRITICAL": "!!!", "HIGH": "!!", "MEDIUM": "!",
                      "LOW": "."}
        print(f"  {sev_marker.get(f.severity, '?')} [{f.severity}] "
              f"{f.file}:{f.line}")
        print(f"      {f.message}")
        print(f"      > {f.code.strip()}")
        if f.suggestion:
            print(f"      FIX: {f.suggestion}")
        print()

    # Summary footer
    from collections import Counter
    by_sev = Counter(f.severity for f in all_findings)
    print(f"{'='*70}")
    print(f"Totals: ", end="")
    print(" | ".join(f"{s}: {by_sev[s]}" for s in
                     ["CRITICAL", "HIGH", "MEDIUM", "LOW"] if by_sev[s]))
    print(f"{'='*70}")


if __name__ == '__main__':
    main()
