#pragma once

#ifndef HP_RACE_DRIVERS_DRIVER_H
#define HP_RACE_DRIVERS_DRIVER_H

#include <nnsys.h>
#include "types.h"

#include "sfx.h"
#include "charKart.h"
#include "math/quaternion.h"
#include "math/vector.h"
#include "physicalParams.h"
#include "input/input.h"
#include "enemy.h"
#include "race/struc_351.h"
#include "fx_convenience.h"

typedef struct mobj_inst_t mobj_inst_t;

typedef enum : int
{
	DRIVER_COLLISION_MODE_NORMAL,
	DRIVER_COLLISION_MODE_STAR, //invincible
	DRIVER_COLLISION_MODE_TERESA, //invisible
	DRIVER_COLLISION_MODE_KILLER,

	DRIVER_COLLISION_MODE_COUNT
} DriverCollisionMode;

typedef enum : int
{
	DRIVER_COLLISION_REACTION_NONE,
	DRIVER_COLLISION_REACTION_1,
	DRIVER_COLLISION_REACTION_SPIN_OUT,
	DRIVER_COLLISION_REACTION_FORWARD_FLIP,
	DRIVER_COLLISION_REACTION_BACKWARD_FLIP,
	DRIVER_COLLISION_REACTION_DOUBLE_BACKWARD_FLIP,
	DRIVER_COLLISION_REACTION_THROW_UP,

	DRIVER_COLLISION_REACTION_COUNT
} DriverCollisionReaction;

typedef enum : int
{
	DRIVER_COLLISION_TYPE_ROAD,
	DRIVER_COLLISION_TYPE_1,
	DRIVER_COLLISION_TYPE_WEAK_OFF_ROAD,
	DRIVER_COLLISION_TYPE_OFF_ROAD,
	DRIVER_COLLISION_TYPE_HEAVY_OFF_ROAD,
	DRIVER_COLLISION_TYPE_SLIPPERY_ROAD_1,
	DRIVER_COLLISION_TYPE_SLIPPERY_ROAD_2,
	DRIVER_COLLISION_TYPE_7,
	DRIVER_COLLISION_TYPE_BOOST, //both boost and jump pads
	DRIVER_COLLISION_TYPE_CANNON_ACTIVATOR,
	DRIVER_COLLISION_TYPE_FALLS_WATER,
	DRIVER_COLLISION_TYPE_LOOPING,

	DRIVER_COLLISION_TYPE_COUNT
} DriverCollisionType;

typedef enum : int
{
	DRIVER_VOICE_TYPE_SPIN,
	DRIVER_VOICE_TYPE_OT,
	DRIVER_VOICE_TYPE_OT1,
	DRIVER_VOICE_TYPE_OT2,
	DRIVER_VOICE_TYPE_OT3,
	DRIVER_VOICE_TYPE_STARTDASH,
	DRIVER_VOICE_TYPE_STARTDASH_OT,
	DRIVER_VOICE_TYPE_ATTACK,
	DRIVER_VOICE_TYPE_JOY,
	DRIVER_VOICE_TYPE_GOAL1,
	DRIVER_VOICE_TYPE_GOAL23,
	DRIVER_VOICE_TYPE_GOAL48,
	DRIVER_VOICE_TYPE_MG_WIN,
	DRIVER_VOICE_TYPE_MG_LOSE
} DriverVoiceType;

typedef struct
{
	bool32 gotStartBoost;
	u32 powerSlideCount;
	int itemHitCount;
	u32 offRoadTime;
	int wallHitCount;
	int damageCount;
	int respawnCount;
} driver_statistics_t;

typedef struct
{
	s16 shroomBoostTimer;
	s16 thunderShrinkTimer;
	s16 thunderGrowTimer;
	s16 starTimer;
	s16 slipstreamStartTimer;
	s16 slipstreamTimer;
	s16 thwompGrowTimer;
	s16 thwompFlatTimer;
	s16 teresaTimer;
	s16 teresaFlickerInterval;
	s16 teresaFlickerIntervalUpdateTimer;
	s16 teresaFlickerTimer;
	s16 teresaFlickerIntervalUpdateWaitTime;
	s16 teresaFlickerIntervalStep;
	s16 gessoInkTimer;
	s16 killerFrameCounter;
	u16 field20;
	bool32 isKillerFinishing;
	s16 killerTargetPlace;
	u8 gap2A[2];
	u8 killerState[0x590 - 0x564];
} driver_timers_t;

