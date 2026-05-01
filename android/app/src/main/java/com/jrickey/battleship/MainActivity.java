package com.jrickey.battleship;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.io.File;

/**
 * Phase 2 scaffold Activity. Confirms the APK installs, the Activity
 * launches, and the .so files are present in the JNI lib dir — without
 * actually loading libmain.so.
 *
 * Why we don't loadLibrary("main") yet: libmain.so DT_NEEDEDs libSDL2.so,
 * and SDL2's JNI_OnLoad calls FindClass("org/libsdl/app/SDLActivity") to
 * register Java callbacks. Phase 3 vendors SDL2's Java glue and supplies
 * that class; until then, dlopen aborts the process with a JNI error.
 *
 * Phase 3 swaps this Activity entirely for a SDLActivity subclass that
 * hands off to SDL_main inside the .so.
 */
public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ApplicationInfo info = getApplicationInfo();
        String libDir = info.nativeLibraryDir;
        boolean haveMain = new File(libDir, "libmain.so").exists();
        boolean haveSDL  = new File(libDir, "libSDL2.so").exists();

        StringBuilder msg = new StringBuilder();
        msg.append("SSB64 PC Port — Android Phase 2 scaffold\n\n");
        msg.append("APK build:    gradle assembleDebug ✓\n");
        msg.append("Target ABI:   arm64-v8a (NDK r29)\n");
        msg.append("Min SDK:      26   Target SDK: 34\n");
        msg.append("Package:      ").append(info.packageName).append("\n\n");
        msg.append("Native libs in ").append(libDir).append(":\n");
        msg.append(haveMain ? "  ✓ libmain.so\n" : "  ✗ libmain.so MISSING\n");
        msg.append(haveSDL  ? "  ✓ libSDL2.so\n" : "  ✗ libSDL2.so MISSING\n");
        msg.append("\n");
        msg.append("Phase 3 will swap this Activity for a SDLActivity subclass\n");
        msg.append("(vendoring SDL2's Java glue) and call SDL_main inside the .so.\n");
        msg.append("System.loadLibrary(\"main\") is deferred to Phase 3 because\n");
        msg.append("libSDL2.so's JNI_OnLoad requires org/libsdl/app/SDLActivity.");

        TextView tv = new TextView(this);
        tv.setText(msg.toString());
        tv.setPadding(48, 48, 48, 48);
        tv.setTextSize(14f);

        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.addView(tv);
        setContentView(root);
    }
}
