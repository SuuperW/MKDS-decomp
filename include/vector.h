#pragma once

// Although we have #pragma once, we must also have traditional header guards because Ghidra does not undertand #pragma once.
#ifndef SD_VECTOR_H
#define SD_VECTOR_H

#include <nitro/fx/fx_vec.h>

#include <fx.h>

static inline void MultiplyVector_T(const VecFx32* in, fx32 scalar, VecFx32* out) {
	out->x = fxMulT(in->x, scalar);
	out->y = fxMulT(in->y, scalar);
	out->z = fxMulT(in->z, scalar);
}
static inline void MultiplyVector_R(const VecFx32* in, fx32 scalar, VecFx32* out) {
	out->x = fxMulR(in->x, scalar);
	out->y = fxMulR(in->y, scalar);
	out->z = fxMulR(in->z, scalar);
}

static fx32 DotProduct_t(const VecFx32* a, VecFx32* b) {
	return fxMulT(a->x, b->x) + fxMulT(a->y, b->y) + fxMulT(a->z, b->z);
}
static fx32 DotProduct_r(const VecFx32* a, VecFx32* b) {
	return fxMulR(a->x, b->x) + fxMulR(a->y, b->y) + fxMulR(a->z, b->z);
}

#endif
