
#include <nitro/types.h>
#include <nitro/fx/fx_vec.h>

#include "math/math.h"

// Rounds all results, not truncate
fx32 Vector_Normalize(VecFx32* in, VecFx32* out)
{
	u16 uVar3;
	int recip_low;
	u32 uVar6;

	s64 sqMag = (s64)in->z * (s64)in->z + (s64)in->x * (s64)in->x + (s64)in->y * (s64)in->y;
	u64 divResult = Divide((s64)1 << 56, sqMag, 2);
	u32 doubleMagnitude = sqrt(sqMag * 4);

	u64 uVar2 = (u64)doubleMagnitude * divResult;
	recip_low = (int)uVar2;
	s32 recip_high = (int)(uVar2 >> 32);

	uVar6 = in->x;
	out->x = (int)(recip_high * uVar6 +
	               recip_low * ((int)uVar6 >> 0x1f) +
	               (int)((u64)uVar6 * (uVar2 & 0xffffffff) >> 0x20) + 0x1000) >>
	         13;
	u32 uVar7 = in->y;
	out->y = (int)(recip_high * uVar7 +
	               recip_low * ((int)uVar7 >> 0x1f) +
	               (int)((u64)uVar7 * (uVar2 & 0xffffffff) >> 0x20) + 0x1000) >>
	         13;
	uVar7 = in->z;
	out->z = (int)(recip_high * uVar7 +
	               recip_low * ((int)uVar7 >> 0x1f) +
	               (int)((u64)uVar7 * (uVar2 & 0xffffffff) >> 0x20) + 0x1000) >>
	         13;
	return (fx32)(doubleMagnitude + 1) >> 1;
}
