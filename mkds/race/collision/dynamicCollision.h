#pragma once

#ifndef HP_RACE_COLLISION_DYNAMICCOLLISION_H
#define HP_RACE_COLLISION_DYNAMICCOLLISION_H

#include "collision.h"
#include "model.h"
#include "race/mapobj/mapobj.h"

typedef enum : int
{
	DCOL_SHAPE_BOX,
	DCOL_SHAPE_CYLINDER
} DColShape;

typedef enum : int {
	DCOL_RESULT_NONE,
	DCOL_RESULT_UP,
	DCOL_RESULT_RADIAL,
	DCOL_RESULT_SIDE,
	DCOL_RESULT_FORWARD,
} DColResult;

typedef struct dynamicCollisionObject
{
	mobj_inst_t mobj;
	MtxFx33 lastMtx;
	MtxFx33 baseMtx;
	VecFx32 lastPosition;
	VecFx32 basePos;
	VecFx32 size;
	fx32 sizeZ2;
	bool32 isFloorYZ;
	bool32 isFloorXZ;
	bool32 isFloorXY;
	bool32 isBoostPanel;
	fx32 floorThreshold;
	VecFx32 field124;
	u32 field130;
	DColShape shape;
	u32 field138;
	u32 field13C;
	model_t* model;
} dynamicCollisionObject;

// array of pointers
extern dynamicCollisionObject* touchedDynamicObjects[16]; // 0x0217b5b0

typedef void (*dcol_render_func_t)(dynamicCollisionObject* instance, const Orientation4D* camMtx, u16 alpha);

void dcol_render(mobj_render_part_t* renderPart, const Orientation4D* camMtx, dcol_render_func_t renderFunc);
void dcol_commonRender(dynamicCollisionObject* instance, const Orientation4D* camMtx, u16 alpha);
void dcol_setModel(mobj_render_part_t* renderPart, model_t* model);

bool32 dcol_CheckSphereCollision(dynamicCollisionObject* instance, const VecFx32* position, fx32 sphereSize, u16 collideFlags,
				  col_response_t* floorResponse, col_response_t* wallResponse, VecFx32* a7,
				  u16* collisionType, VecFx32* a9, u16* a10);
#endif
