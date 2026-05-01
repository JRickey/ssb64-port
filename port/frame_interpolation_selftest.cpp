/**
 * frame_interpolation_selftest.cpp — sanity tests for the recording/lerp layer.
 *
 * Two self-test modes:
 *
 *   SSB64_FRAME_INTERP_UNITTEST=1
 *     Run unit tests at boot (before the game starts). They exercise the
 *     recording API in isolation and assert that lerp produces the expected
 *     interpolated matrices. Logs PASS/FAIL to port_log; on FAIL, the process
 *     exits with status 2 so CI can detect breakage.
 *
 *   SSB64_FRAME_INTERP_TELEMETRY=1
 *     Emit per-second telemetry (op count, child count, last replacement-map
 *     size) to port_log so a human can sanity-check that recording is live
 *     when the game is running. Lightweight (one log line per ~60 frames).
 *
 * The tests are written to be hermetic: they don't touch the game's display
 * list or the renderer. They invoke FrameInterpolation_* directly with stub
 * matrix pointers and verify the post-Interpolate replacement map.
 *
 * Why this lives in its own TU: the unit-test code references syMatrix*F
 * builders to compute expected reference matrices. Linking them in is fine
 * for any TU that goes into the final binary, but we don't want to bloat
 * frame_interpolation.cpp's symbol surface for non-test builds.
 */

#include "frame_interpolation.h"
#include "port_log.h"

#include <fast/types.h>      /* MtxF */

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>

extern "C" {
    typedef float Mtx44f_t[4][4];
    void syMatrixTraF(Mtx44f_t *mf, float x, float y, float z);
}