typedef struct
{
	int field0;
	int field4;
	int field8;
	int fieldC;
	int field10;
	int field14;
	int field18;
	int field1C;
	int field20;
	int field24;
} struc_334;

typedef struct
{
	int field0;
	u8 field4;
	fx32 field8;
	int fieldC;
	int field10; // 0x460
	int field14;
	fx32 field18;
	fx32 field1C;
	fx32 field20; // 0x470
	fx32 field24;
	int prevLapProgress;
	u8 kaidanSfxAlternateCounter;
	int field30; // 0x480
	int field34;
	int sfxId;
	int (*computePitchOffsetFunc)(sfx_emitter_ex_params_t*);
	struc_334 field40; // 0x490
	int field68;
} driver_field450_t;

typedef struct
{
	NNSFndLink link;
	s16 field8;
	u8 gapA[2];
	struc_351 fieldC;
} driver_field514_field8C_entry_t;

#define DRIVER_NET_FLAGS_MINI_JUMP                    (1 << 0)
#define DRIVER_NET_FLAGS_SINGLE_SPINOUT_START         (1 << 2)
#define DRIVER_NET_FLAGS_DOUBLE_SPINOUT_START         (1 << 3)
#define DRIVER_NET_FLAGS_FORWARD_FLIP_START           (1 << 4)
#define DRIVER_NET_FLAGS_SINGLE_BACKWARD_FLIP_START   (1 << 5)
#define DRIVER_NET_FLAGS_DOUBLE_BACKWARD_FLIP_START   (1 << 6)
#define DRIVER_NET_FLAGS_BIT7                         (1 << 7)
#define DRIVER_NET_FLAGS_BIT9                         (1 << 9)
#define DRIVER_NET_FLAGS_BIT10                        (1 << 10)
#define DRIVER_NET_FLAGS_BIT11                        (1 << 11)
#define DRIVER_NET_FLAGS_BIT12                        (1 << 12)
#define DRIVER_NET_FLAGS_STAR_INVINCIBLE_START        (1 << 13)
#define DRIVER_NET_FLAGS_THUNDER_INVOKE               (1 << 14)
#define DRIVER_NET_FLAGS_THUNDER_SHRINK_START         (1 << 15)
#define DRIVER_NET_FLAGS_BIT16                        (1 << 16)
#define DRIVER_NET_FLAGS_THROW_UP_START               (1 << 17)
#define DRIVER_NET_FLAGS_SLIPSTREAM_START             (1 << 18)
#define DRIVER_NET_FLAGS_BLOOPER_INK_START              (1 << 19)
#define DRIVER_NET_FLAGS_KILLER_START                 (1 << 20)
#define DRIVER_NET_FLAGS_KILLER_FINISH                (1 << 21)
#define DRIVER_NET_FLAGS_BIT22                        (1 << 22)
#define DRIVER_NET_FLAGS_BIT23                        (1 << 23)
#define DRIVER_NET_FLAGS_BIT24                        (1 << 24)
#define DRIVER_NET_FLAGS_BIT25                        (1 << 25)
#define DRIVER_NET_FLAGS_TERESA_START                 (1 << 26)

#define DRIVER_NET_FLAGS_BIT29_30                     (3 << 29)

typedef struct
{
	VecFx32 position;
	u16 fieldC;
	u16 fieldE;
	VecFx32 field10;
	int field1C;
	int field20;
	fx32 field24;
	int field28;
	int flags;
	int lastFlags;
	VecFx32 field34;
	s16 driftRotY;
	int field44;
	u32 field48;
	quaternion_t field4C;
	s16 field5C;
	int field60;
	int field64;
	u8 gap68[24];
	NNSFndList field80;
	NNSFndList field8C;
	VecFx32 field98;
	u32 fieldA4;
	VecFx32 fieldA8;
	u16 fieldB4;
	u8 gapB6[1];
	u8 fieldB7;
} driver_net_state_t;

