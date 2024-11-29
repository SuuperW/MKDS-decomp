#pragma once

#include <types.h>
#include <race/mapobj/mapobj.h>

typedef struct
{
	mobj_inst_t mobj;
	quaternion_t rotation;
	VecFx32 prevPosition;
	VecFx32 renderPos;
	Orientation shadowMtx;
	fx32 offsetY;
	u16 swingRange;
	u16 swingVelocity;
	u16 angle;
	VecFx32 size;
} pendulum_t;

extern const mobj_def_t gPendulumMObjDef;
