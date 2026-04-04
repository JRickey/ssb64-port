#include <PR/sp.h>

/*
 * spShow()
 *
 *   Shows a sprite so that it will be drawn at the next spUpdate().
 */
void spShow (Sprite *sp)
{
	spClearAttribute (sp, SP_HIDDEN);
}
