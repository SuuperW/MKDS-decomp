#pragma once

#ifndef HP_LIST_H
#define HP_LIST_H

#include <nnsys/fnd/list.h>

typedef struct
{
	NNSFndLink link;
	NNSFndList* list;
} list_link_t;

void list_initLink(list_link_t* link);
bool32 list_append(NNSFndList* list, void* object);
void list_remove(NNSFndList* list, void* object);

#endif
