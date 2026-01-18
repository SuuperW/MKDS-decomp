#pragma once

#ifndef HP_ANIMATIONMANAGER_H
#define HP_ANIMATIONMANAGER_H

#include "types.h"
#include <nnsys/fnd/heapcommon.h>
#include <nnsys/g3d/kernel.h>
#include "model.h"
#include "animator.h"

typedef enum : int
{
	ANIM_KIND_JNT,
	ANIM_KIND_SRT,
	ANIM_KIND_PAT,
	ANIM_KIND_VIS,
	ANIM_KIND_MAT
} AnimKind;

typedef struct
{
	anim_animator_t animator;
	model_t* model;
	u16 animCount;
	u16 curAnimIdx;
	NNSG3dAnmObj** anmObjs;
	bool32* loopFlags;
	u32 field24; //?
	AnimKind animKind;
	bool32 isBlending;
	fx32 blendSpeed;
	NNSG3dAnmObj* blendAnmObj;
} anim_manager_t;

static_assert(sizeof(anim_manager_t) == 56);

void anim_initManager(AnimKind animKind, anim_manager_t* manager, model_t* model, u32 animCount,
					  NNSFndHeapHandle heapHandle, void* nsbmd);
void anim_registerAnim(anim_manager_t* manager, int idx, void* anim, bool32 loop, NNSFndHeapHandle heapHandle);
void anim_registerAllAnims(anim_manager_t* manager, void* animRes, NNSFndHeapHandle heapHandle);
void anim_setAnim(anim_manager_t* manager, int idx);
void anim_setAnimWithBlend(anim_manager_t* manager, int idx);
void anim_update(anim_manager_t* manager);
void anim_updateBlend(anim_manager_t* manager);
fx32 anim_getAnimLength(anim_manager_t* manager);
void anim_setLoop(anim_manager_t* manager, u16 idx, bool32 loop);

static inline void anim_setFrame(anim_manager_t* manager, fx32 frame)
{
	manager->anmObjs[manager->curAnimIdx]->frame = manager->animator.progress = frame;
}

#endif
