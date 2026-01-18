#pragma once

#ifndef HP_RACE_MAPOBJ_COMMON_SHINEBALLOON_H
#define HP_RACE_MAPOBJ_COMMON_SHINEBALLOON_H

#include "race/mapobj/mapobj.h"
#include "race/drivers/racer.h"

typedef enum : int
{
	SBLLN_STATE_0,
	SBLLN_STATE_1,
	SBLLN_STATE_2,
	SBLLN_STATE_3
} ShineBalloonState;

typedef struct
{
	mobj_inst_t mobj;
	fx32 scale;
	fx32 scaleDelta;
	s32 counter;
	racerData* driver;
	ShineBalloonState state;
} sblln_t;

extern mobj_def_t gShineBalloonMObjDef;

void sblln_spawn(racerData* driver);

#endif
