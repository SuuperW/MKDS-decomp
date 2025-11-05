#pragma once

#ifndef HP_UI_WAITINDICATOR_H
#define HP_UI_WAITINDICATOR_H

#include "jnlib/ui/jnLyt.h"

void wait_unpackData();
void wait_init(NNSFndHeapHandle heapHandle, NNSG2dCellDataBank* targetCellDataBank, const jnui_bncl_res_t* targetBncl,
			   int targetElementId, u16 paletteRow, int engine);
void wait_free();
void wait_update();

#endif