#define DRIVER_FIELD44_SLIP_DRIFT               (1 << 0) // 01
#define DRIVER_FIELD44_HOPPING                  (1 << 1) // 02
#define DRIVER_FIELD44_HOP_LOCK                 (1 << 2) // 04
#define DRIVER_FIELD44_IS_DRIFTING              (1 << 3) // 08
#define DRIVER_FIELD44_IS_ON_FLOOR              (1 << 4) // 10
#define DRIVER_FIELD44_IS_TOUCHING_DRIVER       (1 << 5)
#define DRIVER_FIELD44_IS_TOUCHING_MOBJ_WALL    (1 << 6)
#define DRIVER_FIELD44_IS_TOUCHING_WALL         (1 << 7)
#define DRIVER_FIELD44_IS_GOING_BACKWARDS       (1 << 8)
#define DRIVER_FIELD44_BIT9                     (1 << 9)
#define DRIVER_FIELD44_BIT10                    (1 << 10)
#define DRIVER_FIELD44_BIT11                    (1 << 11)
#define DRIVER_FIELD44_BIT12                    (1 << 12)
#define DRIVER_FIELD44_BOUNCE2_ACTIVE           (1 << 13)
#define DRIVER_FIELD44_BIT14                    (1 << 14)
#define DRIVER_FIELD44_BIT15                    (1 << 15)
#define DRIVER_FIELD44_BIT16                    (1 << 16)
#define DRIVER_FIELD44_BIT18                    (1 << 18)
#define DRIVER_FIELD44_BIT19                    (1 << 19)
#define DRIVER_FIELD44_IS_ON_FALLS_WATER        (1 << 20)
#define DRIVER_FIELD44_BIT21                    (1 << 21)
#define DRIVER_FIELD44_BIT23                    (1 << 23)
#define DRIVER_FIELD44_BIT24                    (1 << 24)
#define DRIVER_FIELD44_BIT26                    (1 << 26)
#define DRIVER_FIELD44_BIT27                    (1 << 27)
#define DRIVER_FIELD44_BIT28                    (1 << 28)
#define DRIVER_FIELD44_BIT29                    (1 << 29)
#define DRIVER_FIELD44_BIT30                    (1 << 30)
#define DRIVER_FIELD44_BIT31                    (1 << 31)

// 0x48
typedef enum driverFlags48 : u32 {
	DRIVER_48_IN_LOOP              = (1 << 0),
	DRIVER_48_A_BUTTON             = (1 << 1),
	DRIVER_48_B_BUTTON             = (1 << 2),
	DRIVER_48_DPAD_LEFT            = (1 << 3),
	DRIVER_48_DPAD_RIGHT           = (1 << 4),
	DRIVER_48_BOOST                = (1 << 5),
	DRIVER_48_RESPAWNING           = (1 << 6),
	DRIVER_48_WHEELSPINNING        = (1 << 7),
	DRIVER_48_BIT8                 = (1 << 8),
	DRIVER_48_BIT9                 = (1 << 9),
	DRIVER_48_BIT10                = (1 << 10),
	DRIVER_48_BIT11                = (1 << 11),
	DRIVER_48_BIT12                = (1 << 12),
	DRIVER_48_FORCE_STOP           = (1 << 13),
	DRIVER_48_BIT14                = (1 << 14),
	DRIVER_48_START_BOOST          = (1 << 15),
	DRIVER_48_CANNON               = (1 << 16),
	DRIVER_48_BIT17                = (1 << 17),
	DRIVER_48_BIT18                = (1 << 18),
	DRIVER_48_DRIFT_BOOST          = (1 << 19),
	DRIVER_48_SHROOM_BOOST         = (1 << 20),
	DRIVER_48_SFX_TRIGGER_HANDLED  = (1 << 21),
	DRIVER_48_MIN_SPEED_BOOST      = (1 << 22),
	DRIVER_48_WATER_RESPAWN        = (1 << 23),
	DRIVER_48_LAVA_RESPAWN         = (1 << 24),
	DRIVER_48_RESPAWN_START        = (1 << 25),
	DRIVER_48_THWOMP_FLATTEN_CAM   = (1 << 26),
	DRIVER_48_FAR_FROM_PLAYER      = (1 << 27),
	DRIVER_48_ON_JUMP_PAD          = (1 << 28),
	DRIVER_48_PRB                  = (1 << 29),
	DRIVER_48_ANTIGRAVITY_CAM      = (1 << 30),
	DRIVER_48_BIT31                = ((u32)1 << 31)
} driverFlags48;