namespace {

bool g_failed = false;
int  g_pass = 0;
int  g_fail = 0;

#define EXPECT(cond, ...)                                                  \
    do {                                                                   \
        if (!(cond)) {                                                     \
            g_failed = true;                                               \
            g_fail++;                                                      \
            char _buf[512];                                                \
            std::snprintf(_buf, sizeof(_buf), __VA_ARGS__);                \
            port_log("SSB64: FRAME_INTERP_SELFTEST FAIL [%s:%d] %s: %s\n", \
                __FILE__, __LINE__, #cond, _buf);                          \
        } else {                                                           \
            g_pass++;                                                      \
        }                                                                  \
    } while (0)

bool feq(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) <= eps;
}

/* ------------------------------------------------------------------------ */
/* Test 1 — recording structure: op count and child count                   */
/* ------------------------------------------------------------------------ */

void test_record_counts() {
    /* dummy Mtx storage; the API only needs a stable pointer */
    static int dummy_mtx_a, dummy_mtx_b, dummy_mtx_c;

    FrameInterpolation_StartRecord();

    /* 2 children, 3 ops total (1 in root, 2 inside children) */
    FrameInterpolation_RecordMatrixTra(&dummy_mtx_a, 1.0f, 2.0f, 3.0f);

    FrameInterpolation_RecordOpenChild((const void *)0x1, 0);
        FrameInterpolation_RecordMatrixTra(&dummy_mtx_b, 4.0f, 5.0f, 6.0f);
    FrameInterpolation_RecordCloseChild();

    FrameInterpolation_RecordOpenChild((const void *)0x2, 0);
        FrameInterpolation_RecordMatrixTra(&dummy_mtx_c, 7.0f, 8.0f, 9.0f);
    FrameInterpolation_RecordCloseChild();

    FrameInterpolation_StopRecord();

    EXPECT(FrameInterpolation_GetLastOpCount() == 3,
        "expected 3 ops, got %d", FrameInterpolation_GetLastOpCount());
    EXPECT(FrameInterpolation_GetLastChildCount() == 2,
        "expected 2 children, got %d", FrameInterpolation_GetLastChildCount());
}

/* ------------------------------------------------------------------------ */
/* Test 2 — identity lerp: same inputs both frames -> same output           */
/* ------------------------------------------------------------------------ */

void test_identity_lerp() {
    static int dest_storage;
    Mtx *dest = reinterpret_cast<Mtx *>(&dest_storage);

    /* Frame N-1 */
    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixTra(dest, 5.0f, 7.0f, 11.0f);
    FrameInterpolation_StopRecord();

    /* Frame N — identical inputs, same dest */
    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixTra(dest, 5.0f, 7.0f, 11.0f);
    FrameInterpolation_StopRecord();

    auto repl = FrameInterpolation_Interpolate(0.5f);
    EXPECT(repl.size() == 1, "expected 1 replacement, got %zu", repl.size());
    auto it = repl.find(dest);
    EXPECT(it != repl.end(), "destination not found in replacement map");
    if (it != repl.end()) {
        const float *mf = &it->second.mf[0][0];
        /* Reference: build the identity translate matrix directly. */
        Mtx44f_t ref;
        syMatrixTraF(&ref, 5.0f, 7.0f, 11.0f);
        for (int i = 0; i < 16; i++) {
            EXPECT(feq(mf[i], reinterpret_cast<float *>(ref)[i]),
                "identity lerp mismatch at element %d: got %f, expected %f",
                i, mf[i], reinterpret_cast<float *>(ref)[i]);
        }
    }
}

/* ------------------------------------------------------------------------ */
/* Test 3 — t=0.5 lerp between two distinct translations                    */
/* ------------------------------------------------------------------------ */

void test_midpoint_lerp() {
    static int dest_storage;
    Mtx *dest = reinterpret_cast<Mtx *>(&dest_storage);

    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixTra(dest, 0.0f, 0.0f, 0.0f);
    FrameInterpolation_StopRecord();

    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixTra(dest, 10.0f, 20.0f, 40.0f);
    FrameInterpolation_StopRecord();

    auto repl = FrameInterpolation_Interpolate(0.5f);
    auto it = repl.find(dest);
    EXPECT(it != repl.end(), "destination not in replacement map");
    if (it != repl.end()) {
        /* Translate row in syMatrixTraF lives at mf[3][0..2]. */
        EXPECT(feq(it->second.mf[3][0],  5.0f), "tx midpoint: got %f", it->second.mf[3][0]);
        EXPECT(feq(it->second.mf[3][1], 10.0f), "ty midpoint: got %f", it->second.mf[3][1]);
        EXPECT(feq(it->second.mf[3][2], 20.0f), "tz midpoint: got %f", it->second.mf[3][2]);
        EXPECT(feq(it->second.mf[3][3],  1.0f), "homogeneous w must be 1: got %f", it->second.mf[3][3]);
        /* And the upper-left should be identity (no rotation/scale). */
        EXPECT(feq(it->second.mf[0][0], 1.0f), "diagonal [0,0]");
        EXPECT(feq(it->second.mf[1][1], 1.0f), "diagonal [1,1]");
        EXPECT(feq(it->second.mf[2][2], 1.0f), "diagonal [2,2]");
        EXPECT(feq(it->second.mf[0][1], 0.0f), "off-diagonal [0,1]");
    }
}

/* ------------------------------------------------------------------------ */
/* Test 4 — t=1.0 fidelity: replacement matches new frame's matrix exactly  */
/* ------------------------------------------------------------------------ */

void test_endpoint_fidelity() {
    static int dest_storage;
    Mtx *dest = reinterpret_cast<Mtx *>(&dest_storage);

    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixTra(dest, 1.0f, 1.0f, 1.0f);
    FrameInterpolation_StopRecord();

    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixTra(dest, 2.0f, 4.0f, 8.0f);
    FrameInterpolation_StopRecord();

    auto repl = FrameInterpolation_Interpolate(1.0f);
    auto it = repl.find(dest);
    EXPECT(it != repl.end(), "dest not in replacement map");
    if (it != repl.end()) {
        Mtx44f_t ref;
        syMatrixTraF(&ref, 2.0f, 4.0f, 8.0f);
        const float *got = &it->second.mf[0][0];
        const float *exp = reinterpret_cast<float *>(ref);
        for (int i = 0; i < 16; i++) {
            EXPECT(feq(got[i], exp[i]),
                "t=1.0 fidelity element %d: got %f, expected %f", i, got[i], exp[i]);
        }
    }
}

/* ------------------------------------------------------------------------ */
/* Test 5 — angle wrap: lerp from 350° to 10° should go forward through 0°  */
/* ------------------------------------------------------------------------ */

void test_angle_wrap() {
    /* Use RotYawR which takes a single radian angle. */
    static int dest_storage;
    Mtx *dest = reinterpret_cast<Mtx *>(&dest_storage);

    const float TWO_PI = 6.28318530718f;

    /* 350° = 6.10865238... radians */
    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixRotYawR(dest, 6.108652f);
    FrameInterpolation_StopRecord();

    /* 10° = 0.17453292... radians */
    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordMatrixRotYawR(dest, 0.174533f);
    FrameInterpolation_StopRecord();

    auto repl = FrameInterpolation_Interpolate(0.5f);
    auto it = repl.find(dest);
    EXPECT(it != repl.end(), "dest not in replacement map");
    if (it != repl.end()) {
        /* Naive lerp would land near pi (~3.14), short-arc lerp lands near 0
         * (the matrix should be ~identity for yaw). cos(0)=1 should appear in
         * the [0][0] and [1][1] of a yaw matrix. */
        float c = it->second.mf[0][0];
        EXPECT(c > 0.95f,
            "short-arc lerp failed: cos(yaw_lerped) = %f, expected near 1.0 "
            "(naive lerp would give cos(pi) = -1)", c);
    }
    (void)TWO_PI;
}

/* ------------------------------------------------------------------------ */
/* Test 6 — mismatched op count: new actor appears in frame N               */
/* ------------------------------------------------------------------------ */

void test_actor_appears() {
    static int dest_a_storage, dest_b_storage;
    Mtx *dest_a = reinterpret_cast<Mtx *>(&dest_a_storage);
    Mtx *dest_b = reinterpret_cast<Mtx *>(&dest_b_storage);

    /* Frame N-1: only actor A */
    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordOpenChild((const void *)0xA, 0);
    FrameInterpolation_RecordMatrixTra(dest_a, 1.0f, 2.0f, 3.0f);
    FrameInterpolation_RecordCloseChild();
    FrameInterpolation_StopRecord();

    /* Frame N: actor A still there, actor B newly spawned */
    FrameInterpolation_StartRecord();
    FrameInterpolation_RecordOpenChild((const void *)0xA, 0);
    FrameInterpolation_RecordMatrixTra(dest_a, 5.0f, 6.0f, 7.0f);
    FrameInterpolation_RecordCloseChild();
    FrameInterpolation_RecordOpenChild((const void *)0xB, 0);
    FrameInterpolation_RecordMatrixTra(dest_b, 100.0f, 200.0f, 300.0f);
    FrameInterpolation_RecordCloseChild();
    FrameInterpolation_StopRecord();

    auto repl = FrameInterpolation_Interpolate(0.5f);
    EXPECT(repl.size() == 2, "expected 2 replacements, got %zu", repl.size());

    /* Actor A: should be midpoint of {1,2,3} and {5,6,7} = {3,4,5}. */
    auto it_a = repl.find(dest_a);
    EXPECT(it_a != repl.end(), "actor A not in map");
    if (it_a != repl.end()) {
        EXPECT(feq(it_a->second.mf[3][0], 3.0f), "A.tx: got %f", it_a->second.mf[3][0]);
        EXPECT(feq(it_a->second.mf[3][1], 4.0f), "A.ty: got %f", it_a->second.mf[3][1]);
    }
    /* Actor B: no old counterpart -> replay solo at t=1, so {100,200,300}. */
    auto it_b = repl.find(dest_b);
    EXPECT(it_b != repl.end(), "actor B not in map");
    if (it_b != repl.end()) {
        EXPECT(feq(it_b->second.mf[3][0], 100.0f), "B.tx (solo): got %f", it_b->second.mf[3][0]);
        EXPECT(feq(it_b->second.mf[3][1], 200.0f), "B.ty (solo): got %f", it_b->second.mf[3][1]);
    }
}

void run_all() {
    port_log("SSB64: FRAME_INTERP_SELFTEST starting unit tests\n");
    g_failed = false;
    g_pass = 0;
    g_fail = 0;

    test_record_counts();
    test_identity_lerp();
    test_midpoint_lerp();
    test_endpoint_fidelity();
    test_angle_wrap();
    test_actor_appears();

    port_log("SSB64: FRAME_INTERP_SELFTEST results: %d passed, %d failed\n",
        g_pass, g_fail);
}

} /* namespace */

