#pragma once

#ifndef HP_RACE_MAPDATA_MRRIVALPASSAREA_H
#define HP_RACE_MAPDATA_MRRIVALPASSAREA_H

#include <types.h>
#include <nnsys/fnd/heapcommon.h>

void area_initMissionRivalPassAreas(NNSFndHeapHandle hnd);
void area_finalizeMissionRivalPassAreas();
bool32 area_updateMissionRivalPassAreas();

typedef struct
{
	u16 index;
	fx32 size;
	u8 prevNrObjsInside;
	u8 passCount;
} area_mission_rival_pass_area_t;

typedef struct
{
	area_mission_rival_pass_area_t* entries;
	u16 count;
} area_mission_rival_pass_area_status_t;

#endif
