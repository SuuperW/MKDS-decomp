#pragma once

#ifndef HP_RACE_MAPOBJ_ENEMIES_PIRANHA_PLANTFIRE_H
#define HP_RACE_MAPOBJ_ENEMIES_PIRANHA_PLANTFIRE_H

#include "jnlib/spa/spaEmitter.h"
#include "race/mapobj/mapobj.h"
#include "race/pathwalker.h"

typedef enum : int
{
	PIRANHA_PLANT_FIRE_STATE_0,
	PIRANHA_PLANT_FIRE_STATE_1
} Piranha_plantFireState;

typedef struct
{
	mobj_inst_t mobj;
	pw_pathwalker_t pathwalker;
	fx32 elevation;
	fx32 elevationVelocity;
	Piranha_plantFireState state;
	spa_emitter_t* emitter;
} piranha_plantfire_t;

void pknf_spawn(pw_pathwalker_t* pathwalker, fx32 elevation);

extern mobj_def_t gPiranha_plantFireMObjDef;

#endif
