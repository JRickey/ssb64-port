/* Standalone offline runner for FrameInterpolation_RunSelfTestIfRequested.
 *
 * Built only when CMake var BUILD_FRAME_INTERP_TEST=ON. The resulting binary
 * runs the unit tests without needing the game to launch — useful for CI or
 * for hacking on the recording layer in isolation.
 *
 * Provides minimal stubs for syMatrix*F builders so we don't have to link
 * the whole game library. Only the functions actually exercised by the
 * tests are stubbed. Add more here if test_* functions grow to use them.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

extern "C" {
    typedef float Mtx44f_t[4][4];

    /* port_log used by frame_interpolation_selftest.cpp. */
    void port_log(const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        std::vprintf(fmt, ap);
        va_end(ap);
    }
    void port_log_close(void) { std::fflush(stdout); }

    /* All syMatrix*F builders the recording layer might invoke. The tests
     * only exercise a subset; unused stubs are still listed so the linker
     * resolves any reference inside frame_interpolation.cpp. */
    static void identity(Mtx44f_t *mf) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                (*mf)[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    void syMatrixTraF(Mtx44f_t *mf, float x, float y, float z) {
        identity(mf);
        (*mf)[3][0] = x; (*mf)[3][1] = y; (*mf)[3][2] = z;
    }
    void syMatrixScaF(Mtx44f_t *mf, float x, float y, float z) {
        identity(mf);
        (*mf)[0][0] = x; (*mf)[1][1] = y; (*mf)[2][2] = z;
    }
    void syMatrixRotRF(Mtx44f_t *mf, float a, float, float, float) {
        identity(mf);
        float c = std::cos(a), s = std::sin(a);
        (*mf)[0][0] = c; (*mf)[0][1] = s;
        (*mf)[1][0] = -s; (*mf)[1][1] = c;
    }
    void syMatrixRotDF(Mtx44f_t *mf, float a, float x, float y, float z) {
        syMatrixRotRF(mf, a * 0.0174533f, x, y, z);
    }
    void syMatrixRotRpyRF(Mtx44f_t *mf, float, float, float)         { identity(mf); }
    void syMatrixRotRpyDF(Mtx44f_t *mf, float, float, float)         { identity(mf); }
    void syMatrixRotPyrRF(Mtx44f_t *mf, float, float, float)         { identity(mf); }
    void syMatrixRotPyRF(Mtx44f_t *mf, float, float)                 { identity(mf); }
    void syMatrixRotRpRF(Mtx44f_t *mf, float, float)                 { identity(mf); }
    /* RotYawR builds a rotation around the Y axis. The angle-wrap test
     * inspects mf[0][0] which should equal cos(yaw). */
    void syMatrixRotYawRF(Mtx44f_t *mf, float y) {
        identity(mf);
        float c = std::cos(y), s = std::sin(y);
        (*mf)[0][0] = c; (*mf)[0][1] = s;
        (*mf)[1][0] = -s; (*mf)[1][1] = c;
    }
    void syMatrixRotPitchRF(Mtx44f_t *mf, float)                     { identity(mf); }

    void syMatrixTraRotRF(Mtx44f_t *mf, float, float, float, float, float, float, float)            { identity(mf); }
    void syMatrixTraRotDF(Mtx44f_t *mf, float, float, float, float, float, float, float)            { identity(mf); }
    void syMatrixTraRotRScaF(Mtx44f_t *mf, float, float, float, float, float, float, float, float, float, float) { identity(mf); }
    void syMatrixTraRotRpyRF(Mtx44f_t *mf, float, float, float, float, float, float)                { identity(mf); }
    void syMatrixTraRotRpyDF(Mtx44f_t *mf, float, float, float, float, float, float)                { identity(mf); }
    void syMatrixTraRotRpyRScaF(Mtx44f_t *mf, float, float, float, float, float, float, float, float, float) { identity(mf); }
    void syMatrixTraRotPyrRF(Mtx44f_t *mf, float, float, float, float, float, float)                { identity(mf); }
    void syMatrixTraRotPyrRScaF(Mtx44f_t *mf, float, float, float, float, float, float, float, float, float)  { identity(mf); }
    void syMatrixTraRotPyRF(Mtx44f_t *mf, float, float, float, float, float)                        { identity(mf); }
    void syMatrixTraRotRpRF(Mtx44f_t *mf, float, float, float, float, float)                        { identity(mf); }
    void syMatrixTraRotYawRF(Mtx44f_t *mf, float, float, float, float)                              { identity(mf); }
    void syMatrixTraRotPitchRF(Mtx44f_t *mf, float, float, float, float)                            { identity(mf); }

    /* Public test entry point declared in frame_interpolation.h. */
    void FrameInterpolation_RunSelfTestIfRequested(void);
}

#include <cmath>

int main(int argc, char **argv) {
    /* Force the env var so the runner enters the test path. */
#ifdef _WIN32
    _putenv_s("SSB64_FRAME_INTERP_UNITTEST", "1");
#else
    setenv("SSB64_FRAME_INTERP_UNITTEST", "1", 1);
#endif
    FrameInterpolation_RunSelfTestIfRequested();
    /* If we reach here, all tests passed (failures call exit(2)). */
    return 0;
}
