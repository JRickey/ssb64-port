package com.jrickey.battleship;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.TextView;

/**
 * On-screen touch controls. Inflates a transparent FrameLayout above the
 * SDL surface, hosts a handful of round buttons, and forwards each
 * press/release to the native virtual SDL gamepad ({@link #setButton}).
 *
 * Phase 6.3 ships a single A button — proves the JNI → SDL_JoystickSet*
 * → LUS pipeline works end to end. Future phases add B/Z/Start/D-pad
 * and a virtual analog stick.
 */
public final class TouchOverlay {

    /** Native methods implemented in port/android_touch_overlay.cpp.
     *  Available once libmain.so is loaded by SDLActivity. */
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

    private TouchOverlay() { /* static factory */ }

    /**
     * Build the overlay and addContentView() it onto the Activity. Must
     * be called AFTER the Activity's super.onCreate so SDLActivity has
     * already installed its surface as the root content view.
     */
    public static void install(Activity activity) {
        FrameLayout root = new FrameLayout(activity);
        root.addView(makeButton(activity,
                                "A",
                                SDL_CONTROLLER_BUTTON_A,
                                Color.argb(0xC0, 0x33, 0xCC, 0x55),
                                Gravity.BOTTOM | Gravity.END));
        activity.addContentView(root,
            new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
    }

    private static View makeButton(Context ctx, String label, int sdlButton,
                                   int fillColor, int gravity) {
        int sizePx   = dp(ctx, 120);
        int marginPx = dp(ctx, 24);

        TextView btn = new TextView(ctx);
        btn.setText(label);
        btn.setTextColor(Color.WHITE);
        btn.setTextSize(28f);
        btn.setGravity(Gravity.CENTER);

        GradientDrawable bg = new GradientDrawable();
        bg.setShape(GradientDrawable.OVAL);
        bg.setColor(fillColor);
        bg.setStroke(dp(ctx, 2), Color.argb(0xE0, 0xFF, 0xFF, 0xFF));
        btn.setBackground(bg);

        FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(sizePx, sizePx, gravity);
        lp.setMargins(marginPx, marginPx, marginPx, marginPx);
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
        return btn;
    }

    private static int dp(Context ctx, int dp) {
        return Math.round(dp * ctx.getResources().getDisplayMetrics().density);
    }
}
