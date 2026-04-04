#include <PR/sp.h>

/*
 * spHide()
 *
 *   Hides a sprite so that it will not be drawn at the next spUpdate().
 */
void spHide (Sprite *sp)
{
	spSetAttribute (sp, SP_HIDDEN);
}
