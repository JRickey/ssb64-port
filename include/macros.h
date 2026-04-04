#ifndef SSB_C_MACROS_H
#define SSB_C_MACROS_H

/// Floating Point Constants
#define FLOAT_MAX 3.4028235e38
#define FLOAT_NEG_MAX -FLOAT_MAX
#define FLOAT_MAX_HALF (FLOAT_MAX / 2) // FLOAT_MAX_HALF

#define F32_MAX 3.40282346639e+38F
#define F32_MIN (-F32_MAX)
#define F32_HALF (F32_MAX / 2)

#define QUART_PI32 0.7853982F
#define HALF_PI32 1.5707964F
#define PI32 3.1415927F
#define DOUBLE_PI32 6.2831855F

#define PI64 3.14159265358979323846
#define DTOR64 (PI64 / 180)
#define RTOD64 (180 / PI64)

#define DTOR32 ((float)DTOR64)
#define RTOD32 ((float)RTOD64)

#define COMBINE_INTEGRAL(a, b)   (((a)&0xffff0000) | (((b) >> 16)))
#define COMBINE_FRACTIONAL(a, b) (((a) << 16) | ((b)&0xffff))

#define SINTABLE_RAD_TO_ID(x)      ((s32)((x) * ((f32)ARRAY_COUNT(gSYSinTable) / PI32)))
#define SINTABLE_MASK_ID (ARRAY_COUNT(gSYSinTable) - 1)

#define ALIGNED(x) __attribute__((aligned(x)))
#define __attribute__(x)

/* This is very stupid. I cannot find a single PI or DTOR constant that works everywhere.
 * Need to alternate between F_CST_DTOR32 and F_CLC_DTOR32. Use with caution when matching.
 */

// Float convert degrees to radians - use this for immediate DTOR const multiplication
#define F_CST_DTOR32(x) ((float)((x) * DTOR32)) 

// Float convert degrees to radians - use this in functions that manually calculate each step of DTOR
#define F_CLC_DTOR32(x) ((float)(((x) * PI32) / 180.0F))

// Float convert radians to degrees
#define F_CST_RTOD32(x) ((float)((x) * RTOD32))

// Float convert radians to degrees
#define F_CLC_RTOD32(x) ((float)(((x) / PI32) * 180.0F))

// Integer convert degrees to radians
#define I_CST_DTOR32(x) (( int )((x) * DTOR32))

// Integer convert degrees to radians
#define I_CLC_DTOR32(x) (( int )(((x) * PI32) / 180.0F))

// Integer convert radians to degrees
#define I_CST_RTOD32(x) (( int )((x) * RTOD32))

// Integer convert radians to degrees
#define I_CLC_RTOD32(x) (( int )(((x) / PI32) * 180.0F))

// Float convert percentage to decimal notation
#define F_PCT_TO_DEC(x) ((float)((x)*0.01F))

#define I_CONTROLLER_RANGE_MAX 80									// Maximum control stick range in integer
#define F_CONTROLLER_RANGE_MAX ((float)I_CONTROLLER_RANGE_MAX)		// Maximum control stick range in float

#define TAKE_MAX(a, b) if ((a) < (b)) (a) = (b)
#define TAKE_MIN(a, b) if ((a) > (b)) (a) = (b)

// Bitfield macros to set up in-game bytecode commands (e.g. colanim events, CPU input scripts)
#define GC_BITFIELD(n) (1 << (n))
#define GC_BITMASK(len) (GC_BITFIELD(len) - 1)
#define GC_FIELDMASK(start, len) (GC_BITMASK(len) << (start))
#define GC_FIELDPREP(x, start, len) (((x)&GC_BITMASK(len)) << (start))
#define GC_FIELDSET(x, start, len)                                                                                     \
	(0 & ~GC_FIELDMASK(start, len) | GC_FIELDPREP(x, start, len)) // I'm too dumb to do it without the 0

#define UPDATE_INTERVAL (60)

