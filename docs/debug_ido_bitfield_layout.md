# IDO BE Bitfield Layout Audit — Method

When a decomp struct has `#if IS_BIG_ENDIAN` bitfield blocks, the `#else` (PORT / clang LE) block is the port author's best guess at matching IDO's physical bit layout. IDO BE's bitfield packing is idiosyncratic — it **does not** always start a new 4-byte-aligned storage unit for each `u32`/`s32` bitfield group. It will pack small bitfields into preceding u16 pad gaps, MSB-first, using a 16-bit storage unit. Mixed signedness bitfields can also change allocation behavior.

This means you cannot tell the true physical bit positions from reading the decomp declaration alone. **You have to compile the struct with IDO and disassemble the reader.** The compiled MIPS load/shift/mask instructions are ground truth. Everything else is speculation.

`docs/bugs/wpattributes_bitfield_padding_2026-04-20.md` is the worked example — Mario Fireball was doing 64% damage because we had guessed wrong about where IDO put `damage`. The fix came from following this audit method. Expect other structs to have the same bug class.

## Prerequisites (one-time setup)

- IDO 5.3 compiler lives at `/Users/jackrickey/Dev/decomp-agent/references/ssb-decomp-re/tools/ido-recomp/5.3/cc`. If you're on a different machine, clone `github.com/Killian-C/ssb-decomp-re` and use its bundled copy.
- Python venv with `rabbitizer` at `/tmp/rvenv`:

  ```bash
  python3 -m venv /tmp/rvenv
  /tmp/rvenv/bin/pip install rabbitizer --quiet
  ```

## The probe

Write a C file that declares the struct **exactly** as the decomp source does (not the `#else` port branch — use the IDO BE branch), plus a reader function that extracts every field into an output array. IDO parses K&R-ish C so keep declarations conservative:

- No `//` comments (use `/* */`).
- No mixed declarations and code.
- Use minimal typedefs (`u8`, `u16`, `u32`, `s8`, `s16`, `s32`, `ub32 = u32`).

```c
/* /tmp/probe_wpattr.c — matches the IDO BE branch of src/wp/wptypes.h */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef u32 ub32;

struct Vec3h { s16 x, y, z; };

typedef struct WPAttributes
{
    void *data;
    void *p_mobjsubs;
    void *anim_joints;
    void *p_matanim_joints;
    struct Vec3h attack_offsets[2];
    s16 map_coll_top;
    s16 map_coll_center;
    s16 map_coll_bottom;
    s16 map_coll_width;
    u16 size;
    s32 angle : 10;
    u32 knockback_scale : 10;
    u32 damage : 8;
    u32 element : 4;
    u32 knockback_weight : 10;
    s32 shield_damage : 8;
    u32 attack_count : 2;
    ub32 can_setoff : 1;
    u32 sfx : 10;
    u32 priority : 3;
    ub32 can_rehit_item : 1;
    ub32 can_rehit_fighter : 1;
    ub32 can_hop : 1;
    ub32 can_reflect : 1;
    ub32 can_absorb : 1;
    ub32 can_shield : 1;
    ub32 unused_0x2F_b6 : 1;
    ub32 unused_0x2F_b7 : 1;
    u32 knockback_base : 10;
} WPAttributes;

void reader(WPAttributes *attr, s32 *out) {
    out[0] = attr->size;
    out[1] = attr->angle;
    out[2] = attr->knockback_scale;
    out[3] = attr->damage;
    out[4] = attr->element;
    out[5] = attr->knockback_weight;
    out[6] = attr->shield_damage;
    out[7] = attr->attack_count;
    out[8] = attr->can_setoff;
    out[9] = attr->sfx;
    out[10] = attr->priority;
    out[11] = attr->knockback_base;
    /* Add entries for every remaining field you care about */
}
```

Compile with the same flags the decomp uses:

```bash
IDO=/Users/jackrickey/Dev/decomp-agent/references/ssb-decomp-re/tools/ido-recomp/5.3/cc
$IDO -c -Wo,-loopunroll,0 -G0 -mips2 -O2 -o /tmp/probe.o /tmp/probe_wpattr.c
```

Disassemble with rabbitizer, parsing the ELF to find `.text`:

```python
# /tmp/rvenv/bin/python - <<'PY'
import rabbitizer, struct
with open('/tmp/probe.o', 'rb') as f:
    data = f.read()
def u32be(b, off): return struct.unpack('>I', b[off:off+4])[0]
def u16be(b, off): return struct.unpack('>H', b[off:off+2])[0]

shoff = u32be(data, 0x20)
shentsize = u16be(data, 0x2E)
shnum = u16be(data, 0x30)
shstrndx = u16be(data, 0x32)
shstr_off = u32be(data, shoff + shentsize * shstrndx + 0x10)
shstr_size = u32be(data, shoff + shentsize * shstrndx + 0x14)
shstr = data[shstr_off:shstr_off + shstr_size]

for i in range(shnum):
    sb = shoff + i * shentsize
    name_off = u32be(data, sb)
    name = shstr[name_off:shstr.index(b'\0', name_off)].decode()
    if name == '.text':
        text_off = u32be(data, sb + 0x10)
        text_size = u32be(data, sb + 0x14)
        for j in range(0, text_size, 4):
            w = u32be(data, text_off + j)
            ins = rabbitizer.Instruction(w)
            print(f"{j:04x}: {w:08x}  {ins.disassemble()}")
        break
# PY
```

## Reading IDO bitfield-access patterns

Typical MIPS idioms IDO emits for bitfield reads:

