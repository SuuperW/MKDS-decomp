#pragma once

#ifndef HP_RACE_MAPOBJ_ENEMIES_EYEROCK_H
#define HP_RACE_MAPOBJ_ENEMIES_EYEROCK_H

#include "race/mapobj/enemies/boss.h"

// eyerock is composed of 3 parts
// 1 for each hand, 1 for the combination?
typedef struct eyerock_part {
	boss_t base;
	u8 unk30c[0x34];
} eyerock_part;

static_assert(sizeof(eyerock_part) == 0x340);

#endif
