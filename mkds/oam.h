#pragma once

#ifndef HP_OAM_H
#define HP_OAM_H

#include "types.h"
#include <nitro/gx/g2_oam.h>

typedef struct
{
	GXOamAttr oam[128];
	u16 objCount;
	u16 affineCount;
} oam_buf_t;

// rsMode 3: Big square, single color taken from one of the texture's pixels

/*
objMode:
0 = normal
1 = blend, if the GPU has blend enabled (0x50 BlendCnt flag 0x0140)
2 = backdrop (color comes from first color of first pallet)
	Applies in item area. Idk what controls where backdrop shows.
3 = ?
*/

typedef struct sprite_t
{
	u32 y:8;

	u32 rsMode:2; // both bits make my thing invisible
	u32 objMode:2;
	u32 mosaic:1; // nothing?
	u32 colorMode:1; // really messes up colors
	// These two cut in half if size >= 2.
	u32 wide:1;
	u32 tall:1;

	u32 x:9;

	u32 rsParam:3; // also nothing? maybe combine with rsMode
	u32 flipH:1;
	u32 flipV:1;
	u32 size:2;

	u32 sourceX:5;
	u32 sourceY:5;
	u32 priority:2; // normal, back, backer,  3 = invis?
	u32 color:4;

	u32 _2:16;
} sprite_t;

typedef struct
{
	oam_buf_t mainOamBuf;
	oam_buf_t subOamBuf;
} oam_buffers_t;

void oam_clearResetBuffers(oam_buf_t* main, oam_buf_t* sub);
void oam_resetBuffers(oam_buf_t* main, oam_buf_t* sub);
void oam_prepareBuffers(oam_buf_t* main, oam_buf_t* sub);
void oam_applyBuffers(oam_buf_t* main, oam_buf_t* sub);
u16 oam_getAffineCount(oam_buf_t* oamBuf);
void oam_increaseAffineCount(oam_buf_t* oamBuf);
u16 oam_getAffineId(oam_buf_t* oamBuf);

#endif
