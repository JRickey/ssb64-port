/**
 * rsp_trace_plugin.c — Mupen64Plus RSP trace plugin
 *
 * A minimal RSP plugin that intercepts GFX and audio tasks, reading
 * display lists / audio command lists from RDRAM to produce structured
 * trace files for comparison against the PC port.
 *
 * This runs as an RSP plugin (not GFX) because the RSP plugin's DoRspCycles
 * is called with DMEM intact, before the HLE overwrites it with ucode data.
 *
 * Build:
 *   cmake -S . -B build && cmake --build build --config Release
 *
 * Usage:
 *   mupen64plus --rsp mupen64plus-rsp-trace.dll --gfx <any_video_plugin> <rom>
 *   Output: emu_trace.gbi + emu_acmd_trace.acmd (in M64P_TRACE_DIR or cwd)
 *
 * Env vars:
 *   M64P_TRACE_DIR=path        Output directory (default: ".")
 *   M64P_TRACE_FRAMES=N        Max GFX frames to trace (0=unlimited, default 300)
 *   M64P_ACMD_TRACE=1          Enable audio command tracing (default: off)
 *   M64P_ACMD_TRACE_TASKS=N    Max audio tasks to trace (0=unlimited, default 600)
 */
#include "m64p_plugin_api.h"
#include "gbi_trace/gbi_decoder.h"
#include "acmd_trace/acmd_decoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================= */
/*  State — GBI (display list) tracing                                       */
/* ========================================================================= */

static RSP_INFO  sRspInfo;
static FILE     *sTraceFile   = NULL;
static int       sFrameNum    = 0;
static int       sCmdIndex    = 0;
static int       sMaxFrames   = 300;
static int       sInitialized = 0;
static char      sTraceDir[512] = ".";

/* N64 segment table — maintained by tracking G_MOVEWORD SEGMENT commands */
static uint32_t  sSegmentTable[16];

/* ========================================================================= */
/*  State — Acmd (audio command) tracing                                     */
/* ========================================================================= */

static FILE     *sAcmdTraceFile  = NULL;
static int       sAcmdTaskNum    = 0;
static int       sAcmdCmdIndex   = 0;
static int       sAcmdMaxTasks   = 600;  /* default: 10 seconds @ 60 tasks/sec */
static int       sAcmdEnabled    = 0;

/* ========================================================================= */
/*  Memory access — M64P stores memory in native byte order                  */
/* ========================================================================= */

/**
 * Read a 32-bit word from DMEM at the given absolute DMEM offset (0x000-0xFFF).
 * M64P stores SP memory as uint32_t[] in native byte order.
 */
static uint32_t dmem_read32(uint32_t offset)
{
	offset &= 0x0FFF;
	return *(uint32_t*)(sRspInfo.DMEM + offset);
}

/**
 * Read a 32-bit word from RDRAM at the given N64 physical address.
 * M64P stores RDRAM as uint32_t[] in native byte order.
 */
static uint32_t rdram_read32(uint32_t addr)
{
	addr &= 0x007FFFFF;  /* Mask to 8MB RDRAM range */
	/* Align to 4-byte boundary */
	addr &= ~3u;
	return *(uint32_t*)(sRspInfo.RDRAM + addr);
}

/**
 * Resolve an N64 segment address to a physical RDRAM address.
 */
static uint32_t resolve_segment(uint32_t segaddr)
{
	uint32_t seg = (segaddr >> 24) & 0x0F;
	uint32_t off = segaddr & 0x00FFFFFF;
	return (sSegmentTable[seg] & 0x00FFFFFF) + off;
}

/* ========================================================================= */
/*  Display list walker                                                      */
/* ========================================================================= */

