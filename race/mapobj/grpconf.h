#pragma once

#ifndef HP_RACE_MAPOBJ_GRPCONF_H
#define HP_RACE_MAPOBJ_GRPCONF_H

#include <types.h>

typedef struct
{
	u16 objectId;
	u16 has3DModel;
	u16 nearClip;
	u16 farClip;
	u16 collisionType;
	u16 width;
	u16 height;
	u16 depth;
} grpconf_entry_t;

#endif
