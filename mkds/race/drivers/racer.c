#include "math/math.h"
#include "math/vector.h"

#include "race/collision/collision.h"
#include "race/mapobj/mapobj.h"
#include "race/mapobj/mapobjInstance.h"

#include "racer.h"

extern u32 FUN_02132440(fx32 param_1);
extern bool FUN_02131860(u32 param_1, u32 param_2);
extern u32 FUN_020a1ee4(int param_1, int param_2);
extern void FUN_0206dbf8(racerData* racer1, racerData* racer2);
extern void FUN_0206de18(racerData* racer1, racerData* racer2, VecFx32* param_3);
extern int DAT_0217c7f4;
extern void FUN_021090c4(racerData* racer, int param_2, int param_3, int param_4);
extern void FUN_02071318(racerData* racer, int param_2);
extern int FUN_0206c354(racerData* racer, VecFx32* param_2, u32 param_3);

mobj_inst_t* getTouchedObject(int id) {
  return touchedObjects[id];
}

void racer_DoMapObjCollisions(racerData* racer) {
	bool bVar5;
	int iVar15;
	int local_60[2];
	s32 local_58;

	const VecFx32 zeroVector = { .x = 0, .y = 0, .z = 0 };
	bool touchedObjWall = false;

	local_60[0] = 0;
	local_60[1] = 0;

	racer->someMaxSpeedAdd = 0;
	racer->field3A0 = racer->someMaxSpeedAdd;

	VecFx32 totalObjPushes;
	int collisionCount = racer_CheckObjectCollisions(racer, &racer->positionForCollision, &totalObjPushes);
	if (collisionCount != 0)
	{
		mobj_inst_t* object = (mobj_inst_t*)0x0;
		if (racer->flagsB.RESPAWNING == 0)
		{
			racer->flagsA.TOUCHED_OBJECT = 1;
			for (int collisionId = collisionCount - 1; collisionId >= 0; collisionId--)
			{
				iVar15 = 0;
				bVar5 = false;
				object = getTouchedObject(collisionId);
				int racerResponse = racer_CollideWithObject(racer, object);
				u32 someResponseFlags = Racer_MObjResponseHandlers[racerResponse](racer, &local_58, object);
				if (someResponseFlags != 0)
				{
					VecFx32* pushVector = &objectPushes[collisionId];
					VecFx32 pushNormalForR1ized;
					fx32 pushMagnitude = vec_normalizeFastInline(pushVector, &pushNormalForR1ized);
					if (pushMagnitude == 0)
					{
						pushNormalForR1ized = zeroVector;
					}
					else
					{
						if (local_58 != 2)
						{
							Racer_MObjWallResponseHandlers[racerResponse](racer, &pushNormalForR1ized);
						}
						if ((someResponseFlags & 8) == 0)
						{
							if ((someResponseFlags & 2) != 0)
							{
								fx32 tmv_push_dot = DotProduct_t(&racer->targetMovementVector, &pushNormalForR1ized);
								fx32 speedMult = min(tmv_push_dot + 0x1000, 0x1000);

								racer->wallMaxSpeedMultiplier = FX_MulFunc(racer->wallMaxSpeedMultiplier, max(speedMult, 0x29));
							}
						}
						else
						{
							racer_touchWall(racer, &pushNormalForR1ized, racer->flagsA.IS_TOUCHING_MOBJ_WALL, 0x1000, -1, -1, local_58);
						}
					}
					if ((someResponseFlags & 1) != 0 &&
						(object->velocity.x != 0 || object->velocity.y != 0 || object->velocity.z != 0))
					{
						bool pushAlignedWithBounce = true;
						VecFx32 objBounceVector;
						if ((someResponseFlags & 4) != 0)
						{
							objBounceVector = object->velocity;
						}
						else
						{
							if (pushMagnitude != 0)
							{
								iVar15 = DotProduct_r(&pushNormalForR1ized, &object->velocity);
								VEC_Multiply_t(iVar15, &pushNormalForR1ized, &objBounceVector);
							}
							else
								objBounceVector = zeroVector;

							if (DotProduct_t(&pushNormalForR1ized, &objBounceVector) < 0)
							{
								pushAlignedWithBounce = false;
							}
						}

						racer->someMaxSpeedAdd = VEC_Mag(&objBounceVector);
						if ((0x1000 < racer->someMaxSpeedAdd) && pushAlignedWithBounce)
						{
							racer->field3A0 = DotProduct_t(&racer->targetMovementVector, &objBounceVector);
							iVar15 = DotProduct_r(&racer->targetMovementVector, &objBounceVector);
							VecFx32 temp;
							VEC_Multiply_t(iVar15, &racer->targetMovementVector, &temp);
							(racer->bounce2_c).x += objBounceVector.x - temp.x;
							(racer->bounce2_c).z += objBounceVector.z - temp.z;
							(racer->verticalVelocity).y += objBounceVector.y - temp.y;
						}
					}
					iVar15 = racer->wallMaxSpeedMultiplier;
					if ((someResponseFlags & 8) != 0)
					{
						touchedObjWall = true;
					}
				}
				fx32 uVar9 = 0x1000;
				if ((object->flags & BIT_200) == NONE)
				{
					uVar9 = FX_MulFunc3(racer->basePosDeltaMag, DAT_021654ec, 0x1000 - iVar15);
				}
				u16 local_7c = 0;
				if (((0x2000 < racer->basePosDeltaMag) || ((object->flags & BIT_200) != NONE)) &&
					(local_7c < 2))
				{
					for (int i = 0; i < 2; i++)
					{
						if (object == racer->mobjHitList[i])
						{
							bVar5 = true;
							break;
						}
					}
					if (!bVar5)
					{
						for (int i = 0; i < 2; i++)
						{
							if ((object != racer->mobjHitList[i]) &&
								(racer->mobjHitEmittedSfx[i] == 0))
							{
								FUN_020d2668(object, racer, local_58, uVar9);
								racer->mobjHitEmittedSfx[i] = 1;
								break;
							}
						}
					}
					local_60[local_7c] = (int)object;
					local_7c++;
				}
				if (racerResponse == 0xb)
				{
					racer->smashDossun = object;
				}
			}
		}
		if (racer->flagsB.THWOMP_FLATTEN_CAM != 0 && racer->smashDossun == object)
		{
			totalObjPushes.z = 0;
			totalObjPushes.x = 0;
		}
		VEC_Add(&racer->position, &totalObjPushes, &racer->position);
	}

	for (int i = 0; i < 2; i++)
	{
		racer->mobjHitList[i] = (mobj_inst_t*)local_60[i];
		if ((racer->mobjHitEmittedSfx[i] != 0) &&
		    (racer->mobjHitSfxTimeout[i] = racer->mobjHitSfxTimeout[i] + 1,
		     10 < racer->mobjHitSfxTimeout[i]))
		{
			racer->mobjHitEmittedSfx[i] = 0;
			racer->mobjHitSfxTimeout[i] = 0;
		}
	}

	racer->flagsA.IS_TOUCHING_MOBJ_WALL = touchedObjWall;
}

