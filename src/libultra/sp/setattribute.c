#include <PR/sp.h>

/*
 * spSetAttribute()
 *
 *   Sets attributes about a sprite.
 */
void spSetAttribute ( Sprite *sp, s32 attr)
{
	sp->attr |= attr;
}