// 0x4C
typedef enum driverFlags4C : u32 {
	DRIVER_4C_BIT0                   = (1 << 0),
	DRIVER_4C_BIT1                   = (1 << 1),
	DRIVER_4C_BIT3                   = (1 << 3),
	DRIVER_4C_BIT4                   = (1 << 4),
	DRIVER_4C_SPIN_OUT               = (1 << 5),
	DRIVER_4C_STAR_INVINCIBLE        = (1 << 6),
	DRIVER_4C_THUNDER_SHRINK         = (1 << 7),
	DRIVER_4C_GROWING                = (1 << 8),
	DRIVER_4C_BIT10                  = (1 << 10),
	DRIVER_4C_BIT11                  = (1 << 11),
	DRIVER_4C_BIT12                  = (1 << 12),
	DRIVER_4C_SLIPSTREAM_FULL        = (1 << 13),
	DRIVER_4C_SLIPSTREAM_BEGIN       = (1 << 14),
	DRIVER_4C_BIT15                  = (1 << 15),
	DRIVER_4C_DOSSUN_FLAT            = (1 << 16),
	DRIVER_4C_BLOOPER_INK_FADING_OUT = (1 << 17),
	DRIVER_4C_BIT18                  = (1 << 18),
	DRIVER_4C_BIT20                  = (1 << 20),
	DRIVER_4C_BIT21                  = (1 << 21),
	DRIVER_4C_BIT22                  = (1 << 22),
	DRIVER_4C_INVISIBLE              = (1 << 23),
	DRIVER_4C_BIT25                  = (1 << 25),
	DRIVER_4C_BIT26                  = (1 << 26),
	DRIVER_4C_BLOOPER_INK            = (1 << 27),
	DRIVER_4C_IN_BULLET              = (1 << 28),
	DRIVER_4C_START_FAILURE          = (1 << 29),
	DRIVER_4C_FALLS_WATER            = (1 << 30),
	DRIVER_4C_BIT31                  = ((u32)1 << 31)
} driverFlags4C;

#define DRIVER_FIELD7C_IS_PLAYER            (1 << 0)
#define DRIVER_FIELD7C_IS_ENEMY             (1 << 1)
#define DRIVER_FIELD7C_IS_GHOST             (1 << 2)
#define DRIVER_FIELD7C_BIT3                 (1 << 3)
#define DRIVER_FIELD7C_CHAR_HAS_NSBCA_ANIM  (1 << 4)
#define DRIVER_FIELD7C_IS_BEING_KILLED      (1 << 5)
#define DRIVER_FIELD7C_IS_KILLED            (1 << 6)
#define DRIVER_FIELD7C_BIT7                 (1 << 7)
#define DRIVER_FIELD7C_BIT8                 (1 << 8)
#define DRIVER_FIELD7C_IS_NET_PLAYER        (1 << 9)
#define DRIVER_FIELD7C_IS_NET_NON_PLAYER    (1 << 10)
#define DRIVER_FIELD7C_BIT12                (1 << 12)
#define DRIVER_FIELD7C_BOO_EFFECT_ACTIVE (1 << 13)
#define DRIVER_FIELD7C_BOO_FLICKER       (1 << 14)
#define DRIVER_FIELD7C_BOO_RENDERING_OFF (1 << 15)
#define DRIVER_FIELD7C_BIT16                (1 << 16)
#define DRIVER_FIELD7C_BIT17                (1 << 17)
#define DRIVER_FIELD7C_MG_KILL              (1 << 18)
#define DRIVER_FIELD7C_BIT19                (1 << 19)
#define DRIVER_FIELD7C_BIT20                (1 << 20)
#define DRIVER_FIELD7C_BIT21                (1 << 21)
#define DRIVER_FIELD7C_MG_KILL_GHOST        (1 << 22)   // eliminated mg player that appears as ghost that can place item boxes
#define DRIVER_FIELD7C_BIT23                (1 << 23)
#define DRIVER_FIELD7C_BIT24                (1 << 24)
#define DRIVER_FIELD7C_BIT25                (1 << 25)
#define DRIVER_FIELD7C_BIT26                (1 << 26)
#define DRIVER_FIELD7C_BIT27                (1 << 27)
#define DRIVER_FIELD7C_BIT28                (1 << 28)
#define DRIVER_FIELD7C_BIT29                (1 << 29)

