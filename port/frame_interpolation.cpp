/**
 * frame_interpolation.cpp — see frame_interpolation.h for design notes.
 *
 * Two recordings are kept: previous_recording (frame N-1) and current_recording
 * (frame N being built). A recording is a tree of Path nodes; each Path has:
 *   - children: map<(stable_ptr, sub_id), vector<Path>>  // multiple instances OK
 *   - ops:      ordered list of typed matrix-build records
 *   - items:    interleaving order of children-vs-ops so replay is deterministic
 *
 * At interpolation time, walk both trees pairwise. For each leaf op pair, lerp
 * the inputs (shortest-arc for angles, linear for translates/scales), invoke
 * the corresponding *F builder against a temporary Mtx44f, and stash the
 * result in the replacement map keyed by the *new* frame's destination Mtx*.
 *
 * If the old tree has no matching node at a given position, the new op is
 * replayed at t=1 (no interpolation). This handles spawning actors gracefully.
 */

#include "frame_interpolation.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <cstring>
#include <cmath>
#include <cstdint>

#include <fast/types.h>  /* MtxF, Mtx (libultraship side) */

/* The SSB64 *F matrix builders we replay during interpolation. These are
 * defined in src/sys/matrix.c and produce a Mtx44f (= float[4][4]) which has
 * the same byte layout as MtxF::mf. We declare the prototypes locally with C
 * linkage so we don't have to pull the whole game's header soup into this TU. */
extern "C" {
    typedef float Mtx44f_t[4][4];
    /* LookAt is the GBI lighting struct used by syMatrixLookAtReflectF; for
     * matrix-only reconstruction we use syMatrixLookAtF (no LookAt out param)
     * which produces the same matrix bytes. */

    void syMatrixScaF(Mtx44f_t *mf, float x, float y, float z);
    void syMatrixTraF(Mtx44f_t *mf, float x, float y, float z);

    /* Camera reconstruction at lerp time. */
    void syMatrixLookAtF(Mtx44f_t *mf,
        float eye_x, float eye_y, float eye_z,
        float at_x, float at_y, float at_z,
        float up_x, float up_y, float up_z);
    void syMatrixPerspFastF(Mtx44f_t mf, unsigned short *persp_norm,
        float fovy, float aspect, float near_, float far_, float scale);

    void syMatrixRotRF(Mtx44f_t *mf, float a, float x, float y, float z);
    void syMatrixRotDF(Mtx44f_t *mf, float a, float x, float y, float z);
    void syMatrixRotRpyRF(Mtx44f_t *mf, float r, float p, float y);
    void syMatrixRotRpyDF(Mtx44f_t *mf, float r, float p, float y);
    void syMatrixRotPyrRF(Mtx44f_t *mf, float r, float p, float y);
    void syMatrixRotPyRF(Mtx44f_t *mf, float p, float y);
    void syMatrixRotRpRF(Mtx44f_t *mf, float r, float p);
    void syMatrixRotYawRF(Mtx44f_t *mf, float y);
    void syMatrixRotPitchRF(Mtx44f_t *mf, float p);

    void syMatrixTraRotRF(Mtx44f_t *mf, float tx, float ty, float tz, float angle, float rx, float ry, float rz);
    void syMatrixTraRotDF(Mtx44f_t *mf, float tx, float ty, float tz, float angle, float rx, float ry, float rz);
    void syMatrixTraRotRScaF(Mtx44f_t *mf, float tx, float ty, float tz, float angle, float rx, float ry, float rz, float sx, float sy, float sz);
    void syMatrixTraRotRpyRF(Mtx44f_t *mf, float tx, float ty, float tz, float r, float p, float y);
    void syMatrixTraRotRpyDF(Mtx44f_t *mf, float tx, float ty, float tz, float r, float p, float y);
    void syMatrixTraRotRpyRScaF(Mtx44f_t *mf, float tx, float ty, float tz, float r, float p, float y, float sx, float sy, float sz);
    void syMatrixTraRotPyrRF(Mtx44f_t *mf, float tx, float ty, float tz, float r, float p, float y);
    void syMatrixTraRotPyrRScaF(Mtx44f_t *mf, float tx, float ty, float tz, float r, float p, float y, float sx, float sy, float sz);
    void syMatrixTraRotPyRF(Mtx44f_t *mf, float tx, float ty, float tz, float p, float y);
    void syMatrixTraRotRpRF(Mtx44f_t *mf, float tx, float ty, float tz, float r, float p);
    void syMatrixTraRotYawRF(Mtx44f_t *mf, float tx, float ty, float tz, float y);
    void syMatrixTraRotPitchRF(Mtx44f_t *mf, float tx, float ty, float tz, float p);
}

