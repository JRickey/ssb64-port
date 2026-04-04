#include <it/item.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801713B0
void itVisualsUpdateColAnim(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ftMainUpdateColAnim(&ip->colanim, item_gobj, FALSE, FALSE) != FALSE)
    {
        itMainClearColAnim(item_gobj);
    }
}

// 0x801713F4
void itVisualsUpdateSpin(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->rotate.vec.f.z += ip->spin_step;
}
