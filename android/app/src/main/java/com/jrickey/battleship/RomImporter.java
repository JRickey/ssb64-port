package com.jrickey.battleship;

import android.content.ContentResolver;
import android.content.Context;
import android.net.Uri;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Bridges the Java first-run UI to libtorch_runner.so's ROM extractor.
 *
 * Two pieces:
 *   1. {@link #stageRomFromUri} — copies the SAF-selected content URI
 *      into a real file in cacheDir, because Torch's Cartridge::open()
 *      uses fopen() and can't read content:// URIs directly.
 *   2. {@link #extractO2R} — JNI entry point into Companion (O2R mode).
 *      Returns 0 on success.
 *
 * This class deliberately lives outside org.libsdl.app — loading
 * libtorch_runner.so does NOT trigger SDL2's JNI_OnLoad and therefore
 * doesn't need the SDLActivity Java class to be registered.
 */
public final class RomImporter {
    private static final String TAG = "ssb64.rom";

    /** Stable load: System.loadLibrary uses linker caching after the first hit. */
    static {
        System.loadLibrary("torch_runner");
    }

    /**
     * Streams a SAF content URI into a real file under cacheDir.
     *
     * @param ctx     for ContentResolver + cacheDir
     * @param romUri  user-picked URI from ACTION_OPEN_DOCUMENT
     * @return        local path on success, or null on I/O failure
     */
    public static File stageRomFromUri(Context ctx, Uri romUri) {
        File outFile = new File(ctx.getCacheDir(), "user_rom.z64");

        ContentResolver cr = ctx.getContentResolver();
        try (InputStream in = cr.openInputStream(romUri);
             OutputStream out = new FileOutputStream(outFile)) {
            if (in == null) {
                Log.e(TAG, "openInputStream returned null for " + romUri);
                return null;
            }
            byte[] buf = new byte[256 * 1024];
            long total = 0;
            int n;
            while ((n = in.read(buf)) > 0) {
                out.write(buf, 0, n);
                total += n;
            }
            Log.i(TAG, "Staged " + total + " bytes from " + romUri + " -> " + outFile);
        } catch (IOException ioe) {
            Log.e(TAG, "stageRomFromUri failed", ioe);
            outFile.delete();
            return null;
        }
        return outFile;
    }

    /**
     * Native entry into Companion(O2R). Heavy work — call from a
     * background thread. Logcat tag "ssb64.torch" is the source of
     * detailed error messages on non-zero return.
     *
     * @param romPath  absolute path to a real file (NOT a content:// URI)
     * @param srcDir   directory with config.yml + yamls/us/*.yml
     *                 (extracted there by AssetExtractor on first run)
     * @param dstDir   where BattleShip.o2r will be written
     * @return 0 on success
     */
    public static native int extractO2R(String romPath, String srcDir, String dstDir);

    private RomImporter() { /* static */ }
}