namespace {

enum class Op : uint8_t {
    OpenChild,
    CloseChild,

    Tra, Sca,
    RotR, RotD,
    RotRpyR, RotRpyD,
    RotPyrR, RotPyR, RotRpR, RotYawR, RotPitchR,
    TraRotR, TraRotD, TraRotRSca,
    TraRotRpyR, TraRotRpyD, TraRotRpyRSca,
    TraRotPyrR, TraRotPyrRSca,
    TraRotPyR, TraRotRpR, TraRotYawR, TraRotPitchR,

    F2L, F2LFixedW,

    /* Composite camera op: stored inputs are
     *   in[0..2]   eye_xyz
     *   in[3..5]   at_xyz
     *   in[6..8]   up_xyz
     *   in[9..13]  fovy, aspect, znear, zfar, scale
     * 14 slots total — see in[14] sizing. */
    Camera,
};

/* All op payloads share a destination pointer (Mtx* cast to void*) and a
 * variable-size float input pack. We use a flat 16-float buffer plus an int
 * count so we don't need per-op POD types. */
struct OpData {
    Op op;
    void* dest;          /* Mtx* destination (Op-specific; null for OpenChild/CloseChild) */
    /* For OpenChild, child_key/child_idx label this node in the parent's children map. */
    const void* child_key_ptr;
    int   child_key_sub;
    int   child_idx;     /* index within parent->children[key] */
    /* Inputs: at most 14 floats (Camera op uses 9 vector + 5 persp). */
    float in[14];
    /* For F2L ops: 16 floats holding the source Mtx44f. */
    float mtx[16];
};

struct Path {
    /* children grouped by (key,sub) label; each may have multiple instances
     * (e.g. an actor that pushes the same scope twice in one frame). */
    std::map<std::pair<const void*, int>, std::vector<Path>> children;
    /* All ops/children in order, so replay matches the recording's sequence. */
    std::vector<OpData> items;
};

struct Recording {
    Path root;
};

bool g_is_recording = false;
bool g_dont_interp_camera = false;
Recording g_current;
Recording g_previous;
std::vector<Path*> g_path_stack;
int g_last_op_count = 0;
int g_last_child_count = 0;

inline OpData& append_op(Op op, void* dest) {
    Path* p = g_path_stack.back();
    p->items.emplace_back();
    OpData& d = p->items.back();
    std::memset(&d, 0, sizeof(d));
    d.op = op;
    d.dest = dest;
    return d;
}

/* -------------------------------------------------------------------------- */
/* Lerp helpers                                                               */
/* -------------------------------------------------------------------------- */

inline float lerp(float a, float b, float t) { return a + (b - a) * t; }

/* Shortest-arc lerp on a radian angle.  If the difference is more than ~PI/2
 * it's almost certainly a snap (teleport, animation cut), so we hold the new
 * value to avoid swirly midpoints. */
inline float lerp_angle_rad(float a, float b, float t) {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;
    float d = std::fmod(b - a, TWO_PI);
    if (d > PI)        d -= TWO_PI;
    else if (d < -PI)  d += TWO_PI;
    if (std::fabs(d) > PI * 0.5f) return b;  /* snap */
    return a + d * t;
}

inline float lerp_angle_deg(float a, float b, float t) {
    float d = std::fmod(b - a, 360.0f);
    if (d > 180.0f)        d -= 360.0f;
    else if (d < -180.0f)  d += 360.0f;
    if (std::fabs(d) > 90.0f) return b;
    return a + d * t;
}

/* -------------------------------------------------------------------------- */
/* Replay one op into the replacement map at lerp factor t                    */
/* -------------------------------------------------------------------------- */

inline void store_mtx44f(std::unordered_map<Mtx*, MtxF>& out, void* dest, const Mtx44f_t& mf) {
    if (dest == nullptr) return;
    MtxF& slot = out[reinterpret_cast<Mtx*>(dest)];
    std::memcpy(slot.mf, mf, sizeof(Mtx44f_t));
}

void replay_op(std::unordered_map<Mtx*, MtxF>& out,
               const OpData& oldOp, const OpData& newOp, float t) {
    Mtx44f_t mf;
    /* For ops with float inputs, lerp old.in[*] with new.in[*]; for F2L ops,
     * lerp old.mtx[*] with new.mtx[*] element-wise. */
    auto L = [&](int i) { return lerp(oldOp.in[i], newOp.in[i], t); };
    auto LR = [&](int i) { return lerp_angle_rad(oldOp.in[i], newOp.in[i], t); };
    auto LD = [&](int i) { return lerp_angle_deg(oldOp.in[i], newOp.in[i], t); };

    switch (newOp.op) {
        case Op::Tra:
            syMatrixTraF(&mf, L(0), L(1), L(2));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::Sca:
            syMatrixScaF(&mf, L(0), L(1), L(2));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotR:
            syMatrixRotRF(&mf, LR(0), L(1), L(2), L(3));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotD:
            syMatrixRotDF(&mf, LD(0), L(1), L(2), L(3));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotRpyR:
            syMatrixRotRpyRF(&mf, LR(0), LR(1), LR(2));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotRpyD:
            syMatrixRotRpyDF(&mf, LD(0), LD(1), LD(2));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotPyrR:
            syMatrixRotPyrRF(&mf, LR(0), LR(1), LR(2));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotPyR:
            syMatrixRotPyRF(&mf, LR(0), LR(1));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotRpR:
            syMatrixRotRpRF(&mf, LR(0), LR(1));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotYawR:
            syMatrixRotYawRF(&mf, LR(0));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::RotPitchR:
            syMatrixRotPitchRF(&mf, LR(0));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotR:
            syMatrixTraRotRF(&mf, L(0), L(1), L(2), LR(3), L(4), L(5), L(6));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotD:
            syMatrixTraRotDF(&mf, L(0), L(1), L(2), LD(3), L(4), L(5), L(6));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotRSca:
            syMatrixTraRotRScaF(&mf, L(0), L(1), L(2), LR(3), L(4), L(5), L(6), L(7), L(8), L(9));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotRpyR:
            syMatrixTraRotRpyRF(&mf, L(0), L(1), L(2), LR(3), LR(4), LR(5));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotRpyD:
            syMatrixTraRotRpyDF(&mf, L(0), L(1), L(2), LD(3), LD(4), LD(5));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotRpyRSca:
            syMatrixTraRotRpyRScaF(&mf, L(0), L(1), L(2), LR(3), LR(4), LR(5), L(6), L(7), L(8));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotPyrR:
            syMatrixTraRotPyrRF(&mf, L(0), L(1), L(2), LR(3), LR(4), LR(5));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotPyrRSca:
            syMatrixTraRotPyrRScaF(&mf, L(0), L(1), L(2), LR(3), LR(4), LR(5), L(6), L(7), L(8));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotPyR:
            syMatrixTraRotPyRF(&mf, L(0), L(1), L(2), LR(3), LR(4));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotRpR:
            syMatrixTraRotRpRF(&mf, L(0), L(1), L(2), LR(3), LR(4));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotYawR:
            syMatrixTraRotYawRF(&mf, L(0), L(1), L(2), LR(3));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::TraRotPitchR:
            syMatrixTraRotPitchRF(&mf, L(0), L(1), L(2), LR(3));
            store_mtx44f(out, newOp.dest, mf);
            break;
        case Op::F2L:
        case Op::F2LFixedW: {
            for (int i = 0; i < 16; i++) {
                reinterpret_cast<float*>(mf)[i] = lerp(oldOp.mtx[i], newOp.mtx[i], t);
            }
            store_mtx44f(out, newOp.dest, mf);
            break;
        }
        case Op::Camera: {
            /* Lerp inputs in their original domains (linear for everything;
             * eye/at/up moves are not angular, even when the camera orbits).
             * For the rare hard cut, the calling code calls
             * FrameInterpolation_DontInterpolateCamera() and t is clamped to 1
             * so we degrade to "snap to current frame". */
            float ex = lerp(oldOp.in[0], newOp.in[0], t);
            float ey = lerp(oldOp.in[1], newOp.in[1], t);
            float ez = lerp(oldOp.in[2], newOp.in[2], t);
            float ax = lerp(oldOp.in[3], newOp.in[3], t);
            float ay = lerp(oldOp.in[4], newOp.in[4], t);
            float az = lerp(oldOp.in[5], newOp.in[5], t);
            float ux = lerp(oldOp.in[6], newOp.in[6], t);
            float uy = lerp(oldOp.in[7], newOp.in[7], t);
            float uz = lerp(oldOp.in[8], newOp.in[8], t);
            float fovy   = lerp(oldOp.in[9],  newOp.in[9],  t);
            float aspect = lerp(oldOp.in[10], newOp.in[10], t);
            float znear  = lerp(oldOp.in[11], newOp.in[11], t);
            float zfar   = lerp(oldOp.in[12], newOp.in[12], t);
            float scale  = lerp(oldOp.in[13], newOp.in[13], t);

            Mtx44f_t persp;
            Mtx44f_t view;
            unsigned short dummy_norm = 0;
            syMatrixPerspFastF(persp, &dummy_norm, fovy, aspect, znear, zfar, scale);
            syMatrixLookAtF(&view, ex, ey, ez, ax, ay, az, ux, uy, uz);

            /* composite = view * persp (matches gmcamera.c's guMtxCatF order). */
            Mtx44f_t composite;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    float s = 0.0f;
                    for (int k = 0; k < 4; k++) {
                        s += view[i][k] * persp[k][j];
                    }
                    composite[i][j] = s;
                }
            }
            store_mtx44f(out, newOp.dest, composite);
            break;
        }
        case Op::OpenChild:
        case Op::CloseChild:
            /* not a leaf */
            break;
    }
}

