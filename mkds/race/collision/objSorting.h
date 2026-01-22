#pragma once

#ifndef HP_RACE_MAPOBJ_MAPOBJSORTING_H
#define HP_RACE_MAPOBJ_MAPOBJSORTING_H

#include "types.h"

typedef struct objByX {
	u8 field0;
	u8 objId;
	s16 x;
} objByX;

extern objByX* objectsSotrtedByLowX; // 0x0217b5a0
extern objByX* objectsSortedByHighX; // 0x0217b594

typedef enum ObjSort : u16 {
	OBJ_SORT_DYNAMIC = 0x1000,
	OBJ_SORT_MAPOBJ  = 0x2000,
	OBJ_SORT_ITEM    = 0x4000,
	OBJ_SORT_RACER   = 0x8000,
} ObjSort;

typedef struct objShortInfo {
	s16 idInHighX;
	s16 idInLowX;
	s32 high_z_edge;
	s32 low_z_edge;
	VecFx32* objPosition;
	s32 someSize;
	u16 flags;
	void* object;
} objShortInfo;

static_assert(sizeof(objShortInfo) == 0x1C);

extern u16 countObjectsInZ;          // 0x0217b588
extern objShortInfo* objectInfosPtr; // 0x0217b598
extern u16* flagsOfObjectsInZ;       // 0x0217b59c
extern void** ptrsToObjectsInZ;       // 0x0217b5a4

#endif
