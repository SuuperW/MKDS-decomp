#pragma once

#ifndef HP_JNLIB_SPA_SPALIST_H
#define HP_JNLIB_SPA_SPALIST_H

#include "types.h"

typedef struct spa_list_link_t
{
	struct spa_list_link_t* next;
	struct spa_list_link_t* prev;
} spa_list_link_t;

typedef struct
{
	spa_list_link_t* head;
	u32 count;
	spa_list_link_t* tail;
} spa_list_t;

void spa_listPrepend(spa_list_t* list, spa_list_link_t* item);
spa_list_link_t* spa_listTakeHead(spa_list_t* list);
spa_list_link_t* spa_listRemove(spa_list_t* list, spa_list_link_t* item);

#endif
