#pragma once

#ifndef HP_JNLIB_SPA_SPAPARTICLE_H
#define HP_JNLIB_SPA_SPAPARTICLE_H

#include "spaList.h"
#include "spaEmitter.h"
#include "spaParticleSet.h"

typedef struct spa_particle_t
{
	spa_list_link_t listLink;
	VecFx32 position;
	VecFx32 velocity;
	u16 rotation;
	s16 rotationVelocity;
	u16 maxAge;
	u16 age;
	u16 progressSpeedLoop;
	u16 progressSpeedNoLoop;
	u8 textureId;
	u8 progressOffset;
	u16 baseAlpha : 5;
	u16 alpha : 5;
	u16 polygonId : 6;
	fx32 baseScale;
	fx16 scale;
	GXRgb color;
	VecFx32 basePosition;
} spa_particle_t;

static_assert(sizeof(spa_particle_t) == 0x44);

void spa_emitParticles(spa_emitter_t* emitter, spa_list_t* freeParticleList);
void spa_emitChildParticles(spa_particle_t* parent, spa_emitter_t* emitter, spa_list_t* freeParticleList);

static inline spa_particle_t* spa_getPreviousParticle(spa_particle_t* particle)
{
	return (spa_particle_t*)particle->listLink.prev;
}

static inline spa_particle_t* spa_getNextParticle(spa_particle_t* particle)
{
	return (spa_particle_t*)particle->listLink.next;
}

static inline void spa_killParticle(spa_particle_t* particle)
{
	particle->age = particle->maxAge;
}

#endif
