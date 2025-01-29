
#pragma once

#ifndef SD_COLLISION_H
#define SD_COLLISION_H

#include <vector.h>
#include <math.h>

#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "collision.h"

typedef s32 fx10_22;
#define FX10_22_SHIFT 22
#define FX_MUL_10(v1, v2) (fx10_22)(((s64)(v1) * (v2)) >> 2)
typedef struct {
	fx10_22 x, y, z;
} Vec3_fx10_22;

// Memory addresses
extern int iRam0217b5fc;
extern int iRam0217b5f8;
extern int iRam0217b600;
extern int DAT_0217b608;
extern int iRam0217b60c;
extern int iRam0217b610;
extern int DAT_0217b614;
extern int iRam0217b618;
extern int iRam0217b61c;
extern u32 uRam0217b620;
extern u32 uRam0217b624;
extern u32 uRam0217b628;

void FUN_01fff434(int dotProductOutVecs, int distance1, int distance2,
	VecFx32* out_param_4, VecFx32* outVec2, VecFx32* outVec1, s64* outTotalDistanceSq)
{
	// The output is the distance between hitbox center projected onto triangle's plane, and the nearest vertex.
	// This does depend on assumptions about what is passed in:
	// 1) The total distance to triangle != projected distance from a line
	// 2) The ordering of d1, d2 and v1, v2 in the params does matter.

	s32 iVar1 = FX_Div(
		fxMulT(dotProductOutVecs, distance1) - distance2,
		fxMulT(dotProductOutVecs, dotProductOutVecs) - 0x1000
	);
	s32 iVar2 = distance1 - fxMulT(iVar1, dotProductOutVecs);

	VecFx32 scaled1;
	VecFx32 scaled2;
	MultiplyVector_T(outVec2, iVar1, &scaled1);
	MultiplyVector_T(outVec1, iVar2, &scaled2);
	VEC_Add(&scaled1, &scaled2, out_param_4);
	
	*outTotalDistanceSq = (s64)out_param_4->z * (s64)out_param_4->z +
				   (s64)out_param_4->x * (s64)out_param_4->x +
				   (s64)out_param_4->y * (s64)out_param_4->y;
	return;
}

extern void FUN_020d5180(VecFx32* a, int b, int c);

