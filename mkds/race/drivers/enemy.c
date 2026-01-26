#include "math/math.h"
#include "racer.h"

#include "enemy.h"

void FUN_01ffd224(enemy_t* enemy, VecFx32* distToTarget, VecFx32* targetMovementVector)
{
	s64 lVar1;
	int iVar3;
	racerData* racer;
	u32 uVar5;
	u32 uVar6;
	VecFx32 facingOrientationUp;
	VecFx32 VStack_4c;
	VecFx32 posDeltaFromTarget;

	racer = enemy->driver;
	if (racer->colReaction != DRIVER_COLLISION_REACTION_NONE)
		return;
	if (racer->flagsA.HOPPING)
		return;
	if (racer->flagsC.IN_BULLET)
		return;
	
	int direction;
	if (enemy->rescueState == 1)
	{
		VecFx32 targetEpoiPos;
		VecFx32 epoiDirection;
		VecFx32 horizontalEpoiDirection;
		GetEpoiTargetPosAndDirection(&targetEpoiPos, &epoiDirection, enemy);
		VEC_Rejection(&epoiDirection, &(racer->facingOrientation).up, &horizontalEpoiDirection);
		if (-0x333 < DotProduct_t(&(racer->facingOrientation).forward, &horizontalEpoiDirection))
		{
			VEC_Subtract(&racer->position, &targetEpoiPos, &posDeltaFromTarget);
			posDeltaFromTarget.y = 0;
			racer_GetFacingOrientationUp(racer, &facingOrientationUp);
			crossProduct_t(&posDeltaFromTarget, &horizontalEpoiDirection, &VStack_4c);
			if (DotProduct_t(&facingOrientationUp, &VStack_4c) > 0)
				direction = 1;
			else
				direction = -1;
			racer->facingAngle += (short)direction * 200;

			if (direction < 1)
				racer->flagsB.DPAD_RIGHT = true;
			else
				racer->flagsB.DPAD_LEFT = true;
			return;
		}
	}

	bool turning = false;
	if (0x2000 < enemy->field70)
	{
		// Which way should we turn, to reach the target position?
		racer_GetFacingOrientationUp(racer, &facingOrientationUp);
		VecFx32 horizontalDistToTarget;
		VEC_Rejection(&distToTarget, &facingOrientationUp, &horizontalDistToTarget);
		VecFx32 VStack_7c;
		crossProduct_t(targetMovementVector, &horizontalDistToTarget, &VStack_7c);
		if (DotProduct_t(&facingOrientationUp, &VStack_7c) > 0)
			direction = 1;
		else
			direction = -1;
		
		uVar5 = min(fxMulT(racer->basePosDeltaMag, *DAT_0217ae40), 0x1000);
		uVar6 = enemy->field70; // some kind of turning rate
		lVar1 = (s64)(int)(uVar6 * 0xb6 + (int)((u64)uVar6 * 0xb60b60b >> 0x20) + 1) *
						   (s64)fxMulT(uVar5, 0xa4);
		turning = true;
		racer->facingAngle +=
		    (short)((int)((u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) << 0x14) >> 0xc) * (short)direction;
	}
	if (!racer->flagsC.BLOOPER_INK || 0x45fff < enemy->field70 || !racer->flagsA.IS_ON_FLOOR ||
	    racer->flags48 & (DRIVER_48_START_BOOST | DRIVER_48_BOOST) != 0 || racer->basePosDeltaMag < 0x3001 ||
	      enemy->driftState == 2 || enemy->rescueState != 0)
	{
		enemy->fieldCA = 0;
		enemy->fieldC8 = 0;
	}
	else
	{
		enemy->fieldC8 += 0x44c;
		enemy->fieldCA = (fx16)fxMulR(FX_SinCosTable[enemy->fieldC8 >> 4][0], 0xfa);
		turning = true;
		racer->facingAngle += enemy->fieldCA * (short)enemy->fieldCC;
	}
LAB_01ffd5f8:
	if (turning)
	{
		if (direction < 1)
			racer->flagsB.DPAD_RIGHT = true;
		else
			racer->flagsB.DPAD_LEFT = true;
	}
}

void enemy_UpdateTargetPosition(enemy_t* enemy)
{
	s64 lVar1;
	u32 uVar5;
	VecFx32 targetPoint;
	VecFx32 epoiLeft;
	VecFx32 posDeltaFromTarget;
	VecFx32 up = { .x = 0, .y = 0x1000, .z = 0 };

	targetPoint = *enemy->epoi.targetEpoi->position;
	VEC_CrossProduct_t(&enemy->epoi.direction, &up, &epoiLeft);
	VEC_Subtract(&enemy->driver->position, &targetPoint, &posDeltaFromTarget);
	fx32 leftDistance = DotProduct_t(&posDeltaFromTarget, &epoiLeft);
	enemy->driftEpoiRadiusScale = FxDivide(leftDistance, enemy->epoi.targetEpoi->radius + 0x1000);
	if (0x1000 < AbsoluteValue(enemy->driftEpoiRadiusScale))
	{
		if (0 < enemy->driftEpoiRadiusScale)
		{
			enemy->driftEpoiRadiusScale = 0x1000;
		}
		enemy->driftEpoiRadiusScale = -0x1000;
	}
	uVar5 = fxMulT(enemy->driftEpoiRadiusScale, enemy->epoi.targetEpoi->radius);
	VEC_Multiply_t(uVar5, &epoiLeft, &enemy->driftOffset);
	VEC_Add(&targetPoint, &enemy->driftOffset, &enemy->targetPos);
	return;
}
