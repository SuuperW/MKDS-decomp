#pragma once
// pokey

#ifndef HP_RACE_MAPOBJ_ENEMIES_POKEY_H
#define HP_RACE_MAPOBJ_ENEMIES_POKEY_H

#include "sinThing.h"
#include "race/pathwalker.h"
#include "race/mapobj/mapobj.h"

typedef enum : int
{
	POKEY_STATE_IDLE,
	POKEY_STATE_1,
	POKEY_STATE_DIEING,
	POKEY_STATE_RESURRECTING
} PokeyState;

typedef struct
{
	VecFx32 dieingPosition;
	VecFx32 dieingVelocity;
	fx32 scaleXY;
	sinthing_t rotZSinThing;
	int rotZ;
	int rotZSpeed;
	int wiggleWaitCounter;
} pokey_part_t;

typedef struct
{
	mobj_inst_t mobj;
	int pwWaitCounter;
	int hitTimeout;
	int resurrectionWaitCounter;
	int sfxTimeout;
	pokey_part_t bodyParts[4];
	u16 bodyPartCount;
	fx32 pwSpeed;
	pw_pathwalker_t pathwalker;
	PokeyState state;
} pokey_t;

extern mobj_def_t gPokeyMObjDef;

#endif
