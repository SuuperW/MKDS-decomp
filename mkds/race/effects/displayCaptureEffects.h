#pragma once

#ifndef HP_RACE_EFFECTS_DISPLAYCAPTUREEFFECTS_H
#define HP_RACE_EFFECTS_DISPLAYCAPTUREEFFECTS_H

#include "types.h"
#include <nnsys/fnd/heapcommon.h>
#include <nitro/gx/g2_oam.h>

typedef enum
{
	DCE_MODE_NONE,
	DCE_MODE_DOUBLE_3D,
	DCE_MODE_BLUR
} DceMode;

#define DCE_FLAGS_DOUBLE_3D_ODD     (1 << 0)
#define DCE_FLAGS_BLUR_INCREASE     (1 << 1)
#define DCE_FLAGS_FADING            (1 << 2)
#define DCE_FLAGS_BIT3              (1 << 3)

typedef struct
{
	DceMode mode;
	GXOamAttr* oamBuf;
	u16 flags;
	u8 blurAmount;
	fx32 blurProgress;
} dce_t;

void dce_init(NNSFndHeapHandle heapHandle);
void dce_finalize(void);
void dce_setMode(DceMode mode);
void dce_setupGX(void);
void dce_updateVBlank(bool32 skipUpdate);
void dce_startBlur(int blurAmount);
void dce_stopBlur(void);
void dce_fadeToBlack(s16 frameCount);
void dce_fadeFromBlack(s16 frameCount);
bool32 dce_2055B1C(void);
bool32 dce_2055AFC(void);
void dce_2055A94(void);

#endif
