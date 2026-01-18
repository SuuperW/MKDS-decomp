#pragma once

#ifndef HP_RACE_MAPOBJ_OBSTACLES_BUMPER_H
#define HP_RACE_MAPOBJ_OBSTACLES_BUMPER_H

#include <nitro/types.h>
#include "race/mapobj/mapobj.h"
#include "race/collision/dynamicCollision.h"
#include "sinThing.h"

typedef struct bumper
{
	mobj_inst_t base;
	int field_0xA0;
	DColResult collisionResult;
	u32 unk_0xA8[3];
	fx32 radius;
	sinthing_t scaleXZ;
	sinthing_t scaleY;
	u32 unk_0xF8[9];
	int field_0x11C;
	s32 cooldownTimerPerRacer[8];
} bumper;

static_assert(sizeof(bumper) == 0x140);

#endif