static void walk_display_list(uint32_t phys_addr, int depth)
{
	uint32_t addr = phys_addr;
	int safety = 0;
	const int MAX_CMDS_PER_DL = 100000;

	while (safety++ < MAX_CMDS_PER_DL) {
		uint32_t w0 = rdram_read32(addr);
		uint32_t w1 = rdram_read32(addr + 4);
		uint8_t opcode = (uint8_t)(w0 >> 24);

		/* Decode and log */
		if (sTraceFile) {
			char decoded[512];
			gbi_decode_cmd(w0, w1, decoded, sizeof(decoded));
			fprintf(sTraceFile, "[%04d] d=%d %s\n", sCmdIndex, depth, decoded);
			sCmdIndex++;
		}

		/* Track segment table updates */
		if (opcode == GBI_G_MOVEWORD) {
			uint32_t index = (w0 >> 16) & 0xFF;
			if (index == 0x06) {  /* G_MW_SEGMENT */
				uint32_t seg = ((w0 & 0xFFFF) >> 2) & 0x0F;
				sSegmentTable[seg] = w1;
			}
		}

		/* Handle display list control flow */
		if (opcode == GBI_G_DL) {
			uint32_t target = resolve_segment(w1);
			if (gbi_dl_is_branch(w0)) {
				/* Branch — replace current DL, continue from target */
				addr = target;
				continue;
			} else {
				/* Call — recurse into sub-DL, then continue here */
				if (depth < 30) {
					walk_display_list(target, depth + 1);
				}
			}
		} else if (opcode == GBI_G_ENDDL) {
			return;
		} else if (opcode == GBI_G_TEXRECT || opcode == GBI_G_TEXRECTFLIP) {
			/* TEXRECT is a 3-word command: log the two RDPHALF words that follow */
			addr += 8;
			if (sTraceFile) {
				uint32_t hw0 = rdram_read32(addr);
				uint32_t hw1 = rdram_read32(addr + 4);
				char dec[512];
				gbi_decode_cmd(hw0, hw1, dec, sizeof(dec));
				fprintf(sTraceFile, "[%04d] d=%d %s\n", sCmdIndex, depth, dec);
				sCmdIndex++;
			}
			addr += 8;
			if (sTraceFile) {
				uint32_t hw0 = rdram_read32(addr);
				uint32_t hw1 = rdram_read32(addr + 4);
				char dec[512];
				gbi_decode_cmd(hw0, hw1, dec, sizeof(dec));
				fprintf(sTraceFile, "[%04d] d=%d %s\n", sCmdIndex, depth, dec);
				sCmdIndex++;
			}
		}

		addr += 8;
	}
}

/* ========================================================================= */
/*  Audio command list walker                                                */
/* ========================================================================= */

/**
 * Walk a linear Acmd list in RDRAM and log each command.
 * Audio command lists are flat (no branching/recursion like GBI DLs).
 * Each Acmd is 8 bytes (w0 + w1).
 */
static void walk_audio_list(uint32_t phys_addr, uint32_t data_size)
{
	uint32_t addr = phys_addr;
	/* data_size is in bytes; each Acmd is 8 bytes */
	int max_cmds = (data_size > 0) ? (int)(data_size / 8) : 4096;
	int i;

	for (i = 0; i < max_cmds; i++) {
		uint32_t w0 = rdram_read32(addr);
		uint32_t w1 = rdram_read32(addr + 4);

		if (sAcmdTraceFile) {
			char decoded[512];
			acmd_decode_cmd(w0, w1, decoded, sizeof(decoded));
			fprintf(sAcmdTraceFile, "[%04d] %s\n", sAcmdCmdIndex, decoded);
			sAcmdCmdIndex++;
		}

		addr += 8;
	}
}

/* ========================================================================= */
/*  RSP Plugin API                                                           */
/* ========================================================================= */

EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle handle,
                                     void *context,
                                     m64p_debug_callback debug_cb)
{
	const char *env;

	if (sInitialized) return M64ERR_ALREADY_INIT;
	sInitialized = 1;

	env = getenv("M64P_TRACE_DIR");
	if (env && env[0]) {
		snprintf(sTraceDir, sizeof(sTraceDir), "%s", env);
	}

	env = getenv("M64P_TRACE_FRAMES");
	if (env) {
		int val = atoi(env);
		if (val > 0) sMaxFrames = val;
		if (val == 0) sMaxFrames = 0;
	}

	/* Audio trace config */
	env = getenv("M64P_ACMD_TRACE");
	if (env && (env[0] == '1' || env[0] == 'y' || env[0] == 'Y')) {
		sAcmdEnabled = 1;
	}

	env = getenv("M64P_ACMD_TRACE_TASKS");
	if (env) {
		int val = atoi(env);
		if (val > 0) sAcmdMaxTasks = val;
		if (val == 0) sAcmdMaxTasks = 0;
	}

	return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginShutdown(void)
{
	if (sTraceFile) {
		fprintf(sTraceFile, "# END OF TRACE — %d frames captured\n", sFrameNum);
		fclose(sTraceFile);
		sTraceFile = NULL;
	}
	if (sAcmdTraceFile) {
		fprintf(sAcmdTraceFile, "# END OF TRACE — %d audio tasks captured\n", sAcmdTaskNum);
		fclose(sAcmdTraceFile);
		sAcmdTraceFile = NULL;
	}
	sInitialized = 0;
	return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *type, int *version,
                                        int *api_version, const char **name,
                                        int *caps)
{
	if (type)        *type = M64PLUGIN_RSP;
	if (version)     *version = 0x010000;     /* 1.0.0 */
	if (api_version) *api_version = 0x020000; /* RSP API 2.0.0 */
	if (name)        *name = "GBI + Acmd Trace RSP Plugin";
	if (caps)        *caps = 0;
	return M64ERR_SUCCESS;
}

EXPORT void CALL InitiateRSP(RSP_INFO rsp_info, unsigned int *cycle_count)
{
	char path[1024];

	sRspInfo = rsp_info;
	memset(sSegmentTable, 0, sizeof(sSegmentTable));
	sFrameNum = 0;

	/* Open trace file */
	snprintf(path, sizeof(path), "%s/emu_trace.gbi", sTraceDir);
	sTraceFile = fopen(path, "w");
	if (!sTraceFile) {
		fprintf(stderr, "[rsp_trace] ERROR: cannot open %s\n", path);
		return;
	}

	fprintf(sTraceFile, "# GBI Trace — Mupen64Plus Emulator (RSP Plugin)\n");
	fprintf(sTraceFile, "# Format: [cmd_index] d=depth OPCODE  w0=XXXXXXXX w1=XXXXXXXX  params...\n");
	fprintf(sTraceFile, "# Source: emu (RDRAM display list walk via RSP task interception)\n");
	fprintf(sTraceFile, "#\n");
	fflush(sTraceFile);

	fprintf(stderr, "[rsp_trace] GBI trace initialized, writing to %s (max %d frames)\n",
	        path, sMaxFrames);

	/* Open audio trace file if enabled */
	if (sAcmdEnabled) {
		snprintf(path, sizeof(path), "%s/emu_acmd_trace.acmd", sTraceDir);
		sAcmdTraceFile = fopen(path, "w");
		if (!sAcmdTraceFile) {
			fprintf(stderr, "[rsp_trace] ERROR: cannot open %s\n", path);
		} else {
			fprintf(sAcmdTraceFile, "# Acmd Trace — Mupen64Plus Emulator (RSP Plugin)\n");
			fprintf(sAcmdTraceFile, "# Format: [cmd_index] A_OPCODE  w0=XXXXXXXX w1=XXXXXXXX  params...\n");
			fprintf(sAcmdTraceFile, "# Source: emu (RDRAM audio command list walk via RSP task interception)\n");
			fprintf(sAcmdTraceFile, "#\n");
			fflush(sAcmdTraceFile);
			fprintf(stderr, "[rsp_trace] Acmd trace initialized, writing to %s (max %d tasks)\n",
			        path, sAcmdMaxTasks);
		}
	}

	if (cycle_count) *cycle_count = 0;
}

/**
 * DoRspCycles — called by M64P when the game triggers the RSP.
 * DMEM contains the intact OSTask at this point.
 */
EXPORT unsigned int CALL DoRspCycles(unsigned int cycles)
{
	uint32_t task_type, dl_addr;

	/* Read task type from DMEM (native byte order) */
	task_type = dmem_read32(TASK_TYPE);

	/* Validate: RSP HLE checks ucode_boot_size <= 0x1000 to detect tasks.
	 * If boot_size is too large, RSP was triggered for a non-task reason. */
	{
		uint32_t boot_size = dmem_read32(TASK_UCODE_BOOT_SIZE);
		if (boot_size > 0x1000) {
			goto done;
		}
	}

	/* Handle audio tasks */
	if (task_type == M_AUDTASK) {
		/* Trace audio command list if enabled */
		if (sAcmdEnabled && sAcmdTraceFile &&
		    (sAcmdMaxTasks == 0 || sAcmdTaskNum < sAcmdMaxTasks))
		{
			uint32_t alist_addr = dmem_read32(TASK_DATA_PTR);
			uint32_t alist_size = dmem_read32(TASK_DATA_SIZE);

			alist_addr &= 0x00FFFFFF; /* Strip KSEG0/KSEG1 */

			fprintf(sAcmdTraceFile, "\n=== AUDIO TASK %d ===\n", sAcmdTaskNum);
			sAcmdCmdIndex = 0;

			walk_audio_list(alist_addr, alist_size);

			fprintf(sAcmdTraceFile, "=== END AUDIO TASK %d — %d commands ===\n",
			        sAcmdTaskNum, sAcmdCmdIndex);
			fflush(sAcmdTraceFile);
			sAcmdTaskNum++;

			if (sAcmdMaxTasks > 0 && sAcmdTaskNum >= sAcmdMaxTasks) {
				fprintf(sAcmdTraceFile, "# TRACE STOPPED — task limit %d reached\n",
				        sAcmdMaxTasks);
				fflush(sAcmdTraceFile);
			}
		}

		/* Still let the audio plugin process normally */
		if (sRspInfo.ProcessAList) {
			sRspInfo.ProcessAList();
		}
		goto done;
	}

	/* Only trace GFX tasks (type 1) below this point */
	if (task_type != M_GFXTASK) {
		goto done;
	}

	if (!sTraceFile) goto done;
	if (sMaxFrames > 0 && sFrameNum >= sMaxFrames) goto done;

	/* Read display list start address from DMEM */
	dl_addr = dmem_read32(TASK_DATA_PTR);

	/* Strip KSEG0/KSEG1 prefix */
	dl_addr &= 0x00FFFFFF;

	/* Begin frame trace */
	fprintf(sTraceFile, "\n=== FRAME %d ===\n", sFrameNum);
	sCmdIndex = 0;

	/* Walk the display list */
	walk_display_list(dl_addr, 0);

	/* End frame */
	fprintf(sTraceFile, "=== END FRAME %d — %d commands ===\n", sFrameNum, sCmdIndex);
	fflush(sTraceFile);
	sFrameNum++;

	/* Tell the GFX plugin to process (for VI updates etc.) */
	if (sRspInfo.ProcessDList) {
		sRspInfo.ProcessDList();
	}

done:
	/* Signal RSP task done: HALT + BROKE, then SP interrupt */
	if (sRspInfo.SP_STATUS_REG) {
		*sRspInfo.SP_STATUS_REG |= SP_STATUS_HALT | SP_STATUS_BROKE;
	}
	if (sRspInfo.MI_INTR_REG) {
		/* MI_INTR_SP (bit 0) — RSP finished */
		*sRspInfo.MI_INTR_REG |= 0x01;
		/* MI_INTR_DP (bit 5) — for GFX tasks, also signal RDP completion.
		 * The game waits for both SP and DP interrupts after a GFX task. */
		if (task_type == M_GFXTASK) {
			*sRspInfo.MI_INTR_REG |= 0x20;
		}
	}
	if (sRspInfo.CheckInterrupts) {
		sRspInfo.CheckInterrupts();
	}

	return cycles;
}

EXPORT void CALL RomClosed(void)
{
	/* Reset segment table */
	memset(sSegmentTable, 0, sizeof(sSegmentTable));
	sAcmdTaskNum = 0;
}
