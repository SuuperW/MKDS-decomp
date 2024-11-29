#pragma once

#include <nnsys/g3d/gecom.h>
#include <nitro/math/rand.h>
#include <nitro/os/common/ownerInfo.h>
#include <nitro/spi/ARM9/pm.h>
#include "snd/sound3.h"
#include "overlay.h"

typedef enum
{
	MKDS_LANGUAGE_JA,
	MKDS_LANGUAGE_US,
	MKDS_LANGUAGE_FR,
	MKDS_LANGUAGE_GE,
	MKDS_LANGUAGE_IT,
	MKDS_LANGUAGE_ES,
	MKDS_LANGUAGE_US2 //huh?
} MKDSLanguage;

typedef enum
{
	MKDS_OVERLAYRELATEDNUM_0,
	MKDS_OVERLAYRELATEDNUM_1,
	MKDS_OVERLAYRELATEDNUM_MULTIPLAYER,
	MKDS_OVERLAYRELATEDNUM_WIFI,
	MKDS_OVERLAYRELATEDNUM_WIFI_UTIL
} MKDSOverlayRelatedNum;

typedef enum
{
	SYSDAT_BACKLIGHT_STATE_ON,
	SYSDAT_BACKLIGHT_STATE_OFF
} SysdatBacklightState;

typedef struct
{
	u32 field0;
	MKDSLanguage language;
	seq_handle_t seqHandle;
	bool32 isSeqPlaying : 1;
	bool32 isSeqLoaded : 1;
	bool32 isMBChild : 1;
	bool32 useG3dFastDma : 1;
	MKDSOverlayRelatedNum overlayRelatedNum; //0x20
	NNSFndHeapHandle dtcmHeapHandle;
	s16 field28;
	u16 nickName[OS_OWNERINFO_NICKNAME_MAX];
	u16 nickNameLength;
	u8 favoriteColor;
	int activatedRaceMenuOption;
	PMBackLightSwitch backLightTop;
	PMBackLightSwitch backLightBottom;
	u16 field50;
	u32 field54;
	u32 field58;
	u32 field5C;
	MATHRandContext32 random;
	u32 field78;
} sysdat_t;

extern sysdat_t* gSystemData;

void sysdat_init(NNSFndHeapHandle heapHandle);
void sysdat_resetDtcmHeap();
void sysdat_loadSeq(int seqId);
void sysdat_startSeq();
void sysdat_stopSeq();
void sysdat_unloadSeq();
void sysdat_204AB64(int fadeFrame);
MATHRandContext32* sysdat_getRandom();
void sysdat_setIsMBChild(bool32 isMBChild);

static inline MKDSOverlayRelatedNum sysdat_getOverlayRelatedNum()
{
	return gSystemData->overlayRelatedNum;
}

static inline void sysdat_setOverlayRelatedNum(MKDSOverlayRelatedNum overlay)
{
	gSystemData->overlayRelatedNum = overlay;
}

static inline void sysdat_setG3dUseFastDma(bool32 fastDma)
{
	NNS_G3dGeUseFastDma(fastDma);
	gSystemData->useG3dFastDma = fastDma;
}
