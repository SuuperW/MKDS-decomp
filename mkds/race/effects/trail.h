#pragma once

#ifndef HP_RACE_EFFECTS_TRAIL_H
#define HP_RACE_EFFECTS_TRAIL_H

#include "model.h"

typedef struct
{
	u32 texImageParam;
	u8 field4[0x14];
	u32 plttAddress;
	fx32 width;
	fx32 height;
} trl_texparams_t;

typedef struct
{
	NNSFndList activeTrailList;
	NNSFndList freeTrailList;
	NNSFndList freePointList;
	model_res_t texture;
	u32 polygonAttr;
} trl_state_t;

typedef struct
{
	NNSFndLink link;
	NNSFndList pointList;
	const VecFx32* targetPosition;
	bool32 shouldDie;
	fx32 initialPointSize;
	u32 driverId;
	VecFx32 driverOffset;
} trl_trail_t;

typedef struct
{
	NNSFndLink link;
	fx32 size;
	u16 age;
	VecFx32 position;
} trl_point_t;

void trl_init(NNSFndHeapHandle heapHandle);
void trl_update();
void trl_render(const Orientation4D* camMtx);
trl_trail_t* trl_createTrail(const VecFx32* targetPosition);
trl_trail_t* trl_createTrailForDriver(u16 driverId);
void trl_killTrail(trl_trail_t* trail);
void trl_setVisibleForAward(bool32 visible);

#endif
