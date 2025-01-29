#pragma once

#ifndef HP_RACE_MAPOBJ_WATER_MISSIONBARRIER_H
#define HP_RACE_MAPOBJ_WATER_MISSIONBARRIER_H

#include "race/mapobj/mapobj.h"
#include "race/collision/dynamicCollision.h"

typedef struct
{
	dynamicCollisionObject dcolMObj;
} mrbarrier_t;

extern const mobj_def_t gMissionBarrierMObjDef;

bool32 mrbar_isMissionBarrier(MObjId objectId);

#endif
