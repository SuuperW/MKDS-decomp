#pragma once

#ifndef HP_RACE_RACE_H
#define HP_RACE_RACE_H

#include "camera.h"

typedef enum : int
{
	RACE_STATE_PRE_START,
	RACE_STATE_STARTED
} RaceState;

typedef enum : int
{
	RACE_DARKENING_FOG_STATE_OFF,
	RACE_DARKENING_FOG_STATE_ON
} RaceDarkeningFogState;

typedef struct
{
	s16 state;
	s16 padding02;
	u32 frameCounter; // pauses when paused
	int frameCounter2; // increments even when paused
	int frameCounterModulo8;
	bool32 isOddFrame; // 0x10
	int frameCounterModuloDriverCount;
	u32 toonTableOffset;
	u32 toonTableUpdateCounter;
	RaceDarkeningFogState darkeningFogState; // 0x20
	RaceDarkeningFogState prevDarkeningFogState;
	bool32 isCamAnimMode;
	bool32 isCamAnimSingleScreen;
	u32 field30; // 0x30
	u32 field34;
	bool32 isAwardStaffRoll;
	u32 field3C;
	VecFx16 light0Dir;
} race_state_t;

static_assert(sizeof(race_state_t) == 0x48);

bool32 race_isCamAnimMode();
bool32 race_isAwardStaffRoll();
void race_init();
void race_initResManagement();
void race_initG3d();
void race_update();
void race_render();
void race_setLight0Direction(const VecFx32* direction);
void race_finalize();
void race_vblank();
RaceState race_getState();
void race_start();
u32 race_getFrameCounter();
int race_getFrameCounterModuloDriverCount();
int race_getFrameCounterModulo8();
bool32 race_getIsOddFrame();
void race_startDarkening();
void race_resetDarkeningFadeWaitCounter();
void race_forceDarkeningFade();
bool32 race_getIsDarkeningActive();
const Orientation4D* race_getCameraMtx();
int race_getCameraField250();
camera_t* race_getCamera();
void race_setField34();
void race_20617D8();
bool32 race_20617A8(u32* a1);
u16 race_getFirstObjectPolygonId();
u16 race_getLastParticlePolygonId();

#endif
