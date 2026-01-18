#include <nitro/fx/fx_vec.h>
#include <nitro/types.h>
#include "math/vector.h"

#include "dynamicCollision.h"

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
