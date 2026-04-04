#ifndef _OBJ_H_
#define _OBJ_H_

#include <sys/objtypes.h>

/* 
 * I have been indecisive as to whether I should have followed HAL Labs' naming convention of combining
 * two keywords to create two-letter prefixes (e.g. object + manager = om) and prefixing functions as such, 
 * so the result now is that all Obj related functions in the sys category are prefixed with "gc"; this is because:
 * 
 * 1. 
 *      a lot of these functions sometimes seem to belong in the wrong module (e.g. gcSetupCommonDObjs in objanim.c);
 * 2. 
 *      telling apart "om" function prefixes from a hypthetical "oa" prefix for Object Animation would
 *      be more of a nuisance than anything;
 * 3. 
 *      even if a common two-letter prefix for objects (say, "ob" or "oj") was found, adding more words to specify
 *      what the module specializes in (obAnim) would mean more excess words;
 * 4.
 *      making excess two-letter prefixes for small modules means running out of them sooner and ending up with
 *      too many to keep track of.
 * 
 * The final decision is that the obj files in sys will keep their proper names, but all functions
 * are now prefixed with "gc." Other files not related to objs may be decided to either break the two-letter
 * prefix format and start with "sys" or go with the "su" prefix which would stand for SysUltra, the
 * hypothetical predecessor to HAL's new engine that followed on the GameCube, titled SysDolphin.
 * 
 *  One last thing: "gc" functions generally assume the main argument type is GObj*, so something like
 * "gcAddAnimJointAll" would mean that the function is adding AnimJoints to each DObj on a GObj, while
 * "gcAddDObjAnimJoint" means that it's adding a single AnimJoint to a single DObj.
 */

// Function headers
#include <sys/objfunctions.h>

#define AObjAnimAdvance(script) \
((script)++)

#define DObjGetStruct(gobj) \
((DObj*)(gobj)->obj)

#define DObjGetChild(dobj) \
((DObj*)(dobj)->child)

#define SObjGetStruct(gobj) \
((SObj*)(gobj)->obj)

#define CObjGetStruct(gobj) \
((CObj*)(gobj)->obj)

#define SObjGetSprite(sobj) \
((Sprite*)&(sobj)->sprite)

#define SObjGetPrev(sobj) \
((SObj*)(sobj)->prev)

#define SObjGetNext(sobj) \
((SObj*)(sobj)->next)

#endif
