#pragma once

/**
 * frame_interpolation — display-rate decoupling for the SSB64 PC port.
 *
 * Adapted from Shipwright's soh/frame_interpolation. Game logic still ticks
 * at the original 30 Hz; the renderer ticks at the user's chosen rate, and
 * intermediate display frames are produced by interpolating the GBI matrices
 * built by syMatrix*() between the previous and current logic ticks.
 *
 * The recording layer instruments matrix.c primitives to capture inputs into
 * a tree of Path nodes. Tree nodes are scoped by (stable_ptr, sub_id) labels
 * via FrameInterpolation_RecordOpenChild/CloseChild — these are typically
 * inserted around per-DObj/per-particle/per-camera draw scopes so the diff
 * between two recordings can match logically-equivalent matrices even when
 * the destination Mtx* pointer is recycled or the actor list changes.
 *
 * The C API takes void* for matrix pointers so the header has no
 * game-type dependency and can be included from any .c file.
 */

#ifdef __cplusplus
#include <unordered_map>
#include <fast/types.h>   // MtxF
typedef MtxF FrameInterpReplacementMtxF;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* Recording lifecycle                                                        */
/* -------------------------------------------------------------------------- */

/* Begin recording a new frame's matrix-build tree. Called by the port harness
 * just before the game's display-list build begins (frame N's logic). The
 * previous recording is moved aside as the "old" reference for interpolation. */
void FrameInterpolation_StartRecord(void);

/* End recording. Called after the game's DL build is complete. */
void FrameInterpolation_StopRecord(void);

/* Hint the harness that the camera was just cut/teleported — interpolation
 * should not lerp the camera matrix this frame. Mirrors Shipwright's
 * DontInterpolateCamera. Optional — call from cutscene transitions. */
void FrameInterpolation_DontInterpolateCamera(void);

/* Returns 1 if recording is currently active, 0 otherwise.
 * Cheap — used to short-circuit Record* calls from hot paths. */
int FrameInterpolation_IsRecording(void);

/* -------------------------------------------------------------------------- */
/* Tree scoping                                                               */
/* -------------------------------------------------------------------------- */

/* Push a labeled child node onto the tree. The (id, sub_id) pair forms the
 * node's identity — typically id is a stable pointer (DObj*, GObj*, CObj*)
 * and sub_id distinguishes multiple matrix scopes within the same object. */
void FrameInterpolation_RecordOpenChild(const void *id, int sub_id);
void FrameInterpolation_RecordCloseChild(void);

/* -------------------------------------------------------------------------- */
/* Matrix-build ops — one per syMatrix*(Mtx*, ...) primitive in src/sys/matrix.c */
/* -------------------------------------------------------------------------- */

/* dest is Mtx* — the GBI matrix the result lands in. void* in the API to
 * keep the header game-type-agnostic. */

/* Direct fixed-point writes (no Mtx44f intermediate) */
void FrameInterpolation_RecordMatrixTra(void *dest, float x, float y, float z);
void FrameInterpolation_RecordMatrixSca(void *dest, float x, float y, float z);

/* Float-domain rotation builders — recorded so we can lerp the input angles
 * (shortest-arc) rather than lerp the resulting basis vectors element-wise.
 * The dest is the final Mtx* after the internal F2L. */
void FrameInterpolation_RecordMatrixRotR(void *dest, float a, float x, float y, float z);
void FrameInterpolation_RecordMatrixRotD(void *dest, float a, float x, float y, float z);
void FrameInterpolation_RecordMatrixRotRpyR(void *dest, float r, float p, float y);
void FrameInterpolation_RecordMatrixRotRpyD(void *dest, float r, float p, float y);
void FrameInterpolation_RecordMatrixRotPyrR(void *dest, float r, float p, float y);
void FrameInterpolation_RecordMatrixRotPyR(void *dest, float p, float y);
void FrameInterpolation_RecordMatrixRotRpR(void *dest, float r, float p);
void FrameInterpolation_RecordMatrixRotYawR(void *dest, float y);
void FrameInterpolation_RecordMatrixRotPitchR(void *dest, float p);