typedef struct {
	int SLIP_DRIFT:1;
	int HOPPING:1;
	int HOP_LOCK:1;
	int IS_DRIFTING:1;
	int IS_ON_FLOOR:1;
	int IS_TOUCHING_DRIVER:1;
	int IS_TOUCHING_MOBJ_WALL:1;
	int IS_TOUCHING_WALL:1;
	int IS_GOING_BACKWARDS:1;
	int BIT9:1;
	int BIT10:1;
	int BIT11:1;
	int BIT12:1;
	int BOUNCE2_ACTIVE:1;
	int BIT14:1;
	int BIT15:1;
	int BIT16:1;
	int BIT17:1;
	int BIT18:1;
	int BIT19:1;
	int IS_ON_FALLS_WATER:1;
	int BIT21:1;
	int BIT22:1;
	int BIT23:1;
	int BIT24:1;
	int BIT25:1;
	int BIT26:1;
	int BIT27:1;
	int BIT28:1;
	int BIT29:1;
	int BIT30:1;
	int BIT31:1;
} racerFlags44;

typedef struct {
	int IN_LOOP:1;
	int A_BUTTON:1;
	int B_BUTTON:1;
	int DPAD_LEFT:1;
	int DPAD_RIGHT:1;
	int BOOST:1;
	int RESPAWNING:1;
	int WHEELSPINNING:1;
	int BIT8:1;
	int BIT9:1;
	int BIT10:1;
	int BIT11:1;
	int BIT12:1;
	int FORCE_STOP:1;
	int BIT14:1;
	int START_BOOST:1;
	int CANNON:1;
	int BIT17:1;
	int BIT18:1;
	int DRIFT_BOOST:1;
	int SHROOM_BOOST:1;
	int SFX_TRIGGER_HANDLED:1;
	int MIN_SPEED_BOOST:1;
	int WATER_RESPAWN:1;
	int LAVA_RESPAWN:1;
	int RESPAWN_START:1;
	int THWOMP_FLATTEN_CAM:1;
	int FAR_FROM_PLAYER:1;
	int ON_JUMP_PAD:1;
	int BIT29:1;
	int ANTIGRAVITY_CAM:1;
	int BIT31:1;
} racerFlags48;

typedef struct {
	int BIT0:1;
	int BIT1:1;
	int BIT2:1;
	int BIT3:1;
	int BIT4:1; // 0x10
	int SPIN_OUT:1;
	int STAR_INVINCIBLE:1;
	int THUNDER_SHRINK:1;
	int GROWING:1; // 0x1_00
	int BIT9:1;
	int BIT10:1;
	int BIT11:1;
	int BIT12:1; // 0x10_00
	int SLIPSTREAM_FULL:1;
	int SLIPSTREAM_BEGIN:1;
	int BIT15:1;
	int DOSSUN_FLAT:1;
	int BLOOPER_INK_FADING_OUT:1;
	int BIT18:1;
	int BIT19:1;
	int BIT20:1;
	int BIT21:1;
	int BIT22:1;
	int INVISIBLE:1;
	int BIT24:1;
	int BIT25:1;
	int BIT26:1;
	int BLOOPER_INK:1;
	int IN_BULLET:1;
	int START_FAILURE:1;
	int FALLS_WATER:1;
	int BIT31:1;
} racerFlags4C;

