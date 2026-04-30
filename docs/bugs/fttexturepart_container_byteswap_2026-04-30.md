# FTTexturePartContainer byte order (2026-04-30)

## Symptom

GitHub issue #30: during Pikachu's forward smash, Pikachu's face texture became garbled and the cheek/body flash did not match N64 reference behavior.

The reported build was commit `585691c2faea` with the US v1.0 baserom SHA1 `e2929e10fccc0aa84e5776227e798abc07cedabf`.

## Root Cause

`FTTexturePartContainer` is ROM-backed fighter data reached through `FTAttributes::textureparts_container`. It stores two `FTTexturePart` records:

```c
struct FTTexturePart
{
    u8 joint_id;
    u8 detail[2];
};
```

The reloc loader's blanket pass1 `BSWAP32` is correct for u32/f32 fields, but not for this u8-only packed container. Pikachu's raw ROM bytes at `PikachuMain` file 243, target offset `0x140`, are:

```text
0b 00 00  0b 01 01  00 00
```

That decodes as:

| Entry | joint_id | high detail slot | low detail slot |
|-------|----------|------------------|-----------------|
| 0 | 11 | 0 | 0 |
| 1 | 11 | 1 | 1 |

After pass1, the port read the same bytes as:

```text
0b 00 00  0b 00 00  01 01
```

Entry 1 therefore became `[joint_id = 11, detail = { 0, 0 }]`. When Pikachu's F-Smash motion script issued `SetTexturePartID` for the alternate face/cheek material slot, `ftParamSetTexturePartID` walked to material index 0 instead of index 1 on joint 11 and changed the wrong `MObj`.

The initial suspicion was an IDO bitfield-layout mismatch in Pikachu's motion/color structs. That was ruled out by compiling IDO reader probes and disassembling with Rabbitizer:

- `FTMotionEventSetTexturePartID`: opcode bits 31..26, texturepart_id 25..20, frame 19..0
- `FTMotionEventSetColAnimID`: opcode bits 31..26, colanim_id 25..18, length 17..0
- `GMColEventSetRGBA2`: byte offsets 0..3 read as r/g/b/a
- `GMColEventMakeEffect1` and `GMColEventSetLight`: current PORT layouts match IDO's physical bit positions

## Fix

Added `portFixupFTTexturePartContainer`, an idempotent struct fixup that `BSWAP32`s both words of the 8-byte container back to byte order that u8 field reads expect. `ftParamInitTexturePartAll`, `ftParamSetTexturePartID`, and `ftParamResetTexturePartAll` now resolve the texture-part container through a local helper that runs the fixup before reading it.

This mirrors the existing use-time fixup pattern for `MObjSub`: the pointed-to file data is corrected at the boundary where game code starts reading sub-u32 struct fields.

## Verification

- IDO + Rabbitizer probe ruled out the suspected F-Smash motion/color bitfield layouts.
- Raw ROM extraction via `debug_tools/reloc_extract/reloc_extract.py extract baserom.us.z64 243 /tmp/pikachu_main.bin` confirmed the Pikachu texture-part records and the exact pass1 corruption.
- Build target: `cmake --build /Users/jackrickey/Dev/ssb64-port/.claude/worktrees/issue30-pikachu/build --target ssb64 -j`.
