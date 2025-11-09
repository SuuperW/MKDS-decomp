#pragma once

#ifndef HP_MENU_UTIL_H
#define HP_MENU_UTIL_H

#include "types.h"

s16 mutil_interpolate(bool32 ease, int from, int to, int t, int tmax);
s16 mutil_getButtonOutEffectOffset(int frame);
u16 mutil_checkKeys(int inputIdx, u16 mask);

#endif