typedef struct {
	int IS_PLAYER:1;
	int IS_ENEMY:1;
	int IS_GHOST:1;
	int BIT3:1;
	int CHAR_HAS_NSBCA_ANIM:1; // 0x10
	int IS_BEING_KILLED:1;
	int IS_KILLED:1;
	int BIT7:1;
	int BIT8:1; // 0x1 00
	int IS_NET_PLAYER:1;
	int IS_NET_NON_PLAYER:1;
	int BIT11:1;
	int BIT12:1; // 0x10 00
	int BOO_EFFECT_ACTIVE:1;
	int BOO_FLICKER:1;
	int BOO_RENDERING_OFF:1;
	int BIT16:1; // 0x1 0000
	int BIT17:1;
	int MG_KILL:1;
	int BIT19:1;
	int BIT20:1; // 0x10 0000
	int BIT21:1;
	int MG_KILL_GHOST:1;
	int BIT23:1;
	int BIT24:1; // 0x1 000000
	int BIT25:1;
	int BIT26:1;
	int BIT27:1;
	int BIT28:1; // 0x10 000000
	int BIT29:1;
	int BIT30:1;
	int BIT31:1;
} racerFlags7C;

struct racerData;
typedef void (*racerFunction) (struct racerData* racer);

typedef struct {
	union {
		racerFunction funcs[9];
		struct {
			racerFunction func1;
			racerFunction oob;
			racerFunction eachFrame1;
			racerFunction eachFrame2;
			racerFunction eachFrame0;
			racerFunction func6;
			racerFunction func7;
			racerFunction func8;
			racerFunction func9;
		};
	};
} racerFunctions;

