#include "racer.h"
#include "math/math.h"
#include "math/vector.h"
#include "race/mapobj/mapobj.h"
#include "race/mapobj/mapobjInstance.h"

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
			racer->flagsA.BIT18 = 1;
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
					VecFx32 pushNormalized;
					fx32 pushMagnitude = vec_normalizeFastInline(pushVector, &pushNormalized);
					if (pushMagnitude == 0)
					{
						pushNormalized = zeroVector;
					}
					else
					{
						if (local_58 != 2)
						{
							Racer_MObjWallResponseHandlers[racerResponse](racer, &pushNormalized);
						}
						if ((someResponseFlags & 8) == 0)
						{
							if ((someResponseFlags & 2) != 0)
							{
								fx32 tmv_push_dot = DotProduct_t(&racer->targetMovementVector, &pushNormalized);
								fx32 speedMult = min(tmv_push_dot + 0x1000, 0x1000);

								racer->wallMaxSpeedMultiplier = FX_MulFunc(racer->wallMaxSpeedMultiplier, max(speedMult, 0x29));
							}
						}
						else
						{
							racer_touchWall(racer, &pushNormalized, racer->flagsA.IS_TOUCHING_MOBJ_WALL, 0x1000, -1, -1, local_58);
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
								iVar15 = DotProduct_r(&pushNormalized, &object->velocity);
								VEC_Multiply_t(iVar15, &pushNormalized, &objBounceVector);
							}
							else
								objBounceVector = zeroVector;

							if (DotProduct_t(&pushNormalized, &objBounceVector) < 0)
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
