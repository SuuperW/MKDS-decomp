#pragma once

#ifndef HP_RACE_MAPOBJ_SCENERY_RAINSTAR_H
#define HP_RACE_MAPOBJ_SCENERY_RAINSTAR_H

#include "race/mapobj/mapobj.h"

typedef struct
{
	mobj_inst_t mobj;
	u16 nsbtaFrame;
} rainstar_t;

extern mobj_def_t gRainStarMObjDef;

#endif