typedef struct racerData
{
	sfx_emitter_t soundEmitter;
	union { u32 flags44; racerFlags44 flagsA; };
	union { u32 flags48; racerFlags48 flagsB; };
	union { u32 flags4C; racerFlags4C flagsC; };
	VecFx32 targetMovementVector;
	VecFx32 tmvSigned;
	VecFx32 movementVector;
	u16 playerId;
	u16 padding;
	InputUnitId inputId;
	union { u32 flags7C; racerFlags7C flagsD; };
	VecFx32 position;
	VecFx32 lastPosition;
	VecFx32 kartTiresPosition;
	VecFx32 basePositionDelta;
	VecFx32 basePosDeltaNormalized;
	VecFx32 scale;
	fx32 fieldC8;
	fx32 targetMaxSpeed;
	fx32 currentMaxSpeed;
	s32 fastFallMaxSpeedMultiplier;
	fx32 slipstreamSpeedMultiplier;
	fx32 offroadSpeedMultiplier;
	quaternion_t fA_quaternion;
	quaternion_t surfaceNormalQuaternion;
	quaternion_t SN_Target;
	quaternion_t facingQuaternion;
	Orientation4D facingOrientation;
	union {
		Orientation4D kartOrientation4D;
		struct {
			Orientation kartOrientation;
			VecFx32 renderLocation;
		};
	};
	DriverCollisionReaction colReaction;
	Orientation4D field184;
	Orientation4D* charKartMtx;
	VecFx32 positionForCollision; // 0x1b8
	VecFx32 preMovementPosForCollision;
	fx32 colSphereSize; // 0x1d0 (radius)
	fx32 itemHitboxOffset;
	VecFx32 itemPos; // Also for racer-racer collisions
	VecFx32 previousItemPos; // Or is it pre-movement?
	VecFx32 colPos2;
	VecFx32 wallBounce1; // Fully preserved when in the air. Happens when you hit a corner or non-aligned wall.
	u32* field208;
	racerFunctions functions;
	racerFunction func230;
	s16 pitch;
	u16 facingAngle;
	s16 boostTimer;
	s16 field23A;
	s16 mtBoostTimer;
	u16 padding2;
	fx32 grip;
	VecFx32 surfaceNormalVector;
	VecFx32 wallNormal;
	VecFx32 verticalVelocity; // It is a vector, but only the Y component is ever used!
	VecFx32 fallsWaterForward;
	fx32 fallsWaterStrength;
	VecFx32 driftlessTMV;
	VecFx32 jumpDriftUp;
	VecFx32 jumpDriftForward;
	DriverCollisionMode collisionMode;
	fx32 maxSpeedFraction; // It's in 3D! // 0x2a0
	fx32 basePosDeltaMag;
	fx32 speed;
	fx32 lastGroundHeight;
	u16 driverHitCheckMask; // 0x2b0
	u16 driverHitMask;
	u16 lastDriverHitMask;
	u8 gap2B6[2];
	int field2B8;
	s32 hopHeightForCamera;
	u16 hopRotation;
	u16 padding3;
	fx32 driftDirection;
	s16 colEntryId1;
	s16 colEntryId2;
	physp_kart_params_t* kartStats;
	physp_char_params_t* characterStats;
	fx32 turnLoss;
	VecFx32 bounce2_c; // copied to b
	VecFx32 bounce2_b; // copied to bounce2
	VecFx32 bounce2; // Decays slowly when in the air. From shroomless wall bounces, bumpers, obstacles, racer collisions...
	s32 driftLeftRightCount;
	u16 driftLeftCount;
	u16 driftRightCount;
	u16* driftInsideCountPtr;
	u16* driftOutsideCountPtr;
	s32 mtChargeTimer;
	enemy_t* enemyState; // 0x310
	u16 field314;
	fx32 field318;
	VecFx32 field31C;
	VecFx32 field328;
	u32 field334;
	u16 field338;
	u32 field33C;
	fx32 field340;
	u32 field344;
	u32 field348;
	quaternion_t field34C;
	s16 colReactionCounter;
	fx32 field360;
	u16 spinOutAngle;
	u16 spinOutSpinCount;
	fx32 spinOutProgress;
	u32 spinOutVelocity;
	u16 field370;
	VecFx32 bounce3; // edge wall and RR slopes
	u32 framesInAir;
	u16 ghostFlickerPhase;
	s16 wallRotYSpeed; // around Y axis, not pitch
	s16 driftAngle;
	fx16 wideDriftTurnSpeed;
	fx32 wallMaxSpeedMultiplier;
	u8 gap390[4];
	u32 someMaxSpeedMultiplier;
	fx32 field398;
	fx32 someMaxSpeedAdd;
	fx32 field3A0;
	u16 tireRotX;
	s32 snQuaternionChangeRate;
	u16 respawnCounter;
	VecFx32 field3B0;
	u16 field3BC;
	s16 field3BE;
	s16 preRespawnCounter;
	s32 respawnId;
	s16 killTimer;
	u32 driverVoiceIdx;
	s16 kartABC;
	s16 field3D2;
	s16 field3D4;
	s32 place;
	DriverCollisionType floorDriverColType;
	u32 floorColType;
	int floorColVariant;
	s16 pitchMomentum;
	u32 yRotSpeedTarget;
	u32 yRotSpeed;
	fx32 field3F4;
	fx32 airSpeed;
	u16 field3FC;
	u16 field3FE;
	u16 field400;
	fx32 field404;
	u32 field408;
	u32 respawnStartFrame;
	u32 respawnAPressFrame;
	fx32 field414;
	fx32 field418;
	VecFx32 growBackScale; // 0x41C
	VecFx32 thunderScale; // 0x428
	fx32 dossunYScale; // 0x434
	mobj_inst_t* mobjHitList[2]; // 0x438
	u16 mobjHitSfxTimeout[2];
	bool32 mobjHitEmittedSfx[2];
	mobj_inst_t* smashDossun;
	driver_field450_t field450;
	fx32 field4BC;
	u32 colFlagsMap2DShadow;
	u32 jumpPadSpeed;
	fx32 field4C8;
	u32 field4CC;
	u32 field4D0;
	fx32 preStartEnginePower;
	s16 fallsWaterDstId;
	s16 wallTouchTimeout;
	s16 floorTouchTimeout;
	s16 someAngleAdd;
	s16 field4E0;
	s16 field4E2;
	u16 field4E4;
	u16 field4E6;
	fx32 field4E8;
	fx32 field4EC;
	VecFx32 idkScale;
	u16 field4FC;
	fx32 waterDepth;
	u16 field504;
	u16 field506;
	VecFx32* ptrPositionForRender;
	quaternion_t* ptrFacingQuaternion;
	VecFx32* somePositionPtr;
	driver_net_state_t* netState;
	sfx_emitter_ex_params_t field518;
	VecFx32* somePositionPtr2;
	driver_timers_t timers; // 0x538
	charkart_t* charKart;
	fx32 field594;
	s16 field598;
	u32 field59C;
	u16 field5A0;
	u8 gap5A2[2];
	fx32 field5A4;
} racerData;
static_assert(sizeof(racerData) == 0x5A8);

