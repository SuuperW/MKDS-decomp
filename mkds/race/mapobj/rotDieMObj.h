#pragma once

#ifndef HP_RACE_MAPOBJ_ROTDIEMOBJ_H
#define HP_RACE_MAPOBJ_ROTDIEMOBJ_H

#include "mapobj.h"

typedef struct
{
	mobj_inst_t mobj;
	fx32 dieMinY;
	fx32 dieYAccel;
	bool32 dieRotZDir;
	u16 dieRotZ;
	u16 dieRotZSpeed;
	fx32 dieInitialYVelo;
	fx32 fieldB4;
} rotdiemobj_t;

#endif
