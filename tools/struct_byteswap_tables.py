#!/usr/bin/env python3
"""
struct_byteswap_tables.py — per-struct byteswap rules for source-compile.

The runtime byteswap pipeline (`port/bridge/lbreloc_byteswap.cpp`) does:

  1. pass1: BSWAP32 every aligned u32 in the data section.
  2. per-struct fixup hooks (e.g. portFixupFTAttributes) called from C code
     after load to rotate16 u16-pair words and bswap32 u8[4] color quads
     back into LE order.

Torch ships ROM bytes (BE-stored). Pass1 + per-struct fixup brings them to
correct LE in-memory layout. Source-compile (clang i686, native LE) emits
LE bytes; bswap32-only would force `pass1` to undo our work and the per-
struct fixup would then SWAP fields whose bytes were already in correct LE
order — flipping u16 pairs and inverting u8 RGBA quads.

Fix: BEFORE the global LE→BE bswap32 pass, apply per-struct rules at known
field offsets so the emitted bytes match Torch's bytes exactly. Then pass1
(at runtime) + the per-struct hook produce identical in-memory state from
both source-compiled and Torch-extracted resources.

The tables here are hand-transcribed from the runtime fixup helpers. Cross-
reference any change with the cited file:line.

  STRUCT_FIELD_FIXUPS[<type_name>] is a list of (kind, word_offset_in_struct).

Kinds:
  "rotate16"  — u16-pair word: bswap16 each of the two u16 halves before the
                global pass is excluded for this slot. Result on disk is the
                BE-stored u16 pair Torch ships. Runtime pass1 then
                fixup_rotate16 produce the correct LE u16 pair in memory.
  "raw_u8"    — bytes that have no endianness (u8 arrays, SYColorRGBA, raw
                filler). Both Torch and clang emit these bytes identically.
                We just need to NOT apply our global LE→BE pass to them.
                Runtime pass1 + (optional fixup_bswap32) net to either
                identity (for fixup-listed slots) or a constant permutation
                of bytes that's never read (for un-fixed-up filler).

For both rule kinds, the affected 4-byte slot is EXCLUDED from the global
LE→BE bswap32 pass.
"""

from __future__ import annotations

# (kind, word_offset_in_struct)
StructRule = tuple[str, int]


# FTAttributes — mirrors port/bridge/lbreloc_byteswap.cpp:2142–2151
# (portFixupFTAttributes). Word offsets cited from the same file's comment
# block at lines 2122–2140.
FT_ATTRIBUTES_FIXUPS: list[StructRule] = [
    # u16 pair words (rotate16 at runtime)
    ("rotate16", 0x2D),  # 0x0B4 — dead_fgm_ids[0], [1]
    ("rotate16", 0x2E),  # 0x0B8 — deadup_sfx, damage_sfx
    ("rotate16", 0x2F),  # 0x0BC — smash_sfx[0], smash_sfx[1]
    ("rotate16", 0x30),  # 0x0C0 — smash_sfx[2], pad
    ("rotate16", 0x39),  # 0x0E4 — itemthrow_vel_scale, itemthrow_damage_scale
    ("rotate16", 0x3A),  # 0x0E8 — heavyget_sfx, pad

    # u8[4] RGBA color quads (runtime pass1+fixup_bswap32 nets to identity)
    ("raw_u8", 0x3C),  # 0x0F0 — shade_color[0]
    ("raw_u8", 0x3D),  # 0x0F4 — shade_color[1]
    ("raw_u8", 0x3E),  # 0x0F8 — shade_color[2]
    ("raw_u8", 0x3F),  # 0x0FC — fog_color

    # u8[16] filler_0x30C (declared filler, never read by C; runtime applies
    # pass1 only — leaves bytes in a constant permutation that doesn't
    # affect game state but isn't byte-equivalent to disk Torch ships).
    ("raw_u8", 0xC3),  # 0x30C..0x30F — filler_0x30C[0..3]
    ("raw_u8", 0xC4),  # 0x310..0x313 — filler_0x30C[4..7]
    ("raw_u8", 0xC5),  # 0x314..0x317 — filler_0x30C[8..11]
    ("raw_u8", 0xC6),  # 0x318..0x31B — filler_0x30C[12..15]
]


