/**
 * xr_runtime.cpp — see xr_runtime.h for design notes and TODO list.
 *
 * Phase 9 SCAFFOLD: this file gets the integration points compile-clean and
 * wired into PortPushFrame. The OpenXR session lifecycle itself is stubbed;
 * `xr_runtime_init` deliberately reports "not implemented" unless the build
 * is configured with -DSSB64_ENABLE_OPENXR=ON, at which point a real
 * implementation (TBD) replaces these stubs.
 *
 * The opt-in env var SSB64_XR_ENABLE=1 turns on the init path; without it,
 * xr_runtime_is_active() returns 0 and all the per-frame hooks are no-ops,
 * meaning this file adds essentially zero overhead to non-VR sessions.
 */

#include "xr_runtime.h"
#include "../port_log.h"

#include <cstdlib>
#include <cstring>

#ifdef SSB64_ENABLE_OPENXR
/* Real OpenXR includes go here once the dependency is set up. Example:
 *   #define XR_USE_GRAPHICS_API_D3D11
 *   #include <openxr/openxr.h>
 *   #include <openxr/openxr_platform.h>
 */
#endif

namespace {

bool g_active = false;
const char *g_status = "uninitialized";

#ifdef SSB64_ENABLE_OPENXR
/* Real session state goes here once xr_runtime_init is implemented:
 *
 *   XrInstance instance = XR_NULL_HANDLE;
 *   XrSystemId system_id = XR_NULL_SYSTEM_ID;
 *   XrSession session = XR_NULL_HANDLE;
 *   XrSpace local_space = XR_NULL_HANDLE;
 *   XrSwapchain quad_swapchain = XR_NULL_HANDLE;
 *   XrFrameState frame_state = {};
 *   bool session_running = false;
 */
#endif

bool env_enabled() {
    const char *e = std::getenv("SSB64_XR_ENABLE");
    return e != nullptr && e[0] != '\0' && e[0] != '0';
}

} // namespace

