#pragma once

#ifndef HP_RACE_MAPOBJ_ENEMIES_SWOOP_H
#define HP_RACE_MAPOBJ_ENEMIES_SWOOP_H

#include "race/mapobj/mapobj.h"
#include "race/pathwalker.h"
#include "math/quaternion.h"

typedef enum
{
	SWOOP_STATE_0,
	SWOOP_STATE_1,
	SWOOP_STATE_2,
	SWOOP_STATE_3
} SwoopState;

typedef struct
{
	mobj_inst_t mobj;
	VecFx32 velocity;
	u16 nsbtpFrame;
	s32 initialCounter;
	s32 state0Counter;
	s32 state2Counter;
	bool32 emitSound;
	u16 mapIconType;
	idk_struct_t rotZ;
	u8 driverHitMask;
	SwoopState state;
} swoop_t;

extern mobj_def_t gSwoopMObjDef;

void basa_spawn(u32 state2Start, const VecFx32* position, const VecFx32* velocity, u32 state0Start, bool32 emitSound,
				u16 mapIconType);

#endif
