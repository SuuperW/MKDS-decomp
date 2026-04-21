#pragma once

#ifndef HP_RACE_MAPOBJ_ENEMIES_BOSS_H
#define HP_RACE_MAPOBJ_ENEMIES_BOSS_H

#include "race/mapobj/mapobj.h"

// This is a big ?
typedef struct boss_stats_t {
	u8 unk000[16];
	// Velocity is multiplied by one of these each frame.
	fx32 groundVelocityMultiplier;
	fx32 airVelocityMultiplier;
	u8 unk018[0xd8];
	int chargeDelay; // for bully at least
	int unk0f4;
	int unk0f8;
	fx32 chargingSpeed; // for bully at least
	fx32 chargingAcceleration; // for bully at least
	int chargeDuration; // for bully at least
	u8 unk108[0x13c];
} boss_stats_t;

static_assert(sizeof(boss_stats_t) == 0x244); // This is the size for level 1 boss.

// The size of this struct is unknown.
typedef struct boss_t {
	mobj_inst_t mobj;
	boss_stats_t* stats;
	int unk0a4;
	int unk0a8;
	int unk0ac;
	s32 actionTimer;
	u32 jumpsRemaining; // bully
	u8 unk0b8[0xbc];
	fx32 speed;
	fx32 maxSpeed;
	fx32 acceleration;
	int unk180;
	u32 unk184;
	u8 unk188[0x20];
	u16 angle;
	VecFx32 unkVec1AC;
	u8 unk1b8[0x10];
	VecFx32 velocity;
	u8 unk1d4[0x1c];
	int actionTriggerTime;
	void* nextAction;
	s32 unk1f8;
	s32 unk1fc;
	VecFx32 unkVec200;
	void* unkAction20C;
	u8 unk210[0x14];
	void* unkAction224;
	void* unkAction228;
	void* unkAction22C;
	int unk230;
	int hitboxMode; // do we have an enum for this already?
	VecFx32 objPos;
	VecFx32 velocity_2;
	VecFx32 targetVelocity;
	VecFx32 sizes;
	VecFx32 targetSizes;
	fx32 velocityAccel;
	fx32 growthRate;
	u8 unk27c[0x4C];
	u32 touchedSurfaceTypes;
	u8 unk2cc[8];
	void* someFunction;
	int unk2d8;
	void* someFunction2;
	u8 unk2e0[0x2C];
} boss_t;

static_assert(sizeof(boss_t) == 0x30C); // this is the size of level 1 boss

#endif
