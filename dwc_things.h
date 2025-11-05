#pragma once
// NitroDWC package is missing a bunch of files that it needs, and I don't know where they are. So...
// Here are a few types from DWC.

#ifndef HPS_DWC_THINGS_H
#define HPS_DWC_THINGS_H

#include <types.h>

typedef struct DWCstAccFlag
{
    u32         flags;
    u32         reserved;
    u32         reserved1;
} DWCAccFlag;

typedef struct DWCstAccLoginId
{
    u32         id_data;
    u32         userid_lo32;
    u32         playerid;
} DWCAccLoginId;

typedef struct DWCstAccFriendKey
{
    u32         id_data;
    u32         friendkey_lo32;
    u32         friendkey_hi32;
} DWCAccFriendKey;

typedef struct DWCstAccGsProfileId
{
    u32         id_data;
    int         id;
    u32         reserved;
} DWCAccGsProfileId;


typedef union DWCstAccFriendData
{
	DWCAccFlag          flags;
	DWCAccLoginId       login_id;
	DWCAccFriendKey     friend_key;
	DWCAccGsProfileId   gs_profile_id;
} DWCAccFriendData;

typedef struct DWCstAccUserData
{
    int                 size;               // 4
    DWCAccLoginId       pseudo;             // 12
    DWCAccLoginId       authentic;          // 12
    int                 gs_profile_id;      // 4
    int                 flag;               // 4
    u32                 gamecode;           // 4

    int                 reserved[5];        // 20
    u32                 crc32;              // 4
} DWCAccUserData;

#endif
