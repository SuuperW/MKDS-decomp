
#include "bumper.h"
#include "race/raceStatus.h"
#include "types.h"

struct spa_emitter_t;

extern void racer_MakeSfx(racerData* racer, int soundId);
extern void ApplyForceToDriverBalloons(u16 racerId, VecFx32* param_2);
extern void FUN_02103544(int param_1, int param_2);
extern struct spa_emitter_t* FUN_MakeAndActivateSpa_idk(int particleId, VecFx32* param_2);

u8 bumper_OnDriverCollide(bumper* bumper, racerData* racer, byte* objResp, u8* driverResp)
{
	if (1 < *objResp - 3)
	{
		(bumper->base).bumpingVelocity.x = 0;
		(bumper->base).bumpingVelocity.y = 0;
		(bumper->base).bumpingVelocity.z = 0;
		return 2;
	}
	if (bumper->cooldownTimerPerRacer[racer->playerId] == 0)
	{
		VecFx32 posDelta;
		VEC_Subtract(&racer->position, &(bumper->base).position, &posDelta);

		VecFx32 posDeltaDirected;
		VEC_Multiply_t(
			DotProduct_r(&racer->basePosDeltaNormalized, &posDelta),
			&racer->basePosDeltaNormalized,
			&posDeltaDirected);

		VecFx32 tangentialPosDelta;
		VEC_Subtract(&posDelta, &posDeltaDirected, &tangentialPosDelta);

		VecFx32 verticalTPD;
		VEC_Multiply_t(
			DotProduct_r(&(bumper->base).mtx.up, &tangentialPosDelta),
			&(bumper->base).mtx.up,
			&verticalTPD);

		VEC_Subtract(&tangentialPosDelta, &verticalTPD, &tangentialPosDelta);
		fx32 magnitude = VEC_Mag(&tangentialPosDelta);
		if (magnitude < 4)
		{
			VEC_Multiply_t((fx32)12.0, &(racer->facingOrientation).left, &(bumper->base).bumpingVelocity);
		}
		else
		{
			vec_normalize(&tangentialPosDelta, &tangentialPosDelta);
			VEC_Multiply_t((fx32)12.0, &tangentialPosDelta, &(bumper->base).bumpingVelocity);
		}
		VEC_Subtract(&verticalTPD, &(bumper->base).mtx.up, &verticalTPD);
		VEC_MultAdd((fx32)0.1f, &verticalTPD, &(bumper->base).bumpingVelocity, &(bumper->base).bumpingVelocity);
		bumper->cooldownTimerPerRacer[racer->playerId] = 0x1f;
		if ((RaceConfig->current).raceMode == RACE_MODE_BATTLE)
		{
			ApplyForceToDriverBalloons(racer->playerId, &(bumper->base).bumpingVelocity);
		}
	}
	else
	{
		(bumper->base).bumpingVelocity.x = 0;
		(bumper->base).bumpingVelocity.y = 0;
		(bumper->base).bumpingVelocity.z = 0;
	}
	if (((RaceConfig->current).drivers[racer->playerId].type != DRIVER_TYPE_GHOST) &&
	    (bumper->field_0x11C == 0))
	{
		bumper->scaleXZ.phase = 0x8000;
		bumper->scaleXZ.baseOffset = 0x1000;
		bumper->scaleXZ.amplitude = 0x400;
		bumper->scaleXZ.amplitudeVelocity = -41;
		bumper->scaleXZ.phaseVelocity = 0x1000;
		bumper->scaleXZ.phaseAcceleration = 0;
		bumper->scaleY.phase = 0;
		bumper->scaleY.baseOffset = 0x1000;
		bumper->scaleY.amplitude = 0x400;
		bumper->scaleY.amplitudeVelocity = -41;
		bumper->scaleY.phaseVelocity = 0x1000;
		bumper->scaleY.phaseAcceleration = 0;

		bumper->field_0x11C = 1;
		if (bumper->base.objTypeId == MOBJ_ID_BUMPER)
		{
			racer_MakeSfx(racer, 0x1cf);
		}
		else if (bumper->base.objTypeId == MOBJ_ID_CREAM_BUMPER)
		{
			racer_MakeSfx(racer, 0x156);
		}
		else if (bumper->base.objTypeId == MOBJ_ID_BERRY_BUMPER)
		{
			racer_MakeSfx(racer, 0x155);
		}

		// Graphical effects
		if (bumper->base.objTypeId - 436 < 2)
		{
			int rngValue = MATH_Rand32(&gRaceStatus->randomRng, 3);
			FUN_02103544((int)racer->playerId, rngValue + 1);

			VecFx32 posDelta2; // opposite sign compared to posDelta
			VEC_Subtract(&(bumper->base).position, &racer->positionForCollision, &posDelta2);

			fx32 magnitude = VEC_Mag(&posDelta2);
			if (magnitude >= 4)
			{
				vec_normalize(&posDelta2, &posDelta2);
			}

			VEC_MultAdd(racer->colSphereSize, &posDelta2, &racer->positionForCollision, &posDelta2);
			if ((u32)racer->playerId == (int)(RaceConfig->current).playerDriverId)
			{
				FUN_MakeAndActivateSpa_idk(0x74, &posDelta2);
			}
			else
			{
				FUN_MakeAndActivateSpa_idk(0x75, &posDelta2);
			}
		}
	}
	return *driverResp;
}
