package com.jrickey.battleship;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Gravity;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.io.File;

/**
 * BootActivity — the launcher entry point.
 *
 * Phase 4.3 responsibilities:
 *   1. Extract bundled APK assets (f3d.o2r, ssb64.o2r, config.yml, yamls/)
 *      into externalFilesDir if they're missing or stale (different
 *      versionCode than last extracted).
 *   2. Decide whether the user has supplied their ROM yet — i.e. is
 *      BattleShip.o2r present in externalFilesDir?
 *      - Yes → start BattleShipActivity (the SDL game Activity).
 *      - No  → Phase 4.4 will show a ROM picker and run libtorch_runner.so
 *              against the user's .z64. For now we render a placeholder.
 *
 * Asset extraction runs on a background thread so the UI thread stays
 * responsive (the .o2r files are tens of MB; copying them blocks for a
 * second or two on a slow device).
 */
public class BootActivity extends Activity {
    private static final String TAG = "ssb64.boot";

    private TextView mStatus;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mStatus = new TextView(this);
        mStatus.setGravity(Gravity.CENTER);
        mStatus.setPadding(64, 64, 64, 64);
        mStatus.setTextSize(18f);
        mStatus.setText("Preparing assets…");

        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setGravity(Gravity.CENTER);
        root.addView(mStatus);
        setContentView(root);

        // Background asset extraction; post the result to the UI thread.
        new Thread(() -> {
            String err = AssetExtractor.extractIfNeeded(getApplicationContext());
            Handler ui = new Handler(Looper.getMainLooper());
            if (err != null) {
                ui.post(() -> showError(err));
                return;
            }
            ui.post(this::routeNext);
        }, "ssb64-boot-extract").start();
    }

    private void routeNext() {
        if (AssetExtractor.haveExtractedRom(this)) {
            startGame();
            return;
        }
        // Phase 4.4 will replace this with a SAF ROM picker that calls
        // libtorch_runner.so to produce BattleShip.o2r. For now, surface
        // the missing-ROM state clearly so the developer flow is unambiguous:
        // adb push BattleShip.o2r /storage/emulated/0/Android/data/<pkg>/files/
        File extDir = getExternalFilesDir(null);
        String path = extDir == null ? "<external dir unavailable>" : extDir.getPath();
        mStatus.setText(
            "BattleShip.o2r not found.\n\n" +
            "Phase 4.3 stops here — Phase 4.4 will add a ROM picker.\n\n" +
            "Dev workaround:\n" +
            "  adb push BattleShip.o2r " + path + "/\n\n" +
            "then relaunch the app."
        );
        Log.i(TAG, "BattleShip.o2r missing; awaiting Phase 4.4 ROM picker");
    }

    private void showError(String message) {
        mStatus.setText("Asset extraction failed:\n\n" + message);
        Log.e(TAG, message);
    }

    private void startGame() {
        Log.i(TAG, "Assets ready, launching BattleShipActivity");
        startActivity(new Intent(this, BattleShipActivity.class));
        finish();
    }
}
