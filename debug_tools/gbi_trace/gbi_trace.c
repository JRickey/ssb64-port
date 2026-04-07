/**
 * gbi_trace.c — Port-side GBI display list trace system
 *
 * Writes structured per-frame traces to debug_traces/ directory.
 * Output format is designed to be diffed against M64P trace plugin output.
 */
#include "gbi_trace.h"
#include "gbi_decoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

/* ========================================================================= */
/*  State                                                                    */
/* ========================================================================= */

static int      sEnabled     = 0;
static int      sInitialized = 0;
static int      sFrameNum    = 0;
static int      sCmdIndex    = 0;
static int      sMaxFrames   = 300;  /* default: 5 seconds */
static FILE    *sTraceFile   = NULL;
static char     sTraceDir[512] = "debug_traces";

/* Per-frame rolling file or single file modes */
/* We use a single file with frame delimiters for easier diffing */
static char     sTraceFilePath[1024];

/* ========================================================================= */
/*  Initialization                                                           */
/* ========================================================================= */

void gbi_trace_init(void)
{
	const char *env;

	if (sInitialized) return;
	sInitialized = 1;

	env = getenv("SSB64_GBI_TRACE");
	if (env && (env[0] == '1' || env[0] == 'y' || env[0] == 'Y')) {
		sEnabled = 1;
	}

	env = getenv("SSB64_GBI_TRACE_FRAMES");
	if (env) {
		int val = atoi(env);
		if (val > 0) sMaxFrames = val;
		if (val == 0) sMaxFrames = 0; /* unlimited */
	}

	env = getenv("SSB64_GBI_TRACE_DIR");
	if (env && env[0]) {
		snprintf(sTraceDir, sizeof(sTraceDir), "%s", env);
	}

	if (!sEnabled) return;

	/* Create output directory */
	MKDIR(sTraceDir);

	/* Open the trace file */
	snprintf(sTraceFilePath, sizeof(sTraceFilePath), "%s/port_trace.gbi", sTraceDir);
	sTraceFile = fopen(sTraceFilePath, "w");
	if (!sTraceFile) {
		fprintf(stderr, "[gbi_trace] ERROR: cannot open %s for writing\n", sTraceFilePath);
		sEnabled = 0;
		return;
	}

	/* Header */
	fprintf(sTraceFile, "# GBI Trace — SSB64 PC Port\n");
	fprintf(sTraceFile, "# Format: [cmd_index] d=depth OPCODE  w0=XXXXXXXX w1=XXXXXXXX  params...\n");
	fprintf(sTraceFile, "# Source: port (Fast3D interpreter execution order)\n");
	fprintf(sTraceFile, "#\n");
	fflush(sTraceFile);

	fprintf(stderr, "[gbi_trace] Trace enabled, writing to %s (max %d frames)\n",
	        sTraceFilePath, sMaxFrames);
}

void gbi_trace_shutdown(void)
{
	if (sTraceFile) {
		fprintf(sTraceFile, "# END OF TRACE — %d frames captured\n", sFrameNum);
		fclose(sTraceFile);
		sTraceFile = NULL;
	}
	sEnabled = 0;
	sInitialized = 0;
}

/* ========================================================================= */
/*  Runtime control                                                          */
/* ========================================================================= */

void gbi_trace_set_enabled(int enabled)
{
	if (enabled && !sInitialized) {
		/* Force init */
		sEnabled = 1;
		gbi_trace_init();
		return;
	}
	sEnabled = enabled;
}

int gbi_trace_is_enabled(void)
{
	return sEnabled && sTraceFile != NULL;
}

void gbi_trace_set_max_frames(int max_frames)
{
	sMaxFrames = max_frames;
}

/* ========================================================================= */
/*  Frame lifecycle                                                          */
/* ========================================================================= */

void gbi_trace_begin_frame(void)
{
	if (!sEnabled || !sTraceFile) return;

	/* Check frame limit */
	if (sMaxFrames > 0 && sFrameNum >= sMaxFrames) {
		if (sFrameNum == sMaxFrames) {
			fprintf(sTraceFile, "# TRACE STOPPED — frame limit %d reached\n", sMaxFrames);
			fflush(sTraceFile);
			fprintf(stderr, "[gbi_trace] Frame limit %d reached, stopping trace\n", sMaxFrames);
		}
		sFrameNum++;
		return;
	}

	fprintf(sTraceFile, "\n=== FRAME %d ===\n", sFrameNum);
	sCmdIndex = 0;
}

void gbi_trace_end_frame(void)
{
	if (!sEnabled || !sTraceFile) return;
	if (sMaxFrames > 0 && sFrameNum >= sMaxFrames) {
		sFrameNum++;
		return;
	}

	fprintf(sTraceFile, "=== END FRAME %d — %d commands ===\n", sFrameNum, sCmdIndex);
	fflush(sTraceFile);
	sFrameNum++;
}

/* ========================================================================= */
/*  Command logging                                                          */
/* ========================================================================= */

void gbi_trace_log_cmd(unsigned long long w0, unsigned long long w1, int depth)
{
	char decoded[512];

	if (!sEnabled || !sTraceFile) return;
	if (sMaxFrames > 0 && sFrameNum >= sMaxFrames) return;

	/* Extract lower 32 bits for the shared decoder (N64-compatible representation).
	 * Also log full 64-bit w1 when it differs (pointer addresses on PC). */
	uint32_t w0_lo = (uint32_t)(w0 & 0xFFFFFFFF);
	uint32_t w1_lo = (uint32_t)(w1 & 0xFFFFFFFF);

	gbi_decode_cmd(w0_lo, w1_lo, decoded, sizeof(decoded));

	/* Check if w1 upper bits are nonzero (widened pointer on PC) */
	uint32_t w1_hi = (uint32_t)(w1 >> 32);

	if (w1_hi != 0) {
		fprintf(sTraceFile, "[%04d] d=%d %s  (w1_64=%016llX)\n",
		        sCmdIndex, depth, decoded, w1);
	} else {
		fprintf(sTraceFile, "[%04d] d=%d %s\n",
		        sCmdIndex, depth, decoded);
	}

	sCmdIndex++;
}
