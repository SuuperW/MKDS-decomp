#pragma once

#ifndef HP_SND_SOUND_VAR_H
#define HP_SND_SOUND_VAR_H

#include "types.h"
#include <nnsys/snd/player.h>

typedef struct
{
	s16 value;
	s16 field2;
	s32 field4;
	s8 id;
} sound_var_t;

void sndvar_update(NNSSndHandle* handle);
bool32 sndvar_getVarField4IsZero(int var);
int sndvar_getVarField4(int var);
int sndvar_getDemoRaceDelay();

#endif
