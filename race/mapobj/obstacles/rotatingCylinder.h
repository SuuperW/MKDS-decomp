#pragma once

#ifndef HP_RACE_MAPOBJ_OBSTACLES_ROTATINGCYLINDER_H
#define HP_RACE_MAPOBJ_OBSTACLES_ROTATINGCYLINDER_H

#include "race/mapobj/mapobj.h"
#include "race/collision/dynamicCollision.h"

typedef enum
{
	ROTCYL_STATE_BEGIN_ROTATE,
	ROTCYL_STATE_ROTATE,
	ROTCYL_STATE_END_ROTATE,
	ROTCYL_STATE_IDLE,

	ROTCYL_STATE_COUNT
} RotatingCylinderState;

typedef enum
{
	ROTCYL_TYPE_TEST_CYLINDER,
	ROTCYL_TYPE_GEAR_BLACK,
	ROTCYL_TYPE_GEAR_WHITE,
	ROTCYL_TYPE_ROTARY_ROOM,
	ROTCYL_TYPE_ROTARY_BRIDGE
} RotatingCylinderType;

typedef struct
{
	dynamicCollisionObject dcol;
	u16 startStopDuration;
	u16 rotateDuration;
	u16 idleDuration;
	s16 rotYVelocity;
	fx32 velocityProgress;
	fx32 startStopSpeed;
	u32 field154;
	RotatingCylinderType type;
	u32 sfxId;
} rotcyl_t;

typedef struct
{
	bool32 isXZFloor;
	fx32 sizeX;
	fx32 sizeY;
	RotatingCylinderType type;
	u32 dcolFloorThreshold;
	u32 dcolField138;
	u32 sfxId;
} rotcyl_params_t;

extern const rotcyl_params_t gTestCylinderParams;
extern const rotcyl_params_t gGearParams;
extern const rotcyl_params_t gRotaryRoomParams;
extern const rotcyl_params_t gRotaryBridgeParams;

extern const mobj_def_t gTestCylinderMObjDef;
extern const mobj_def_t gGearMObjDef;
extern const mobj_def_t gRotaryRoomMObjDef;
extern const mobj_def_t gRotaryBridgeMObjDef;

void rotcyl_renderAll(const Orientation4D* camMtx);
bool32 rotcyl_isRotatingCylinder(MObjId objectId);

#endif
