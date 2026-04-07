/**
 * acmd_trace.c — Port-side audio command (Acmd) trace system
 *
 * Writes structured per-task traces to debug_traces/ directory.
 * Output format is designed to be diffed against M64P RSP plugin output.
 */
#include "acmd_trace.h"
#include "acmd_decoder.h"

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
static int      sTaskNum     = 0;
static int      sCmdIndex    = 0;
static int      sMaxTasks    = 600;  /* default: 10 seconds @ 60 tasks/sec */
static FILE    *sTraceFile   = NULL;
static char     sTraceDir[512] = "debug_traces";
static char     sTraceFilePath[1024];

/* ========================================================================= */
/*  Initialization                                                           */
/* ========================================================================= */

void acmd_trace_init(void)
{
	const char *env;

	if (sInitialized) return;
	sInitialized = 1;

	env = getenv("SSB64_ACMD_TRACE");
	if (env && (env[0] == '1' || env[0] == 'y' || env[0] == 'Y')) {
		sEnabled = 1;
	}

	env = getenv("SSB64_ACMD_TRACE_TASKS");
	if (env) {
		int val = atoi(env);
		if (val > 0) sMaxTasks = val;
		if (val == 0) sMaxTasks = 0; /* unlimited */
	}

	env = getenv("SSB64_ACMD_TRACE_DIR");
	if (env && env[0]) {
		snprintf(sTraceDir, sizeof(sTraceDir), "%s", env);
	}

	if (!sEnabled) return;

	/* Create output directory */
	MKDIR(sTraceDir);

	/* Open the trace file */
	snprintf(sTraceFilePath, sizeof(sTraceFilePath), "%s/port_acmd_trace.acmd", sTraceDir);
	sTraceFile = fopen(sTraceFilePath, "w");
	if (!sTraceFile) {
		fprintf(stderr, "[acmd_trace] ERROR: cannot open %s for writing\n", sTraceFilePath);
		sEnabled = 0;
		return;
	}

	/* Header */
	fprintf(sTraceFile, "# Acmd Trace — SSB64 PC Port\n");
	fprintf(sTraceFile, "# Format: [cmd_index] A_OPCODE  w0=XXXXXXXX w1=XXXXXXXX  params...\n");
	fprintf(sTraceFile, "# Source: port (n_alAudioFrame Acmd buffer capture)\n");
	fprintf(sTraceFile, "#\n");
	fflush(sTraceFile);

	fprintf(stderr, "[acmd_trace] Trace enabled, writing to %s (max %d tasks)\n",
	        sTraceFilePath, sMaxTasks);
}

void acmd_trace_shutdown(void)
{
	if (sTraceFile) {
		fprintf(sTraceFile, "# END OF TRACE — %d audio tasks captured\n", sTaskNum);
		fclose(sTraceFile);
		sTraceFile = NULL;
	}
	sEnabled = 0;
	sInitialized = 0;
}

/* ========================================================================= */
/*  Runtime control                                                          */
/* ========================================================================= */

void acmd_trace_set_enabled(int enabled)
{
	if (enabled && !sInitialized) {
		sEnabled = 1;
		acmd_trace_init();
		return;
	}
	sEnabled = enabled;
}

int acmd_trace_is_enabled(void)
{
	return sEnabled && sTraceFile != NULL;
}

void acmd_trace_set_max_tasks(int max_tasks)
{
	sMaxTasks = max_tasks;
}

/* ========================================================================= */
/*  Task lifecycle                                                           */
/* ========================================================================= */

void acmd_trace_begin_task(void)
{
	if (!sEnabled || !sTraceFile) return;

	/* Check task limit */
	if (sMaxTasks > 0 && sTaskNum >= sMaxTasks) {
		if (sTaskNum == sMaxTasks) {
			fprintf(sTraceFile, "# TRACE STOPPED — task limit %d reached\n", sMaxTasks);
			fflush(sTraceFile);
			fprintf(stderr, "[acmd_trace] Task limit %d reached, stopping trace\n", sMaxTasks);
		}
		sTaskNum++;
		return;
	}

	fprintf(sTraceFile, "\n=== AUDIO TASK %d ===\n", sTaskNum);
	sCmdIndex = 0;
}

void acmd_trace_end_task(void)
{
	if (!sEnabled || !sTraceFile) return;
	if (sMaxTasks > 0 && sTaskNum >= sMaxTasks) {
		sTaskNum++;
		return;
	}

	fprintf(sTraceFile, "=== END AUDIO TASK %d — %d commands ===\n", sTaskNum, sCmdIndex);
	fflush(sTraceFile);
	sTaskNum++;
}

/* ========================================================================= */
/*  Command logging                                                          */
/* ========================================================================= */

void acmd_trace_log_cmd(uint32_t w0, uint32_t w1)
{
	char decoded[512];

	if (!sEnabled || !sTraceFile) return;
	if (sMaxTasks > 0 && sTaskNum >= sMaxTasks) return;

	acmd_decode_cmd(w0, w1, decoded, sizeof(decoded));
	fprintf(sTraceFile, "[%04d] %s\n", sCmdIndex, decoded);
	sCmdIndex++;
}

void acmd_trace_log_buffer(const void *cmd_list, int cmd_count)
{
	const uint32_t *words = (const uint32_t *)cmd_list;
	int i;

	if (!sEnabled || !sTraceFile) return;
	if (sMaxTasks > 0 && sTaskNum >= sMaxTasks) return;

	for (i = 0; i < cmd_count; i++) {
		uint32_t w0 = words[i * 2];
		uint32_t w1 = words[i * 2 + 1];
		acmd_trace_log_cmd(w0, w1);
	}
}
