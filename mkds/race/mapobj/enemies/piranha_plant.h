#pragma once
// piranha?

#ifndef HP_RACE_MAPOBJ_ENEMIES_PIRANHA_PLANT_H
#define HP_RACE_MAPOBJ_ENEMIES_PIRANHA_PLANT_H

#include "race/mapobj/mapobj.h"
#include "race/pathwalker.h"

typedef enum : int
{
	PIRANHA_PLANT_STATE_0,
	PIRANHA_PLANT_STATE_1,
	PIRANHA_PLANT_STATE_2,
	PIRANHA_PLANT_STATE_3,
	PIRANHA_PLANT_STATE_4,
	PIRANHA_PLANT_STATE_5,
	PIRANHA_PLANT_STATE_6
} Piranha_plantState;

typedef struct
{
	mobj_inst_t mobj;
	u16 polygonId;
	int nsbcaFrame;
	int fieldA8;
	int fieldAC;
	int fieldB0;
	Piranha_plantState state;
	pw_pathwalker_t pathwalkers[7];
	s32 counter;
	u16 curPath;
	u16 pathCount;
	int field1BC[7];
	int field1D8;
	int mouthRotY;
	int mouthRotX;
	int field1E4;
	int field1E8;
	int field1EC;
	int scale;
	int scaleVelocity;
	int headElevation;
	int fireballElevation;
} piranha_plant_t;

extern mobj_def_t gPiranha_plantMObjDef;

#endif
