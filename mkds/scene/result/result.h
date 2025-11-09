#pragma once

#ifndef HP_SCENE_RESULT_RESULT_H
#define HP_SCENE_RESULT_RESULT_H

#include "oam.h"
#include "race/raceStatus.h"
#include "scene/sceneProc.h"

typedef struct
{
	oam_buf_t mainOamBuf;
	oam_buf_t subOamBuf;
	RaceMode raceMode;
} result_t;

void result_init(NNSFndHeapHandle heapHandle);
void result_update(scene_manager_t* sceneManager);
void result_finalize();
void result_vblank();
int result_getResultType();

#endif
