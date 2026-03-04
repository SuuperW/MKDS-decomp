#pragma once
// cheep cheep

#ifndef HP_RACE_MAPOBJ_ENEMIES_CHEEPCHEEP_H
#define HP_RACE_MAPOBJ_ENEMIES_CHEEPCHEEP_H

#include "race/objectShadow.h"
#include "race/mapobj/mapobj.h"
#include "race/mapobj/rotDieMObj.h"

typedef enum : int
{
	CHEEP_STATE_0,
	CHEEP_STATE_1,
	CHEEP_STATE_2,
	CHEEP_STATE_3,

	CHEEP_STATE_COUNT
} CheepcheepState;

typedef struct
{
	rotdiemobj_t rdmobj;
	VecFx32 fieldB8;
	u16 fieldC4;
	objshadow_t shadow;
	u32 fieldFC;
	fx32 field100;
	fx32 field104;
	fx32 field108;
} cheepcheep_t;

extern const mobj_def_t gCheepcheepMObjDef;

#endif
