#pragma once

#ifndef HP_RACE_RANKPOINT_H
#define HP_RACE_RANKPOINT_H

#include <types.h>

typedef struct
{
	u8 points[8];
} rankpoint_entry_t;

typedef struct
{
	u32 signature;
	u32 fileSize;
	rankpoint_entry_t entries[1];
} rankpoint_t;

#endif
