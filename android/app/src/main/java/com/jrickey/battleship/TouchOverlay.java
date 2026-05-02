package com.jrickey.battleship;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.hardware.input.InputManager;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.InputDevice;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.TextView;

/**
 * On-screen touch controls for the SSB64 port.
 *
 * Layout:
 *   - Left half of the screen hosts a floating-anchor virtual analog
 *     stick (see {@link AnalogStickView}). Drag anywhere to move; the
 *     stick visualizes at the touch-down point.
 *   - Right half clusters face buttons in a fighting-game-friendly
 *     arrangement:
 *
 *        [Z]    [Start]
 *           [B]
 *               [A]
 *
 *     where:
 *       A     = jump / aerial-attack            (SDL_CONTROLLER_BUTTON_A)
 *       B     = special                         (SDL_CONTROLLER_BUTTON_B)
 *       Z     = shield / grab                   (SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
 *       Start = pause                           (SDL_CONTROLLER_BUTTON_START)
 *
 * LUS's SDLButtonToAnyMapping picks the virtual gamepad up via the
 * Xbox-360 vendor/product signature attached on the native side
 * (port/android_touch_overlay.cpp), so user-configured remappings via
 * the LUS controller config menu still apply.
 */
public final class TouchOverlay {

    /** Native methods implemented in port/android_touch_overlay.cpp. */
    public static native void setButton(int sdlButton, boolean down);
    public static native void setAxis(int sdlAxis, int value);

    /** SDL_GameControllerButton mirrors. */
    public static final int SDL_CONTROLLER_BUTTON_A             = 0;
    public static final int SDL_CONTROLLER_BUTTON_B             = 1;
    public static final int SDL_CONTROLLER_BUTTON_X             = 2;
    public static final int SDL_CONTROLLER_BUTTON_Y             = 3;
    public static final int SDL_CONTROLLER_BUTTON_BACK          = 4;
    public static final int SDL_CONTROLLER_BUTTON_START         = 6;
    public static final int SDL_CONTROLLER_BUTTON_LEFTSHOULDER  = 9;
    public static final int SDL_CONTROLLER_BUTTON_RIGHTSHOULDER = 10;
    public static final int SDL_CONTROLLER_BUTTON_DPAD_UP       = 11;
    public static final int SDL_CONTROLLER_BUTTON_DPAD_DOWN     = 12;
    public static final int SDL_CONTROLLER_BUTTON_DPAD_LEFT     = 13;
    public static final int SDL_CONTROLLER_BUTTON_DPAD_RIGHT    = 14;

    private TouchOverlay() { /* static */ }

