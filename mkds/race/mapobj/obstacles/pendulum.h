#pragma once

#ifndef HP_RACE_MAPOBJ_OBSTACLES_PENDULUM_H
#define HP_RACE_MAPOBJ_OBSTACLES_PENDULUM_H

#include "types.h"
#include <race/mapobj/mapobj.h>

typedef struct
{
	mobj_inst_t base;
	quaternion_t rotation;
	VecFx32 prevPosition;
	VecFx32 renderPos;
	Orientation4D shadowMtx;
	fx32 offsetY;
	u16 swingRange;
	u16 swingVelocity;
	u16 angle;
	VecFx32 size;
} pendulum_t;

extern const mobj_def_t gPendulumMObjDef;

#endif
