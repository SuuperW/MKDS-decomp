#pragma once

#ifndef HP_RACE_MAPOBJ_COMMON_SHINECONTROLLER_H
#define HP_RACE_MAPOBJ_COMMON_SHINECONTROLLER_H

#include "race/mapobj/mapobj.h"

typedef struct
{
	mobj_inst_t mobj;
	bool32 hasSpawned;
	u32 fieldA4;
	int counter;
} shinc_t;

extern const mobj_def_t gShineControllerMObjDef;

#endif
