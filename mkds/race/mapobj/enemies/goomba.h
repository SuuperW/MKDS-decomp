#pragma once
// goomba

#ifndef HP_RACE_MAPOBJ_ENEMIES_GOOMBA_H
#define HP_RACE_MAPOBJ_ENEMIES_GOOMBA_H

#include "race/mapobj/mapobj.h"
#include "race/pathwalker.h"
#include "math/quaternion.h"

typedef enum
{
	GOOMBA_STATE_ROUTE_IDLE,
	GOOMBA_STATE_GROUND_ROAM,
	GOOMBA_STATE_HIT_GROW,
	GOOMBA_STATE_HIT_SHRINK,
	GOOMBA_STATE_DEAD,
	GOOMBA_STATE_REAPPEAR
} GoombaState;

typedef struct
{
	mobj_inst_t mobj;
	fx32 fieldA0;
	quaternion_t direction;
	quaternion_t targetDir;
	fx32 squashRatio;
	fx32 squashVelocity;
	pw_pathwalker_t pathWalker;
	u16 frame;
	bool32 fieldF4;
	u16 dirInterpRatio;
	bool32 reappearAfterHit;
	u16 alpha;
	s16 field102;
	u32 field104;
} goomba_t;

extern const mobj_def_t gGoombaMObjDef;

void goomba_resetRoaming(goomba_t* instance);

#endif
