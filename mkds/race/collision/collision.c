#include <nitro/fx/fx_vec.h>
#include <nitro/types.h>
#include "math/vector.h"

#include "dynamicCollision.h"
#include "race/drivers/racer.h"

#include "collision.h"

DColResult CheckBoxHitbox(VecFx32* toucheeLocation, VecFx32* objectLocation, Orientation* orientation, VecFx32* frontSize, VecFx32* backSize, fx32 toucheeRadius, VecFx32* out)
{
	VecFx32 locationDelta;
	VEC_Subtract(toucheeLocation, objectLocation, &locationDelta);

	fx32 distanceX = DotProduct_t(&locationDelta, &orientation->left);
	fx32 totalSizeX = frontSize->x + toucheeRadius;
	fx32 totalSizeX_back = backSize->x + toucheeRadius;
	if (totalSizeX <= distanceX || -totalSizeX_back > distanceX)
		return DCOL_RESULT_NONE;

	fx32 distanceY = DotProduct_t(&locationDelta, &orientation->up);
	fx32 totalSizeY = frontSize->y + toucheeRadius;
	fx32 totalSizeY_back = backSize->y + toucheeRadius;
	if (totalSizeY <= distanceY || -totalSizeY_back > distanceY)
		return DCOL_RESULT_NONE;

	fx32 distanceZ = DotProduct_t(&locationDelta, &orientation->forward);
	fx32 totalSizeZ = frontSize->z + toucheeRadius;
	fx32 totalSizeZ_back = backSize->z + toucheeRadius;
	if (totalSizeZ <= distanceZ || -totalSizeZ_back > distanceZ)
		return DCOL_RESULT_NONE;

	fx32 overlapX;
	if (distanceX < 1)
		overlapX = distanceX + totalSizeX_back;
	else
		overlapX = totalSizeX - distanceX;
	fx32 overlapY;
	if (distanceY < 1)
		overlapY = distanceY + totalSizeY_back;
	else
		overlapY = totalSizeY - distanceY;
	fx32 overlapZ;
	if (distanceZ < 1)
		overlapZ = distanceZ + totalSizeZ_back;
	else
		overlapZ = totalSizeZ - distanceZ;

	s64 lVar1;
	if (overlapX <= overlapZ)
	{
		if ((overlapY < overlapX) && (0 < distanceY))
		{
			if (out != NULL)
				VEC_Multiply_t(overlapY, &orientation->up, out);
			return DCOL_RESULT_UP;
		}
		if (out != NULL)
		{
			if (distanceX < 1)
				overlapX = -overlapX;
			
			VEC_Multiply_t(overlapX, &orientation->left, out);
			out->y = 0;
		}
		return DCOL_RESULT_SIDE;
	}
	if ((overlapY < overlapZ) && (0 < distanceY))
	{
		if (out != NULL)
			VEC_Multiply_t(overlapY, &orientation->up, out);
		return DCOL_RESULT_UP;
	}
	if (out != NULL)
	{
		if (distanceZ < 1)
			overlapZ = -overlapZ;

		VEC_Multiply_t(overlapZ, &orientation->forward, out);
		out->y = 0;
	}
	return DCOL_RESULT_FORWARD;
}

void UpdateMapObjectsInZ(int objectId, u16 typesToConsider)
{
	if (objectId < 0)
		return;
	if (0xff < objectId)
		return;
	if ((*collisionEntrys)[objectId].position == NULL)
	{
		colQueryResultCount = 0;
		return;
	}

	s32 mainIdInLowX = (int)(*collisionEntrys)[objectId].segmentLeftEndpoint;

	s32 firstOverlapIndexHigh = (int)(*collisionEntrys)[objectId].segmentRightEndpoint;
	col_segment_right_endpoint_t* objectHighEndpoint = objectsSortedByHighX + firstOverlapIndexHigh;
	colQueryResultCount = 0;
	while (7 < firstOverlapIndexHigh && objectsSortedByLowX[mainIdInLowX].xPos <= objectsSortedByHighX[firstOverlapIndexHigh - 8].xPos)
		firstOverlapIndexHigh -= 8;
	while (firstOverlapIndexHigh > 0 && objectsSortedByLowX[mainIdInLowX].xPos <= objectsSortedByHighX[firstOverlapIndexHigh - 1].xPos)
		firstOverlapIndexHigh--;


	int lastOverlapIndexLow = mainIdInLowX;
	while (lastOverlapIndexLow < someObjectCount - 8 && objectsSortedByLowX[lastOverlapIndexLow + 8].xPos <= objectHighEndpoint->xPos)
		lastOverlapIndexLow += 8;
	while (lastOverlapIndexLow < someObjectCount - 1 && objectsSortedByLowX[lastOverlapIndexLow + 1].xPos <= objectHighEndpoint->xPos)
		lastOverlapIndexLow++;


	u8 someColEntryId = objectsSortedByHighX[firstOverlapIndexHigh].minLeftEndpoint;
	// If I understand correctly, this return should never happen.
	if (lastOverlapIndexLow < someColEntryId)
		return;
	
	fx32 mainObjLowZ = (*collisionEntrys)[objectId].zMin;
	fx32 mainObjHighZ = (*collisionEntrys)[objectId].zMax;
	while (true)
	{
		if ((int)(u32)objectsSortedByLowX[lastOverlapIndexLow].rightEndpoint < firstOverlapIndexHigh ||
		    lastOverlapIndexLow == mainIdInLowX)
		{
			if (lastOverlapIndexLow == 0)
			{
				return;
			}
		}
		else
		{
			u8 id = objectsSortedByLowX[lastOverlapIndexLow].colEntryId;
			u16 objFlags = (*collisionEntrys)[id].flags;
			if (mainObjLowZ <= (*collisionEntrys)[id].zMax &&
			    (*collisionEntrys)[id].zMin <= mainObjHighZ &&
				objFlags & typesToConsider != 0 &&
			    objFlags & COL_ENTRY_FLAGS_DISABLED == 0)
			{
				*(u16*)(colQueryResultEntryIds + colQueryResultCount * 2) = (u16)id;
				colQueryResultFlags[colQueryResultCount] = objFlags;
				colQueryResultObjects[colQueryResultCount] = (int*)(*collisionEntrys)[id].object;
				colQueryResultCount = colQueryResultCount + 1;
				if (colQueryResultCount == 0x80)
				{
					return;
				}
				if (lastOverlapIndexLow == 0)
				{
					return;
				}
			}
		}
		lastOverlapIndexLow--;
		if (lastOverlapIndexLow < (short)(u16)someColEntryId)
			return;
	}
}
