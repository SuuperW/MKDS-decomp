#pragma once

#ifndef HP_INCLUDE_TYPES_H
#define HP_INCLUDE_TYPES_H

// Although we have #pragma once, we must also have traditional header guards because Ghidra does not undertand #pragma once.
#ifndef HPS_TYPES_H
#define HPS_TYPES_H

#pragma pack(4) // Redundant with compiler args, but necessary for VSCode's stupid "intelli"Sense

// Some static assertions to validate compiler behavior.
struct __test__pack {
	int a;
	long long b;
};
static_assert(sizeof(struct __test__pack) == 12);
static_assert('abcd' == 0x61626364);

#include <stdint.h>
#include <stdbool.h>
#include <nitro/fx/fx_vec.h>

#define BIT(n) (1 << (n))

#ifdef GHIDRA_PARSER
// I do not understand what C is doing. It's a mess.
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef uint8_t __uint8_t;
typedef uint16_t __uint16_t;
typedef uint32_t __uint32_t;
typedef uint64_t __uint64_t;
typedef int8_t __int8_t;
typedef int16_t __int16_t;
typedef int32_t __int32_t;
typedef int64_t __int64_t;
#endif


typedef uint8_t uint8;
typedef uint8_t  byte;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef volatile uint8_t vuint8;
typedef volatile uint16_t vuint16;
typedef volatile uint32_t vuint32;
typedef volatile uint64_t vuint64;
typedef volatile int8_t vint8;
typedef volatile int16_t vint16;
typedef volatile int32_t vint32;
typedef volatile int64_t vint64;

typedef u32 bool32;

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// NDS has no floats, only fixed-point values
static inline fx32 fxMulR(fx32 v1, fx32 v2) {
	return (fx32)(((s64)v1 * v2 + 0x800) >> FX_SHIFT);
}
static inline fx32 fxMulT(fx32 v1, fx32 v2) {
	return (fx32)(((s64)v1 * v2) >> FX_SHIFT);
}

#define FLOAT_TO_FX32(x) ((fx32)(((x) > 0) ? \
                                 ((x) * FX_ONE + 0.5f ) : \
                                 ((x) * FX_ONE - 0.5f )))

#ifdef __cplusplus
extern "C" {
#endif

//fx10.22 (used for intermediate values in collision checks)
typedef s32 fx10_22;
#define FX10_22_SHIFT 22
#define FX_MUL_10(v1, v2) (fx10_22)(((s64)(v1) * (v2)) >> 2)

typedef struct Quaternion { fx32 k, j, i, r; } Quaternion;
typedef struct Orientation { VecFx32 left, up, forward; } Orientation; // Also used as a transform matrix.
typedef struct Orientation4D { VecFx32 left, up, forward, fourth; } Orientation4D;

#ifdef __cplusplus
}
#endif
#endif

#endif
