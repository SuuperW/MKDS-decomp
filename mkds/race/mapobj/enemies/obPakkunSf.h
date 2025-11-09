#pragma once
// piranha?

#ifndef HP_RACE_MAPOBJ_ENEMIES_OBPAKKUNSF_H
#define HP_RACE_MAPOBJ_ENEMIES_OBPAKKUNSF_H

#include "race/mapobj/rotDieMObj.h"

typedef struct
{
	rotdiemobj_t rotDieMObj;
	u16 counter;
} obpakkunsf_t;

extern const mobj_def_t gObPakkunSfMObjDef;

#endif