/* Replay newOp at t=1 (no old counterpart available — fresh actor). */
void replay_op_solo(std::unordered_map<Mtx*, MtxF>& out, const OpData& newOp) {
    /* Use the same op data as both old and new; t=1 collapses to new's inputs. */
    replay_op(out, newOp, newOp, 1.0f);
}

/* -------------------------------------------------------------------------- */
/* Tree walk                                                                  */
/* -------------------------------------------------------------------------- */

void interpolate_branch(std::unordered_map<Mtx*, MtxF>& out,
                        const Path* old_path, const Path* new_path, float t) {
    /* Walk new_path's items and look for the same op at the same position in
     * old_path's items. If positions match and ops match, lerp; otherwise
     * replay solo at t=1. */
    const auto& newItems = new_path->items;
    const auto& oldItems = old_path ? old_path->items : decltype(new_path->items){};

    /* Track per-(key,sub) child instance counter so OpenChild dispatches into
     * the right vector slot. */
    std::map<std::pair<const void*, int>, size_t> new_child_cursor;

    for (size_t i = 0; i < newItems.size(); i++) {
        const OpData& n = newItems[i];

        if (n.op == Op::OpenChild) {
            auto key = std::make_pair(n.child_key_ptr, n.child_key_sub);
            size_t idx = new_child_cursor[key]++;
            const Path* new_child = nullptr;
            const Path* old_child = nullptr;
            if (auto it = new_path->children.find(key);
                it != new_path->children.end() && idx < it->second.size()) {
                new_child = &it->second[idx];
            }
            if (old_path) {
                if (auto it = old_path->children.find(key);
                    it != old_path->children.end() && idx < it->second.size()) {
                    old_child = &it->second[idx];
                }
            }
            if (new_child) {
                interpolate_branch(out, old_child, new_child, t);
            }
            /* CloseChild for this OpenChild appears later in items; we skip it
             * because the recursive call drained the child's items already. */
            continue;
        }
        if (n.op == Op::CloseChild) {
            continue;
        }

        /* Try to find the same op at the same items[] index in the old recording. */
        bool matched = false;
        if (i < oldItems.size() && oldItems[i].op == n.op && oldItems[i].dest != nullptr) {
            replay_op(out, oldItems[i], n, t);
            matched = true;
        }
        if (!matched) {
            replay_op_solo(out, n);
        }
    }
}

} /* namespace */

