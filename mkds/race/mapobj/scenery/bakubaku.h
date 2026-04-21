#pragma once

#ifndef HP_RACE_MAPOBJ_SCENERY_BAKUBAKU_H
#define HP_RACE_MAPOBJ_SCENERY_BAKUBAKU_H

#include "race/mapobj/mapobj.h"

typedef enum
{
	BAKUBAKU_STATE_IDLE,
	BAKUBAKU_STATE_JUMP_OUT,
	BAKUBAKU_STATE_JUMPING,
	BAKUBAKU_STATE_JUMP_IN,
	BAKUBAKU_STATE_TIMEOUT
} BakubakuState;

typedef struct
{
	mobj_inst_t mobj;
	s32 waitCounter;
	VecFx32 triggerPos;
	u32 state;
	u32 fieldB4;
} bakubaku_t;

extern mobj_def_t gBakubakuMObjDef;

#endif
