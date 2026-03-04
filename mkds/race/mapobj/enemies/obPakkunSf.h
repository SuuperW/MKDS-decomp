#pragma once
// piranha?

#ifndef HP_RACE_MAPOBJ_ENEMIES_OBPIRANHA_PLANTSF_H
#define HP_RACE_MAPOBJ_ENEMIES_OBPIRANHA_PLANTSF_H

#include "race/mapobj/rotDieMObj.h"

typedef struct
{
	rotdiemobj_t rotDieMObj;
	u16 counter;
} obpiranha_plantsf_t;

extern const mobj_def_t gObPiranha_plantSfMObjDef;

#endif
