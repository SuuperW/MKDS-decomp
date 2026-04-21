#pragma once

#ifndef HP_RACE_DRIVERS_DRIVERCORE_H
#define HP_RACE_DRIVERS_DRIVERCORE_H

#include "racer.h"
#include "race/mapobj/mapobjInstance.h"

typedef enum
{
	DRIVER_HIT_COL_REACTION_NONE,
	DRIVER_HIT_COL_REACTION_1,
	DRIVER_HIT_COL_REACTION_2,
	DRIVER_HIT_COL_REACTION_SINGLE_SPIN_OUT,
	DRIVER_HIT_COL_REACTION_DOUBLE_SPIN_OUT,
	DRIVER_HIT_COL_REACTION_DOUBLE_SPIN_OUT_FIRE,
	DRIVER_HIT_COL_REACTION_DOUBLE_SPIN_OUT_WATER,
	DRIVER_HIT_COL_REACTION_FORWARD_FLIP,
	DRIVER_HIT_COL_REACTION_SINGLE_BACKWARD_FLIP_1,
	DRIVER_HIT_COL_REACTION_SINGLE_BACKWARD_FLIP_2,
	DRIVER_HIT_COL_REACTION_DOUBLE_BACKWARD_FLIP,
	DRIVER_HIT_COL_REACTION_THWOMP_SMASH,
	DRIVER_HIT_COL_REACTION_12,
	DRIVER_HIT_COL_REACTION_13,
	DRIVER_HIT_COL_REACTION_SINGLE_SPIN_ELECTRICITY
} DriverHitColReaction;

#define DRIVER_COL_REACTION_FLAGS_BIT0              (1 << 0)
#define DRIVER_COL_REACTION_FLAGS_BIT1              (1 << 1)
#define DRIVER_COL_REACTION_FLAGS_BIT2              (1 << 2)
#define DRIVER_COL_REACTION_FLAGS_HANDLE_AS_WALL    (1 << 3)

typedef bool(*driver_off_217AC14_t)(racerData* driver, u32 colFlags, const VecFx32* a3);

typedef u32 (*driver_init_reaction_func_t)(racerData* driver, u32* a1, mobj_inst_t* mobj);
typedef void (*driver_handle_reaction_func_t)(racerData* driver, VecFx32* a1, mobj_inst_t* mobj);

extern const VecFx32 driverCore_sOneVec;
extern const VecFx32 driverCore_sForwardVec;
extern const VecFx32 driverCore_sUpVec;
extern const VecFx32 driverCore_sRightVec;
extern const VecFx32 driverCore_sZeroVec;

void driver_invokeThunder(racerData* driver);
void driver_startShroomBoost(racerData* driver);
void driver_startStarInvincibility(racerData* driver);
void driver_startTeresaEffect(racerData* driver);
void driver_startGessoInk(racerData* driver, int splashCount);
void sub_2069044(racerData* driver);
bool32 driver_isKilled(racerData* driver);
#endif
