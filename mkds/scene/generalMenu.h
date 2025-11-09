#pragma once

#ifndef HP_SCENE_GENERALMENU_H
#define HP_SCENE_GENERALMENU_H

#include "types.h"
#include <nnsys/fnd/heapcommon.h>
#include <nnsys/g2d/g2d_Font.h>
#include <nitro/gx/struct_2d.h>

typedef struct
{
	bool32 unknownLoaded;
	bool32 selectChoisesLoaded;
	bool32 selectReturnLoaded;
	u32 fieldC;
	GXScrFmtText screenTmpBuf[0x400];
	u32 charVramLeft;
} gmenu_context_t;

typedef struct
{
	bool32 loadUnknown; //Wireless related
	const NNSG2dFont* unkFont;
	u32 field8;
	u32 fieldC;
	u32 field10;
	bool32 loadSelectChoises;
	const NNSG2dFont* selectChoisesFont;
	bool32 loadSelectReturn;
	bool32 loadBackground;
	u32 field24;
} gmenu_config_t;

typedef struct
{
	u32 field0;
	u32 field4;
	u32 field8;
	u32 fieldC;
	GXScrFmtText screenTmpBuf[0x400];
	u32 field810;
	u32 field814;
	u32 field818;
	u32 field81C;
	bool32 field820;
	u32 seqArcIndex;
} gmenu_select_return_context_t;

void sub_2128310(int a);
void gmenu_setSeqArcIndex(int a);
void sub_2128374();
bool32 sub_2128294();
void sub_21282C8();

void gmenu_loadSystemGraphics();

bool32 gmenu_checkSelectReturnTouch(int x, int y);
void gmenu_init(NNSFndHeapHandle heapHandle, const gmenu_config_t* config);
void gmenu_finish();
void gmenu_render();
void gmenu_vblank();

#endif
