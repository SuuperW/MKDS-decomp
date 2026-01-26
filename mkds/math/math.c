#include "math.h"

int AbsoluteValue(int value)
{
  if (value < 1) return -value;
  return value;
}

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
	} else if ((divideType & 1) == 1) {
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
		// Anything else is invalid. MelonDS just doesn't set any MM registers. (has no return)
		return 0;
	}
}

fx32 FxDivide(fx32 numerator, fx32 denominator) {
	s64 raw = Divide((s64)numerator << 0x20, (s32)denominator, 1);
	return (raw + 0x80000) >> (0x20 - FX_SHIFT);
}
