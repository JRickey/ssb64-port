/**
 * gfx_trace_plugin.c — Mupen64Plus GFX trace plugin
 *
 * A minimal Mupen64Plus video plugin that renders nothing but captures
 * every F3DEX2/RDP display list command from RDRAM. Output format matches
 * the port-side gbi_trace system for comparison.
 *
 * Build:
 *   gcc -shared -o mupen64plus-video-trace.dll gfx_trace_plugin.c ../gbi_trace/gbi_decoder.c -I..
 *   (Linux: gcc -shared -fPIC -o mupen64plus-video-trace.so ...)
 *
 * Usage:
 *   mupen64plus --gfx mupen64plus-video-trace.dll <rom>
 *   Output: emu_trace.gbi in current directory (or M64P_TRACE_DIR env var)
 *
 * The plugin implements ProcessDList by reading the OSTask from DMEM,
 * extracting the display list start address, and walking the DL tree
 * through RDRAM — exactly as the RSP would.
 */
#include "m64p_plugin_api.h"
#include "gbi_trace/gbi_decoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================= */
/*  State                                                                    */
/* ========================================================================= */

static GFX_INFO  sGfxInfo;
static FILE     *sTraceFile   = NULL;
static int       sFrameNum    = 0;
static int       sCmdIndex    = 0;
static int       sMaxFrames   = 300;
static int       sInitialized = 0;
static char      sTraceDir[512] = ".";

/* N64 segment table — maintained by tracking G_MOVEWORD SEGMENT commands */
static uint32_t  sSegmentTable[16];

/* DL call stack for walking sub-display-lists */
#define DL_STACK_MAX 32
static uint32_t  sDLStack[DL_STACK_MAX];
static int       sDLStackTop = 0;

/* ========================================================================= */
/*  Big-endian memory helpers                                                */
/* ========================================================================= */

/**
 * Read a big-endian 32-bit word from RDRAM at the given N64 physical address.
 */
static uint32_t rdram_read32(uint32_t addr)
{
	uint8_t *p;
	addr &= 0x007FFFFF;  /* Mask to 8MB RDRAM range */
	p = sGfxInfo.RDRAM + addr;
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
	       ((uint32_t)p[2] << 8)  | (uint32_t)p[3];
}

/**
 * Read a big-endian 32-bit word from DMEM at the given offset.
 */
static uint32_t dmem_read32(uint32_t offset)
{
	uint8_t *p;
	offset &= 0x0FFF;
	p = sGfxInfo.DMEM + offset;
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
	       ((uint32_t)p[2] << 8)  | (uint32_t)p[3];
}

/**
 * Resolve an N64 segment address to a physical RDRAM address.
 * Segment address format: 0x0S??????  where S = segment number.
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

/**
 * Walk one display list starting at the given N64 physical address.
 * Recursively follows G_DL CALL commands, branches for G_DL BRANCH.
 * Stops at G_ENDDL or after a safety limit of commands.
 */
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
				/* Branch — replace current DL */
				addr = target;
				continue;
			} else {
				/* Call — recurse into sub-DL, then continue */
				if (depth < DL_STACK_MAX - 1) {
					walk_display_list(target, depth + 1);
				}
			}
		} else if (opcode == GBI_G_ENDDL) {
			/* Return from this DL */
			return;
		} else if (opcode == GBI_G_TEXRECT || opcode == GBI_G_TEXRECTFLIP) {
			/* TEXRECT is a 3-word command: skip the two RDPHALF words that follow.
			 * On N64, G_TEXRECT is followed by G_RDPHALF_1 and G_RDPHALF_2
			 * containing S/T coordinates and dS/dT gradients. */
			addr += 8;
			if (sTraceFile) {
				uint32_t half1_w0 = rdram_read32(addr);
				uint32_t half1_w1 = rdram_read32(addr + 4);
				char dec1[512];
				gbi_decode_cmd(half1_w0, half1_w1, dec1, sizeof(dec1));
				fprintf(sTraceFile, "[%04d] d=%d %s\n", sCmdIndex, depth, dec1);
				sCmdIndex++;
			}
			addr += 8;
			if (sTraceFile) {
				uint32_t half2_w0 = rdram_read32(addr);
				uint32_t half2_w1 = rdram_read32(addr + 4);
				char dec2[512];
				gbi_decode_cmd(half2_w0, half2_w1, dec2, sizeof(dec2));
				fprintf(sTraceFile, "[%04d] d=%d %s\n", sCmdIndex, depth, dec2);
				sCmdIndex++;
			}
		}

		/* Advance to next command (each Gfx is 8 bytes) */
		addr += 8;
	}
}

