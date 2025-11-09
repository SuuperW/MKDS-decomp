#pragma once

#ifndef HP_RACE_MAPOBJ_SCENERY_TOWNMONTE_H
#define HP_RACE_MAPOBJ_SCENERY_TOWNMONTE_H

#include "race/mapobj/mapobj.h"

typedef struct
{
	mobj_inst_t mobj;
	u16 nsbtpFrame;
	u16 sfxId;
	idk_struct_t fieldA4;
} townmonte_t;

extern mobj_def_t gTownMonteMObjDef;

#endif