void FrameInterpolation_RecordMatrixTraRotR(void *dest, float tx, float ty, float tz, float a, float rx, float ry, float rz);
void FrameInterpolation_RecordMatrixTraRotD(void *dest, float tx, float ty, float tz, float a, float rx, float ry, float rz);
void FrameInterpolation_RecordMatrixTraRotRSca(void *dest, float tx, float ty, float tz, float a, float rx, float ry, float rz, float sx, float sy, float sz);
void FrameInterpolation_RecordMatrixTraRotRpyR(void *dest, float tx, float ty, float tz, float r, float p, float y);
void FrameInterpolation_RecordMatrixTraRotRpyD(void *dest, float tx, float ty, float tz, float r, float p, float y);
void FrameInterpolation_RecordMatrixTraRotRpyRSca(void *dest, float tx, float ty, float tz, float r, float p, float y, float sx, float sy, float sz);
void FrameInterpolation_RecordMatrixTraRotPyrR(void *dest, float tx, float ty, float tz, float r, float p, float y);
void FrameInterpolation_RecordMatrixTraRotPyrRSca(void *dest, float tx, float ty, float tz, float r, float p, float y, float sx, float sy, float sz);
void FrameInterpolation_RecordMatrixTraRotPyR(void *dest, float tx, float ty, float tz, float p, float y);
void FrameInterpolation_RecordMatrixTraRotRpR(void *dest, float tx, float ty, float tz, float r, float p);
void FrameInterpolation_RecordMatrixTraRotYawR(void *dest, float tx, float ty, float tz, float y);
void FrameInterpolation_RecordMatrixTraRotPitchR(void *dest, float tx, float ty, float tz, float p);

/* Catch-all: snapshot a Mtx44f source and the Mtx* destination at F2L time.
 * Used for non-primitive composition paths. Interpolation lerps the source
 * matrix elements; this is incorrect for view/rotation matrices because the
 * lerp of two rotation bases isn't itself a rotation. Prefer RecordCamera()
 * or input-domain primitives whenever the inputs are available. */
void FrameInterpolation_RecordMatrixF2L(const void *src_mtx44f, void *dest);
void FrameInterpolation_RecordMatrixF2LFixedW(const void *src_mtx44f, void *dest);

/* Camera composite — input-domain record for view*projection matrices.
 *
 * Captures the eye/at/up vectors and perspective parameters that produced
 * the final GBI projection matrix. At lerp time we lerp the *inputs*
 * (eye/at/up linearly, perspective params linearly), rebuild lookat_F and
 * persp_F, multiply them, and write the result as the replacement MtxF.
 *
 * Compared to RecordMatrixF2L on the composite, this preserves rigid-body
 * motion of the camera — no warping, no doubling artefacts when the camera
 * is panning or dollying. */
void FrameInterpolation_RecordCamera(void *dest,
    float ex, float ey, float ez,
    float ax, float ay, float az,
    float ux, float uy, float uz,
    float fovy, float aspect, float znear, float zfar, float scale);

/* -------------------------------------------------------------------------- */
/* Diagnostics                                                                */
/* -------------------------------------------------------------------------- */

/* Returns counts from the most recent finished recording. For self-tests. */
int  FrameInterpolation_GetLastOpCount(void);
int  FrameInterpolation_GetLastChildCount(void);

/* Self-test hooks (see frame_interpolation_selftest.cpp).
 *
 * RunSelfTestIfRequested: invoke at boot. If env SSB64_FRAME_INTERP_UNITTEST
 * is set, runs offline unit tests against the recording API and exits with
 * status 2 on failure (so CI catches breakage).
 *
 * TelemetryTick: invoke once per game frame. If env SSB64_FRAME_INTERP_TELEMETRY
 * is set, logs op/child counts every ~60 ticks. No-op otherwise. */
void FrameInterpolation_RunSelfTestIfRequested(void);
void FrameInterpolation_TelemetryTick(void);

#ifdef __cplusplus
} // extern "C"

/* C++-only: produce the matrix-replacement map for an intermediate frame.
 * t in [0, 1] where 0 = previous game tick, 1 = current game tick.
 *
 * Self-test: at t = 1.0 the returned MtxF for any matrix should match the
 * value that the game's *real* Mtx* would unpack to (modulo fixed-point
 * round-trip), because the inputs being replayed are the current frame's
 * inputs. The driver should opt out of replacement on the t=1 frame for
 * efficiency and to keep this property bit-exact. */
std::unordered_map<Mtx*, MtxF> FrameInterpolation_Interpolate(float t);

#endif
