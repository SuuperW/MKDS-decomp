#pragma once

#ifndef HP_RACE_MAPOBJ_ENEMIES_PODOBOO_H
#define HP_RACE_MAPOBJ_ENEMIES_PODOBOO_H

#include "race/mapobj/mapobj.h"
#include "race/mapobj/mapobjInstance.h"
#include "race/pathwalker.h"
#include "sinThing.h"

// podoboo: the fireballs that jump out of lava

typedef enum
{
	PODOBOO_STATE_IDLE,
	PODOBOO_STATE_BOUNCING
} PodobooState;

typedef struct
{
	mobj_inst_t mobj;
	u16 nsbtpFrame;
	u32 fieldA4;
	u32 fieldA8;
	u32 fieldAC;
	u32 fieldB0;
	u32 width;
	sinthing_t scaleXZSinThing;
	sinthing_t scaleYSinThing;
	pw_pathwalker_t pathwalker;
	PodobooState state;
	u32 driverHitTimeouts[RACE_DRIVER_COUNT_MAX];
} podoboo_t;

extern mobj_def_t gPodobooMObjDef;
extern mobj_def_t gCreamMObjDef;
extern mobj_def_t gBerryMObjDef;

bool32 bnd_handleBallHit(int index);

#endif
