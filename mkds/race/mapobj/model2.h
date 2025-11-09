#pragma once

#ifndef HP_RACE_MAPOBJ_MODEL2_H
#define HP_RACE_MAPOBJ_MODEL2_H

#include "model.h"

typedef struct
{
	model_t model;
	u16 polygonId;
	u16 alpha;
	u16 flags;
} shadowmodel_t;

void model2_createFromNsbmd(model_t** model, void* file, int offset);

#endif
