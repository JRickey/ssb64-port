# M3.P15 — Source-compile bitfield-init relocData files

**Status as of 2026-05-04:** 66 of 76 originally-skipped files now byte-identical and source-compiled (FTAttributes 27/27, MPGroundData 39/41). 10 deferred: WPAttributes (8) + 2 MPGroundData edge cases. See `tools/struct_byteswap_tables.py` for the per-struct field-byteswap tables that drive this; mirrors the runtime helpers in `port/bridge/lbreloc_byteswap.cpp`.

## What landed

Build-side per-struct byteswap pass (`tools/build_reloc_resource.py`'s `apply_struct_aware_bswap`) runs before the global LE→BE pass and handles fields whose bytes don't match the bswap32-uniform contract. Three rule kinds:

- `rotate16` — bswap16 each u16 half (for u16-pair words like `dead_fgm_ids`, `camera_bound_*`, `deadup_sfx`)
- `raw_u8` — leave clang's bytes alone, skip the global pass (for u8 RGBA quads, raw filler bytes from `.data.inc.c`)
- `raw_u8_all` — apply raw_u8 to every word in the symbol's footprint (for variable-length flex-array u8 structs like `MPItemWeights`)

Plus two additional pipeline fixes that were prerequisites:

- `-fno-zero-initialized-in-bss` clang flag — forces all-NULL pointer arrays into `.data` so they actually get emitted (otherwise modelparts_container[25] = {NULL,...} silently dropped).
- LE-bitfield positional-init reversal — preprocesses the .c source to reverse the 22-line `is_have_*` init block, because the LE struct declares those fields in reverse order vs BE and positional init fills declaration-order.

Tables now exist for: `FTAttributes`, `FTSkeleton`, `FTModelPart`, `FTCommonPart`, `FTCommonPartContainer`, `FTTexturePartContainer`, `MPGroundData`, `MPItemWeights`. Plus generic u8[] array detection from .c source text.

## Remaining work — 10 files

### WPAttributes (8 files) — `204_MarioSpecial1`, `210_FoxSpecial1`, `218_SamusSpecial1`, `222_LuigiSpecial1`, `226_LinkSpecial1`, `240_NessSpecial1`, `244_PikachuSpecial1`, `251_ITCommonData`

**Blocker:** the .c sources declare a single 52-byte `WPAttributes` struct, but Torch's resources are 64 bytes (12 bytes of trailing per-file data the .c doesn't declare). For `244` and `251` the trailing tail is 8 bytes. So the file is missing ~12 bytes of real ROM data.

**Fix:** extend each WPAttributes .c with an explicit trailing declaration that captures whatever those bytes are. Without ROM disassembly to identify the trailing field's purpose, the simplest portable form is `u8 dXXX_pad_after_attr[12] = { #include <XXX/pad_after_attr.data.inc.c> };` and let `extract_inc_c.py` generate the bytes from `BattleShip.o2r`. Then the file becomes byte-equivalent without any new struct knowledge.

The `sfx:10` truncation warning observed during early compilation was a red herring — clang's LE bitfield struct definition is correct; the warning fires for legal values that exceed 10 bits when fed to a byte-sized init slot. Not a blocker for byte-equivalence.

### MPGroundData (2 files) — `264_GRYamabukiMap`, `295_GRBonus3Map`

**Blocker:** these files declare `u32 dXXX_AttackEvents[8] = { #include <...inc.c> };` but the inc.c contains u8 hex bytes (`0x00, 0x5A, 0x47, 0x80, ...`). clang interprets each comma-separated value as one full u32 element, taking only the first 8 values into the array. The intended pack-4-u8s-per-u32 layout is silently lost.

**Fix:** the cleaner direction is fixing `tools/extract_inc_c.py` so that when the declaration's element type is `u32`, it emits comma-separated `0xAABBCCDD` hex u32 values (4 bytes each), not individual `0xAA, 0xBB, ...` u8 sequences. Alternatively, change the .c source to declare these as `u8[32]` rather than `u32[8]` — same on-disk bytes either way.

### Wider question — should this go upstream?

The decomp build runs IDO MIPS BE, where MSB-first bitfield packing matches the runtime. The port runs clang LE, which packs LSB-first. Our LE struct branches in `decomp/src/ft/fttypes.h` etc. already reverse field order to compensate for bit positions, but they require positional-init reversal at the source-compile step. A future cleanup: convert the .c initializers from positional to designated form (`{ .is_have_attack11 = 1, ... }`) — would be unconditionally correct on both BE and LE and remove the .c source preprocess step.

## How to make further progress

1. **WPAttributes byte-extension:** for each of the 8 files, extract the 8 or 12 trailing bytes from `BattleShip.o2r` and either inline-init them in the .c or add a `#include` of a generated `.inc.c`. Then drop `WPAttributes` from `_BITFIELD_TYPE_RE` in `tools/gen_reloc_cmake.py`.
2. **u32[N] inc.c fix:** rework the `data` format emitter in `tools/extract_inc_c.py` to honour the declared element type. Then re-validate Yamabuki and Bonus3.
3. **Designated init migration (optional, upstream-friendly):** for each FTAttributes / WPAttributes init in `decomp/src/relocData/`, replace positional-with-trailing-comment-form with designated-init form. Removes the LE preprocess step and is also easier for modders to read.