// 0x0206d7c8
void racer_BumpRacer(racerData* racer1, racerData* racer2, VecFx32* pushNormalForR2, bool firstCallInPair, VecFx32* param_5)
{
	int weightRatio;
	int weight2;
	int weight1;

	weight1 = (int)racer1->kartStats->weight + racer1->characterStats->weight;
	weight2 = (int)racer2->kartStats->weight + racer2->characterStats->weight;
	if (racer1->flagsC.THUNDER_SHRINK)
	{
		weight1 = weight1 >> 3;
		weight2 = weight2 >> 3;
	}
	if (racer1->flagsB.SHROOM_BOOST)
	{
		weight1 = weight1 << 1;
	}
	if (racer2->flagsB.SHROOM_BOOST)
	{
		weight1 = weight1 >> 2;
	}
	weightRatio = FxDivide(weight1, weight2);
	if (0x5000 < weightRatio)
	{
		weightRatio = 0x5000;
	}

	fx32 speedFraction = racer1->maxSpeedFraction;
	bool isWifi = racer1->flagsD.IS_NET_PLAYER || racer1->flagsD.IS_NET_NON_PLAYER;
	if (isWifi)
	{
		u32 var = FUN_02132440(speedFraction);
		if (FUN_02131860(var, 0x384ccccd) != 0)
		{
			speedFraction = 0x333;
		}
	}
	else
	{
		if (speedFraction < 0x801)
		{
			speedFraction = max(speedFraction * 2, 0x333);
		}
		else
		{
			speedFraction = 0x1000;
		}
	}
	fx32 pushStrength = fxMulT3(weight1, weightRatio, speedFraction);
	VecFx32 signedTMV = racer2->targetMovementVector;
	if (racer2->flagsA.IS_GOING_BACKWARDS)
	{
		signedTMV.x = -signedTMV.x; // Sign is actually pointless, we only use this for a vector rejection.
		signedTMV.y = -signedTMV.y;
		signedTMV.z = -signedTMV.z;
	}
	VecFx32 bumpVector;
	VEC_Multiply_t(pushStrength, pushNormalForR2, &bumpVector); // Useless multiplication, it'll get normalized.
	VEC_Rejection(&bumpVector, &signedTMV, &bumpVector);
	VecFx32 bumpDirection;
	fx32 var_d = vec_normalizeFastInline(&bumpVector, &bumpDirection);
	if (var_d == 0)
		return;

	if (firstCallInPair)
	{
		param_5->x = bumpDirection.x;
		param_5->y = bumpDirection.y;
		param_5->z = bumpDirection.z;
	}
	else if ((param_5->x != 0 || param_5->y != 0 || param_5->z != 0) && 0x19a < DotProduct_r(&bumpDirection, param_5))
	{
		bumpDirection.x = 0;
		bumpDirection.y = 0;
		bumpDirection.z = 0;
	}

	VecFx32 VStack_40;
	VEC_Multiply_t(pushStrength, &bumpDirection, &VStack_40);
	if (VStack_40.y > 0)
	{
		VStack_40.y = 0;
	}
	VecFx32 finalBounce;
	VEC_Rejection(&VStack_40, &racer2->surfaceNormalVector, &finalBounce);
	racer2->bounce2_c.z += finalBounce.z;
	racer2->verticalVelocity.y += finalBounce.y;
	if (racer2->charKart->isKartInvisible) // Isn't this a rendering distance thing?
		return;

	fx32 absSpeed = racer2->speed;
	if (absSpeed < 1)
		absSpeed = -absSpeed;
	if (absSpeed < 0x3001)
		return;

	fx32 uVar6 = max(fxMulR(pushStrength, 0xaab), 0x1000);
	VecFx32 VStack_34;
	VEC_CrossProduct_r(&bumpDirection, &racer2->driftlessTMV, &VStack_34);
	if (VStack_34.y < 0)
	{
		uVar6 = -uVar6;
	}
	u32 uVar4;
	if (0 < uVar6)
	{
		uVar4 = uVar6;
	}
	else if (uVar6 < 1)
	{
		uVar4 = -uVar6;
	}
	racer2->flagsC.BIT25 = true;
	if ((int)uVar4 <= racer2->field594)
	{
		return;
	}
	racer2->field594 = uVar4;
	s16 sVar5;
	if (0 < uVar6)
	{
		sVar5 = 1;
	}
	else if (uVar6 < 1)
	{
		sVar5 = -1;
	}
	racer2->field598 = sVar5;
	return;
}