FT_SKELETON_FIXUPS: list[StructRule] = [
    # Word 1: union {Gfx* dl, Gfx** dls}; pad/flags asymmetric — BE has flags
    # at byte 4 + 3 trailing pad; LE has 3 leading pad + flags. No runtime
    # fixup helper for FTSkeleton; runtime relies on Torch's BE-stored bytes
    # passing through pass1 bswap32 to land flags at byte 7 in LE memory.
    # Source path: leave clang's bytes raw, skip global pass.
    ("raw_u8", 0x1),
]


FT_MODELPART_FIXUPS: list[StructRule] = [
    # Word 4 of 5: pad/flags asymmetric word at byte +0x10. Same pattern.
    ("raw_u8", 0x4),
]


FT_COMMONPART_FIXUPS: list[StructRule] = [
    # Word 3 of 4: pad/flags asymmetric word at byte +0xC. Same pattern.
    ("raw_u8", 0x3),
]


FT_COMMONPART_CONTAINER_FIXUPS: list[StructRule] = [
    # FTCommonPartContainer = FTCommonPart[2] (sizeof 32 = 2×16). Rules for
    # each element's flags+pad word: word 3 of element 0 (byte +0xC) and
    # word 3 of element 1 (byte +0x1C). Files declare via the container
    # type rather than a plain FTCommonPart array, so the find-regions
    # regex needs the container as a separate entry.
    ("raw_u8", 0x3),  # commonparts[0].flags word
    ("raw_u8", 0x7),  # commonparts[1].flags word
]


MP_GROUNDDATA_FIXUPS: list[StructRule] = [
    # Mirrors mpCollisionFixGroundDataLayout in decomp/src/mp/mpcollision.c
    # at line 3969+ (read at planning time). Offsets computed from
    # MPGroundData layout in decomp/src/mp/mptypes.h:
    #   gr_desc[4]            0x00..0x3F  (4 × MPGroundDesc(16))
    #   map_geometry          0x40        u32 token
    #   layer_mask + pad      0x44        u8 + 3 pad   ── raw_u8 (word 0x11)
    #   wallpaper             0x48        u32 token
    #   fog_color (3 u8)      0x4C        ── raw_u8 (word 0x13)
    #   fog_alpha (u8)        0x4F        ── still in word 0x13
    #   emblem_colors[4][3]   0x50..0x5B  ── raw_u8 (words 0x14..0x16)
    #   unused                0x5C        s32        ── raw_u8 (word 0x17)
    #   light_angle           0x60..0x6B  Vec3f (3 f32) — bswap32 ok
    #   camera_bound_*        0x6C..0x7B  4 u16 pairs ── rotate16 (words 0x1B..0x1E)
    #   bgm_id                0x7C        u32
    #   map_nodes             0x80        u32 token
    #   item_weights          0x84        u32 token
    #   alt_warning           0x88        s16 (start of "team" rotate16 region)
    #   camera_bound_team_*   0x8A..0x91  4 s16
    #   map_bound_team_*      0x92..0x99  4 s16
    #   zoom_start (Vec3h)    0x9A..0x9F  3 s16
    #   zoom_end (Vec3h)      0xA0..0xA5  3 s16
    #   total                 0xA8        (rounded up to 4-byte boundary)
    # Runtime portFixupStructU16 from team_off rotates (end_off - team_off + 3)/4
    # words = (0xA6 - 0x88 + 3)/4 = 8 words → words 0x22..0x29.
    ("raw_u8",  0x11),
    ("raw_u8",  0x13),  # fog_color (u8[3]) + fog_alpha (u8)
    ("raw_u8",  0x14),  # emblem_colors[0] + start of [1]
    ("raw_u8",  0x15),  # emblem_colors[1] tail + start of [2]
    ("raw_u8",  0x16),  # emblem_colors[2] tail + emblem_colors[3]
    # word 0x17 (unused): .c source declares as `s32 unused = 0xFFFFFF00`
    # — clang emits LE u32 bytes `00 FF FF FF`. Torch ships ROM bytes
    # `FF FF FF 00` (BE u32). The global LE→BE pass converts clang's bytes
    # to Torch's, so we deliberately do NOT mark this word raw_u8 even
    # though the runtime calls portFixupStructU32 on it (the runtime's
    # 2× bswap32 nets to identity, so memory ends up matching ROM bytes
    # interpreted as the u8[4] emblem-color the runtime really uses).
    ("rotate16", 0x1B),
    ("rotate16", 0x1C),
    ("rotate16", 0x1D),
    ("rotate16", 0x1E),
    ("rotate16", 0x22),
    ("rotate16", 0x23),
    ("rotate16", 0x24),
    ("rotate16", 0x25),
    ("rotate16", 0x26),
    ("rotate16", 0x27),
    ("rotate16", 0x28),
    ("rotate16", 0x29),
]