/* -------------------------------------------------------------------------- */
/* C API implementation                                                       */
/* -------------------------------------------------------------------------- */

extern "C" {

void FrameInterpolation_StartRecord(void) {
    g_previous = std::move(g_current);
    g_current = Recording{};
    g_path_stack.clear();
    g_path_stack.push_back(&g_current.root);
    g_dont_interp_camera = false;
    g_is_recording = true;
}

void FrameInterpolation_StopRecord(void) {
    g_is_recording = false;
    /* Tally for diagnostics. */
    int ops = 0, children = 0;
    /* Iterative tree walk. */
    std::vector<const Path*> stack;
    stack.push_back(&g_current.root);
    while (!stack.empty()) {
        const Path* p = stack.back();
        stack.pop_back();
        for (const auto& kv : p->children) {
            for (const auto& c : kv.second) {
                children++;
                stack.push_back(&c);
            }
        }
        for (const auto& it : p->items) {
            if (it.op != Op::OpenChild && it.op != Op::CloseChild) ops++;
        }
    }
    g_last_op_count = ops;
    g_last_child_count = children;
}

void FrameInterpolation_DontInterpolateCamera(void) {
    g_dont_interp_camera = true;
}

int FrameInterpolation_IsRecording(void) {
    return g_is_recording ? 1 : 0;
}

int FrameInterpolation_GetLastOpCount(void)    { return g_last_op_count; }
int FrameInterpolation_GetLastChildCount(void) { return g_last_child_count; }

void FrameInterpolation_RecordOpenChild(const void *id, int sub_id) {
    if (!g_is_recording) return;
    auto key = std::make_pair(id, sub_id);
    Path* parent = g_path_stack.back();
    auto& vec = parent->children[key];
    vec.emplace_back();
    Path* child = &vec.back();

    /* Append a marker into items so replay can dispatch into this child at
     * the right interleaving point. */
    OpData& d = append_op(Op::OpenChild, nullptr);
    d.child_key_ptr = id;
    d.child_key_sub = sub_id;
    d.child_idx     = static_cast<int>(vec.size() - 1);

    g_path_stack.push_back(child);
}

void FrameInterpolation_RecordCloseChild(void) {
    if (!g_is_recording) return;
    if (g_path_stack.size() <= 1) return; /* unbalanced — drop on the floor */
    g_path_stack.pop_back();
    /* Marker in parent for replay-time book-keeping (currently unused but
     * cheap to keep, and forms a sanity check). */
    append_op(Op::CloseChild, nullptr);
}

/* Record helpers — pack the inputs and stash dest. */

#define REC1(OP, ...)                                                    \
    do {                                                                 \
        if (!g_is_recording) return;                                     \
        OpData& d = append_op(OP, dest);                                 \
        float vals[] = { __VA_ARGS__ };                                  \
        for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); i++) {      \
            d.in[i] = vals[i];                                           \
        }                                                                \
    } while (0)

