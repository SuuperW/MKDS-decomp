#pragma once

#ifndef HP_RACE_ITEMS_ITEM_H
#define HP_RACE_ITEMS_ITEM_H

#include "sfx.h"
#include "race/light.h"

typedef enum MKDSItem : int
{
	ITEM_KOURA_G,
	ITEM_KOURA_R,
	ITEM_BANANA,
	ITEM_MUSHROOM,
	ITEM_STAR,
	ITEM_KOURA_W,
	ITEM_THUNDER,
	ITEM_F_BOX,
	ITEM_ITEMBOX,
	ITEM_BOMB,
	ITEM_BLOOPER,
	ITEM_BOO,
	ITEM_GOLD_MUSHROOM,
	ITEM_KILLER,

	ITEM_COUNT,
	ITEM_NONE
} MKDSItem;

typedef enum ItemConfigId : int {
	ITEM_CONF_GREEN_SHELL,
	ITEM_CONF_RED_SHELL,
	ITEM_CONF_BANANA,
	ITEM_CONF_FAKE_ITEM_BOX,
	ITEM_CONF_MUSHROOM,
	ITEM_CONF_TRIPLE_MUSHROOM,
	ITEM_CONF_BOMB,
	ITEM_CONF_BLUE_SHELL,
	ITEM_CONF_LIGHTNING,
	ITEM_CONF_TRIPLE_GREENS,
	ITEM_CONF_TRIPLE_BANANA,
	ITEM_CONF_TRIPLE_REDS,
	ITEM_CONF_STAR,
	ITEM_CONF_GOLD_MUSHROOM,
	ITEM_CONF_BULLET_BILL,
	ITEM_CONF_BLOOPER,
	ITEM_CONF_BOO,
	ITEM_CONF_INVALID17,
	ITEM_CONF_COUNT,
	ITEM_CONF_NONE,
} ItemConfigId;

typedef struct it_itemconfig_t
{
	bool32 enabled;
	bool32 wifiEnabled;
	MKDSItem type;
	s32 count;
	u32 field10;
	u32 field14;
	void* activateFunc;
} it_itemconfig_t;

typedef struct it_item_def_t
{
	u32 instanceSize;
	u32 limit;
	u32 field8;
	int instanceCount;
	int field10;
	void* loadFunc;
	void* initInstanceFunc;
	void* field1C;
	void* field20;
	void* field24;
	void* updateFunc;
	void* renderFunc;
	void* visibilityFlagCalcFunc;
	void* field34;
	void* field38;
	void* destroyInstFunc;
	int field40;
	int field44;
	void* field48;
	int field4C;
	void* field50;
	u32 gap54;
	void* field58;
	int field5C;
	int field60;
	int colSphereRadius;
	int sphereRadius1;
	int sphereRadius2;
	int field70;
	int scale;
	int field78;
	int field7C;
	int field80;
	int field84;
	int field88;
	u32 gap8C;
	int field90;
	int field94;
	int field98;
	int field9C;
	int fieldA0;
	int fieldA4;
} it_item_def_t;

static_assert(sizeof(it_item_def_t) == 0xa8);

#define ITEM_INST_FLAGS_BIT28    (1 << 28)

typedef struct
{
	sfx_emitter_t sfxEmitter;
	u32 type; // 0x44
	u32 field48;
	u16 field4C;
	u16 field4E;
	VecFx32 position; // 0x50
	VecFx32 velocity; // 0x5c
	VecFx32 scale; // 0x68
	u32 flags; // 0x74
	u16 field78;
	u16 field7A;
	light_t light;
	light_t* lightPtr;
	Orientation4D mtx;
	VecFx32 VectorC4;
	VecFx32* fieldD0;
	u32 visibilityFlags;
	s16 alpha;
	s16 colEntryId;
	fx32 collisionRadius;
	fx32 sphereSize;
	VecFx32 preMovementPos;
	VecFx32 fieldF0;
	u32 fieldFC;
	u32 field100;
	u32 field104;
	VecFx32 field108;
	u16 field114;
	u16 field116;
	u16 field118;
	u16 field11A;
	u32 field11C;
	void* field120;
	void* field124; // (it_item_inst_t*, unknown, VecFx32*)
	u8 field128DriverMask;
	u32 field12C;
	u32 field130;
} it_item_inst_t;

static_assert(sizeof(it_item_inst_t) == 0x134); // Uncertain.
// blue shell: 0x31C (shares something with red shell, for following the course path)
// red shell: 0x274
// bomb: 0x1a4
// green shell: 0x188
// fib: 0x144
// mushroom: 0x138
// banana: 0x134
// star: 0x12C

typedef struct it_itemset_t
{
	u32 id;
	it_item_inst_t** instances;
	u32 maximumCount; // length of the instances array
	s32 limitForSingle; // Not sure. But it seems to be the same as limit, except +2 for items that can come in triples. 
	u32 instanceCount; // how many instances (out-of-roullete items) exist
	u32 activeInstanceCount; // how many instances exist, excluding dragged items
	u32 droppedCount; // count of how many are sitting on the ground, not moving
	s32 countAll; // Includes all items in play including items in players' roulletes even if the roullete hasn't stopped yet.
	u32 limit; // 0x20 If this many exist, they will not be given by boxes and old ones will fade away?
	void* renderFunc;
	void* visibilityFlagCalcFunc;
	u32 itemParamsField70;
	fx32 scale;
	u32 itemParamsField78;
	u32 itemParamsField5C;
	u32 itemParamsFieldA4;
	bool32 renderingDisabled;
} it_itemset_t;

static_assert(sizeof(it_itemset_t) == 0x44);

//action buffer entry struct
typedef struct itnet_action_t_ {
	u8 data[20];
	MKDSItem itemType;
	u32 action;
	u32 field1C;
} itnet_action_t;

typedef struct item_probability_set { u16 probabilities[ITEM_CONF_COUNT]; } item_probability_set;
typedef struct item_probabilitis_by_placement {
	u32 placements;
	item_probability_set* sets; // ptr to array of size placements+1
} item_probabilitis_by_placement;

extern item_probabilitis_by_placement cpu_item_chances; // 0x0217bc44
extern item_probabilitis_by_placement player_item_chances; // 0x0217bc4c
extern item_probabilitis_by_placement special_item_chances; // 0x0217bc54

extern it_itemconfig_t* itemConfigs; // 0x0217bc28
extern it_itemset_t* itemSets; // DTCM, 0x27e00cc, points to array of 14
extern it_item_def_t* DAT_0217bc14; // 0x0217bc14

extern int blooper_available_timer; // 0x0217bc38
extern int blue_shell_available_timer; // 0x0217bc3c
extern int shock_available_timer; // 0x0217bc40

it_item_inst_t* it_spawnItem(MKDSItem item, u32 flags, const VecFx32* position, const VecFx32* up);
void it_startThrowAnim(it_item_inst_t* item, u32 a2, const VecFx32* position, const VecFx32* a4, const VecFx32* up);

#endif
