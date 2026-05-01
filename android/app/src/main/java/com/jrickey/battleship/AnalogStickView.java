package com.jrickey.battleship;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.MotionEvent;
import android.view.View;

/**
 * Floating-anchor virtual analog stick. The stick "appears" wherever the
 * user first touches inside its hosting region; subsequent drag delta
 * (clamped to a configurable radius) sets SDL_CONTROLLER_AXIS_LEFTX/Y.
 * On release, the axes return to centered.
 *
 * Floating over fixed: SSB64 movement is expressive (grace-of-the-stick
 * matters), and a fixed-position stick forces the user to keep their
 * thumb on a small target. A floating stick anchored at the touch-down
 * point is the standard mobile fighting-game convention.
 */
public final class AnalogStickView extends View {

    /* SDL_GameControllerAxis mirrors. */
    private static final int SDL_AXIS_LEFTX = 0;
    private static final int SDL_AXIS_LEFTY = 1;

    /** Pixels of drag distance that map to full deflection on the SDL axis. */
    private final int mDeflectionPx;

    /** Visual radius for the outer ring. */
    private final int mRingPx;
    /** Visual radius for the moving thumb. */
    private final int mThumbPx;

    private final Paint mRingPaint;
    private final Paint mThumbPaint;

    /** Active touch pointer ID, or -1 when idle. */
    private int  mActivePointerId = -1;
    /** Anchor (touch-down position). */
    private float mAnchorX, mAnchorY;
    /** Current touch position. */
    private float mTouchX, mTouchY;

    public AnalogStickView(Context ctx) {
        super(ctx);
        float density = ctx.getResources().getDisplayMetrics().density;
        mDeflectionPx = Math.round(80f * density);
        mRingPx       = Math.round(70f * density);
        mThumbPx      = Math.round(34f * density);

        mRingPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mRingPaint.setStyle(Paint.Style.STROKE);
        mRingPaint.setStrokeWidth(Math.max(2f, 2f * density));
        mRingPaint.setColor(Color.argb(0xC0, 0xFF, 0xFF, 0xFF));

        mThumbPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mThumbPaint.setColor(Color.argb(0xA0, 0x33, 0x99, 0xFF));
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (mActivePointerId < 0) {
            return;
        }
        canvas.drawCircle(mAnchorX, mAnchorY, mRingPx,  mRingPaint);
        canvas.drawCircle(mTouchX,  mTouchY,  mThumbPx, mThumbPaint);
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        switch (ev.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN: {
                if (mActivePointerId >= 0) {
                    // Already tracking a pointer in this view — ignore others.
                    return true;
                }
                int idx = ev.getActionIndex();
                mActivePointerId = ev.getPointerId(idx);
                mAnchorX = ev.getX(idx);
                mAnchorY = ev.getY(idx);
                mTouchX  = mAnchorX;
                mTouchY  = mAnchorY;
                emitAxis(0f, 0f);
                invalidate();
                return true;
            }
            case MotionEvent.ACTION_MOVE: {
                int idx = ev.findPointerIndex(mActivePointerId);
                if (idx < 0) return true;
                mTouchX = ev.getX(idx);
                mTouchY = ev.getY(idx);

                float dx = mTouchX - mAnchorX;
                float dy = mTouchY - mAnchorY;
                float len = (float) Math.hypot(dx, dy);
                float clampedDx = dx, clampedDy = dy;
                if (len > mDeflectionPx) {
                    clampedDx = dx * mDeflectionPx / len;
                    clampedDy = dy * mDeflectionPx / len;
                }
                emitAxis(clampedDx / mDeflectionPx,
                         clampedDy / mDeflectionPx);
                invalidate();
                return true;
            }
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_CANCEL: {
                int upId = ev.getPointerId(ev.getActionIndex());
                if (upId != mActivePointerId) {
                    return true;
                }
                mActivePointerId = -1;
                emitAxis(0f, 0f);
                invalidate();
                return true;
            }
            default:
                return false;
        }
    }

    /** @param nx,ny normalized [-1, 1]; positive Y is downward (matches SDL). */
    private void emitAxis(float nx, float ny) {
        // SDL signed-16 axis range
        int sx = (int) Math.round(Math.max(-1f, Math.min(1f, nx)) * 32767f);
        int sy = (int) Math.round(Math.max(-1f, Math.min(1f, ny)) * 32767f);
        TouchOverlay.setAxis(SDL_AXIS_LEFTX, sx);
        TouchOverlay.setAxis(SDL_AXIS_LEFTY, sy);
    }
}
