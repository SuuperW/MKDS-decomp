#pragma once

#ifndef HP_RACE_DRIVERS_CHARKART_H
#define HP_RACE_DRIVERS_CHARKART_H

#include "animationManager.h"
#include "kartOffsetData.h"
#include "race/light.h"
#include "race/raceConfig.h"

typedef struct
{
	int field0;
	int field4;
	int field8;
	int fieldC;
	u16 field10;
	int field14;
	u16 field18;
	u16 field1A;
} charkart_field24_t;

static_assert(sizeof(charkart_field24_t) == 0x1C);

typedef struct
{
	GXRgb diffuse;
	GXRgb emission;
	GXRgb ambient;
	u16 diffR;
	u16 diffG;
	u16 diffB;
	s16 diffRDelta;
	s16 diffGDelta;
	s16 diffBDelta;
	u16 emiR;
	u16 emiG;
	u16 emiB;
	s16 emiRDelta;
	s16 emiGDelta;
	s16 emiBDelta;
	u16 ambiR;
	u16 ambiG;
	u16 amibB;
	s16 amibRDelta;
	s16 ambiGDelta;
	s16 ambiBDelta;
	fx16 progress;
} charkart_colors_t;

static_assert(sizeof(charkart_colors_t ) == 0x2C);

typedef struct
{
	CharacterId characterId;
	s32 kartId;
	anim_manager_t* characterNsbcaAnim;
	anim_manager_t* characterNsbtpAnim;
	model_t* characterModel; // 0x10
	model_t* kartModel;
	model_t* kartTireModel;
	model_t* kartShadowModel;
	const kofs_entry_t* kartOffsetData; // 0x20
	charkart_field24_t field24;
	light_t light; // 0x40
	u32 field54; // 0x54
	bool32 isKartInvisible;
	bool32 isCharacterInvisible;
	bool32 useSeparateTires; // 0x60
	bool32 inStarToonMode;
	u16 kartABC;
	u16 padding;
	charkart_colors_t colors; // 0x68
	anim_animator_t field98; // 0x94
	bool32 nsbtpAnimDisabled;
	u32 fieldB0;
} charkart_t;

static_assert(sizeof(charkart_t ) == 0xB4);

#endif