# MPItemWeights is `struct { u8 values[N]; }` (variable-length flex array).
# Every byte is a u8 weight (0..15). Whole struct is raw u8 — no bswap.
# Use the `raw_u8_all` rule which expands to one raw_u8 per word in the
# symbol's allocated footprint at apply time.
MP_ITEMWEIGHTS_FIXUPS: list[StructRule] = [
    ("raw_u8_all", 0),
]


FT_TEXTUREPART_CONTAINER_FIXUPS: list[StructRule] = [
    # FTTexturePartContainer = FTTexturePart[2] where FTTexturePart is 3 u8s
    # (joint_id + detail[2]). Total 6 bytes + 2 byte-pad = 8 bytes. All u8 —
    # no bswap32 needed; runtime treats raw bytes the same way Torch ships
    # them (pass1 bswap32 happens but the field accessors are u8 so the
    # bswap effectively reorders the byte indices for u8 access — both
    # Torch's and our bytes are equally raw, so byte-equivalence is the goal).
    ("raw_u8", 0x0),
    ("raw_u8", 0x1),
]


STRUCT_FIELD_FIXUPS: dict[str, list[StructRule]] = {
    "FTAttributes":            FT_ATTRIBUTES_FIXUPS,
    "FTSkeleton":              FT_SKELETON_FIXUPS,
    "FTModelPart":             FT_MODELPART_FIXUPS,
    "FTCommonPart":            FT_COMMONPART_FIXUPS,
    "FTCommonPartContainer":   FT_COMMONPART_CONTAINER_FIXUPS,
    "FTTexturePartContainer":  FT_TEXTUREPART_CONTAINER_FIXUPS,
    "MPGroundData":            MP_GROUNDDATA_FIXUPS,
    "MPItemWeights":           MP_ITEMWEIGHTS_FIXUPS,
    # WPAttributes added in P15 phase 3.
}


# Symbol-name suffixes for files where the relocData .c declares the field
# with a primitive type (e.g. `u8 dXXX_item_weights[20]`) instead of the
# semantically-equivalent struct type. Symbols whose names match the regex
# are treated as if they had the corresponding fixup-table type.
import re as _re
SYMBOL_NAME_TYPE_OVERRIDES: list[tuple[_re.Pattern[str], str]] = [
    # Stage-specific item-weight u8 arrays. The .c convention is
    # `u8 d<Stage>_item_weights[N]`. The header struct (MPItemWeights) is
    # rarely used directly; both forms hold raw u8 weights.
    (_re.compile(r"_item_weights$"), "MPItemWeights"),
]


# Struct-size sanity check against the ELF symbol's declared size, to catch
# silent struct-layout drift between the runtime headers and these tables.
# Source: _Static_assert(sizeof(<TYPE>) == 0x...) lines in the decomp headers.
STRUCT_SIZE: dict[str, int] = {
    "FTAttributes":           0x348,  # decomp/src/ft/fttypes.h:1406
    "FTSkeleton":             0x008,  # decomp/src/ft/fttypes.h:1052
    "FTModelPart":            0x014,  # decomp/src/ft/fttypes.h
    "FTCommonPart":           0x010,  # decomp/src/ft/fttypes.h
    "FTCommonPartContainer":  0x020,  # 2× FTCommonPart(16)
    "MPGroundData":           0x0A8,  # 0xA6 + 2-byte alignment pad
    # FTTexturePartContainer not listed: ELF symbol size is 6 (just 2×3 u8
    # struct) but the next symbol is allocated 8 bytes after — the 2 trailing
    # padding bytes get raw_u8 treatment via word 1 of the table, which
    # extends past the 6-byte sizeof.
    # MPItemWeights not listed: variable-length u8[N] flex array, size
    # determined per-symbol by ELF (raw_u8_all rule expands accordingly).
}