#define TIME_SEC (UPDATE_INTERVAL * 1)
#define TIME_MIN (TIME_SEC * 60)
#define TIME_HRS (TIME_MIN * 60)

/* Count how many bits are in a type of a certain number
(WARNING: includes padding in structs that are not byte-aligned) */
#define NBITS(t) ((int) (sizeof(t) * 8) )

#define DISTANCE(a, b) (((a) < (b)) ? -((a) - (b)) : ((a) - (b)))

#define I_SEC_TO_TICS(q) 	((int)((q)*TIME_SEC))
#define I_TICS_TO_SEC(q) 	((int)((q) / TIME_SEC))
#define I_MIN_TO_SEC(q)  	((int)((q)*TIME_SEC))
#define I_MIN_TO_TICS(q) 	((int)((q)*TIME_MIN))
#define I_HRS_TO_TICS(q) 	((int)((q)*TIME_HRS))
#define I_HRS_TO_SEC(q)	 	((int)((q)*TIME_MIN))

#define F_TIME_TO_TICS(q, u) ((int)((q) * (u)))

#define F_SEC_TO_TICS(q) ((float)((q)*TIME_SEC))
#define F_FRAMES_TO_SEC(q) ((float)((q) / TIME_SEC))
#define F_MIN_TO_SEC(q) ((float)((q)*TIME_SEC))
#define F_MIN_TO_TICS(q) ((float)((q)*TIME_MIN))
#define F_HRS_TO_TICS(q) ((float)((q)*TIME_HRS))

#define I_TIME_TO_TICS(h, m, s, f) (I_HRS_TO_TICS(h) + I_MIN_TO_TICS(m) + I_SEC_TO_TICS(s) + (f))

#define U8_MAX 0xFF
#define S8_MAX 0x7F
#define S8_MIN 0x80

#define U16_MAX 0xFFFF
#define S16_MAX 0x7FFF
#define S16_MIN 0x8000

#define U32_MAX 0xFFFFFFFF
#define S32_MAX 0x7FFFFFFF
#define S32_MIN 0x80000000

/// Get the number of elements in a static array
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

/// Align value to size

#ifndef ALIGN
#define ALIGN(x, align) (((x) + ((align)-1)) & ~((align)-1))
#endif

/// Math Functions
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define BIQUAD(x) ((x) * (x) * (x) * (x))

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define ABSF(x) ((x) < 0.0F ? -(x) : (x))

// Helpful to have some defines that are explicitly float-sized
#define M_PI_F ((f32)M_PI)
#define M_DTOR_F(x) ((f32)((x) * (f32)M_DTOR))

/// Avoid compiler warnings for unused variables
#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

/// Apply compiler printf format checking to function
#ifdef __GNUC__
#define PRINTF_CHECK(fmtpos, vargpos) __attribute__((__format__(__printf__, (fmtpos), (vargpos))))
#else
#define PRINTF_CHECK(fmtpos, vargpos)
#endif

#define GLUE(a, b) a##b
#define GLUE2(a, b) GLUE(a, b)

/// Static assertions
#ifdef __GNUC__
#define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
#define STATIC_ASSERT(cond, msg) typedef char GLUE2(static_assertion_failed, __LINE__)[(cond) ? 1 : -1]
#endif

/// Convert from a physical addresss to a ROM (0xB0) address
#define PHYSICAL_TO_ROM(x) ((uintptr_t)(x) + 0xB0000000)

/// Turn off syntax check diagnostic
#ifdef __GNUC__
#define DO_PRAGMA(x) _Pragma(#x)
#define DIAGNOSTIC_SAVE() _Pragma("GCC diagnostic push")
#define DIAGNOSTIC_IGNORE(d) DO_PRAGMA(GCC diagnostic ignored d)
#define DIAGNOSTIC_RESTORE() _Pragma("GCC diagnostic pop")

#else
#define DIAGNOSTIC_SAVE()
#define DIAGNOSTIC_IGNORE(d)
#define DIAGNOSTIC_RESTORE()

#endif
#endif /* SSB_C_MACROS_H */
