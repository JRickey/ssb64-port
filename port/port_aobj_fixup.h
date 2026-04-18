#pragma once

/**
 * port_aobj_fixup.h — per-stream un-halfswap for AObjEvent32 animation data.
 *
 * Fighter reloc files (`reloc_animations/FT*`, `reloc_submotions/FT*`) go
 * through portRelocFixupFighterFigatree at load time, which u16-halfswaps
 * every non-reloc u32 slot.  That is correct for AObjEvent16 figatree
 * data (u16 pairs packed into u32 slots) but corrupts AObjEvent32 u32
 * bitfield commands — opcode lands in bits 9-15 instead of 25-31 and
 * flags splits across the halfswap boundary in a way no bitfield can
 * express.
 *
 * The file contains both stream types, and at load time we can't tell
 * which token targets which kind of stream (the choice is runtime, via
 * fp->anim_desc.flags.is_anim_joint per motion).  So the fix is lazy:
 * the first time an EVENT32 reader touches a stream, we walk it from the
 * head, un-halfswap each data u32 in place, skip token slots, and record
 * the head in a visited set so subsequent passes are no-ops.
 *
 * Callers: gcParseDObjAnimJoint and gcParseMObjMatAnimJoint in
 * src/sys/objanim.c (at function entry, wrapped in #ifdef PORT).
 *
 * The head is passed as void* so the header has no game-type dependency.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Walk an AObjEvent32 stream starting at @p head, un-halfswapping every
 * data u32 encountered (command words, flag words, f32 payloads) until
 * the End opcode (0) or a stream-terminating Jump/SetAnim.  Token slots
 * inside Jump/SetAnim/SetInterp events are left alone (the reloc chain
 * already wrote native u32 token indices into those).  Jump and SetAnim
 * targets are walked recursively.  Idempotent via an internal visited
 * set — calling on an already-walked head is a no-op.
 *
 * Safe on NULL.  Aborts with a warning log on unrecognised opcodes
 * (≥24) or if a per-stream step limit is exceeded.
 */
void port_aobj_event32_unhalfswap_stream(void *head);

/**
 * Register a memory region (from a fighter-figatree reloc file) that was
 * u16-halfswapped at load time.  The walker refuses to touch pointers
 * outside any registered region — protects against accidentally
 * un-halfswapping data that was never halfswapped in the first place
 * (e.g. EVENT32 streams in non-fighter files, or EVENT16 streams whose
 * bytes happen to be walked by mistake).  Called from lbreloc_bridge.cpp
 * after portRelocFixupFighterFigatree.
 */
void port_aobj_register_halfswapped_range(void *base, unsigned long size);

/**
 * Clear the visited set and registered ranges.  Called on scene reset
 * so streams that get unloaded and reloaded are re-walked on next
 * access.  Currently not wired in (tokens get invalidated on reset
 * anyway) — exposed for future integration with portRelocResetPointerTable.
 */
void port_aobj_event32_unhalfswap_reset(void);

/**
 * Returns 1 if @p lies in any registered halfswapped range, 0 otherwise.
 * Used by ftkey.c to decide whether a FTKeyEvent stream needs runtime
 * Vec2b byte-swap compensation (stick operand halfwords).
 */
int port_aobj_is_in_halfswapped_range(const void *p);

#ifdef __cplusplus
}
#endif
