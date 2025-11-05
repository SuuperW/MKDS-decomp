#pragma once

#ifndef HP_RACE_MAPOBJ_SCENERY_AIRSHIP_H
#define HP_RACE_MAPOBJ_SCENERY_AIRSHIP_H

#include "sinThing.h"
#include "race/pathwalker.h"
#include "race/mapobj/mapobj.h"

typedef enum
{
	ASHIP_STATE_IDLE,
	ASHIP_STATE_TAKING_OFF,
	ASHIP_STATE_FLYING
} AirshipState;

typedef struct
{
	mobj_inst_t mobj;
	fx32 baseElevation;
	fx32 speed;
	sinthing_t fieldA8;
	pw_pathwalker_t pathwalker;
	AirshipState state;
} airship_t;

extern mobj_def_t gAirshipMObjDef;

#endif
