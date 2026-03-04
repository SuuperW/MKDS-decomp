#pragma once
// thwomp

#ifndef HP_RACE_MAPOBJ_ENEMIES_THWOMP_H
#define HP_RACE_MAPOBJ_ENEMIES_THWOMP_H

#include "race/mapobj/mapobj.h"
#include "race/pathwalker.h"

typedef enum : int
{
	THWOMP_STATE_0,
	THWOMP_STATE_1,
	THWOMP_STATE_2,
	THWOMP_STATE_3,
	THWOMP_STATE_4,
	THWOMP_STATE_5,
	THWOMP_STATE_6
} ThwompState;

typedef enum : int
{
	THWOMP_STAR_HIT_ANIM_STATE_INACTIVE,
	THWOMP_STAR_HIT_ANIM_STATE_1,
	THWOMP_STAR_HIT_ANIM_STATE_2
} ThwompStarHitAnimState;

typedef struct
{
	mobj_inst_t mobj;
	ThwompState state;
	int stateCounter;
	fx32 someSpeed;
	fx32 floorY;
	bool32 isSmashing;
	ThwompStarHitAnimState starHitAnimState;
	s16 rotYDelta;
	s16 rotY;
	u32 lastStarHitFrame;
	u16 noStarHitPlayerMask;
	u16 sinAng;
	fx32 sinAmplitude;
	pw_pathwalker_t pathwalker;
	u16 initialPathPoint;
	bool32 isHorizontalMoveType;
	VecFx32 fieldF4;
	fx32 field100;
	fx32 someAcceleration;
	fx32 anotherSpeed;
} thwomp_t;

extern const mobj_def_t gThwompMObjDef;

fx32 thwomp_getMapElevation(thwomp_t* thwomp);

#endif
