#pragma once

#ifndef HP_RACE_RACE2D_MAP2D_SCENERY_RAINSTARICON_H
#define HP_RACE_RACE2D_MAP2D_SCENERY_RAINSTARICON_H

#include "oam.h"

bool32 mpicn_createRainStarIcon();
void mpicn_destroyRainStarIcon();
void mpicn_updateRainStarIcon();
void mpicn_renderRainStarIcon(oam_buf_t* oamBuf);

#endif