// 0x0206dfd0
void racer_CheckCollisionWithOtherRacers(racerData* racer)
{
	racer->field3D4--;
	if (racer->field3D4 < 1)
	{
		racer->field3D4 = 0;
	}

	if (racer->flagsD.BOO_EFFECT_ACTIVE || racer->flagsD.MG_KILL_GHOST)
		return;
	if (racer->flagsB.RESPAWNING)
		return;
	if (colQueryResultCount < 1)
		return;

	int local_d4 = 0;
	for (int i = 0; i < colQueryResultCount; i++)
	{
		if (colQueryResultFlags[i] & COL_ENTRY_FLAGS_IS_DRIVER1 == 0)
			continue;
		racerData* otherRacer = (racerData*)colQueryResultObjects[i];
		if (((racer->driverHitCheckMask & (1 << otherRacer->playerId)) != 0))
			continue;
		if (otherRacer->flagsD.IS_BEING_KILLED || otherRacer->flagsD.BOO_EFFECT_ACTIVE || otherRacer->flagsD.MG_KILL_GHOST)
			continue;
		if (otherRacer->flagsB.RESPAWNING)
			continue;

		otherRacer->driverHitCheckMask |= (u16)(1 << racer->playerId);
		racer->driverHitCheckMask |= (u16)(1 << otherRacer->playerId);

		fx32 totalRadius = racer->colSphereSize + otherRacer->colSphereSize;

		// check if hitoxes are touching
		bool collide = false;
		fx32 zDelta = (racer->itemPos).z - (otherRacer->itemPos).z;
		if (zDelta <= totalRadius && zDelta >= -totalRadius)
		{
			fx32 xDelta = (racer->itemPos).x - (otherRacer->itemPos).x;
			if (xDelta <= totalRadius || xDelta >= -totalRadius)
			{
				fx32 yDelta = (racer->itemPos).y - (otherRacer->itemPos).y;
				if (yDelta <= totalRadius || yDelta >= -totalRadius)
				{
					s64 sqDistance = (s64)zDelta * zDelta + (s64)xDelta * xDelta + (s64)yDelta * yDelta;
					s64 sqRadius = (s64)totalRadius * totalRadius;
					if (sqDistance <= sqRadius)
					{
						collide = true;
					}
				}
			}
		}

		if (!collide)
			continue;

		VecFx32 collisionPush;
		PushFromSphericalHitbox(&racer->itemPos, &otherRacer->itemPos, totalRadius, &collisionPush);
		VecFx32 pushNormalForR1;
		fx32 pushMag = vec_normalizeFastInline(&collisionPush, &pushNormalForR1);
		if (pushMag > 0)
		{
			// collision push
			VecFx32 push1;
			VecFx32 push2;
			push1.x = collisionPush.x >> 1;
			push1.y = collisionPush.y >> 1;
			push1.z = collisionPush.z >> 1;
			push2.x = -push1.x;
			push2.y = -push1.y;
			push2.z = -push1.z;
			if (racer->flagsA.IS_ON_FLOOR)
				VEC_Rejection(&push1, &racer->surfaceNormalVector, &push1);
			if (otherRacer->flagsA.IS_ON_FLOOR)
				VEC_Rejection(&push2, &otherRacer->surfaceNormalVector, &push2);

			if (racer->flagsA.IS_TOUCHING_WALL && DotProduct_r(&racer->surfaceNormalVector, &push1) < 0)
				VEC_Rejection(&push1, &racer->wallNormal, &push1);
			if (otherRacer->flagsA.IS_TOUCHING_WALL && DotProduct_r(&otherRacer->surfaceNormalVector, &push2) < 0)
				VEC_Rejection(&push2, &otherRacer->wallNormal, &push2);

			VEC_Add(racer->somePositionPtr, &push1, racer->somePositionPtr);
			VEC_Add(otherRacer->somePositionPtr, &push2, otherRacer->somePositionPtr);

			// speed
			if (otherRacer->flagsA.IS_TOUCHING_WALL || otherRacer->flagsA.IS_TOUCHING_MOBJ_WALL)
			{
				VecFx32 vec;
				VEC_Subtract(&otherRacer->basePositionDelta, &racer->basePositionDelta, &vec);
				VEC_Project(&vec, &pushNormalForR1, &vec);
				racer->speed += DotProduct_r(&racer->targetMovementVector, &vec);
			}
			if (racer->flagsA.IS_TOUCHING_WALL || racer->flagsA.IS_TOUCHING_MOBJ_WALL)
			{
				VecFx32 vec;
				// Order of subtraction is swapped, rather than negating pushNormal
				VEC_Subtract(&racer->basePositionDelta, &otherRacer->basePositionDelta, &vec);
				VEC_Project(&vec, &pushNormalForR1, &vec);
				otherRacer->speed += DotProduct_r(&otherRacer->targetMovementVector, &vec);
			}

			u32 uVar5 = 0;
			if ((RaceConfig->current).raceMode == RACE_MODE_BATTLE)
			{
				uVar5 = FUN_020a1ee4((u32)racer->playerId, (u32)otherRacer->playerId);
			}

			VecFx32 pushNormalForR2;
			pushNormalForR2.y = -pushNormalForR1.y;
			pushNormalForR2.x = -pushNormalForR1.x;
			pushNormalForR2.z = -pushNormalForR1.z;

			bool skipBounce = false;
			bool r1_bullet = racer->flagsC.IN_BULLET;
			bool r1_star = racer->flagsC.STAR_INVINCIBLE;
			bool r2_bullet = otherRacer->flagsC.IN_BULLET;
			bool r2_star = otherRacer->flagsC.STAR_INVINCIBLE;
			bool r1_normal = !r1_bullet && !r1_star;
			bool r2_normal = !r2_bullet && !r2_star;
			if (r1_bullet && r2_normal)
			{
				skipBounce = true;
				FUN_0206dbf8(racer, otherRacer);
			}
			else if (r2_bullet && r1_normal)
			{
				skipBounce = true;
				FUN_0206dbf8(otherRacer, racer);
			}
			else if (!r2_bullet && !r1_bullet)
			{
				if (r1_star && !r2_star)
				{
					skipBounce = true;
					FUN_0206de18(racer, otherRacer, &pushNormalForR2);
				}
				else if (r2_star && !r1_star)
				{
					skipBounce = true;
					FUN_0206de18(otherRacer, racer, &pushNormalForR1);
				}
				else if (!r1_star && !r2_star)
				{
					if ((racer->field3D4 < 1) && ((0x1000 < racer->basePosDeltaMag || (0x1000 < otherRacer->basePosDeltaMag))))
					{
						if (0x78 < DAT_0217c7f4)
						{
							FUN_021090c4(racer, 0x80, 0x7f, 0);
						}
						if (otherRacer->basePosDeltaMag < racer->basePosDeltaMag)
							otherRacer->field3D2 = 20;
						else
							racer->field3D2 = 20;

						FUN_02071318(racer, 2);
						FUN_02071318(otherRacer, 2);
						racer->field3D4 = 15;
						if (local_d4 == 0)
						{
							local_d4 = FUN_0206c354(racer, &pushNormalForR1, uVar5);
							if (local_d4 == 0)
							{
								local_d4 = FUN_0206c354(otherRacer, &pushNormalForR2, uVar5);
							}
						}
					}
				}
			}
			if (pushMag > 0x3000)
			{
				racer->flagsC.BIT20 = true;
				otherRacer->flagsC.BIT20 = true;
			}
			if (!skipBounce)
			{
				VecFx32 zero = {.x = 0, .y = 0, .z = 0};
				racer_BumpRacer(otherRacer, racer, &pushNormalForR1, true, &zero);
				racer_BumpRacer(racer, otherRacer, &pushNormalForR2, false, &zero);
			}
		}

		racer->flagsA.IS_TOUCHING_DRIVER = true;
		otherRacer->flagsA.IS_TOUCHING_DRIVER = true;
		racer->driverHitMask |= (u16)(1 << otherRacer->playerId);
		otherRacer->driverHitMask |= (u16)(1 << racer->playerId);
	} // for colQueryResultCount
}