extern "C" {

int xr_runtime_init(void) {
    if (!env_enabled()) {
        g_status = "disabled (SSB64_XR_ENABLE not set)";
        return 1;
    }

#ifdef SSB64_ENABLE_OPENXR
    /* TODO: real session init.
     *
     * Sketch (Windows D3D11 path):
     *   XrApplicationInfo app_info = { "BattleShip", 1, "ssb64-pc-port", 0, XR_CURRENT_API_VERSION };
     *   const char *exts[] = { XR_KHR_D3D11_ENABLE_EXTENSION_NAME };
     *   XrInstanceCreateInfo ici = { XR_TYPE_INSTANCE_CREATE_INFO };
     *   ici.applicationInfo = app_info;
     *   ici.enabledExtensionCount = 1;
     *   ici.enabledExtensionNames = exts;
     *   if (xrCreateInstance(&ici, &instance) != XR_SUCCESS) goto fail;
     *
     *   XrSystemGetInfo sgi = { XR_TYPE_SYSTEM_GET_INFO };
     *   sgi.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
     *   if (xrGetSystem(instance, &sgi, &system_id) != XR_SUCCESS) goto fail;
     *
     *   PFN_xrGetD3D11GraphicsRequirementsKHR pfnGetReq = nullptr;
     *   xrGetInstanceProcAddr(instance, "xrGetD3D11GraphicsRequirementsKHR",
     *                         (PFN_xrVoidFunction*)&pfnGetReq);
     *   XrGraphicsRequirementsD3D11KHR reqs = { XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR };
     *   pfnGetReq(instance, system_id, &reqs);
     *
     *   ID3D11Device *device = ...;       // pull from libultraship's DX11 backend
     *   XrGraphicsBindingD3D11KHR binding = { XR_TYPE_GRAPHICS_BINDING_D3D11_KHR };
     *   binding.device = device;
     *
     *   XrSessionCreateInfo sci = { XR_TYPE_SESSION_CREATE_INFO };
     *   sci.next = &binding;
     *   sci.systemId = system_id;
     *   if (xrCreateSession(instance, &sci, &session) != XR_SUCCESS) goto fail;
     *
     *   XrReferenceSpaceCreateInfo rsci = { XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
     *   rsci.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
     *   rsci.poseInReferenceSpace = { {0,0,0,1}, {0,0,0} };  // identity
     *   xrCreateReferenceSpace(session, &rsci, &local_space);
     *
     *   // Create a swapchain for the quad layer (single image, e.g. 1920x1080)
     *   ...
     *
     *   g_active = true;
     *   g_status = "session created";
     *   return 0;
     *
     * fail:
     *   g_status = "init failed";
     *   return 1;
     */
    port_log("SSB64: XR_ENABLE set but openxr stubs not implemented yet\n");
    g_status = "stub: SSB64_ENABLE_OPENXR build flag set but session init not implemented";
    return 1;
#else
    port_log("SSB64: XR_ENABLE set but binary not built with -DSSB64_ENABLE_OPENXR=ON\n");
    g_status = "build flag SSB64_ENABLE_OPENXR not defined";
    return 1;
#endif
}

int xr_runtime_shutdown(void) {
    if (!g_active) return 0;
#ifdef SSB64_ENABLE_OPENXR
    /* TODO:
     *   if (session)       xrDestroySession(session);
     *   if (instance)      xrDestroyInstance(instance);
     */
#endif
    g_active = false;
    g_status = "shutdown";
    return 0;
}

int xr_runtime_is_active(void) {
    return g_active ? 1 : 0;
}

void xr_runtime_begin_frame(void) {
    if (!g_active) return;
#ifdef SSB64_ENABLE_OPENXR
    /* TODO:
     *   xrWaitFrame(session, &waitFrameInfo, &frame_state);
     *   XrFrameBeginInfo bfi = { XR_TYPE_FRAME_BEGIN_INFO };
     *   xrBeginFrame(session, &bfi);
     */
#endif
}

void xr_runtime_end_frame(void) {
    if (!g_active) return;
#ifdef SSB64_ENABLE_OPENXR
    /* TODO:
     *   // Acquire/wait/release the quad-layer swapchain image
     *   uint32_t img_idx;
     *   xrAcquireSwapchainImage(quad_swapchain, ..., &img_idx);
     *   xrWaitSwapchainImage(quad_swapchain, ...);
     *
     *   // Copy libultraship's main framebuffer into the swapchain image.
     *   // Source: Fast3dWindow::GetGfxFrameBuffer() returns a uintptr_t to
     *   // the platform texture handle; on D3D11 it's an ID3D11Texture2D*.
     *   ID3D11DeviceContext *ctx = ...;
     *   ID3D11Texture2D *src = (ID3D11Texture2D *)Fast3dWindow::GetGfxFrameBuffer();
     *   ID3D11Texture2D *dst = (ID3D11Texture2D *)swapchain_images[img_idx].texture;
     *   ctx->CopyResource(dst, src);
     *
     *   xrReleaseSwapchainImage(quad_swapchain, ...);
     *
     *   // Build the quad layer (world-locked, ~2m in front, 3m wide)
     *   XrCompositionLayerQuad quad = { XR_TYPE_COMPOSITION_LAYER_QUAD };
     *   quad.space = local_space;
     *   quad.eyeVisibility = XR_EYE_VISIBILITY_BOTH;
     *   quad.subImage.swapchain = quad_swapchain;
     *   quad.pose = { {0,0,0,1}, {0, 0, -2.0f} };
     *   quad.size = { 3.2f, 1.8f };
     *
     *   const XrCompositionLayerBaseHeader *layers[] = {
     *       reinterpret_cast<const XrCompositionLayerBaseHeader *>(&quad)
     *   };
     *   XrFrameEndInfo efi = { XR_TYPE_FRAME_END_INFO };
     *   efi.displayTime = frame_state.predictedDisplayTime;
     *   efi.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
     *   efi.layerCount = frame_state.shouldRender ? 1 : 0;
     *   efi.layers = layers;
     *   xrEndFrame(session, &efi);
     */
#endif
}

const char *xr_runtime_status(void) {
    return g_status;
}

} // extern "C"
