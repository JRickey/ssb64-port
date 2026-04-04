#ifndef LIBC_MATH_H
#define LIBC_MATH_H

#include <PR/ultratypes.h>

#define M_PI 3.14159265358979323846f
#define M_DTOR (M_PI / 180.0f)
#define M_RTOD (180.0f / M_PI)
#define M_SQRT2 1.41421356237309504880f
#define M_SQRT1_2 0.70710678118654752440f /* 1/sqrt(2) */
#define FLT_MAX 340282346638528859811704183484516925440.0f
#define SHT_MAX 32767.0f
#define SHT_MINV (1.0f / SHT_MAX)

typedef union
{
	struct
	{
		u32 hi;
		u32 lo;
	} word;

	f64 d;
} du;

typedef union
{
	u32 i;
	f32 f;
} fu;

extern f32 __libm_qnan_f;

float fabsf(float f);
#pragma intrinsic(fabsf)
#ifdef __GNUC__
#define fabsf(f) __builtin_fabsf((float)(f))
#endif

double fabs(double f);
#pragma intrinsic(fabs)
#ifdef __GNUC__
#define fabs(f) __builtin_fabs((double)(f))
#endif

double sqrt(double d);
#pragma intrinsic(sqrt)

f32 __floorf(f32);
f64 __floor(f64);
s32 __lfloorf(f32);
s32 __lfloor(f64);
f32 __ceilf(f32);
f64 __ceil(f64);
s32 __lceilf(f32);
s32 __lceil(f64);
f32 __truncf(f32);
f64 __trunc(f64);
s32 __ltruncf(f32);
s32 __ltrunc(f64);
f32 __roundf(f32);
f64 __round(f64);
s32 __lroundf(f32);
s32 __lround(f64);
f32 __nearbyintf(f32);
f64 __nearbyint(f64);
s32 __lnearbyintf(f32);
s32 __lnearbyint(f64);

#endif
