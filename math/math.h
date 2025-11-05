#pragma once

#ifndef HP_MATH_MATH_H
#define HP_MATH_MATH_H

#include <types.h>

//for some reason the rounding constant is different than the one in the sdk???
#define FX_DEG_TO_IDX2(deg) ((u16) ((FX64C_65536_360 * (deg) + 0x80000000LL) >> 44))

inline int math_min(int a, int b)
{
	return a < b ? a : b;
}

inline int math_max(int a, int b)
{
	return a > b ? a : b;
}

inline int math_absInline(int a)
{
	return a > 0 ? a : -a;
}

// NDS's ARM processor does not have division.
// NDS instead uses memory-mapped registers to control division hardware.
// The logic for this function was taken from melonDS.
s64 Divide(s64 numerator, s64 denominator, int divideType) {
	if (divideType == 0) {
		numerator = (s32)numerator;
		denominator = (s32)denominator;
		if (denominator == 0) {
			// Divide by 0. This seems pretty weird, but it's what melonDS does.
			return numerator < 0 ? 0xffffffff00000001 : (int)-1;
			// Remainder is numerator.
		} else if ((u64)numerator == INT32_MIN && denominator == -1) {
			return INT32_MIN;
			// Remainder is not set. Melon bug? Idk.
		} else {
			return numerator / denominator;
			// Remainder is numerator % denominator.
		}
	} else if (divideType & 1 == 1) {
		denominator = (s32)denominator;
		if (denominator == 0) {
			return numerator < 0 ? 1 : (s64)-1;
			// Remainder is numerator.
		} else if (numerator == INT64_MAX && denominator == -1) {
			return INT64_MAX;
			// Remainder is 0.
		} else {
			return numerator / denominator;
			// Remainder is numerator % denominator.
		}
	} else if (divideType == 2) {
		if (denominator == 0) {
			return numerator < 0 ? 1 : (s64)-1;
			// Remainder is numerator.
		 } else if (numerator == INT64_MAX && denominator == -1) {
			return INT64_MAX;
			// Remainder is 0.
		} else {
			return numerator / denominator;
			// Remainder is numerator % denominator.
		}
	} else {
		// Anything else is invalid. MelonDS doesn't throw here, it just doesn't set any MM registers.
		throw "Invalid division type.";
	}
}

fx32 FxDivide(fx32 numerator, fx32 denominator) {
	s64 raw = Divide((s64)numerator << 0x20, (s32)denominator, 1);
	return (raw + 0x80000) >> (0x20 - FX_SHIFT);
}

#endif
