#pragma once

#ifndef HP_COMMON_H
#define HP_COMMON_H

// #if __MWERKS__ > 0
// #define ATTRIBUTE_NEVER_INLINE __attribute__((never_inline))
// #ifdef    __cplusplus
// #include "../build/common.mch++"		/* If a compile error occurs in this line, please precompile "Nitro_TS.pch++" */
// #else
// /* include precompiled header for C source */
// #include "../build/common.mch"		/* If a compile error occurs in this line, please precompile "Nitro_TS.pch" */
// #endif
// #else
// #define ATTRIBUTE_NEVER_INLINE
// #define ATTRIBUTE_ALIGN(x)
// #ifdef    __cplusplus
// #include "common.pch++"
// #else
// /* include precompiled header for C source */
// #include "common.pch"
// #endif
// #endif

#include "types.h"
#include <nitro/mi/memory.h>

//check nitro system version - HP use a version from late 2007? But MKDS was released in 2005, Nov (US) - Dec (JP).
#include <nnsys/version.h>
//static_assert(NNSYS_VERSION_DATE == 20071126);
static_assert(NNSYS_VERSION_DATE == 20050901);

typedef u8 uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;

typedef s8 int8_t;
typedef s16 int16_t;
typedef s32 int32_t;
typedef s64 int64_t;

typedef struct
{
	fx32 x;
	fx32 y;
} vec2_t;

//fix for changes in the sdk we currently use
static inline void MI_CpuFillFast_fix(void* dest, u32 data, u32 size)
{
	vuint32 tmp = (vuint32)data;

	SDK_ASSERTMSG((size & 3) == 0, "size & 3 must be 0");
	SDK_ASSERTMSG(((u32)dest & 3) == 0, "source address must be in 4-byte alignment");

	MIi_CpuClearFast(tmp, dest, size);
}

#define MI_CpuFillFast(a,b,c) MI_CpuFillFast_fix(a,b,c)

static inline void MI_CpuClearFast_fix(void* dest, u32 size)
{
	MI_CpuFillFast(dest, 0, size);
}

#define MI_CpuClearFast(a,b) MI_CpuClearFast_fix(a,b)

#include "mkds_rom_config.h"
#include "snd/sfxIds.h"
#include "particles/particleIds.h"

#endif
