#pragma once

#ifndef HP_RACE_OBJECTSHADOW_H
#define HP_RACE_OBJECTSHADOW_H

#include "types.h"

typedef struct
{
	Orientation4D mtx;
	u16 alpha;
} objshadow_t;

void oshd_setPositionXZ(objshadow_t* objShadow, const VecFx32* position);
void oshd_setPosition(objshadow_t* objShadow, const VecFx32* position);
void oshd_applyMaterial();
void oshd_render(objshadow_t* objShadow, fx32* scale, const Orientation4D* camMtx, u16 alpha);
void oshd_renderMat(objshadow_t* objShadow, fx32* scale, const Orientation4D* camMtx, u16 alpha);
void oshd_renderJgShadowTransformed(const VecFx32* scale, const Orientation4D* mtx, u16 alpha, u16 maxAlpha);
void oshd_renderJgShadow(u16 alpha, u16 maxAlpha);
void oshd_init();
bool32 oshd_setParams(objshadow_t* objShadow, const VecFx32* position, fx32 scale, u16 alpha);
bool32 oshd_isPointOnShadowFloor(const VecFx32* position);

#endif