void FrameInterpolation_RecordMatrixTra(void *dest, float x, float y, float z) { REC1(Op::Tra, x, y, z); }
void FrameInterpolation_RecordMatrixSca(void *dest, float x, float y, float z) { REC1(Op::Sca, x, y, z); }

void FrameInterpolation_RecordMatrixRotR(void *dest, float a, float x, float y, float z) { REC1(Op::RotR, a, x, y, z); }
void FrameInterpolation_RecordMatrixRotD(void *dest, float a, float x, float y, float z) { REC1(Op::RotD, a, x, y, z); }
void FrameInterpolation_RecordMatrixRotRpyR(void *dest, float r, float p, float y) { REC1(Op::RotRpyR, r, p, y); }
void FrameInterpolation_RecordMatrixRotRpyD(void *dest, float r, float p, float y) { REC1(Op::RotRpyD, r, p, y); }
void FrameInterpolation_RecordMatrixRotPyrR(void *dest, float r, float p, float y) { REC1(Op::RotPyrR, r, p, y); }
void FrameInterpolation_RecordMatrixRotPyR(void *dest, float p, float y)           { REC1(Op::RotPyR, p, y); }
void FrameInterpolation_RecordMatrixRotRpR(void *dest, float r, float p)           { REC1(Op::RotRpR, r, p); }
void FrameInterpolation_RecordMatrixRotYawR(void *dest, float y)                   { REC1(Op::RotYawR, y); }
void FrameInterpolation_RecordMatrixRotPitchR(void *dest, float p)                 { REC1(Op::RotPitchR, p); }

