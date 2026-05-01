// android_touch_overlay.cpp — virtual SDL gamepad fed by Java touch events.
//
// Architecture:
//   1. SDL_main thread (port.cpp's pre-init block) brings up
//      SDL_INIT_GAMECONTROLLER. We attach a virtual joystick on first
//      JNI call from TouchOverlay so we don't pay the cost in builds
//      where the user has a paired Bluetooth/USB gamepad and never
//      touches the on-screen buttons.
//
//   2. Java's TouchOverlay overlays buttons on top of the SDL surface
//      and forwards touch DOWN/UP events to setButton() / setAxis()
//      JNI methods. SDL_JoystickSetVirtualButton/Axis is documented as
//      thread-safe; SDL_PumpEvents on the SDL thread translates the
//      state changes into SDL_CONTROLLERBUTTONDOWN/UP events that LUS
//      already knows how to map to OSContPad bits via SDLButtonToAnyMapping.
//
// Lifetime: the virtual joystick lives for the duration of the process.

#if defined(__ANDROID__)

#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

#include <android/log.h>
#include <jni.h>

#include <atomic>

#define LOG_TAG "ssb64.touch"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace {

SDL_Joystick *sJoystick = nullptr;
int sDeviceIndex = -1;
std::atomic<bool> sAttachAttempted{false};

// Xbox-360-style descriptor. SDL2 promotes any virtual joystick with this
// vendor/product signature to SDL_GameController, so LUS's existing
// SDLButtonToAnyMapping pipeline picks us up without changes.
constexpr int kAxisCount   = SDL_CONTROLLER_AXIS_MAX;        // 6
constexpr int kButtonCount = SDL_CONTROLLER_BUTTON_MAX;      // 15

bool ensureAttached() {
    if (sJoystick != nullptr) {
        return true;
    }
    if (sAttachAttempted.exchange(true)) {
        return false;
    }

    SDL_VirtualJoystickDesc desc;
    SDL_zero(desc);
    desc.version    = SDL_VIRTUAL_JOYSTICK_DESC_VERSION;
    desc.type       = SDL_JOYSTICK_TYPE_GAMECONTROLLER;
    desc.naxes      = kAxisCount;
    desc.nbuttons   = kButtonCount;
    desc.vendor_id  = 0x045E;
    desc.product_id = 0x028E;
    desc.name       = "SSB64 Touch Overlay";

    sDeviceIndex = SDL_JoystickAttachVirtualEx(&desc);
    if (sDeviceIndex < 0) {
        LOGE("SDL_JoystickAttachVirtualEx failed: %s", SDL_GetError());
        return false;
    }
    sJoystick = SDL_JoystickOpen(sDeviceIndex);
    if (sJoystick == nullptr) {
        LOGE("SDL_JoystickOpen(%d) failed: %s", sDeviceIndex, SDL_GetError());
        SDL_JoystickDetachVirtual(sDeviceIndex);
        sDeviceIndex = -1;
        return false;
    }
    LOGI("Virtual joystick attached (device_index=%d, instance_id=%d, %d axes, %d buttons)",
         sDeviceIndex, SDL_JoystickInstanceID(sJoystick), kAxisCount, kButtonCount);
    return true;
}

} // namespace

extern "C" {

JNIEXPORT void JNICALL
Java_com_jrickey_battleship_TouchOverlay_setButton(
    JNIEnv * /*env*/, jclass /*clazz*/, jint btn, jboolean down) {
    if (!ensureAttached()) return;
    if (btn < 0 || btn >= kButtonCount) {
        LOGW("setButton: out-of-range btn=%d", (int)btn);
        return;
    }
    SDL_JoystickSetVirtualButton(sJoystick, btn, down ? 1 : 0);
}

JNIEXPORT void JNICALL
Java_com_jrickey_battleship_TouchOverlay_setAxis(
    JNIEnv * /*env*/, jclass /*clazz*/, jint axis, jint value) {
    if (!ensureAttached()) return;
    if (axis < 0 || axis >= kAxisCount) {
        LOGW("setAxis: out-of-range axis=%d", (int)axis);
        return;
    }
    if (value < -32768) value = -32768;
    if (value >  32767) value =  32767;
    SDL_JoystickSetVirtualAxis(sJoystick, axis, (Sint16)value);
}

} // extern "C"

#endif // __ANDROID__