extern "C" void FrameInterpolation_RunSelfTestIfRequested(void)
{
    const char *e = std::getenv("SSB64_FRAME_INTERP_UNITTEST");
    if (e == nullptr || e[0] == '\0' || e[0] == '0') {
        return;
    }
    run_all();
    if (g_failed) {
        port_log("SSB64: SELFTEST failed -- exiting with status 2\n");
        port_log_close();
        std::exit(2);
    }
    port_log("SSB64: SELFTEST passed\n");
}

/* Lightweight per-frame telemetry for the running game. Called from
 * gameloop.cpp's PortPushFrame. Logs once per second. */
extern "C" void FrameInterpolation_TelemetryTick(void)
{
    static int sEnabled = -1;
    if (sEnabled < 0) {
        const char *e = std::getenv("SSB64_FRAME_INTERP_TELEMETRY");
        sEnabled = (e != nullptr && e[0] != '\0' && e[0] != '0') ? 1 : 0;
        if (sEnabled) {
            port_log("SSB64: FRAME_INTERP_TELEMETRY enabled\n");
        }
    }
    if (!sEnabled) return;

    static int sCounter = 0;
    if (++sCounter < 60) return;
    sCounter = 0;
    port_log("SSB64: frame_interp telemetry: ops=%d children=%d\n",
        FrameInterpolation_GetLastOpCount(),
        FrameInterpolation_GetLastChildCount());
}
