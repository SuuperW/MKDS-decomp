#pragma once

#ifndef HP_RACE_MAPOBJ_MAPOBJSORTING_H
#define HP_RACE_MAPOBJ_MAPOBJSORTING_H

#include "types.h"

struct mobj_inst_t;

typedef struct objByX {
	u8 field0;
	u8 objId;
	s16 x;
} objByX;

extern objByX* objectsSotrtedByLowX; // 0x0217b5a0
extern objByX* objectsSortedByHighX; // 0x0217b594

typedef struct objShortInfo {
	s16 idInHighX;
	s16 idInLowX;
	s32 high_z_edge;
	s32 low_z_edge;
	VecFx32* objPosition;
	s32 someSize;
	u16 flags;
	struct mobj_inst_t* object;
} objShortInfo;

extern objShortInfo* objectInfosPtr; // 0x0217b598

#endif
