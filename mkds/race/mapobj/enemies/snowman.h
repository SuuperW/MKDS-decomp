#pragma once

#ifndef HP_RACE_MAPOBJ_ENEMIES_SNOWMAN_H
#define HP_RACE_MAPOBJ_ENEMIES_SNOWMAN_H

#include "race/mapobj/mapobj.h"

typedef enum : int
{
	SMAN_STATE_IDLE,
	SMAN_STATE_HIT,
	SMAN_STATE_FALL,
	SMAN_STATE_RISE
} SmanState;

typedef struct
{
	mobj_inst_t mobj;
	bool32 headRotEnabled;
	s32 headRotZ;
	s32 headRotZVelocity;
	s32 counter;
	fx32 headElevationProgress;
	fx32 bottomScale;
	fx32 headElevation;
	fx32 headElevationVelocity;
	fx32 headMaxElevation;
	fx32 headMinElevation;
	SmanState state;
} snowman_t;

extern mobj_def_t gSmanMObjDef;

#endif
