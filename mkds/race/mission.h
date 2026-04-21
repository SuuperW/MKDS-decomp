#pragma once

#ifndef HP_RACE_MISSION_H
#define HP_RACE_MISSION_H

#include "types.h"

typedef enum
{
	MISSION_TASK_GATES, ///< Drive through all gates
	MISSION_TASK_COINS, ///< Collect coins
	MISSION_TASK_LAPS, ///< Drive n laps
	MISSION_TASK_DESTROY, ///< Destroy objects
	MISSION_TASK_POWERSLIDES, ///< Power sliding
	MISSION_TASK_NONE, ///< ?? 
	MISSION_TASK_FINISHBEFORE, ///< Reach the finish line before the opponent
	MISSION_TASK_BOSS, ///< Boss fight
	MISSION_TASK_8,
	MISSION_TASK_9,
	MISSION_TASK_10
} MissionTask;

#define MISSION_FLAGS_NO_FORWARD          (1 << 0)
#define MISSION_FLAGS_DAMAGE_FORBIDDEN    (1 << 1)
#define MISSION_FLAGS_WALL_HIT_FORBIDDEN  (1 << 2)
#define MISSION_FLAGS_RESPAWN_FORBIDDEN   (1 << 3)
#define MISSION_FLAGS_MIRROR              (1 << 4)
#define MISSION_FLAGS_BACKWARDS           (1 << 5)

typedef struct
{
	u16 timeLimit;
	u16 rankTime;
	s16 timeTolerance;
	u8 id;
	u8 task;
	u8 course;
	u8 ccMode;
	u8 character;
	u8 kart;
	u8 menuId;
	u8 fieldD;
	u8 camParamsIdx;
	u8 targetValue;
	u16 winDelay;
	u16 gap12;
	u16 objectIds[4];
	u16 flags;
	u8 enemyCharacter;
	u8 enemyKart;
	char name[12];
} mission_config_t;

static_assert(sizeof(mission_config_t) == 0x2C);

typedef struct
{
	u32 signature; ///< File identifier: NKMR
	u32 nrMissions;
	mission_config_t missions[];
} mission_mr_t;

typedef struct mission_state_t {
	int unk00;
	s16 missionProgress;
	s16 unk06;
	s16 coinsBeingCollected;
	s16 cooldown;
	s16 unk0c;
	s16 unk0e;
	s16 unk10;
	s16 unk12;
	u8 unk14[0x34];
	struct boss_t* bossParts[3]; // array length unknown
	u8 unk54[0x14];
} mission_state_t;

static_assert(sizeof(mission_state_t) == 0x68);

extern mission_state_t* missionState; // 0x021a9b70

#endif