void FrameInterpolation_RecordMatrixTraRotR(void *dest, float tx, float ty, float tz, float a, float rx, float ry, float rz) {
    REC1(Op::TraRotR, tx, ty, tz, a, rx, ry, rz);
}
void FrameInterpolation_RecordMatrixTraRotD(void *dest, float tx, float ty, float tz, float a, float rx, float ry, float rz) {
    REC1(Op::TraRotD, tx, ty, tz, a, rx, ry, rz);
}
void FrameInterpolation_RecordMatrixTraRotRSca(void *dest, float tx, float ty, float tz, float a, float rx, float ry, float rz, float sx, float sy, float sz) {
    REC1(Op::TraRotRSca, tx, ty, tz, a, rx, ry, rz, sx, sy, sz);
}
void FrameInterpolation_RecordMatrixTraRotRpyR(void *dest, float tx, float ty, float tz, float r, float p, float y) {
    REC1(Op::TraRotRpyR, tx, ty, tz, r, p, y);
}
void FrameInterpolation_RecordMatrixTraRotRpyD(void *dest, float tx, float ty, float tz, float r, float p, float y) {
    REC1(Op::TraRotRpyD, tx, ty, tz, r, p, y);
}
void FrameInterpolation_RecordMatrixTraRotRpyRSca(void *dest, float tx, float ty, float tz, float r, float p, float y, float sx, float sy, float sz) {
    REC1(Op::TraRotRpyRSca, tx, ty, tz, r, p, y, sx, sy, sz);
}
void FrameInterpolation_RecordMatrixTraRotPyrR(void *dest, float tx, float ty, float tz, float r, float p, float y) {
    REC1(Op::TraRotPyrR, tx, ty, tz, r, p, y);
}
void FrameInterpolation_RecordMatrixTraRotPyrRSca(void *dest, float tx, float ty, float tz, float r, float p, float y, float sx, float sy, float sz) {
    REC1(Op::TraRotPyrRSca, tx, ty, tz, r, p, y, sx, sy, sz);
}
void FrameInterpolation_RecordMatrixTraRotPyR(void *dest, float tx, float ty, float tz, float p, float y) {
    REC1(Op::TraRotPyR, tx, ty, tz, p, y);
}
void FrameInterpolation_RecordMatrixTraRotRpR(void *dest, float tx, float ty, float tz, float r, float p) {
    REC1(Op::TraRotRpR, tx, ty, tz, r, p);
}
void FrameInterpolation_RecordMatrixTraRotYawR(void *dest, float tx, float ty, float tz, float y) {
    REC1(Op::TraRotYawR, tx, ty, tz, y);
}
void FrameInterpolation_RecordMatrixTraRotPitchR(void *dest, float tx, float ty, float tz, float p) {
    REC1(Op::TraRotPitchR, tx, ty, tz, p);
}

#undef REC1

static void rec_f2l_impl(Op op, const void *src_mtx44f, void *dest) {
    if (!g_is_recording) return;
    OpData& d = append_op(op, dest);
    /* Mtx44f is float[4][4] = 16 floats, same layout as MtxF::mf. */
    if (src_mtx44f != nullptr) {
        std::memcpy(d.mtx, src_mtx44f, sizeof(d.mtx));
    }
}
void FrameInterpolation_RecordMatrixF2L(const void *src, void *dest)        { rec_f2l_impl(Op::F2L, src, dest); }
void FrameInterpolation_RecordMatrixF2LFixedW(const void *src, void *dest)  { rec_f2l_impl(Op::F2LFixedW, src, dest); }

void FrameInterpolation_RecordCamera(void *dest,
    float ex, float ey, float ez,
    float ax, float ay, float az,
    float ux, float uy, float uz,
    float fovy, float aspect, float znear, float zfar, float scale)
{
    if (!g_is_recording) return;
    OpData& d = append_op(Op::Camera, dest);
    d.in[0]  = ex;  d.in[1]  = ey;  d.in[2]  = ez;
    d.in[3]  = ax;  d.in[4]  = ay;  d.in[5]  = az;
    d.in[6]  = ux;  d.in[7]  = uy;  d.in[8]  = uz;
    d.in[9]  = fovy;
    d.in[10] = aspect;
    d.in[11] = znear;
    d.in[12] = zfar;
    d.in[13] = scale;
}

} /* extern "C" */

/* -------------------------------------------------------------------------- */
/* C++ API                                                                    */
/* -------------------------------------------------------------------------- */

std::unordered_map<Mtx*, MtxF> FrameInterpolation_Interpolate(float t) {
    std::unordered_map<Mtx*, MtxF> out;
    if (g_dont_interp_camera) {
        /* Caller's responsibility to map specific Mtx* to identity replacement
         * if needed; for v1 this flag just disables the global lerp by
         * clamping t to 1 (no interpolation = current frame). */
        t = 1.0f;
    }
    interpolate_branch(out, &g_previous.root, &g_current.root, t);
    return out;
}
