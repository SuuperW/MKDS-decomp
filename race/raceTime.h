#pragma once

#ifndef HP_RACE_RACETIME_H
#define HP_RACE_RACETIME_H

#include <types.h>

typedef struct
{
	u16 milliseconds;
	u8 minutes;
	u8 seconds;
} race_time_t;

#endif
