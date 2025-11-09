#include "driverItem.h"
#include "race/drivers/racer.h"
#include "race/raceStatus.h"
#include "fx_convenience.h"

#include <nitro/fx/fx_vec.h>

int throwItem_B(it_driver_dragitem_t* dragItem, int param_2, int param_3, fx32 param_4, VecFx32* out)
{
	int ret;
	racerData* racer;
	VecFx32 local_28;

	ret = VEC_Mag(&dragItem->racerTotalPosDelta);
	if (dragItem->fastThrow == 0)
	{
		racer = dragItem->driver;
		if ((VEC_Mag(&racer->wallBounce1) < 0x10))
		{
			ret = param_2 - ret;
			local_28 = dragItem->targetOrientation.forward;
		}
		else
		{
			ret = param_2 + FX_MulFunc(ret, param_3);
			VEC_MultAdd(param_4, &dragItem->targetOrientation.forward, &racer->wallBounce1, &local_28);
		}
	}
	else
	{
		ret = param_2 + FX_MulFunc(ret, param_3);
		VEC_MultAdd(param_4, &dragItem->targetOrientation.forward, &dragItem->racerTotalPosDelta, &local_28);
	}
	if ((dragItem->driver->flags48 & 1) == 0)
	{
		local_28.y = 0;
	}
	// Is this potential uninitialised memory? Or will local_28 always be non-zero?
	if (VEC_Mag(&local_28) < 0x10)
	{
		VEC_Normalize(&local_28, out);
	}
	return ret;
}

void FUN_020f3150(it_item_inst_t* item, it_driver_dragitem_t* dragItem, void* param_3, int param_4,
                  int param_5, int param_6, int param_7)
{
	s64 lVar1;
	s64 lVar2;
	s64 lVar3;
	u32 uVar4;
	int iVar5;
	fx32 fVar6;
	u32 uVar8;
	fx32 fVar9;
	fx32 fVar10;
	int iVar11;
	int iVar12;
	int iVar13;
	VecFx32* base;
	int someVar;
	u32 uVar15;
	VecFx32 local_30;
	race_status_t* raceStatus;

	uVar4 = DotProduct_t(&dragItem->racerTotalPosDelta, &(dragItem->targetOrientation).forward);
	uVar4 = FX_MulInline(uVar4, 0x1ec); // 0.120
	if ((int)uVar4 < 0x666) // 0.4
	{
		uVar4 = 0x666;
	}
	if (param_4 == 0)
	{
		iVar5 = FUN_020edb8c(dragItem, uVar4, 0x1000, 0x7000, &local_30);
	}
	else
	{
		iVar5 = throwItem_B(dragItem, param_5, 0x1b33, 0x7000, &local_30);
		if (0 < dragItem->field7C)
		{
			iVar5 += FX_MulInline(dragItem->field7C, 0x14cd); // 1.3
		}
	}
	raceStatus = gRaceStatus;
	if (dragItem->field68 < 0x3000)
	{
		local_30.z += MATH_Rand32(&gRaceStatus->safeRng, 246) - 123;
		local_30.x += MATH_Rand32(&gRaceStatus->safeRng, 246) - 123;
	}
	VEC_Mult(iVar5, &local_30, &local_30);
	if (!dragItem->driver->flagsB.IN_LOOP)
	{
		if (param_4 == 0)
		{
			iVar5 = (dragItem->racerTotalPosDelta).y;
			if (iVar5 < (fx32)1)
			{
				iVar11 = 0x1333;
			}
			else
			{
				iVar11 = 0x666;
			}
			fVar6 = param_7 + FX_MulInline(iVar11, iVar5);
		}
		else
		{
			uVar8 = dragItem->field68;
			uVar4 = (dragItem->racerTotalPosDelta).y;
			fVar6 = param_6
				// wtf is this
				+ ((uVar8 << 9) >> 0xc | (((int)uVar8 >> 0x1f) << 9 | uVar8 >> 0x17) << 0x14)
				+ ((uVar4 << 0xd) >> 0xc | (uVar4 >> 0x13) << 0x14);
		}
		VEC_MultAdd(fVar6, &(dragItem->targetOrientation).up, &local_30, &item->velocity);
	}
	if (param_4 == 0)
	{
		AddVectors(&item->velocity, &dragItem->driver->wallBounce1, &item->velocity);
		if (((dragItem->field7C != 0) && (dragItem->field68 < 0x2000)) && ((item->scale).z < 0xccd))
		{
			SubtractVector(&(dragItem->targetOrientation).up, &(dragItem->targetOrientation).forward, &local_30);
			VEC_MultAdd(0x2000, &local_30, &item->position, &item->position);
		}
	}
	else
	{
		racerData* racer = dragItem->driver;
		base = &item->position;
		base->x = (racer->positionForCollision).x;
		base->y = (racer->positionForCollision).y;
		base->z = (racer->positionForCollision).z;
		VEC_MultAdd((item->scale).z * 10, &(dragItem->targetOrientation).forward, base, base);
	}
	if ((item->scale).z < 0xccd)
	{
		(item->scale).x = 0xccd;
		(item->scale).y = 0xccd;
		(item->scale).z = 0xccd;
	}
	item->field12C = param_3;
	item->field124 = (u32)FUN_020f2dac;
	item->flags = item->flags & 0xf8ffffff;
	if (dragItem->driver->flagsB.IN_LOOP)
	{
		iVar5 = DotProductRounded(&(dragItem->targetOrientation).up, &item->velocity);
		lVar3 = (s64)(dragItem->targetOrientation).up.x * (s64)iVar5;
		lVar1 = (s64)(dragItem->targetOrientation).up.y * (s64)iVar5;
		lVar2 = (s64)(dragItem->targetOrientation).up.z * (s64)iVar5;
		(item->velocity).x =
		    (item->velocity).x - ((u32)lVar3 >> 0xc | (int)((u64)lVar3 >> 0x20) << 0x14);
		(item->velocity).y =
		    (item->velocity).y - ((u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) << 0x14);
		(item->velocity).z =
		    (item->velocity).z - ((u32)lVar2 >> 0xc | (int)((u64)lVar2 >> 0x20) << 0x14);
		if (param_4 == 0)
		{
			fVar6 = -0x1000;
		}
		else
		{
			fVar6 = 0x8800;
		}
		VEC_MultAdd(fVar6, &(dragItem->targetOrientation).up, &item->velocity, &item->velocity);
		item->field12C = (void*)0x0;
		return;
	}
	return;
}
