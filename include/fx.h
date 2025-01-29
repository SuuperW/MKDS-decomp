#pragma once

#ifndef SD_FH_H
#define SD_FH_H

#include <nitro/fx/fx.h>

static inline fx32 fxMulR(fx32 v1, fx32 v2) {
	return (fx32)(((s64)v1 * v2 + 0x800) >> FX_SHIFT);
}
static inline fx32 fxMulT(fx32 v1, fx32 v2) {
	return (fx32)(((s64)v1 * v2) >> FX_SHIFT);
}

#endif