bool col_collide(
	VecFx32* position, VecFx32* prevPosition,
	VecFx32* direction, fx32 radius,
	colliderType collider, short colEntryId,
	VecFx32* out_pushback, VecFx32* out_floorNormal,
	VecFx32* out_wallNormal, u32* out_ColFlags,
	VecFx32* out_param_11, VecFx32* out_param_12,
	u16 *out_objTurnRacer, struct dynamicCollisionObject*** dcolResults)
{
	s64 lVar1;
	s64 lVar2;
	u32 uVar4;
	VecFx32 *pVVar7;
	u32 vertexId;
	fx32 nUpDistance;
	bool kindoffar;
	bool hasOut8Plus;
	u16 *triangleEnumerator;
	bool touchedSomething;
	u32 accumulatedSurfaceProps;
	s64 outDistance_2D_sq;
	VecFx32 playerPosition;
	VecFx32 _previousPosition;
	VecFx32 totalFloorPush;
	VecFx32 previousDistanceFromVertex;
	VecFx32 maxLandablePushComponents;
	VecFx32 minLandablePushComponents;
	int maxLandableOutwardPushDist;
	VecFx32 maxLandablePushNormal;
	VecFx32 maxRegWallPushComponents;
	VecFx32 minRegWallPushComponents;
	int maxWallOutwardPushDist;
	VecFx32 biggestWallPushDirection;
	Vec3_fx10_22 maxEdgeWallPushComponents;
	VecFx32 maxWallPushyComponents;
	Vec3_fx10_22 minWallPushyComponents;
	VecFx32 maybe_distanceToTriangle_LargestVertical;
	VecFx32 *triangleVertexes;
	VecFx32 *triangleVectors = collisionVectors;

	triangleVertexes = ::collisionVertexes;
	touchedSomething = false;
	hasOut8Plus = true;
	if (out_pushback == NULL && out_floorNormal == NULL && out_wallNormal == NULL) {
		hasOut8Plus = false;
	}
	VecFx32 trash;


	accumulatedSurfaceProps = 0;
	maxLandablePushComponents.x = 0;
	maxLandablePushComponents.y = 0;
	maxLandablePushComponents.z = 0;
	minLandablePushComponents.x = 0;
	minLandablePushComponents.y = 0;
	minLandablePushComponents.z = 0;
	maxLandableOutwardPushDist = -1;
	maxLandablePushNormal.x = 0;
	maxLandablePushNormal.y = 0;
	maxLandablePushNormal.z = 0;
	maxRegWallPushComponents.x = 0;
	maxRegWallPushComponents.y = 0;
	maxRegWallPushComponents.z = 0;
	minRegWallPushComponents.x = 0;
	minRegWallPushComponents.y = 0;
	minRegWallPushComponents.z = 0;
	maxWallOutwardPushDist = -1;
	biggestWallPushDirection.x = 0;
	biggestWallPushDirection.y = 0;
	biggestWallPushDirection.z = 0;
	maxEdgeWallPushComponents.x = 0;
	maxEdgeWallPushComponents.y = 0;
	maxEdgeWallPushComponents.z = 0;
	maxWallPushyComponents.x = 0;
	maxWallPushyComponents.y = 0;
	maxWallPushyComponents.z = 0;
	minWallPushyComponents.x = 0;
	minWallPushyComponents.y = 0;
	minWallPushyComponents.z = 0;
	if (out_param_11 != NULL) {
		maybe_distanceToTriangle_LargestVertical.x = 0;
		maybe_distanceToTriangle_LargestVertical.y = radius << 1;
		maybe_distanceToTriangle_LargestVertical.z = 0;
	}
	touchedSurfaceCount = 0;
	playerPosition.x = position->x;
	playerPosition.y = position->y;
	playerPosition.z = position->z;
	if (prevPosition != NULL) {
		_previousPosition.x = prevPosition->x;
		_previousPosition.y = prevPosition->y;
		_previousPosition.z = prevPosition->z;
	}
	// Check if position is below bounds of course.
	fx32 xFromBoundary = (playerPosition.x - DAT_0217b608) >> 0xc;
	if ((xFromBoundary & DAT_0217b614) != 0) {
		return 0;
	}
	fx32 yFromBoundary = (playerPosition.y - iRam0217b60c) >> 0xc;
	if ((yFromBoundary & iRam0217b618) != 0) {
		return 0;
	}
	fx32 zFromBoundary = (playerPosition.z - iRam0217b610) >> 0xc;
	if ((zFromBoundary & iRam0217b61c) != 0) {
		return 0;
	}
	// Is this some kind of 3D mapping?
	u8 shift = (u8)uRam0217b620;
	uVar4 = *(u32*)(iRam0217b600 + (
		(zFromBoundary >> shift) << (uRam0217b628 & 0xff) |
		(yFromBoundary >> shift) << (uRam0217b624 & 0xff) |
		xFromBoundary >> shift
	) * 4);
	int iVarA = iRam0217b600;
	while (-1 < (int)uVar4) {
		iVarA += uVar4;
		shift -= 1;
		uVar4 = *(u32*)(iVarA + (
			(xFromBoundary >> shift & 1) |
			(yFromBoundary >> shift & 1) << 1 |
			(zFromBoundary >> shift & 1) << 2
		) * 4);
	}
	iVarA += uVar4 & 0x7fffffff;
	for (triangleEnumerator = (u16*)(iVarA + 2); triangleEnumerator != 0; triangleEnumerator++) {
		// Gather surface data
		triangle_collision_data triangle = collisionTriangles[*triangleEnumerator];
		VecFx32 triangleVertex = triangleVertexes[triangle.vertexId];

		// Check each boundary vector to see if we are potentially colliding
		VecFx32 distanceFromVertex;
		VEC_Subtract(&playerPosition, &triangleVertex, &distanceFromVertex);
		VecFx32 inVector = triangleVectors[triangle.inId];
		fx32 inDistance = DotProduct_t(&distanceFromVertex, &inVector);
		s32 overShootInDistance = inDistance - triangle.size;
		if (-radius > inDistance || overShootInDistance > radius)
			continue;
		VecFx32 outVector1 = triangleVectors[triangle.outId1];
		fx32 outDistance1 = DotProduct_t(&distanceFromVertex, &outVector1);
		if (outDistance1 > radius)
			continue;
		VecFx32 outVector2 = triangleVectors[triangle.outId2];
		fx32 outDistance2 = DotProduct_t(&distanceFromVertex, &outVector2);
		if (outDistance2 > radius)
			continue;
		VecFx32 surfaceNormalVector = triangleVectors[triangle.surfaceNormalId];
		fx32 upDistance = DotProduct_t(&distanceFromVertex, &surfaceNormalVector);
		fx32 distanceIntoSurfaceFace = radius - upDistance;
		if (0 > distanceIntoSurfaceFace || distanceIntoSurfaceFace >= 0x1e001) // walls have thickness of 30 units? ignore radius for that thickness
			continue;
		
		// At this point, we are inside all the bounding areas. (Not necessarily touching as a spherical hitbox, though)
		const u32 default_cant_touch = 0x2010;
		if ((triangle.surfaceProperties & default_cant_touch == 0 ||
				((!collider.isItem || !triangle.unknownBit2000) &&
					(!collider.isRacer || !triangle.unknownBit10))) &&
				(!collider.isLakitu || triangle.surfaceProperties & 0xa000 == 0)) {
			VEC_Subtract(&_previousPosition, &triangleVertex, &previousDistanceFromVertex);
			kindoffar = false;

			VecFx32 distanceToTriangle;
			MultiplyVector_T(&surfaceNormalVector, -upDistance, &distanceToTriangle);
			u64 radiusSq = (s64)radius * radius;

			fx32 largestOutDistance = outDistance1;
			VecFx32 largestVector = outVector1;
			VecFx32 middleVector;
			fx32 middleDistance;
			if (outDistance2 > largestOutDistance) {
				largestOutDistance = outDistance2;
				largestVector = outVector2;
			}
			if (overShootInDistance > largestOutDistance) {
				largestOutDistance = overShootInDistance;
				largestVector = inVector;
			}
			if (outDistance1 > outDistance2) {
				middleDistance = outDistance1;
				middleVector = outVector1;
			} else {
				middleDistance = outDistance2;
				middleVector = outVector2;
			}
			if (largestOutDistance <= 0) {
				// Center of our hitbox is within the three triangle edges
				if (upDistance < 0 && prevPosition != NULL) {
					if (DotProduct_t(&previousDistanceFromVertex, &surfaceNormalVector) < 0 &&
							(collider.bit20 ||
								0 < DotProduct_t(&previousDistanceFromVertex, &outVector1) ||
								0 < DotProduct_t(&previousDistanceFromVertex, &outVector2) ||
							triangle.size < DotProduct_t(&previousDistanceFromVertex, &inVector))) continue;
				}
			} else {
				// Center of out hitbox is outside at least one of the three triangle edges
				if (upDistance < 0) {
					if (prevPosition == NULL)
						continue;
					if (-1 >= DotProduct_t(&previousDistanceFromVertex, &surfaceNormalVector) ||
					    DotProduct_t(&previousDistanceFromVertex, &largestVector) > (&largestVector == &inVector ? triangle.size : 1))
						continue;
				}

				s32 iVar_a = DotProduct_t(&largestVector, &middleVector);
				if (fxMulT(iVar_a, largestOutDistance) <= middleDistance) {
					// The order of these parameters is odd. It DOES matter, though.
					// out1, in, out2
					if (middleDistance == outDistance1 && largestOutDistance == overShootInDistance ||
						middleDistance == outDistance2 && largestOutDistance == outDistance1 ||
						middleDistance == overShootInDistance && largestOutDistance == outDistance2) {
							FUN_01fff434(iVar_a, middleDistance, largestOutDistance, &totalFloorPush, &largestVector, &middleVector, &outDistance_2D_sq);
					} else {
						FUN_01fff434(iVar_a, largestOutDistance, middleDistance, &totalFloorPush, &middleVector, &largestVector, &outDistance_2D_sq);
					}
					if (outDistance_2D_sq > 0 && out_param_11 != NULL) {
						VEC_Subtract(&distanceToTriangle, &totalFloorPush, &distanceToTriangle);
					}

					outDistance_2D_sq = radiusSq - outDistance_2D_sq;
				} else {
					s64 lgOutDistSq = (s64)largestOutDistance * largestOutDistance;
					outDistance_2D_sq = radiusSq - lgOutDistSq;
					if (0x424 >= largestVector.y && largestVector.y >= -0x424) {
						kindoffar = true;
					}
					if (out_param_11 != NULL) {
						VEC_MultAdd(-largestOutDistance, &largestVector, &distanceToTriangle, &distanceToTriangle);
					}
				}
			}
			nUpDistance = sqrt64(outDistance_2D_sq) - upDistance;
			if (nUpDistance < 0) continue;

			/*  outDistance3 = 1 << ((int)(uVar6 & 0x1f00) >> 8) |
                1 << ((int)(uVar6 & 0xc) >> 2) + 0x1a;
                outDistance1 = outDistance3 | (uVar6 & 2) << 0x17;*/
			u32 someProperties = (1 << triangle.type) | (1 << (triangle.light + 0x1a));
			u32 modifiedProperties = someProperties | triangle.shadow << 0x18; // 24 (shadow)
			const u32 m_under_bridge = 0x0000CCCC; // ????
			const u32 fake_boost = 0x07000000;

			// Multiply two f20.12 into a f10.22?
			Vec3_fx10_22 outwardPush;
			outwardPush.x = FX_MUL_10(nUpDistance, surfaceNormalVector.x);
			outwardPush.y = FX_MUL_10(nUpDistance, surfaceNormalVector.y);
			outwardPush.z = FX_MUL_10(nUpDistance, surfaceNormalVector.z);

			if (prevPosition != NULL) {
				VecFx32 positionDelta;
				VEC_Subtract(&playerPosition, &_previousPosition, &positionDelta);

				s32 previousUpDistance = DotProduct_t(&previousDistanceFromVertex, &surfaceNormalVector);
				if (previousUpDistance < 0) {
					if (kindoffar && (someProperties & COL_FLAGS_TYPE_WALL_MASK) != 0 && direction != NULL) {
						maxWallPushyComponents.x = max(maxWallPushyComponents.x, outwardPush.x);
						minWallPushyComponents.x = min(minWallPushyComponents.x, outwardPush.x);
						maxWallPushyComponents.z = max(maxWallPushyComponents.z, outwardPush.z);
						minWallPushyComponents.z = min(minWallPushyComponents.z, outwardPush.z);
					}
					if ((collider.flags & 0x3b) != 0 || previousUpDistance < -0xa000)
						continue; // This is what prevents touching a wall while inside of it.
				}
				if (819 < DotProduct_t(&positionDelta, &surfaceNormalVector)) { // B Delta, upper limit for touching floors?
					// This would be NOT touching the surface
					if (kindoffar && (someProperties & COL_FLAGS_TYPE_WALL_MASK) != 0 && direction != NULL) {
						maxWallPushyComponents.x = max(maxWallPushyComponents.x, outwardPush.x);
						minWallPushyComponents.x = min(minWallPushyComponents.x, outwardPush.x);
						maxWallPushyComponents.z = max(maxWallPushyComponents.z, outwardPush.z);
						minWallPushyComponents.z = min(minWallPushyComponents.z, outwardPush.z);
					}
					continue;
				}
			}
			// for the main racerData call, flags == 1
			if (!collider.isCamera || (someProperties & (1 << COL_TYPE_INVISIBLE_WALL)) == 0) {
				if (out_param_11 != NULL && distanceToTriangle.y < maybe_distanceToTriangle_LargestVertical.y) {
					maybe_distanceToTriangle_LargestVertical.x = distanceToTriangle.x;
					maybe_distanceToTriangle_LargestVertical.y = distanceToTriangle.y;
					maybe_distanceToTriangle_LargestVertical.z = distanceToTriangle.z;
				}
				if (hasOut8Plus) {
					if ((someProperties & COL_FLAGS_TYPE_FLOOR_MASK) == 0) {
						if ((someProperties & COL_FLAGS_TYPE_WALL_MASK) != 0) {
							if ((someProperties & (1 << COL_TYPE_EDGE_WALL)) == 0) {
								maxRegWallPushComponents.y = max(maxRegWallPushComponents.y, outwardPush.y);
								minRegWallPushComponents.y = min(minRegWallPushComponents.y, outwardPush.y);
							}
							else {
								modifiedProperties |= 0x2000000; // why is there a second flag for edge wall?

								maxEdgeWallPushComponents.y = max(maxEdgeWallPushComponents.y, outwardPush.y);
							}
							// because objects stuff uses regwall, maybe I should keep edge stuff seperate...
							maxRegWallPushComponents.x = max(maxRegWallPushComponents.x, outwardPush.x);
							minRegWallPushComponents.x = min(minRegWallPushComponents.x, outwardPush.x);
							maxRegWallPushComponents.z = max(maxRegWallPushComponents.z, outwardPush.z);
							minRegWallPushComponents.z = min(minRegWallPushComponents.z, outwardPush.z);

							if (out_wallNormal != NULL && maxWallOutwardPushDist <= nUpDistance) {
								biggestWallPushDirection.x = surfaceNormalVector.x;
								biggestWallPushDirection.y = surfaceNormalVector.y;
								biggestWallPushDirection.z = surfaceNormalVector.z;
								maxWallOutwardPushDist = nUpDistance;
							}
							if (direction != NULL) {
								maxWallPushyComponents.x = max(maxWallPushyComponents.x, outwardPush.x);
								minWallPushyComponents.x = min(minWallPushyComponents.x, outwardPush.x);
								maxWallPushyComponents.z = max(maxWallPushyComponents.z, outwardPush.z);
								minWallPushyComponents.z = min(minWallPushyComponents.z, outwardPush.z);
							}
						}
					}
					else {
						maxLandablePushComponents.x = max(maxLandablePushComponents.x, outwardPush.x);
						minLandablePushComponents.x = min(minLandablePushComponents.x, outwardPush.x);
						maxLandablePushComponents.y = max(maxLandablePushComponents.y, outwardPush.y);
						minLandablePushComponents.y = min(minLandablePushComponents.y, outwardPush.y);
						maxLandablePushComponents.z = max(maxLandablePushComponents.z, outwardPush.z);
						minLandablePushComponents.z = min(minLandablePushComponents.z, outwardPush.z);

						if (out_floorNormal != NULL && maxLandableOutwardPushDist <= nUpDistance) {
							maxLandablePushNormal.x = surfaceNormalVector.x;
							maxLandablePushNormal.y = surfaceNormalVector.y;
							maxLandablePushNormal.z = surfaceNormalVector.z;
							maxLandableOutwardPushDist = nUpDistance;
						}
					}
				}
				accumulatedSurfaceProps = accumulatedSurfaceProps | 0x80000000 | modifiedProperties;
				if (touchedSurfaceCount == 0x10) {
					touchedSurfaceCount = 0xf;
				}
				touchedSomething = true;
				// Shared global state.
				touchedSurfacePropFlags[touchedSurfaceCount] = modifiedProperties;
				touchedSurfaceProperties[touchedSurfaceCount] = triangle.surfaceProperties;
				touchedSurfaceCount += 1;
			}
		}
	} // surfaces loop

	u32 uVar_b;
	if (out_param_12 != NULL) {
		out_param_12->x = 0;
		out_param_12->y = 0;
		out_param_12->z = 0;
	}
	if (out_objTurnRacer != NULL) {
		*out_objTurnRacer = 0;
	}
	if (dcolResults != NULL) {
		*dcolResults = (int)&touchedDynamicObjects;
		uRam0217b5b4 = 0;
	}
	int iVar15;
	if ((DAT_0217b5f0 != 0) && (iVar9 = (int)colEntryId, iVar9 != -2)) {
		iVar15 = 0;
		if (iVar9 == -1) {
			FUN_020d5180(position, radius, 0x1000);
		}
		else {
			UpdateObjectsInZ(iVar9, 0x1000);
		}
		iVar9 = 0;
		if (0 < countObjectsInZ) {
			do {
				u32 theObject = *(u32*)(ptrsToObjectsInZ + iVar9 * 4);
				s32 iVar10 = iVar15;
				VecFx32* pVVar13 = NULL;
				u16 outShort; // This is not initialized. However it is definitely set by checkRacerObjectCollision if it returns non-zero...? (That is, assuming the functions that it calls and gives out_param_8 set it.)
				if (out_param_11 != NULL)
					pVVar13 = &maybe_distanceToTriangle_LargestVertical;
				if (checkRacerObjectCollision(theObject, position, radius, collider,
				  &maxLandablePushComponents, &maxRegWallPushComponents,
				  pVVar13, &outShort, out_param_12, out_objTurnRacer) != 0) {
					accumulatedSurfaceProps = accumulatedSurfaceProps | 0x40000000 | 1 << (outShort & 0xff);
					if (touchedSurfaceCount == 0x10) {
						touchedSurfaceCount = 0xf;
					}
					uVar_b = 1 << (outShort & 0xff);
					touchedSurfacePropFlags[touchedSurfaceCount] = uVar_b;
					touchedSurfaceProperties[touchedSurfaceCount] = outShort << 8;
					touchedSurfaceCount += 1;
					if (dcolResults != NULL) {
						if ((uVar_b & LANDABLE) == 0) {
							(&DAT_0217b5b0)[iVar15] = theObject;
						}
						else {
							(&DAT_0217b5b0)[iVar15] = DAT_0217b5b0;
							DAT_0217b5b0 = theObject;
						}
						iVar10 = iVar15 + 1;
						(&DAT_0217b5b0)[iVar10] = 0;
						if (0xe < iVar10) {
							iVar10 = iVar15;
						}
					}
					touchedSomething = true;
				}
				iVar9 += 1;
				iVar15 = iVar10;
			} while (iVar9 < countObjectsInZ);
		}
	}
	if (!touchedSomething) {
		return false;
	}
	if (out_floorNormal != NULL) {
		out_floorNormal->x = maxLandablePushNormal.x;
		out_floorNormal->y = maxLandablePushNormal.y;
		out_floorNormal->z = maxLandablePushNormal.z;
	}
	if (out_wallNormal != NULL) {
		out_wallNormal->x = biggestWallPushDirection.x;
		out_wallNormal->y = biggestWallPushDirection.y;
		out_wallNormal->z = biggestWallPushDirection.z;
	}
	if (out_param_11 != NULL) {
		out_param_11->x = maybe_distanceToTriangle_LargestVertical.x;
		out_param_11->y = maybe_distanceToTriangle_LargestVertical.y;
		out_param_11->z = maybe_distanceToTriangle_LargestVertical.z;
	}

	if (out_pushback != NULL) {
		Vec3_fx10_22 somePositionChange;
		somePositionChange.y = max(maxLandablePushComponents.y, maxRegWallPushComponents.y);
		somePositionChange.y += min(minLandablePushComponents.y, minRegWallPushComponents.y);
		if ((accumulatedSurfaceProps & (1 << EdgeWall)) != 0) {
			if (out_param_11 == NULL || 
				  (accumulatedSurfaceProps & LANDABLE) == 0 ||
			      (accumulatedSurfaceProps & (1 << OobFloor)) != 0 ||
			      (accumulatedSurfaceProps & (1 << FallBoundary)) != 0
				) {
				accumulatedSurfaceProps &= ~(1 << EdgeWall);
			}
			else {
				maybe_distanceToTriangle_LargestVertical.z = (int)maybe_distanceToTriangle_LargestVertical.z >> 4;
				maybe_distanceToTriangle_LargestVertical.x = (int)maybe_distanceToTriangle_LargestVertical.x >> 4;
				maybe_distanceToTriangle_LargestVertical.y = (int)maybe_distanceToTriangle_LargestVertical.y >> 4;
				if ((int)(maybe_distanceToTriangle_LargestVertical.x * maybe_distanceToTriangle_LargestVertical.x + maybe_distanceToTriangle_LargestVertical.z * maybe_distanceToTriangle_LargestVertical.z) <= (int)(maybe_distanceToTriangle_LargestVertical.y * maybe_distanceToTriangle_LargestVertical.y)) {
					accumulatedSurfaceProps &= ~(1 << EdgeWall);
					goto LAB_01fff184;
				}
				somePositionChange.y = 0;
				accumulatedSurfaceProps &= ~LANDABLE;
				if (out_floorNormal != NULL) {
					out_floorNormal->x = 0;
					out_floorNormal->y = 0;
					out_floorNormal->z = 0;
				}
			}
			accumulatedSurfaceProps |= (1 << Wall);
			if (somePositionChange.y < maxEdgeWallPushComponents.y) {
				somePositionChange.y = maxEdgeWallPushComponents.y;
			}
		}
	LAB_01fff184:
		somePositionChange.x = max(maxLandablePushComponents.x, maxRegWallPushComponents.x) +
		                       min(minLandablePushComponents.x, minRegWallPushComponents.x);
		somePositionChange.z = max(maxLandablePushComponents.z, maxRegWallPushComponents.z) +
		                       min(minLandablePushComponents.z, minRegWallPushComponents.z);
		out_pushback->x = somePositionChange.x >> 10;
		out_pushback->y = somePositionChange.y >> 10;
		out_pushback->z = somePositionChange.z >> 10;

		if (accumulatedSurfaceProps & ALL_WALLS != 0 && direction != NULL &&
			out_param_12 != NULL && out_wallNormal != NULL) {
			VecFx32 totalPushy;
			totalPushy.x = maxWallPushyComponents.x + minWallPushyComponents.x;
			totalPushy.z = maxWallPushyComponents.z + minWallPushyComponents.z;
			s64 xInZ = (s64)totalPushy.x * (s64)out_wallNormal->z;
			s64 zInX = (s64)totalPushy.z * (s64)out_wallNormal->x;
			s64 difference = xInZ - zInX;
			// SBORROW4 checkings, this condition may be wrong
			if (difference > 0x10 || difference < -0x10) {
				VecFx32 someVector;
				someVector.x = FX_MUL(direction->x, totalPushy.x);
				someVector.z = FX_MUL(direction->z, totalPushy.z);

				totalPushy.x -= FX_MUL(someVector.x, direction->x);
				totalPushy.z -= FX_MUL(someVector.z, direction->z);
				// totalPushy is now: p(1 - m^2)

				AddVector(&maxLandablePushComponents, &minLandablePushComponents, &totalFloorPush);
				iVar9 = DotProduct_t(&totalPushy, &totalFloorPush);
				if (-1 < iVar9) { // Does this mean that target movement vector is away from wall? (experimentally: no)
					lVar1 = (s64)totalPushy.z * totalPushy.z +
							(s64)totalPushy.x * totalPushy.x +
							(s64)totalPushy.y * totalPushy.y;
					if (lVar1 >= 0x10) { // ??? I'm not sure of this condition. 0x1fff384
						NormalizeVector(&totalPushy, &totalPushy);
					}
					lVar1 = (s64)totalPushy.x * (s64)(radius >> 5);
					lVar2 = (s64)totalPushy.z * (s64)(radius >> 5);
					out_param_12->x = (u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) << 0x14;
					out_param_12->z = (u32)lVar2 >> 0xc | (int)((u64)lVar2 >> 0x20) << 0x14;
				}
			}
		}
	}
	if (out_ColFlags != NULL) {
		*out_ColFlags = accumulatedSurfaceProps;
	}
	return touchedSomething;
}

#endif
