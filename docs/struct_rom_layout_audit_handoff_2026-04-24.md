# Struct ROM-Layout Audit — Handoff (2026-04-24)

## Status

Paused. `ITAttributes` audit done (commit `f4d656e`); the rest of the ROM-loaded attribute structs have not been checked against this method and may have the same class of bug.

## Why this audit exists

The upstream decomp's C struct declarations are not always faithful to the actual ROM bytes. Three failure modes have been observed in the port:

1. **Decomp declaration just wrong** — the upstream struct lists fields in positions that don't match what IDO emitted against the original source. ITAttributes `type`/`hitstatus` were declared in a final u32 that doesn't exist in ROM; they actually live inside the u32 at `0x3C` alongside `priority`/`kb_base`/`can_*`.
2. **IDO-specific bitfield packing rules the port's LE branch doesn't mirror** — IDO packs small fields into preceding `u16` pad gaps, folds a `u8` sitting next to a bitfield into the top byte of the u32 storage unit, and packs a plain `u16` declared after a partial u32 bitfield into the remaining low bits of that u32 (pad-gap packing, see `project_ido_bitfield_pad_packing.md`). Modern clang does none of these. When the port's LE branch doesn't account for them, field offsets drift.
3. **The port's own LE-matching pads are wrong** — in trying to make clang reproduce IDO's layout, earlier work added `u32 : N` pads that forced clang to start new u32 storage units where IDO packs through, inflating sizeof and shifting downstream fields. The ITAttributes rewrite removed four such pads; others may still be hiding in FTAttributes / event structs.

Symptoms cover a wide range — wrong damage numbers, hitboxes that don't trigger, items clipping through floors, SIGBUS from misrouted status IDs. They're often latent for items/fields that happen to read 0 or a small positive value, and only surface when one specific item/fighter combination exercises a field with a meaningful negative value or an unusual bit pattern.

## Method

The audit is mechanical once tooling is up. Per struct:

1. **Find the reader function's VRAM address.** The decomp annotates functions with `// 0x801XXXXX` comments; for ROM-loaded attribute structs this is typically the *Make* / *Setup* / per-frame update function that does `lw $X, offset($attr_ptr)` for every field. `itManagerMakeItem` at `0x8016E174` was our anchor for ITAttributes; FTAttributes' readers are `ftParamSetAttributes`, `ftParamSetInitVars`, etc.
2. **Disassemble with rabbitizer via the port's wrapper:**

   ```bash
   /tmp/rvenv/bin/python debug_tools/rom_disasm/disasm.py <vram-hex> \
       --rom baserom.us.z64 --count 800 --base v1 > /tmp/reader.asm
   ```

   `--base` must be whichever register the original code keeps the struct pointer in. `$v1` is common for attribute readers; check the first ~20 instructions to see where the pointer ends up. If the function calls `lbRelocGetFileData` early and stores into `$s0`/`$s1`, the attribute pointer tends to be in `$v1` by the time fields start getting read.
