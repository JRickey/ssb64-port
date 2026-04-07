/**
 * m64p_plugin_api.h — Minimal Mupen64Plus plugin API definitions
 *
 * Extracted from the Mupen64Plus-Core project (GPLv2).
 * Only the types and function signatures needed by a GFX trace plugin.
 *
 * Full API reference: https://github.com/mupen64plus/mupen64plus-core
 */
#ifndef M64P_PLUGIN_API_H
#define M64P_PLUGIN_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/*  Types                                                                    */
/* ========================================================================= */

typedef void * m64p_dynlib_handle;
typedef void (*m64p_debug_callback)(void *context, int level, const char *message);

typedef enum {
	M64ERR_SUCCESS = 0,
	M64ERR_NOT_INIT,
	M64ERR_ALREADY_INIT,
	M64ERR_INCOMPATIBLE,
	M64ERR_INPUT_ASSERT,
	M64ERR_INPUT_INVALID,
	M64ERR_INPUT_NOT_FOUND,
	M64ERR_NO_MEMORY,
	M64ERR_FILES,
	M64ERR_INTERNAL,
	M64ERR_INVALID_STATE,
	M64ERR_PLUGIN_FAIL,
	M64ERR_SYSTEM_FAIL,
	M64ERR_UNSUPPORTED,
	M64ERR_WRONG_TYPE
} m64p_error;

typedef enum {
	M64PLUGIN_NULL = 0,
	M64PLUGIN_RSP = 1,
	M64PLUGIN_GFX = 2,
	M64PLUGIN_AUDIO = 3,
	M64PLUGIN_INPUT = 4,
	M64PLUGIN_CORE = 5
} m64p_plugin_type;

/* ========================================================================= */
/*  GFX Plugin Info Structure                                                */
/* ========================================================================= */

/**
 * This structure is passed to InitiateGFX by the core.
 * It provides access to the N64 memory and registers.
 */
typedef struct {
	/* Host address of N64 RDRAM (4MB or 8MB) */
	uint8_t  *RDRAM;
	/* Host address of N64 DMEM (4KB) */
	uint8_t  *DMEM;
	/* Host address of N64 IMEM (4KB) */
	uint8_t  *IMEM;

	/* Pointers to MI registers */
	uint32_t *MI_INTR_REG;

	/* Pointers to DPC (RDP command) registers */
	uint32_t *DPC_START_REG;
	uint32_t *DPC_END_REG;
	uint32_t *DPC_CURRENT_REG;
	uint32_t *DPC_STATUS_REG;
	uint32_t *DPC_CLOCK_REG;
	uint32_t *DPC_BUFBUSY_REG;
	uint32_t *DPC_PIPEBUSY_REG;
	uint32_t *DPC_TMEM_REG;

	/* Pointers to VI (video interface) registers */
	uint32_t *VI_STATUS_REG;
	uint32_t *VI_ORIGIN_REG;
	uint32_t *VI_WIDTH_REG;
	uint32_t *VI_INTR_REG;
	uint32_t *VI_V_CURRENT_LINE_REG;
	uint32_t *VI_TIMING_REG;
	uint32_t *VI_V_SYNC_REG;
	uint32_t *VI_H_SYNC_REG;
	uint32_t *VI_LEAP_REG;
	uint32_t *VI_H_START_REG;
	uint32_t *VI_V_START_REG;
	uint32_t *VI_V_BURST_REG;
	uint32_t *VI_X_SCALE_REG;
	uint32_t *VI_Y_SCALE_REG;

	/* SP (RSP) registers */
	uint32_t *SP_STATUS_REG;

	/* Callback for RDP interrupts */
	void (*CheckInterrupts)(void);
} GFX_INFO;

/* ========================================================================= */
/*  RSP Task structure in DMEM                                               */
/* ========================================================================= */

/**
 * OSTask structure as it appears at DMEM offset 0xFC0.
 * Fields are big-endian in N64 memory.
 */
#define TASK_DMEM_OFFSET  0xFC0

typedef struct {
	uint32_t type;          /* 0: M_GFXTASK=1, M_AUDTASK=2 */
	uint32_t flags;
	uint32_t ucode_boot;
	uint32_t ucode_boot_size;
	uint32_t ucode;
	uint32_t ucode_size;
	uint32_t ucode_data;
	uint32_t ucode_data_size;
	uint32_t dram_stack;
	uint32_t dram_stack_size;
	uint32_t output_buff;
	uint32_t output_buff_size;
	uint32_t data_ptr;       /* Display list start address */
	uint32_t data_size;
	uint32_t yield_data_ptr;
	uint32_t yield_data_size;
} OSTask_t;

#define M_GFXTASK  1
#define M_AUDTASK  2

/* ========================================================================= */
/*  Plugin export macros                                                     */
/* ========================================================================= */

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define CALL   __cdecl
#else
#define EXPORT __attribute__((visibility("default")))
#define CALL
#endif

#ifdef __cplusplus
}
#endif

#endif /* M64P_PLUGIN_API_H */
