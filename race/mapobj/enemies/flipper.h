#pragma once
#include "jnlib/spa/spaEmitter.h"
#include "race/mapobj/mapobj.h"

#define FLIP_ANIM_FRAME_COUNT       30

typedef enum
{
	FLIP_STATE_INIT,
	FLIP_STATE_IDLE,
	FLIP_STATE_FLIP_IN,
	FLIP_STATE_FLIP_OUT,
	FLIP_STATE_BALL_HIT_WAIT,
	FLIP_STATE_BALL_HIT_FLIP_IN,
	FLIP_STATE_BALL_HIT_FLIP_OUT
} FlipperState;

typedef struct
{
	mobj_inst_t mobj;
	int fieldA0;
	bool32 modelFlip;
	Orientation baseMatrices[FLIP_ANIM_FRAME_COUNT];
	Orientation extraColMatrices[FLIP_ANIM_FRAME_COUNT];
	VecFx32 ptclEmitterPositions[FLIP_ANIM_FRAME_COUNT];
	VecFx32 ptclEmitterTargets[FLIP_ANIM_FRAME_COUNT];
	int waitCounter;
	int ballHitFrameCounter;
	int animFrame;
	u16 nsbtpFrame;
	u16 nsbtaFrame;
	bool32 electricityActive;
	FlipperState state;
	spa_emitter_t* ptclEmitter;
	int driverHitTimeouts[8];
} flipper_t;

extern mobj_def_t gFlipperMObjDef;

bool32 flip_handleBallHit(int index);
