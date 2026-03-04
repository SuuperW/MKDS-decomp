#pragma once

#ifndef HP_MATH_MATH_H
#define HP_MATH_MATH_H

#include "types.h"
#include <nitro/math/rand.h>

//for some reason the rounding constant is different than the one in the sdk???
#define FX_DEG_TO_IDX2(deg) ((u16) ((FX64C_65536_360 * (deg) + 0x80000000LL) >> 44))

extern fx16 FX_SinCosTable[4096][2];

inline int min(int a, int b)
{
	return a < b ? a : b;
}

inline int max(int a, int b)
{
	return a > b ? a : b;
}

inline int math_absInline(int a)
{
	return a > 0 ? a : -a;
}

extern int AbsoluteValue(int value);

// NDS's ARM processor does not have division.
// NDS instead uses memory-mapped registers to control division hardware.
// The logic for this function was taken from melonDS.
static s64 Divide(s64 numerator, s64 denominator, int divideType);
extern fx32 FxDivide(fx32 numerator, fx32 denominator);

extern int sqrt(u64 in);

// A random fixed point value in the range [0,1)
static inline fx32 MATH_RandFx(MATHRandContext32* context)
{
	return MATH_Rand32(context, 0) >> 20;
}

#endif
