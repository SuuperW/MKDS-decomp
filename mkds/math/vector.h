#pragma once

#ifndef HP_MATH_VECTOR_H
#define HP_MATH_VECTOR_H

#include "types.h"
#include <nitro/fx/fx_trig.h>

#define VEC_MAG_SQUARED(vec) ((fx64)((vec)->x) * (fx64)((vec)->x) + (fx64)((vec)->y) * (fx64)((vec)->y) + (fx64)((vec)->z) * (fx64)((vec)->z))

fx32 vec_normalize(const VecFx32* src, VecFx32* dst);
void VEC_CrossProduct_r(const VecFx32* a, const VecFx32* b, VecFx32* dst);
void VEC_CrossProduct_t(const VecFx32* a, const VecFx32* b, VecFx32* dst);

static inline fx32 vec_normalizeFastInline(const VecFx32* src, VecFx32* dst)
{
	if (src->x * (s64)src->x + src->y * (s64)src->y + src->z * (s64)src->z <
		FX32_CONST((1.f / 16) * (1.f / 16))) //below a length of 1/16 no normalization
		return 0;
	return vec_normalize(src, dst);
}

static inline void vec_toRenderSpace(const VecFx32* src, VecFx32* dst)
{
	dst->x = src->x >> 4;
	dst->y = src->y >> 4;
	dst->z = src->z >> 4;
}

static inline bool32 vec_isNearbyXZ(const VecFx32* vecA, const VecFx32* vecB, fx32 distance)
{
	fx32 deltaX = vecA->x - vecB->x;
	fx32 deltaZ = vecA->z - vecB->z;
	return deltaX * (s64)deltaX + deltaZ * (s64)deltaZ < (s64)distance * (s64)distance;
}

static inline u16 vec_toYAngle(const VecFx32* direction)
{
	return FX_Whole(fxMulT(FX32_CONST(10430.3782), FX_Atan2(direction->x, direction->z)));
}

static inline void vec_toVecFx16(const VecFx32* src, VecFx16* dst)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

static inline void vec_min(const VecFx32* a, const VecFx32* b, VecFx32* dst)
{
	dst->x = a->x < b->x ? a->x : b->x;
	dst->y = a->y < b->y ? a->y : b->y;
	dst->z = a->z < b->z ? a->z : b->z;
}

static inline void vec_max(const VecFx32* a, const VecFx32* b, VecFx32* dst)
{
	dst->x = a->x > b->x ? a->x : b->x;
	dst->y = a->y > b->y ? a->y : b->y;
	dst->z = a->z > b->z ? a->z : b->z;
}

static inline void VEC_Multiply_t(fx32 scalar, const VecFx32* in, VecFx32* out) {
	out->x = fxMulT(in->x, scalar);
	out->y = fxMulT(in->y, scalar);
	out->z = fxMulT(in->z, scalar);
}
static inline void VEC_Multiply_r(fx32 scalar, const VecFx32* in, VecFx32* out) {
	out->x = fxMulR(in->x, scalar);
	out->y = fxMulR(in->y, scalar);
	out->z = fxMulR(in->z, scalar);
}

static fx32 DotProduct_t(const VecFx32* a, const VecFx32* b) {
	return fxMulT(a->x, b->x) + fxMulT(a->y, b->y) + fxMulT(a->z, b->z);
}
static fx32 DotProduct_r(const VecFx32* a, const VecFx32* b) {
	return fxMulR(a->x, b->x) + fxMulR(a->y, b->y) + fxMulR(a->z, b->z);
}

extern void VEC_MultAdd_t(fx32 scalar, VecFx32* add, VecFx32* base, VecFx32* dst);
extern void VEC_MultSubtract_t(fx32 scalar, VecFx32* subtract, VecFx32* base, VecFx32* dst);

// NOTE: The dot product is rounded, the multiply is truncated.
static inline void VEC_Project(const VecFx32* vector, const VecFx32* normal, VecFx32* out) {
	// X = (N dot V) * N
	fx32 dot = DotProduct_r(vector, normal);
	VEC_Multiply_t(dot, normal, out);
}
// NOTE: The dot product is rounded, the multiply is truncated.
static inline void VEC_Rejection(const VecFx32* vector, const VecFx32* normal, VecFx32* out) {
	// X = V - (N dot V) * N
	fx32 dot = DotProduct_r(vector, normal);
	VecFx32 projection;
	VEC_Multiply_t(dot, normal, &projection);
	VEC_Subtract(vector, &projection, out);
}

#endif
