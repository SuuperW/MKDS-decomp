#pragma once

#ifndef HP_RACE_MAPOBJ_MAPOBJCOLLISION_H
#define HP_RACE_MAPOBJ_MAPOBJCOLLISION_H

#include "race/items/item.h"
#include "mapobj.h"

u32 mobj_getDriverHitResponse(bool32 useSimpleHitResp, int collisionMode, MObjGroup objGroup, int objIdx);
u32 mobj_getItemHitObjResponse(bool32 useSimpleHitResp, MKDSItem itemType, MObjGroup objGroup, int objIdx);
u32 mobj_getItemHitResponse(bool32 useSimpleHitResp, MKDSItem itemType, MObjGroup objGroup, int objIdx, bool32 a5);
u32 mobj_handleDriverCollision(driver_t* driver, mobj_inst_t* mobj);
u32 mobj_handleItemCollision(it_item_inst_t* item, mobj_inst_t* mobj, u32* objRespOut);
bool32 mobj_getCollidesWithDriver(int collisionMode, mobj_inst_t* mobj);
bool32 mobj_getCollidesWithItem(MKDSItem itemType, mobj_inst_t* mobj);
u32 mobj_getItemHitObjResponseForMObj(MKDSItem itemType, mobj_inst_t* mobj);

#endif
