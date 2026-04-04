#pragma once

/**
 * port_types.h — Minimal type definitions for the bridge layer.
 *
 * The bridge needs N64 decomp types (u32, s32, etc.) but cannot include
 * the decomp's include/ directory because it shadows system headers
 * (assert.h, stdlib.h, string.h) and breaks C++ standard library includes.
 *
 * This header provides the same typedefs using <cstdint>, letting the
 * bridge compile as C++ while remaining ABI-compatible with the decomp code.
 */

#include <cstdint>
#include <cstddef>

// N64 SDK integer types (from include/PR/ultratypes.h)
// On MSVC (LLP64): unsigned long is 4 bytes, matching N64's u32.
// On GCC/Clang LP64: unsigned long is 8 bytes — use unsigned int instead.
#ifdef _MSC_VER
typedef unsigned long u32;
typedef long s32;
#else
typedef uint32_t u32;
typedef int32_t s32;
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int64_t s64;
typedef float f32;
typedef double f64;

// Boolean types from the decomp
typedef u8 ub8;
typedef u16 ub16;
typedef u32 ub32;
typedef s8 sb8;
typedef s16 sb16;
typedef s32 sb32;

// Macros the bridge needs (from include/macros.h)
#ifndef ARRAY_COUNT
#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif
