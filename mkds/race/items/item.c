#include "item.h"
#include "race/raceStatus.h"
#include "types.h"

extern u32 FUN_020fa0e8(MKDSItem itemType, s32 param_2);
extern int LAB_0217a9f4;

bool EnsureItemIsntNone(ItemConfigId* item_param) {
	ItemConfigId item = *item_param;
	it_itemconfig_t config = itemConfigs[item];
	if (item != ITEM_CONF_NONE)
	{
		int countUntilLimit = itemSets[config.type].limitForSingle - itemSets[config.type].countAll;
		if (countUntilLimit >= config.count)
			return FALSE;
		
		if ((RaceConfig->current).raceMode == RACE_MODE_WIFI)
		{
			u32 uVar2 = FUN_020fa0e8(config.type, 0);
			if ((int)(config.count * uVar2) <= (int)(u32)*(u16*)(LAB_0217a9f4 + 0x724))
			{
				return FALSE;
			}
		}
	}

	if ((RaceConfig->current).raceMode == RACE_MODE_WIFI)
		*item_param  = ITEM_CONF_MUSHROOM;
	else
		*item_param  = ITEM_CONF_BANANA;

	return TRUE;
}

ItemConfigId RngItembox(int kart_ABC, int special_box_type, int placement, bool32 isNotEnemy, int isPlayer, int maxItemId_exclusive)
{
	item_probabilitis_by_placement* p_by_placement;
	u32 placementIndex;
	if (special_box_type == 0)
	{
		placementIndex = placement - 1;
		if (isNotEnemy)
			p_by_placement = &player_item_chances;
		else
			p_by_placement = &cpu_item_chances;
	}
	else
	{
		placementIndex = special_box_type - 1;
		kart_ABC = 0;
		p_by_placement = &special_item_chances;
	}
	item_probability_set* p_set = p_by_placement->sets + placementIndex;

	int nonDroppedCount = 0;
	if ((RaceConfig->current).raceMode != RACE_MODE_MISSION)
	{
		for (int i = 0; i < ITEM_COUNT; i++)
		{
			nonDroppedCount += DAT_0217bc14[i].field90 * (itemSets[i].countAll - itemSets[i].droppedCount);
		}
	}

	u32 totalProbability = 0;
	u16 probability_table[ITEM_CONF_COUNT]; // It's actually a global at 0x0217bc5c
	for (int i = 0; i < maxItemId_exclusive; i++)
	{
		bool itemIsRestrictedByTime =
		    (i == ITEM_CONF_LIGHTNING && shock_available_timer > 0) ||
		    (i == ITEM_CONF_BLUE_SHELL && blue_shell_available_timer > 0) ||
		    (i == ITEM_CONF_BLOOPER && blooper_available_timer > 0);
		if (itemIsRestrictedByTime) continue;

		it_itemconfig_t* config = itemConfigs + i;
		MKDSItem type = config->type;
		bool itemIsAvailable =
		    itemSets[type].limit > itemSets[type].countAll &&
		    config->enabled;
		if (itemIsAvailable)
		{
			int iVar20 = DAT_0217bc14[type].field90 * config->count;
			if (iVar20 == 0 || (iVar20 + nonDroppedCount < 41))
			{
				if ((i < 9) || (kart_ABC == 0)) // standard kart
				{
					totalProbability += p_set->probabilities[i];
				}
				else if (kart_ABC == 1) // 1-shroom kart
				{
					totalProbability += p_set->probabilities[i] >> 2;
				}
				else // 3-shroom kart
				{
					totalProbability += p_set->probabilities[i] * 3;
				}
			}
		}
		probability_table[i] = (short)totalProbability;
	}

	int random_value = 0;
	ItemConfigId selectedItem = ITEM_CONF_NONE;
	if (totalProbability != 0)
	{
		random_value = MATH_Rand32(&gRaceStatus->safeRng, totalProbability);
		selectedItem = 0;
		while (selectedItem < maxItemId_exclusive)
		{
			if (random_value < probability_table[selectedItem]) break;
			selectedItem++;
		}
	}

	if (EnsureItemIsntNone(&selectedItem))
	{
		// FUN_020faa78(selectedItem); // Something to do with sound.
	}

	// Looks like this is setting which items show in the spinning roullete.
	int DAT_0217bc30;
	int DAT_0217bc80[ITEM_CONF_COUNT];
	if (isPlayer != 0)
	{
		DAT_0217bc30 = 0;
		if (special_box_type != 0)
		{
			for (int i = 0; i < maxItemId_exclusive; i++)
			{
				if ((i == selectedItem) || p_set->probabilities[i] != 0)
				{
					DAT_0217bc80[DAT_0217bc30] = i;
					DAT_0217bc30++;
				}
			}
		}
	}
	return selectedItem;
}