void racer_HandleSlipstream(racerData* racer)
{
	fx32 posDeltaMagnitude = 0;
	bool isStartingSlipstream = false;
	GetHitboxLocation(&racer->colPos2, racer->somePositionPtr, 0x400, racer);
	fx32 radius4 = fxMulT(racer->colSphereSize, -0x4000);
	VEC_MultAdd(radius4, &racer->targetMovementVector, &racer->colPos2, &racer->colPos2);
	if ((racer->playerId == GetFrameCountMod8()) && ((racer->flags4C & (DRIVER_4C_IN_BULLET | DRIVER_4C_SLIPSTREAM_FULL)) == NONE))
	{
		if (0x4000 < racer->basePosDeltaMag && racer->colReaction == DRIVER_COLLISION_REACTION_NONE)
		{
			for (int i = 0; i < colQueryResultCount; i++)
			{
				racerData* otherRacer = (racerData*)colQueryResultObjects[i];
				if (0x1fff < otherRacer->basePosDeltaMag && colQueryResultFlags[i] & COL_ENTRY_FLAGS_IS_DRIVER2 != 0 &&
				     otherRacer != racer && otherRacer->flagsD.BOO_EFFECT_ACTIVE == 0 && otherRacer->flagsD.MG_KILL_GHOST == 0)
				{
					VecFx32 posDelta;
					VEC_Subtract(&otherRacer->position, &racer->position, &posDelta);
					posDeltaMagnitude = VEC_Mag(&posDelta);
					fx32 fVar3 = DotProduct_t(&otherRacer->targetMovementVector, &posDelta);
					if (fxMulT(posDeltaMagnitude, 0xe7f) < fVar3)
					{
						isStartingSlipstream = true;
						racer->timers.slipstreamStartTimer++;
						break;
					}
				}
			}
		}
		if (racer->flagsD.IS_NET_NON_PLAYER == 0)
		{
			if (isStartingSlipstream)
			{
				racer->flagsC.SLIPSTREAM_BEGIN = true;
				FUN_02081060(racer);
				if (0xf < racer->timers.slipstreamStartTimer && posDeltaMagnitude < 0x3c000)
				{
					// full slipstream
					FUN_02069068(racer);
				}
			}
			else
			{
				(racer->timers).slipstreamStartTimer = 0;
				racer->flagsC.SLIPSTREAM_BEGIN = false;
				if (racer->flagsC.SLIPSTREAM_FULL == 0)
				{
					racer->slipstreamSpeedMultiplier = 0x1000;
				}
				FUN_02081000((int)racer);
			}
		}
	}
	if (racer->flagsC.SLIPSTREAM_BEGIN)
	{
		racer->slipstreamSpeedMultiplier += 7;
		if (0x1333 < racer->slipstreamSpeedMultiplier)
		{
			racer->slipstreamSpeedMultiplier = 0x1333;
		}
		return;
	}
	return;
}