| Asm pattern | Meaning |
|---|---|
| `lhu $t, OFF($a)` | Load u16 at OFF (no sign extend) — unsigned storage |
| `lh $t, OFF($a); sra $t, N` | Load s16 at OFF, sign-extend bits `15-N` down to position 0 — **signed bitfield in a u16 storage at OFF**, width `16-N` bits |
| `lbu $t, OFF($a); srl $t, N` | Load u8 at OFF, keep top `8-N` bits at position 0 — unsigned bitfield at top of byte OFF |
| `lbu $t, OFF($a); andi $t, MASK` | Load u8, keep low bits — unsigned bitfield at bottom of byte OFF |
| `lw $t, OFF($a); srl $t, N` | Load u32, keep top `32-N` bits — bitfield at bits `31..N` of u32 at OFF |
| `lw $t, OFF($a); sll $t, A; srl $t, B` | Mid-word bitfield. Width = `32-B`. Position: after `sll A`, the first bit of the field is at position 31. Original position was bit `(31-A)` down to `(31-A) - (32-B) + 1 = (B-A) - 1`. **Simpler mental model:** the field occupies bits `[31-A, 31-A - (32-B) + 1] = [31-A, B - A - 1]`, width `32-B`, mask `(1 << (32-B)) - 1`. |
| `lw $t, OFF($a); andi $t, MASK` | Low-end bitfield. Width = popcount(MASK). Position: bits `[width-1, 0]`. |

For each field, the compiler tells you **where** (load offset + shift amounts) and **width** (via mask or via the shift delta). Record these as `(offset, shift_left, shift_right, width, signed)` tuples and translate into physical bit positions in the u32/u16 at that offset.

## Mapping IDO output to a PORT struct

On clang LE, bitfields pack **LSB-first** in their storage unit, which is the opposite of IDO BE's MSB-first. To get the same physical bit positions, declare the fields in the `#else` (LE) block in **reverse** declaration order of the IDO BE block, using `u32` as the base type throughout (to prevent clang from splitting storage units on signedness changes), with `BITFIELD_SEXT<N>()` applied at the read site for signed fields.

For fields IDO packed into u16 pad gaps (e.g. `angle` in `WPAttributes`), declare an explicit raw `u16 _<fieldname>_raw;` in the PORT struct and extract at the read site via:

```c
s32 val = ((s16)attr->_field_raw) >> (16 - width);   /* signed, top-anchored */
s32 val = (attr->_field_raw >> (16 - width)) & ((1 << width) - 1);  /* unsigned */
```

## Known audit targets (structs with `#if IS_BIG_ENDIAN` bitfields)

These files each declare one or more bitfield-heavy ROM-backed structs. Every struct in these files should be audited against IDO output before we trust its port layout. Known-audited and known-broken-but-fixed items are noted; everything else is unverified.

| File | Notes |
|---|---|
| `src/wp/wptypes.h` | `WPAttributes` — **audited & fixed** (2026-04-20). See `docs/bugs/wpattributes_bitfield_padding_2026-04-20.md`. |
| `src/it/ittypes.h` | `ITAttributes`, `ITAttackColl` — analogous to `WPAttributes`; Word B1/B2/B3 layout almost certainly wrong in port. `type` field fixed separately in `docs/bugs/itattributes_type_field_offset_2026-04-20.md`. **Audit remaining fields.** |
| `src/gm/gmscript.h` | Animation/script event structs (`GMScriptEvent`, etc.) — event decoding is load-bearing for every fighter/item animation. |
| `src/gm/gmtypes.h` | `GMStatFlags`, attack records, status state. Combat logic touches these every frame. |
| `src/sys/objtypes.h` | `AObjEvent32`, `AObjEvent16`, `MObjSub` — sprite/mesh animation events; halfswap fixup already has known interactions (see `project_aobjevent32_halfswap.md`). |
| `src/ft/fttypes.h` | `FTAttributes`, fighter hitbox state. Fighter melee appears to work, so bit positions are probably correct by luck — but worth confirming. |
| `src/ft/ftdef.h` | Motion/status descriptor bitfields. |

## Audit workflow (per struct)

1. Copy the decomp's `#if IS_BIG_ENDIAN` branch into a `/tmp/probe_<name>.c` file with a reader function that extracts every field.
2. Compile with IDO and disassemble the reader's `.text`.
3. Record each field's `(load_offset, load_width, shift_ops, signed)`.
4. Decode: `physical_bits[hi:lo]` for each field in the u32/u16 at `load_offset`.
5. Compare to the port's `#else` block's implied LSB-first bit positions. If they don't match, the port is broken for that field.
6. Pick a real ROM record for that struct, extract via `debug_tools/reloc_extract/reloc_extract.py`, and decode manually using both the IDO layout and the port layout. The discrepancy is the bug's magnitude.
7. If broken: rewrite the PORT `#else` block using reversed field order (LSB-first packing → MSB-first physical layout); extract any pad-gap bitfields via explicit `u<N>` raw storage fields with manual shift at read site.
8. Build. Write the bug doc as `docs/bugs/<struct>_layout_<date>.md`.

## Fingerprints that this bug class is biting you

- A damage / angle / size value reads as ~10-100× larger than expected.
- A count / flag / boolean field reads as 0 when it should be non-zero (or vice versa).
- Enum-valued fields (element, type, hitstatus) return garbage (values outside the enum range, or plausible-but-wrong enumerants).
- Hit detection fails silently (loop iterates zero times because `attack_count` decodes to 0).
- Sibling fields all decode to plausible-but-wrong values — one wrong field isn't a coincidence, it's a whole misaligned word.