/* ========================================================================= */
/*  Plugin API implementation                                                */
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

	return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginShutdown(void)
{
	if (sTraceFile) {
		fprintf(sTraceFile, "# END OF TRACE — %d frames captured\n", sFrameNum);
		fclose(sTraceFile);
		sTraceFile = NULL;
	}
	sInitialized = 0;
	return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *type, int *version,
                                        int *api_version, const char **name,
                                        int *caps)
{
	if (type)        *type = M64PLUGIN_GFX;
	if (version)     *version = 0x010000;   /* 1.0.0 */
	if (api_version) *api_version = 0x020600; /* GFX API 2.6.0 */
	if (name)        *name = "GBI Trace Plugin";
	if (caps)        *caps = 0;
	return M64ERR_SUCCESS;
}

EXPORT int CALL InitiateGFX(GFX_INFO gfx_info)
{
	char path[1024];

	sGfxInfo = gfx_info;
	memset(sSegmentTable, 0, sizeof(sSegmentTable));
	sFrameNum = 0;

	/* Open trace file */
	snprintf(path, sizeof(path), "%s/emu_trace.gbi", sTraceDir);
	sTraceFile = fopen(path, "w");
	if (!sTraceFile) {
		fprintf(stderr, "[gfx_trace] ERROR: cannot open %s\n", path);
		return 0;
	}

	fprintf(sTraceFile, "# GBI Trace — Mupen64Plus Emulator\n");
	fprintf(sTraceFile, "# Format: [cmd_index] d=depth OPCODE  w0=XXXXXXXX w1=XXXXXXXX  params...\n");
	fprintf(sTraceFile, "# Source: emu (RDRAM display list walk)\n");
	fprintf(sTraceFile, "#\n");
	fflush(sTraceFile);

	fprintf(stderr, "[gfx_trace] Trace plugin initialized, writing to %s\n", path);
	return 1; /* success */
}

EXPORT void CALL RomOpen(void)
{
	/* Nothing needed */
}

EXPORT void CALL RomClosed(void)
{
	/* Nothing needed */
}

/**
 * ProcessDList — called by Mupen64Plus when the game submits a GFX task.
 * This is where we walk the display list and log every command.
 */
EXPORT void CALL ProcessDList(void)
{
	uint32_t dl_addr;

	if (!sTraceFile) return;
	if (sMaxFrames > 0 && sFrameNum >= sMaxFrames) return;

	/* Read the OSTask from DMEM to find the display list start address */
	dl_addr = dmem_read32(TASK_DMEM_OFFSET + 48); /* data_ptr field (offset 0x30 = 48) */

	/* Resolve segment address if needed */
	if ((dl_addr >> 24) > 0x00 && (dl_addr >> 24) < 0x10) {
		dl_addr = resolve_segment(dl_addr);
	}
	dl_addr &= 0x007FFFFF;

	/* Begin frame */
	fprintf(sTraceFile, "\n=== FRAME %d ===\n", sFrameNum);
	sCmdIndex = 0;
	sDLStackTop = 0;

	/* Walk the display list tree */
	walk_display_list(dl_addr, 0);

	/* End frame */
	fprintf(sTraceFile, "=== END FRAME %d — %d commands ===\n", sFrameNum, sCmdIndex);
	fflush(sTraceFile);
	sFrameNum++;
}

EXPORT void CALL ProcessRDPList(void)
{
	/* Not needed for DL tracing */
}

EXPORT void CALL ShowCFB(void)
{
	/* Not applicable */
}

/**
 * UpdateScreen — called by Mupen64Plus on VI interrupt (once per frame).
 * We don't render anything, so this is a no-op.
 */
EXPORT void CALL UpdateScreen(void)
{
	/* No-op — we don't render */
}

EXPORT void CALL ViStatusChanged(void)
{
	/* No-op */
}

EXPORT void CALL ViWidthChanged(void)
{
	/* No-op */
}

EXPORT void CALL ChangeWindow(void)
{
	/* No-op */
}

EXPORT void CALL ReadScreen2(void *dest, int *width, int *height, int front)
{
	if (width) *width = 320;
	if (height) *height = 240;
}

EXPORT void CALL SetRenderingCallback(void (*callback)(int))
{
	/* No-op */
}

EXPORT void CALL ResizeVideoOutput(int width, int height)
{
	/* No-op */
}

EXPORT void CALL FBRead(unsigned int addr)
{
	/* No-op */
}

EXPORT void CALL FBWrite(unsigned int addr, unsigned int size)
{
	/* No-op */
}

EXPORT void CALL FBGetFrameBufferInfo(void *info)
{
	/* No-op */
}
