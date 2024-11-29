#pragma once

#include <types.h>

typedef struct
{
	fx32 field0;
	fx32 field4;
	VecFx32 camPos;
	VecFx32 camForward;
	VecFx32 frustumLeftVec;
	VecFx32 frustumRightVec;
	VecFx32 frustumTopVec;
	VecFx32 frustumBottomVec;
	bool32 field50;
	bool32 isObjFadeDisabled;
} struc_217B4F8;

int mobj_calcVisibilityFlags2d(const VecFx32* position, fx32 nearClip, fx32 farClip, int sfxMaxDistanceSquare, u16 alpha);
int mobj_calcVisibilityFlags3d(const VecFx32* position, fx32 nearClip, fx32 farClip, int sfxMaxDistanceSquare, u16 alpha);
int mobj_calcXZCamDistSquare(const VecFx32* position);
