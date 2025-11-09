#pragma once

#ifndef HP_RACE_EFFECTS_EXPLOSION_H
#define HP_RACE_EFFECTS_EXPLOSION_H

#include "animationManager.h"
#include "model.h"

typedef enum
{
	EXPL_TYPE_BOMBCORE,
	EXPL_TYPE_BOMBCORE2,
	EXPL_TYPE_SPLASH,
	EXPL_TYPE_BAKUBAKU_WAVE,
	EXPL_TYPE_MISSION_SPLASH,

	EXPL_TYPE_COUNT
} ExplType;

typedef enum
{
	EXPL_STATE_NORMAL,
	EXPL_STATE_SHOULD_DIE
} ExplState;

typedef struct expl_inst_t expl_inst_t;

typedef void (*expl_inst_init_func_t)(expl_inst_t* instance, bool32 emitParticles);
typedef void (*expl_inst_update_func_t)(expl_inst_t* instance);

typedef struct
{
	u16 instanceCount;
	bool32 hasNsbca;
	bool32 hasNsbma;
	bool32 hasNsbta;
	expl_inst_init_func_t initInstFunc;
	expl_inst_update_func_t updateInstFunc;
	u32 instanceSize;
	model_res_t modelRes;
} expl_def_t;

typedef struct expl_inst_t
{
	NNSFndLink link;
	model_t model;
	VecFx32 position;
	expl_inst_init_func_t initFunc;
	expl_inst_update_func_t updateFunc;
	ExplState state;
	ExplType type;
	anim_manager_t* nsbcaAnim;
	anim_manager_t* nsbmaAnim;
	anim_manager_t* nsbtaAnim;
	VecFx32 scale;
	u32 frameCounter;
	u32 lifeTime;
	u16 polygonId;
	bool32 visible;
} expl_inst_t;

typedef struct
{
	NNSFndList activeInstanceList;
	NNSFndList freeInstanceLists[EXPL_TYPE_COUNT];
	expl_inst_t** instances[EXPL_TYPE_COUNT];
	u16 curPolygonId;
} expl_state_t;

void expl_init(NNSFndHeapHandle heapHandle);
expl_inst_t* expl_spawnEffect(ExplType type, const VecFx32* position, bool32 emitParticles);
void expl_update();
void expl_render();
void expl_spawnSplash(const VecFx32* position, bool32 showSplashModel);

#endif
