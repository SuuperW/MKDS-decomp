#pragma once

#ifndef HP_SINTHING_H
#define HP_SINTHING_H

#include "include/types.h"

typedef struct
{
	u16 phase;
	fx32 value;
	fx32 velocity;
	fx32 baseOffset;
	fx32 amplitude;
	fx32 amplitudeVelocity;
	int phaseVelocity;
	int phaseAcceleration;
} sinthing_t;

static inline fx32 updateSinThing(sinthing_t* sinThing);

static inline fx32 updateSinThing2(sinthing_t* sinThing);

#endif
