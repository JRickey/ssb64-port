#ifndef _MAP_H_
#define _MAP_H_

#include "mptypes.h"
#include "mpfunctions.h"

extern f32 dMPCollisionMaterialFrictions[/* */];

extern MPGroundData *gMPCollisionGroundData;
extern MPAllBounds gMPCollisionBounds;
extern MPYakumonoDObj *gMPCollisionYakumonoDObjs;
extern MPLineGroup gMPCollisionLineGroups[/* */];
extern MPGeometryData *gMPCollisionGeometry;
extern MPVertexInfoContainer *gMPCollisionVertexInfo;
extern MPVertexArray *gMPCollisionVertexIDs;
extern MPVertexLinks *gMPCollisionVertexLinks;   //
extern MPVertexPosContainer *gMPCollisionVertexData; // Vertex positions
extern Vec3f *gMPCollisionSpeeds;
extern MPMapObjContainer *gMPCollisionMapObjs;
extern s32 gMPCollisionLinesNum;
extern SYColorRGBA gMPCollisionLightColor;
extern s32 gMPCollisionYakumonosNum;
extern f32 gMPCollisionLightAngleX;
extern f32 gMPCollisionLightAngleY;
extern u16 gMPCollisionUpdateTic;
extern u32 gMPCollisionBGMCurrent;
extern u32 gMPCollisionBGMDefault;

#endif
