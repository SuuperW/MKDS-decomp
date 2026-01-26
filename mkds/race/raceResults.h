#pragma once

#ifndef HP_RACE_RACERESULTS_H
#define HP_RACE_RACERESULTS_H

#include "raceTime.h"
#include "raceConfig.h"

typedef struct
{
	u16 totalGpPoints;
	u8 globalPlace;
	u8 place;
	u8 gpPoints;
	u8 winCount;
	race_time_t raceTime;
} race_driver_result_t;

typedef struct
{
	u16 totalGpPoints;
	u8 winCount;
	u8 flags;
} race_team_result_t;

typedef struct
{
	race_driver_result_t driverResults[4][RACE_DRIVER_COUNT_MAX];
	s32 totalSkillGpPoints;
	race_team_result_t teamResults[4][2];
	u32 field164;
} race_results_t;

extern race_results_t* raceResults; // 0x0217a9fc

u8 rres_205DB28(void);
u8 rres_205DC7C(u16);
u8 rres_getGlobalRank(u16);
void rres_getRaceTime(race_time_t* raceTime, u16 driverId, u16 raceIdx);

static inline race_team_result_t* rres_getTeamResults(u8 raceIdx, int team)
{
	race_team_result_t* result;
	result = &raceResults->teamResults[raceIdx][0];
	result = (race_team_result_t*)(result + team);
	return result;
}

static inline race_driver_result_t * rres_getDriverResults(u8 raceIdx, u16 driverId)
{
	driverId = rres_205DC7C(driverId);
	race_driver_result_t* result;
	result = &raceResults->driverResults[raceIdx][0];
	result = (race_driver_result_t*)(result + driverId);
	return result;
}

#endif
