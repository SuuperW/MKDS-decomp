#pragma once

#ifndef HP_RACE_COURSEMODEL_H
#define HP_RACE_COURSEMODEL_H

#include "model.h"
#include "animationManager.h"

typedef struct
{
	model_t* model;
	model_t* modelV;
	anim_manager_t nsbtpAnim;
	anim_manager_t nsbtaAnim;
	anim_manager_t nsbtaAnimV;
	Orientation4D mtx;
	bool32 modelHasPartialFog;
	s64 modelFogFlags;
	bool32 modelVHasPartialFog;
	u16 modelVFogFlags;
} crsmdl_t;

static_assert(sizeof(crsmdl_t) == 244);

void crsmdl_init(NNSFndHeapHandle heapHandle);
void crsmdl_update();
void crsmdl_renderCourse();
void crsmdl_renderSky();
void crsmdl_setupLights();
void crsmdl_setFogFlags(bool32 enabled);
int crsmdl_getNsbtpProgress();

#endif
