# FTTexturePartContainer post-pass1 byte order — non-destructive fix (2026-04-30)

## Symptom

GitHub issue #30: during Pikachu's forward smash, Pikachu's face texture became garbled and the cheek/body flash did not match N64 reference behavior.

## Background — first attempt and why it was reverted

`FTTexturePartContainer` is reached through `attr->textureparts_container` and read by `ftparam.c`'s three texture-part functions as `FTTexturePart textureparts[2]` where each entry is `{u8 joint_id; u8 detail[2]}` = 3 bytes (sizeof = 6). Pass1 BSWAP32 reverses bytes within each u32 word at file load, so for the 6-byte container the u8 fields read as the wrong byte (post-pass1 byte 0 = original byte 3, etc.).

The first fix (commit `435c944`, reverted in `1f2b694` after CSS regression) ran `bswap32(&w[0]); bswap32(&w[1])` on the resolved container address — i.e., wrote 8 corrected bytes back into the file image in place. This worked for Pikachu (his raw bytes are `0b 00 00 0b 01 01 00 00`, with zero pad at offsets 6-7, so bswapping word 1 was a no-op for those bytes). But for **other fighters whose `attr->textureparts_container` resolves to memory aliased with adjacent reloc-token-shaped data**, writing 8 bytes back corrupted those adjacent tokens. Symptom: 1P CSS crashed with `SIGSEGV fault_addr=0x0` inside `ftMainSetStatus` (`status=0x10003` = CSS win-pose) the moment the player locked in any fighter, because `ftParamResetTexturePartAll → ftParamGetTexturePartsContainer` ran the fixup and corrupted an adjacent token that `ftMainSetStatus`'s next `PORT_RESOLVE` call decoded to NULL.

A per-byte attempt that touched only bytes 0-5 (leaving bytes 6-7 in pass1-state) also crashed for some fighters with a different fault address `0xffff000100000037`. Diagnostic dumps showed e.g. for Mario:

```
PRE  (post-pass1):  00 00 00 0c | ae 06 00 82 | af 06 00 82 | b0 06 00 82
POST (per-byte):    0c 00 00 00 | 82 00 00 82 | af 06 00 82 | b0 06 00 82
                                ^^^^^^^^^^^^
                                this 4-byte word is actually a u32 reloc token
                                (0x820006AE) — the per-byte write changed it to
                                0x82000082, which then PORT_RESOLVE'd to nonsense
```

So the issue isn't *which* bytes we write back — it's that **for some fighters the resolved container address aliases with adjacent reloc-token data, and any write through that pointer corrupts those tokens**. The C struct's claim of "always 6 bytes of textureparts data" doesn't hold for every fighter's file layout (some have 4 bytes of container + adjacent tokens at offset 4+).

## Fix

`portFixupFTTexturePartContainer` no longer modifies the input memory. It returns a static-storage **8-byte buffer** holding the 6 corrected bytes (plus 2 zero pad). Callers dereference the returned pointer for `textureparts[i]` reads, getting the correct decoded values without ever writing through the file image.

```c
static uint8_t sFixed[8];
uint8_t *src = static_cast<uint8_t *>(container);
sFixed[0] = src[3];   /* bswap of word 0 (bytes 0..3) */
sFixed[1] = src[2];
sFixed[2] = src[1];
sFixed[3] = src[0];
sFixed[4] = src[7];   /* pass1 moved original byte 4 → position 7 */
sFixed[5] = src[6];   /* pass1 moved original byte 5 → position 6 */
sFixed[6] = 0;
sFixed[7] = 0;
return sFixed;
```

The static buffer is single-instance: callers must not interleave lookups for two different containers without consuming the first. `ftparam.c`'s three call sites each take a single snapshot inside one function — safe.

## Trade-off

The static buffer means `textureparts_container` no longer points at the file image; its lifetime is "until the next call to `portFixupFTTexturePartContainer`". Code that captures the pointer and uses it across function boundaries would observe stale data. None of the three current call sites do that — they each take the snapshot, run their loop locally, and don't escape the pointer. If new call sites are added, they should follow the same pattern.

## Verification

- 1P character select: lock in any fighter, no `SIGSEGV` at `ftMainSetStatus` (the regressed-fix's symptom).
- Real playthrough: Pikachu vs CPU on a stage, perform F-Smash, observe the face/cheek texture changes flash correctly through the move's frames. User-confirmed.
- Build: clean.

## Class

This is one instance of a broader category: **C structs declared as the full layout of file data don't hold uniformly across all loaded files**. Some fighters' `attr->textureparts_container` points at memory whose layout is `[6-byte container][2-byte pad]` (Pikachu), but for others it points at `[4-byte container][4-byte adjacent token]` or even `[token data masquerading as container]`. The fact that pass1 BSWAP32 corrupts u8 fields means a fix is needed, but **any in-place write risks corrupting whatever the per-fighter file layout happens to share with the container's address range**. Returning a corrected copy is the safe pattern for this class.