typedef s32 (racer_MObjResponseHandler)(racerData* racer, s32* someWallTypeOutput, mobj_inst_t* object);
extern racer_MObjResponseHandler* Racer_MObjResponseHandlers[15];

typedef s32 (racer_MObjWallResponseHandler)(racerData* racer, VecFx32* wallNormal);
extern racer_MObjWallResponseHandler* Racer_MObjWallResponseHandlers[15];

extern fx32 DAT_021654ec;

racerData* driver_getById(u32 driverId);

driver_statistics_t* driver_getPlayerStatistics();

static inline bool32 driver_isPlayer(racerData* driver)
{
	return (driver->flags7C & DRIVER_FIELD7C_IS_PLAYER) != 0;
}

static inline bool32 driver_isEnemy(racerData* driver)
{
	return (driver->flags7C & DRIVER_FIELD7C_IS_ENEMY) != 0;
}

static inline const VecFx32* driver_getPositionById(u16 driverId)
{
	return &driver_getById(driverId)->position;
}

static inline bool32 driver_isNearby(u16 driverId, const VecFx32* position, fx32 distance)
{
	const VecFx32* driverPos = driver_getPositionById(driverId);
	return vec_isNearbyXZ(driverPos, position, distance);
}

static inline bool32 driver_isAnyNearby(const VecFx32* position, fx32 distance)
{
	int i;
	for (i = 0; i < (int)rconf_getDriverCount(); i++)
	{
		if (driver_isNearby(i, position, distance))
			return TRUE;
	}
	return FALSE;
}

void driver_updateTargetMaxSpeed(racerData* racer)
{
	physp_kart_params_t* stats = racer->kartStats;
	racer->targetMaxSpeed = FX_MulFunc4(
		stats->maxSpeed,
		racer->offroadSpeedMultiplier,
		racer->fastFallMaxSpeedMultiplier,
		racer->slipstreamSpeedMultiplier
	);

	// If is a ghost:
	if ((racer->flags7C & (DRIVER_FIELD7C_MG_KILL_GHOST | DRIVER_FIELD7C_BOO_EFFECT_ACTIVE)) != 0) {
		// Basically, undo fastfall, offroad, and slipstreams.
		racer->targetMaxSpeed = stats->maxSpeed;
		racer->offroadSpeedMultiplier = 0x1000;
	}

	if ((racer->flags48 & DRIVER_48_ON_JUMP_PAD) != 0) {
		racer->targetMaxSpeed = racer->jumpPadSpeed;
		racer->offroadSpeedMultiplier = 0x1000;
	}

	const u32 invincibleFlags = DRIVER_4C_STAR_INVINCIBLE | DRIVER_4C_IN_BULLET;
	bool invincible = (racer->flags4C & invincibleFlags) != 0;
	if ((racer->flags48 & DRIVER_48_BOOST) != 0)
	{
		if (invincible || (racer->flags48 & DRIVER_48_PRB)) {
			racer->targetMaxSpeed = stats->maxSpeed;
			racer->offroadSpeedMultiplier = 0x1000;
		}
		// *= 1.3
		racer->targetMaxSpeed = FX_MulFunc(racer->targetMaxSpeed, 0x14cd);		
		if ((racer->flags48 & (DRIVER_48_MIN_SPEED_BOOST | DRIVER_48_IN_LOOP)) != 0) {
			if (racer->targetMaxSpeed < 0x8800) {
				racer->targetMaxSpeed = 0x8800;
				racer->offroadSpeedMultiplier = 0x1000;
			}
		}
		return;
	}
	else if (invincible)
	{
		// x1.2, with no other multipliers
		racer->targetMaxSpeed = FX_MulFunc(stats->maxSpeed, 0x1333);
		racer->offroadSpeedMultiplier = 0x1000;
	}
}

// 0x020d41e0
extern int racer_CheckObjectCollisions(racerData* racerData, VecFx32* racerPosition, VecFx32* out_totalPush);

// 0x020d6be0
extern int racer_CollideWithObject(racerData* racer, mobj_inst_t* object);

// 0x0206f04c
extern bool racer_touchWall(racerData* racerData, VecFx32* wallPushDirection, bool wasTouchingWall, int param_4, int param_5, int param_6, int param_7);

#endif