3. **Extract bit positions automatically:**

   ```bash
   /tmp/rvenv/bin/python debug_tools/rom_disasm/decode_bitfields.py \
       --base '$v1' /tmp/reader.asm
   ```

   This picks up the idiomatic IDO `lw $X, OFF($BASE); sll $Y, $X, K; srl $Z, $Y, M` extraction sequences and prints `(attr_offset, [high:low], width, signed)` per field. Pure `srl` (without preceding `sll`) = field at the top of the u32; plain `lh`/`lhu`/`lbu` loads = non-bitfield half-word or byte at that offset. Re-run the scan manually for `andi 0xF` extractions the current decoder misses (they're low-nibble fields).
4. **Compare against the port's declared bit positions.** Count bits in each `#else` (LE) branch u32. For each field: what's its position per the port, and what does IDO emit? Any mismatch → bug.
5. **Sanity-check per-item values.** Extract the reloc file with

   ```bash
   /tmp/rvenv/bin/python debug_tools/reloc_extract/reloc_extract.py extract \
       baserom.us.z64 <file_id> /tmp/<name>.bin
   ```

   then decode each field from the raw u32 bytes using the bit positions you just derived. For ITAttributes the gameplay-value sanity check was: `type` matches the `nITType*` enum per category, `damage` is 1-20, `size` is 60-300, `kb_base`/`kb_scale` are 20-150, `attack_count` is 1 (or 2 for multi-hitbox items). Equivalent sanity checks exist for FTAttributes (per-fighter damage, knockback constants, `jab_attack_*_damage` values against known frame data).
6. **Rewrite the LE branch** to mirror IDO positions. LSB-first declaration order in the LE branch produces the same numeric bit positions as MSB-first declaration order in the BE branch. For fields that IDO packed unusually (e.g., plain `u16 spin_speed` folded into a u32 bitfield's low half), declare them as bitfields on LE to force clang to pack them the same way. **Count bits per u32 carefully** — off-by-N pad counts produce silent wrong values. An off-by-2 in ITAttributes Word `0x3C` shifted `type` into bits 8-11 and made items route to the wrong status on A-press.
7. **Update the `portFixupStructU16` calls** in the corresponding manager's load-time fixup. Cover every u32 word from the first plain s16/u16 field to the last one. Bitfield-only u32s don't need fixup (pass1 BSWAP32 alone is correct for them).
8. **Remove any pre-existing PORT workarounds** that were compensating for the broken struct. Two examples from ITAttributes: the `ip->type = (*(u32*)(attr+0x3C) >> 10) & 0xF` bypass, and the `-attr->map_coll_bottom` negation. Once the struct is right, workarounds like these become actively wrong.
9. **Build + playtest.** Symptoms of residual bugs: values that are suspiciously uniform across items (fixed wrong bit); values that are off by a small factor (wrong width); values that look right for some items and garbage for others (bit-position bug where the wrong bit happens to be 0 for most items).

## Candidate structs, prioritized

High — same class as ITAttributes / WPAttributes, loaded from ROM, bitfield-heavy, known or suspected readers we can disassemble:

- **FTAttributes** (`src/ft/fttypes.h` / `ftattr.h`) — Fighter attribute data loaded per-character. Biggest struct of the group. Any bug here manifests as wrong damage / knockback / hurtbox / attack offsets for specific moves. Partial audit was done via `BITFIELD_SEXT*` and the `is_have_attacks4/attackhi4` swap commit (`3a05d31`) but not a systematic IDO-reader disassembly.
- **WPAttributes** — commit `0a9824c` + `ad6650a` landed one audit round (Mario fireball 64% damage fix). The method was used but not exhaustively; it's worth a full re-verify now that we know the decomp itself can be wrong (not just the port's LE mirror).
- **FTAttackEvent / FTMotionState / FTStatusEvent** — animation-driven attack event streams read per-frame in `ftMain*` paths. The ITAttackEvent fix (`87c4fe8`) established the IDO `u8 timer` packing; FT-side equivalents may have the same pattern.

Medium — loaded-from-ROM structs with fewer/no port overrides so far:

- **GRAttackColl / GRSectorDesc** (`src/gr/`) — stage hazard / sector data.
- **MPGroundData / MPSurface / MPVertex** (`src/mp/`) — collision geometry. Some targeted fixes already shipped (`mpgrounddata_layer_mask_u8_byteswap`, `mpvertex_byte_swap`) but not a full reader-disasm audit.
- Any `_ItemAttributes` / `_WeaponAttributes` in per-fighter reloc files that aren't file 251 — e.g., `llYoshiMainEggThrowWeaponAttributes`, `llPikachuSpecial1ThunderJolt*WeaponAttributes`.

Low — in-memory-only structs or ones already BSWAP32-clean:

- `GObj`, runtime state, anything allocated by the port and never loaded from ROM.
- Plain non-bitfield structs — subject to the usual pass1 BSWAP32 / fixup discipline but not the IDO packing drift.

## Tooling cheatsheet

- **rabbitizer venv**: `/tmp/rvenv/bin/python`. If missing: `python3 -m venv /tmp/rvenv && /tmp/rvenv/bin/pip install rabbitizer --quiet`.
- **ROM extraction**: `debug_tools/reloc_extract/reloc_extract.py extract baserom.us.z64 <file_id> <out.bin>`.
- **Disassembly**: `debug_tools/rom_disasm/disasm.py <vram> --rom baserom.us.z64 --count N --base <reg>`.
- **Bitfield decoder**: `debug_tools/rom_disasm/decode_bitfields.py <disasm.asm> --base '$v1'`. Misses pure `srl` (top-bit extraction) and `andi 0xF` (low-nibble). Scan the disasm output manually for those.
- **IDO round-trip verification** (optional, stronger): the method in `docs/debug_ido_bitfield_layout.md` — write a C probe with the rewritten struct, compile with IDO 5.3 BE at `references/ssb-decomp-re/tools/ido-recomp/5.3/cc`, disassemble the generated reader, compare instruction-for-instruction against the same extraction we did from `baserom.us.z64`. If they match, the struct is correct by construction. Worth a half-hour at the end of each struct rewrite if confidence matters.

## Pitfalls / lessons from the ITAttributes rewrite

- **"The decomp is 96% matching" is about code, not data structures.** Struct layouts are reverse-engineered from reader code and are more error-prone than the function bodies. Don't assume any struct is correct without checking.
- **IDO packs what clang doesn't.** Every time you see a plain `u16` or `u8` declared right after a bitfield in upstream, suspect pad-gap packing. The fix on LE is usually to declare the small field as a bitfield of the same width.
- **Artificial `u32 : N` pads in the port's LE branch are suspect.** They were added to force clang to mirror IDO, but if they add up wrong (by as little as 2 bits) they shift downstream fields. Always recount each u32's declared widths against the upstream field-list sum; IDO auto-pads the remainder, so the port's pad should equal `32 - sum(field widths in that word)`.
- **Previous workarounds in reader code become wrong once the struct is right.** The `ITAttributes.type` bypass and the `-attr->map_coll_bottom` negation both had to be deleted; leaving them in would have broken the new struct. Check `grep -rn '#ifdef PORT' src/ | grep <struct_field_name>` before finalizing.
- **Sanity-check decoded values against gameplay constants.** Wrong bit positions often produce "values that look like small numbers but aren't the right small number". Cross-reference published frame data or observed gameplay values (shield damage numbers, knockback angles, damage percentages).
- **Never negate to compensate.** If a field reads with the wrong sign, the struct is wrong; don't negate in the reader. The ITAttributes `map_coll_bottom` workaround was a cautionary case — the "positive magnitude" observation was purely an artifact of the wrong struct reading `map_coll_width`'s bytes.

## Resuming this work

Pick a struct from the priority list, find its reader's VRAM address, follow the nine-step method above. The ITAttributes commit (`f4d656e`) and its doc at `docs/bugs/itattributes_rom_layout_rewrite_2026-04-24.md` are worked examples — the diff shows the exact pattern a rewrite takes (delete artificial pads, restore upstream field flow, reverse LE branch for LSB-first, update `portFixupStructU16` range, remove compensating workarounds, update `_Static_assert(sizeof == ...)` to the true ROM stride).

Expected time per struct: 1-3 hours depending on field count and whether the reader code is cleanly one function or scattered across several.
