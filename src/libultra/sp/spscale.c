#include <PR/sp.h>

/*
 * spScale()
 *
 *   Scales the sprite by a vertical and horizontal factor.
 */
void spScale (Sprite *sp, float sx, float sy)
{
	sp->scalex = sx;
	sp->scaley = sy;

	if( (sx == 1.0F) && (sy == 1.0F) )
	spClearAttribute( sp, SP_SCALE );
	else
	spSetAttribute( sp, SP_SCALE );
}