    /**
     * Build the overlay and addContentView() it onto the Activity. Must
     * be called AFTER super.onCreate so SDLActivity has already installed
     * its surface as the root content view.
     */
    public static void install(Activity activity) {
        FrameLayout root = new FrameLayout(activity);

        // Left half: analog stick capture region. Half-screen-wide
        // FrameLayout that hosts an AnalogStickView filling its bounds.
        FrameLayout.LayoutParams stickLp = new FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT,
            Gravity.START | Gravity.FILL_VERTICAL);
        // Constrain horizontally to half-screen by setting marginEnd to
        // half the screen width via post — addContentView gives us
        // MATCH_PARENT initially.
        AnalogStickView stick = new AnalogStickView(activity);
        stick.setLayoutParams(new FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT));
        FrameLayout stickHost = new FrameLayout(activity);
        stickHost.addView(stick);
        // Half-width via post-layout: simpler than negotiating with
        // SDLActivity's surface to know its dimensions ahead of time.
        stickHost.post(() -> {
            ViewGroup.LayoutParams lp = stickHost.getLayoutParams();
            lp.width = ((ViewGroup) stickHost.getParent()).getWidth() / 2;
            stickHost.setLayoutParams(lp);
        });
        root.addView(stickHost, stickLp);

        // Right-cluster layout, sized for landscape. A is the primary
        // action (largest, bottom-right corner). B sits to its immediate
        // left at the same height. Z stacks above A. Start is the
        // smallest, top-right corner — least often pressed.
        //
        // dp offsets keep the buttons inside the screen on phones from
        // ~640dp wide (landscape Pixel 4a) up. Tablets get extra empty
        // space, which is fine — still thumb-reachable.
        //
        // Diameter dp:        A=120  B=110  Z=90  Start=70
        // marginEnd dp:       24      150    24    24
        // marginBottom dp:    24      24     160   --   (Z above A)
        // marginTop dp:       --      --     --    24
        addButton(activity, root, "A",     SDL_CONTROLLER_BUTTON_A,
                  Color.argb(0xC0, 0x33, 0xCC, 0x55), 120,
                  Gravity.BOTTOM | Gravity.END,    24,  24);
        addButton(activity, root, "B",     SDL_CONTROLLER_BUTTON_B,
                  Color.argb(0xC0, 0xCC, 0x33, 0x55), 110,
                  Gravity.BOTTOM | Gravity.END,    150, 24);
        addButton(activity, root, "Z",     SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                  Color.argb(0xC0, 0x99, 0x66, 0xCC), 90,
                  Gravity.BOTTOM | Gravity.END,    24,  160);
        addButton(activity, root, "Start", SDL_CONTROLLER_BUTTON_START,
                  Color.argb(0xA0, 0xAA, 0xAA, 0xAA), 70,
                  Gravity.TOP    | Gravity.END,    24,  24);

        activity.addContentView(root,
            new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));

        // Auto-hide overlay when a physical controller is paired. SDL2's
        // SDLControllerManager reports those controllers to the native side
        // independently of our virtual joystick, so when a real one is
        // present the user gets two controllers visible to LUS — the
        // virtual one (silent) plus the real one. Hiding the overlay
        // visually preserves the right-thumb area for cinematic viewing
        // and stops accidental touch events. (The virtual joystick still
        // exists in SDL's joystick list; a future polish step can detach
        // it to remove the duplicate from LUS's controller-config UI.)
        installControllerWatcher(activity, root);
    }

    /**
     * Toggle the {@code overlayRoot} visibility based on whether any
     * Android InputDevice exposes SOURCE_GAMEPAD or SOURCE_JOYSTICK.
     * Listens via {@link InputManager.InputDeviceListener} for hot-plug
     * (Bluetooth pair, USB connect/disconnect).
     */
    private static void installControllerWatcher(Activity activity, View overlayRoot) {
        Object svc = activity.getSystemService(Context.INPUT_SERVICE);
        if (!(svc instanceof InputManager)) {
            return;
        }
        final InputManager im = (InputManager) svc;
        final Handler ui = new Handler(Looper.getMainLooper());

        Runnable refresh = () -> {
            boolean hasGamepad = countGamepads() > 0;
            overlayRoot.setVisibility(hasGamepad ? View.GONE : View.VISIBLE);
        };

        // Initial visibility — race-safe even if the listener hasn't fired
        // yet for an already-paired controller.
        ui.post(refresh);

        im.registerInputDeviceListener(new InputManager.InputDeviceListener() {
            @Override public void onInputDeviceAdded(int id)   { ui.post(refresh); }
            @Override public void onInputDeviceRemoved(int id) { ui.post(refresh); }
            @Override public void onInputDeviceChanged(int id) { ui.post(refresh); }
        }, ui);
    }

    /** Count physical gamepads / joysticks visible to Android's InputManager. */
    private static int countGamepads() {
        int count = 0;
        for (int id : InputDevice.getDeviceIds()) {
            InputDevice dev = InputDevice.getDevice(id);
            if (dev == null) continue;
            int sources = dev.getSources();
            boolean isGamepad =
                (sources & InputDevice.SOURCE_GAMEPAD)  == InputDevice.SOURCE_GAMEPAD
             || (sources & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK;
            if (isGamepad) {
                count++;
            }
        }
        return count;
    }

    /**
     * Build a circular touch button and attach it to {@code root} at the
     * given gravity + dp offsets. Press/release forwards to {@link #setButton}.
     */
    private static void addButton(Context ctx, FrameLayout root,
                                  String label, int sdlButton,
                                  int fillColor, int diameterDp,
                                  int gravity,
                                  int marginEndDp, int marginVerticalDp) {
        int sizePx        = dp(ctx, diameterDp);
        int marginEndPx   = dp(ctx, marginEndDp);
        int marginVertPx  = dp(ctx, marginVerticalDp);

        TextView btn = new TextView(ctx);
        btn.setText(label);
        btn.setTextColor(Color.WHITE);
        btn.setTextSize(label.length() <= 1 ? 26f : 16f);
        btn.setGravity(Gravity.CENTER);

        GradientDrawable bg = new GradientDrawable();
        bg.setShape(GradientDrawable.OVAL);
        bg.setColor(fillColor);
        bg.setStroke(dp(ctx, 2), Color.argb(0xE0, 0xFF, 0xFF, 0xFF));
        btn.setBackground(bg);

        FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(sizePx, sizePx, gravity);
        // marginVerticalDp is interpreted relative to the gravity edge:
        // BOTTOM → bottomMargin, TOP → topMargin.
        if ((gravity & Gravity.BOTTOM) != 0) lp.bottomMargin = marginVertPx;
        if ((gravity & Gravity.TOP)    != 0) lp.topMargin    = marginVertPx;
        lp.rightMargin = marginEndPx;
        btn.setLayoutParams(lp);

        btn.setOnTouchListener((v, ev) -> {
            switch (ev.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN:
                    setButton(sdlButton, true);
                    return true;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_POINTER_UP:
                case MotionEvent.ACTION_CANCEL:
                    setButton(sdlButton, false);
                    return true;
                default:
                    return false;
            }
        });
        root.addView(btn);
    }

    private static int dp(Context ctx, int dp) {
        return Math.round(dp * ctx.getResources().getDisplayMetrics().density);
    }
}
