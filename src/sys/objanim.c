#include <string.h>
#include <sys/obj.h>
#ifdef PORT
#include <port_log.h>
extern void portFixupMObjSub(void *mobjsub);
#endif

extern void syInterpCubic(Vec3f*, void*, f32);

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

DObj* gcGetTreeDObjNext(DObj *dobj)
{
    if (dobj->child != NULL)
    {
        dobj = dobj->child;
    }
    else if (dobj->sib_next != NULL)
    {
        dobj = dobj->sib_next;
    }
    else while (TRUE)
    {
        if (dobj->parent == DOBJ_PARENT_NULL)
        {
            dobj = NULL;

            break;
        }
        else if (dobj->parent->sib_next != NULL)
        {
            dobj = dobj->parent->sib_next;

            break;
        }
        else dobj = dobj->parent;
    }
    return dobj;
}

void gcSetAnimSpeed(GObj *gobj, f32 anim_speed)
{
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        dobj->anim_speed = anim_speed;
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcSetAllAnimSpeed(GObj *gobj, f32 anim_speed)
{
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        MObj *mobj = dobj->mobj;

        dobj->anim_speed = anim_speed;

        while (mobj != NULL)
        {
            mobj->anim_speed = anim_speed;
            mobj = mobj->next;
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcSetMatAnimJointSpeed(GObj *gobj, f32 anim_speed)
{
    MObj *mobj;
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        mobj = dobj->mobj;

        while (mobj != NULL)
        {
            mobj->anim_speed = anim_speed;
            mobj = mobj->next;
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcRemoveAnimJointAll(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL) 
    {
        gcRemoveAObjFromDObj(dobj);
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcRemoveAnimAll(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        MObj *mobj;

        gcRemoveAObjFromDObj(dobj);

        mobj = dobj->mobj;

        while (mobj != NULL) 
        {
            gcRemoveAObjFromMObj(mobj);
            mobj = mobj->next;
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcRemoveMatAnimJointAll(GObj *gobj) 
{
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        MObj *mobj = dobj->mobj;

        while (mobj != NULL) 
        {
            gcRemoveAObjFromMObj(mobj);
            mobj = mobj->next;
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcAddDObjAnimJoint(DObj *dobj, AObjEvent32 *anim_joint, f32 anim_frame) 
{
    AObj *aobj = dobj->aobj;

    while (aobj != NULL) 
    {
        aobj->kind = nGCAnimKindNone;
        aobj = aobj->next;
    }
    dobj->anim_joint.event32 = anim_joint;
    dobj->anim_wait = AOBJ_ANIM_CHANGED;
    dobj->anim_frame = anim_frame;
}

void gcAddMObjMatAnimJoint(MObj *mobj, AObjEvent32 *matanim_joint, f32 anim_frame) 
{
    AObj *aobj = mobj->aobj;

    while (aobj != NULL) 
    {
        aobj->kind = nGCAnimKindNone;
        aobj = aobj->next;
    }
    mobj->matanim_joint.event32 = matanim_joint;
    mobj->anim_wait = AOBJ_ANIM_CHANGED;
    mobj->anim_frame = anim_frame;
}

void gcAddAnimJointAll(GObj *gobj, AObjEvent32 **anim_joints, f32 anim_frame) 
{
    DObj *dobj = DObjGetStruct(gobj);
    ub8 is_anim_root = TRUE;
    
    gobj->anim_frame = anim_frame;

    while (dobj != NULL) 
    {
        if (anim_joints != NULL)
        {
 #ifdef PORT
            u32 anim_joint_token = *(u32*)anim_joints;

            if (anim_joint_token != 0)
            {
                AObjEvent32 *anim_joint = (AObjEvent32*)PORT_RESOLVE(anim_joint_token);

                if (anim_joint != NULL)
                {
                    gcAddDObjAnimJoint(dobj, anim_joint, anim_frame);
                    dobj->is_anim_root = is_anim_root;
                    is_anim_root = FALSE;
                }
                else
                {
                    dobj->anim_wait = AOBJ_ANIM_NULL;
                    dobj->is_anim_root = FALSE;
                }
            }
            else
            {
                dobj->anim_wait = AOBJ_ANIM_NULL;
                dobj->is_anim_root = FALSE;
            }
            anim_joints = (AObjEvent32**)(void*)(((u32*)anim_joints) + 1);
 #else
            if (*anim_joints != NULL)
            {
                gcAddDObjAnimJoint(dobj, *anim_joints, anim_frame);
                dobj->is_anim_root = is_anim_root;
                is_anim_root = FALSE;
            } 
            else 
            {
                dobj->anim_wait = AOBJ_ANIM_NULL;
                dobj->is_anim_root = FALSE;
            }
            anim_joints++;
 #endif
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcAddMatAnimJointAll(GObj *gobj, AObjEvent32 ***p_matanim_joints, f32 anim_frame)
{
    DObj *dobj = DObjGetStruct(gobj);
    
    gobj->anim_frame = anim_frame;

    while (dobj != NULL)
    {
        if (p_matanim_joints != NULL)
        {
#ifdef PORT
            u32 matanim_joints_token = *(u32*)p_matanim_joints;

            if (matanim_joints_token != 0)
            {
                u32 *matanim_joints = (u32*)PORT_RESOLVE(matanim_joints_token);
                MObj *mobj = dobj->mobj;

                if (matanim_joints != NULL)
                {
                    while (mobj != NULL)
                    {
                        u32 matanim_joint_token = *matanim_joints;

                        if (matanim_joint_token != 0)
                        {
                            AObjEvent32 *matanim_joint = (AObjEvent32*)PORT_RESOLVE(matanim_joint_token);

                            if (matanim_joint != NULL)
                            {
                                gcAddMObjMatAnimJoint(mobj, matanim_joint, anim_frame);
                            }
                        }
                        matanim_joints++;
                        mobj = mobj->next;
                    }
                }
            }
#else
            if (*p_matanim_joints != NULL)
            {
                AObjEvent32 **matanim_joints = *p_matanim_joints;
                MObj *mobj = dobj->mobj;

                while (mobj != NULL)
                {
                    if (*matanim_joints != NULL)
                    { 
                        gcAddMObjMatAnimJoint(mobj, *matanim_joints, anim_frame); 
                    }
                    matanim_joints++;
                    mobj = mobj->next;
                }
            }
            p_matanim_joints++;
#endif
#ifdef PORT
            p_matanim_joints = (AObjEvent32***)(void*)(((u32*)p_matanim_joints) + 1);
#endif
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcAddAnimAll(GObj *gobj, AObjEvent32 **anim_joints, AObjEvent32 ***p_matanim_joints, f32 anim_frame)
{
    DObj *dobj = DObjGetStruct(gobj);
    ub8 is_anim_root = TRUE;

    gobj->anim_frame = anim_frame;

    while (dobj != NULL) 
    {
        if (anim_joints != NULL)
        {
#ifdef PORT
            u32 anim_joint_token = *(u32*)anim_joints;

            if (anim_joint_token != 0)
            {
                AObjEvent32 *anim_joint = (AObjEvent32*)PORT_RESOLVE(anim_joint_token);

                if (anim_joint != NULL)
                {
                    gcAddDObjAnimJoint(dobj, anim_joint, anim_frame);
                    dobj->is_anim_root = is_anim_root;
                    is_anim_root = FALSE;
                }
                else
                {
                    dobj->anim_wait = AOBJ_ANIM_NULL;
                    dobj->is_anim_root = FALSE;
                }
            } 
            else 
            {
                dobj->anim_wait = AOBJ_ANIM_NULL;
                dobj->is_anim_root = FALSE;
            }
            anim_joints = (AObjEvent32**)(void*)(((u32*)anim_joints) + 1);
#else
            if (*anim_joints != NULL)
            {
                gcAddDObjAnimJoint(dobj, *anim_joints, anim_frame);
                dobj->is_anim_root = is_anim_root;
                is_anim_root = FALSE;
            } 
            else 
            {
                dobj->anim_wait = AOBJ_ANIM_NULL;
                dobj->is_anim_root = FALSE;
            }
            anim_joints++;
#endif
        }
        if (p_matanim_joints != NULL) 
        {
#ifdef PORT
            u32 matanim_joints_token = *(u32*)p_matanim_joints;

            if (matanim_joints_token != 0)
            {
                u32 *matanim_joints = (u32*)PORT_RESOLVE(matanim_joints_token);
                MObj *mobj = dobj->mobj;

                if (matanim_joints != NULL)
                {
                    while (mobj != NULL)
                    {
                        u32 matanim_joint_token = *matanim_joints;

                        if (matanim_joint_token != 0)
                        {
                            AObjEvent32 *matanim_joint = (AObjEvent32*)PORT_RESOLVE(matanim_joint_token);

                            if (matanim_joint != NULL)
                            {
                                gcAddMObjMatAnimJoint(mobj, matanim_joint, anim_frame);
                            }
                        }
                        matanim_joints++;
                        mobj = mobj->next;
                    }
                }
            }
#else
            if (*p_matanim_joints != NULL) 
            {
                AObjEvent32 **matanim_joints = *p_matanim_joints;
                MObj *mobj = dobj->mobj;

                while (mobj != NULL) 
                {
                    if (*matanim_joints != NULL) 
                    { 
                        gcAddMObjMatAnimJoint(mobj, *matanim_joints, anim_frame); 
                    }
                    matanim_joints++;
                    mobj = mobj->next;
                }
            }
            p_matanim_joints++;
#endif
#ifdef PORT
            p_matanim_joints = (AObjEvent32***)(void*)(((u32*)p_matanim_joints) + 1);
#endif
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcParseDObjAnimJoint(DObj *dobj)
{
    AObj *track_aobjs[nGCAnimTrackJointEnd - nGCAnimTrackJointStart + 1];
    AObj *aobj;
    s32 i;
    u32 command_kind;
    u32 flags;
    f32 payload;

#ifdef PORT
    /* Un-halfswap this EVENT32 stream on first access. See
     * port/port_aobj_fixup.{h,cpp} — file-loaded AObjEvent32 data was
     * corrupted by portRelocFixupFighterFigatree's u16-halfswap, and the
     * fix is lazy per-stream at the first EVENT32 reader touch.
     * Idempotent; subsequent calls on the same head are no-ops. */
    extern void port_aobj_event32_unhalfswap_stream(void *head);
    if (dobj->anim_joint.event32 != NULL) {
        port_aobj_event32_unhalfswap_stream(dobj->anim_joint.event32);
    }
#endif

    if (dobj->anim_wait != AOBJ_ANIM_NULL)
    {
        if (dobj->anim_wait == AOBJ_ANIM_CHANGED)
        {
            dobj->anim_wait = -dobj->anim_frame;
        }
        else
        {
            dobj->anim_wait -= dobj->anim_speed;
            dobj->anim_frame += dobj->anim_speed;
            dobj->parent_gobj->anim_frame = dobj->anim_frame;

            if (dobj->anim_wait > 0.0F)
            {
                return;
            }
        }
        for (i = 0; i < ARRAY_COUNT(track_aobjs); i++)
        {
            track_aobjs[i] = NULL;
        }
        aobj = dobj->aobj;

        while (aobj != NULL)
        {
            if ((aobj->track >= nGCAnimTrackJointStart) && (aobj->track <= nGCAnimTrackJointEnd))
            {
                track_aobjs[aobj->track - nGCAnimTrackJointStart] = aobj;
            }
            aobj = aobj->next;
        }
        do
        {
            if (dobj->anim_joint.event32 == NULL)
            {
                aobj = dobj->aobj;

                while (aobj != NULL)
                {
                    if (aobj->kind)
                    {
                        aobj->length += dobj->anim_speed + dobj->anim_wait;
                    }
                    aobj = aobj->next;
                }
                dobj->anim_frame = dobj->anim_wait;
                dobj->parent_gobj->anim_frame = dobj->anim_wait;
                dobj->anim_wait = AOBJ_ANIM_END;

                return;
            }
            command_kind = dobj->anim_joint.event32->command.opcode;

            switch (command_kind)
            {
            case nGCAnimEvent32SetVal0RateBlock:
            case nGCAnimEvent32SetVal0Rate:
                payload = dobj->anim_joint.event32->command.payload;
                flags = AObjAnimAdvance(dobj->anim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForDObj(dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = dobj->anim_joint.event32->f;

                        AObjAnimAdvance(dobj->anim_joint.event32);

                        track_aobjs[i]->rate_base = track_aobjs[i]->rate_target;
                        track_aobjs[i]->rate_target = 0.0F;
                        track_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        track_aobjs[i]->length = -dobj->anim_wait - dobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetVal0RateBlock)
                {
                    dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetValBlock:
            case nGCAnimEvent32SetVal:
                payload = dobj->anim_joint.event32->command.payload;
                flags = AObjAnimAdvance(dobj->anim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForDObj(dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = dobj->anim_joint.event32->f;

                        AObjAnimAdvance(dobj->anim_joint.event32);

                        track_aobjs[i]->kind = nGCAnimKindLinear;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->rate_base = (track_aobjs[i]->value_target - track_aobjs[i]->value_base) / payload;
                        }
                        track_aobjs[i]->length = -dobj->anim_wait - dobj->anim_speed;
                        track_aobjs[i]->rate_target = 0.0F;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValBlock)
                {
                    dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetValRateBlock:
            case nGCAnimEvent32SetValRate:
                payload = dobj->anim_joint.event32->command.payload;
                flags = AObjAnimAdvance(dobj->anim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForDObj(dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = dobj->anim_joint.event32->f;

                        AObjAnimAdvance(dobj->anim_joint.event32);

                        track_aobjs[i]->rate_base = track_aobjs[i]->rate_target;
                        track_aobjs[i]->rate_target = dobj->anim_joint.event32->f;

                        AObjAnimAdvance(dobj->anim_joint.event32);

                        track_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        track_aobjs[i]->length = -dobj->anim_wait - dobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValRateBlock)
                {
                    dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetTargetRate:
                flags = AObjAnimAdvance(dobj->anim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForDObj(dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->rate_target = dobj->anim_joint.event32->f;

                        AObjAnimAdvance(dobj->anim_joint.event32);
                    }
                }
                break;

            case nGCAnimEvent32Wait:
                dobj->anim_wait += AObjAnimAdvance(dobj->anim_joint.event32)->command.payload;
                break;

            case nGCAnimEvent32SetValAfterBlock:
            case nGCAnimEvent32SetValAfter:
                payload = dobj->anim_joint.event32->command.payload;
                flags = AObjAnimAdvance(dobj->anim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForDObj(dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = dobj->anim_joint.event32->f;

                        AObjAnimAdvance(dobj->anim_joint.event32);

                        track_aobjs[i]->kind = nGCAnimKindStep;
                        track_aobjs[i]->length_invert = payload;
                        track_aobjs[i]->length = -dobj->anim_wait - dobj->anim_speed;
                        track_aobjs[i]->rate_target = 0.0F;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValAfterBlock)
                {
                    dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetAnim:
                AObjAnimAdvance(dobj->anim_joint.event32);
                dobj->anim_joint.event32 = PORT_RESOLVE(dobj->anim_joint.event32->p);
                dobj->anim_frame = -dobj->anim_wait;
                dobj->parent_gobj->anim_frame = -dobj->anim_wait;

                if ((dobj->is_anim_root != FALSE) && (dobj->parent_gobj->func_anim != NULL))
                {
                    dobj->parent_gobj->func_anim(dobj, -2, 0);
                }
                break;

            case nGCAnimEvent32Jump:
                AObjAnimAdvance(dobj->anim_joint.event32);
                dobj->anim_joint.event32 = PORT_RESOLVE(dobj->anim_joint.event32->p);

                if ((dobj->is_anim_root != FALSE) && (dobj->parent_gobj->func_anim != NULL))
                {
                    dobj->parent_gobj->func_anim(dobj, -2, 0);
                }
                break;

            case ANIM_CMD_12:
                payload = dobj->anim_joint.event32->command.payload;
                flags = AObjAnimAdvance(dobj->anim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForDObj(dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->length += payload;
                    }
                }
                break;

            case nGCAnimEvent32SetInterp:
                AObjAnimAdvance(dobj->anim_joint.event32);

                if (track_aobjs[nGCAnimTrackTraI - nGCAnimTrackJointStart] == NULL) 
                { 
                    track_aobjs[nGCAnimTrackTraI - nGCAnimTrackJointStart] = gcAddAObjForDObj(dobj, nGCAnimTrackTraI); 
                }
                track_aobjs[nGCAnimTrackTraI - nGCAnimTrackJointStart]->interpolate = PORT_RESOLVE(dobj->anim_joint.event32->p);

                AObjAnimAdvance(dobj->anim_joint.event32);
                break;

            case nGCAnimEvent32End:
                aobj = dobj->aobj;

                while (aobj != NULL)
                {
                    if (aobj->kind != nGCAnimKindNone)
                    {
                        aobj->length += dobj->anim_speed + dobj->anim_wait;
                    }
                    aobj = aobj->next;
                }
                dobj->anim_frame = dobj->anim_wait;
                dobj->parent_gobj->anim_frame = dobj->anim_wait;
                dobj->anim_wait = AOBJ_ANIM_END;

                if ((dobj->is_anim_root != FALSE) && (dobj->parent_gobj->func_anim != NULL))
                {
                    dobj->parent_gobj->func_anim(dobj, -1, 0);
                }
                return; // not break

            case nGCAnimEvent32SetFlags:
                dobj->flags = dobj->anim_joint.event32->command.flags;
                dobj->anim_wait += AObjAnimAdvance(dobj->anim_joint.event32)->command.payload;
                break;

            case ANIM_CMD_16:
                if (dobj->parent_gobj->func_anim != NULL)
                {
                    // only seems to match when spelled out...
                    dobj->parent_gobj->func_anim
                    (
                        dobj,
                        dobj->anim_joint.event32->command.flags >> 8,
                        (u8)dobj->anim_joint.event32->command.flags
                    );
                }
                dobj->anim_wait += AObjAnimAdvance(dobj->anim_joint.event32)->command.payload;
                break;

            case ANIM_CMD_17:
                flags = dobj->anim_joint.event32->command.flags;
                dobj->anim_wait += AObjAnimAdvance(dobj->anim_joint.event32)->command.payload;

                for (i = 4; i < 14; i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (dobj->parent_gobj->func_anim != NULL)
                        {
                            dobj->parent_gobj->func_anim(dobj, i, dobj->anim_joint.event32->f);
                        }
                        AObjAnimAdvance(dobj->anim_joint.event32);
                    }
                }
                break;

                // empty, but necessary
            default:
#ifdef PORT
                /* PORT: safety net.  The port_aobj_event32_unhalfswap_stream
                 * call at function entry should have made the command word
                 * byte layout correct before we got here, so unhandled
                 * opcodes would only happen on truly corrupt data (or an
                 * opcode 23 we don't implement).  The original N64 `break;`
                 * would re-parse the same event and spin — terminate the
                 * animation instead. */
                /* Include the raw u32 word too — a halfswap-corrupted
                 * stream surfaces here as opcode in the high 7 bits of a
                 * shifted command word, and seeing the byte pattern
                 * makes the corruption shape diagnosable from the log
                 * alone.  E.g. opcode=64 with raw_u32=0x80000a03 is the
                 * halfswapped form of a real SetValRateBlock (opcode=5)
                 * whose stream-level un-halfswap fixup was skipped. */
                port_log("SSB64: gcParseDObjAnimJoint UNHANDLED opcode=%u ev=%p raw_u32=0x%08x — ending anim\n",
                    command_kind, (void*)dobj->anim_joint.event32,
                    *(u32*)dobj->anim_joint.event32);
                dobj->anim_wait = AOBJ_ANIM_END;
                return;
#else
                break;
#endif
            }
        }
        while (dobj->anim_wait <= 0.0F);
    }
}

f32 gcGetInterpValueCubic(f32 length_invert, f32 length, f32 value_base, f32 value_target, f32 rate_base, f32 rate_target) 
{
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 temp_f10;
    f32 temp_f16;
    f32 temp_f18;                                   // length_invert^2
    f32 temp_f2;                                    // length^2

    temp_f2  = SQUARE(length);
    temp_f18 = SQUARE(length_invert);
    temp_f16 = temp_f2 * length * temp_f18;         // length^3 * length_invert^2
    temp_f10 = 2.0F * temp_f16 * length_invert;     // 2.0F * length^3 * length_invert^3
    sp14     = 3.0F * temp_f2 * temp_f18;           // 3 * length^2 * length_invert^2
    sp18     = temp_f2 * length_invert;             // length_invert^3
    sp10     = temp_f16 - sp18;                     // length^3 * length_invert^2 - length_invert^3

    return (((temp_f10 - sp14) + 1.0F) * value_base) + (value_target * (sp14 - temp_f10)) + 
                            (rate_base * ((sp10 - sp18) + length)) + (rate_target * sp10);
}

f32 gcGetInterpRateCubic(f32 length_invert, f32 length, f32 value_base, f32 value_target, f32 rate_base, f32 rate_target) 
{
    f32 temp_f18;
    f32 temp_f16;
    f32 temp_f2;

    temp_f2  = 2.0F * length * length_invert;
    temp_f16 = 3.0F * length * length * length_invert * length_invert;
    temp_f18 = 6.0F * length;

    return (((temp_f18 * length * length_invert * length_invert * length_invert) - (temp_f18 * length_invert * length_invert)) * (value_base - value_target)) + 
                (rate_base * ((temp_f16 - (2.0F * temp_f2)) + 1.0F)) + (rate_target * (temp_f16 - temp_f2));
}

f32 gcGetAObjValue(AObj *aobj)
{
    switch (aobj->kind) 
    {
    case nGCAnimKindLinear:
        return aobj->value_base + (aobj->length * aobj->rate_base);

    case nGCAnimKindCubic:
        return gcGetInterpValueCubic
        (
            aobj->length_invert, aobj->length, aobj->value_base, aobj->value_target, aobj->rate_base, aobj->rate_target
        );

    case nGCAnimKindStep: 
        return (aobj->length_invert <= aobj->length) ? aobj->value_target : aobj->value_base;
    }
#if defined (AVOID_UB)
    return 0.0F;
#endif
}

f32 gcGetAObjRate(AObj *aobj) 
{
    switch (aobj->kind) 
    {
    case nGCAnimKindLinear: 
        return aobj->rate_base;

    case nGCAnimKindCubic:
        return gcGetInterpRateCubic
        (
            aobj->length_invert, aobj->length, aobj->value_base, aobj->value_target, aobj->rate_base, aobj->rate_target
        );

    case nGCAnimKindStep:
        return 0.0F;
    }
#if defined (AVOID_UB)
    return 0.0F;
#endif
}

void gcPlayDObjAnimJoint(DObj *dobj)
{
    f32 value; // array_dobjs
    AObj *aobj;
    f32 temp_f16;
    f32 temp_f12;
    f32 temp_f18;
    f32 temp_f14;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
#ifdef PORT
    static s32 sGCPlayDObjTraILogCount = 0;
#endif

    if (dobj->anim_wait != AOBJ_ANIM_NULL) 
    {
        aobj = dobj->aobj;

        while (aobj != NULL)
        {
            if (aobj->kind != nGCAnimKindNone)
            {
                if (dobj->anim_wait != AOBJ_ANIM_END) 
                { 
                    aobj->length += dobj->anim_speed; 
                }
                if (!(dobj->parent_gobj->flags & GOBJ_FLAG_NOANIM)) 
                {
                    switch (aobj->kind) 
                    {
                    case nGCAnimKindLinear:
                        value = aobj->value_base + (aobj->length * aobj->rate_base); 
                        break;
                        
                    case nGCAnimKindCubic:
                        temp_f16 = SQUARE(aobj->length_invert);
                        temp_f12 = SQUARE(aobj->length);
                        temp_f18 = aobj->length_invert * temp_f12;
                        temp_f14 = aobj->length * temp_f12 * temp_f16;
                        temp_f20 = 2.0F * temp_f14 * aobj->length_invert;
                        temp_f22 = 3.0F * temp_f12 * temp_f16;
                        temp_f24 = temp_f14 - temp_f18;

                        value = 
                            (aobj->value_base * ((temp_f20 - temp_f22) + 1.0F)) + 
                            (aobj->value_target * (temp_f22 - temp_f20)) + 
                            (aobj->rate_base * ((temp_f24 - temp_f18) + aobj->length)) + 
                            (aobj->rate_target * temp_f24);
                        break;
                        
                    case nGCAnimKindStep: 
                        value = (aobj->length_invert <= aobj->length) ? aobj->value_target : aobj->value_base; 
                        break;

                    default:
                        break;
                    }
                    switch (aobj->track)
                    {
                    case nGCAnimTrackRotX: 
                        dobj->rotate.vec.f.x = value;
                        break;

                    case nGCAnimTrackRotY: 
                        dobj->rotate.vec.f.y = value;
                        break;

                    case nGCAnimTrackRotZ: 
                        dobj->rotate.vec.f.z = value;
                        break;

                    case nGCAnimTrackTraI:
                        if (value < 0.0F) 
                        {
                            value = 0.0F;
                        } 
                        else if (value > 1.0F) 
                        {
                            value = 1.0F;
                        }
#ifdef PORT
                        if (sGCPlayDObjTraILogCount < 16)
                        {
                            sGCPlayDObjTraILogCount++;
                            port_log("SSB64: gcPlayDObjAnimJoint - TraI dobj=%p aobj=%p interp=%p value=%f kind=%u len=%f wait=%f speed=%f\n",
                                dobj, aobj, aobj->interpolate, value, aobj->kind, aobj->length, dobj->anim_wait, dobj->anim_speed);
                        }
#endif
                        syInterpCubic(&dobj->translate.vec.f, aobj->interpolate, value);
                        break;

                    case nGCAnimTrackTraX: 
                        dobj->translate.vec.f.x = value; 
                        break;

                    case nGCAnimTrackTraY: 
                        dobj->translate.vec.f.y = value; 
                        break;

                    case nGCAnimTrackTraZ: 
                        dobj->translate.vec.f.z = value; 
                        break;

                    case nGCAnimTrackScaX: 
                        dobj->scale.vec.f.x = value; 
                        break;
                        
                    case nGCAnimTrackScaY: 
                        dobj->scale.vec.f.y = value; 
                        break;

                    case nGCAnimTrackScaZ: 
                        dobj->scale.vec.f.z = value; 
                        break;
                    }
                }
            }
            aobj = aobj->next;
        }
        if (dobj->anim_wait == AOBJ_ANIM_END) 
        { 
            dobj->anim_wait = AOBJ_ANIM_NULL;
        }
    }
}

void gcParseMObjMatAnimJoint(MObj *mobj)
{
    AObj *mat_aobjs[10];
    AObj *matspecial_aobjs[5];
    AObj *aobj;
    s32 i;
    u32 command_kind;
    u32 flags;
    f32 payload;

#ifdef PORT
    /* See gcParseDObjAnimJoint — MObj matanim streams share the same
     * AObjEvent32 layout and the same halfswap corruption. */
    extern void port_aobj_event32_unhalfswap_stream(void *head);
    if (mobj->matanim_joint.event32 != NULL) {
        port_aobj_event32_unhalfswap_stream(mobj->matanim_joint.event32);
    }
#endif

    if (mobj->anim_wait != AOBJ_ANIM_NULL)
    {
        if (mobj->anim_wait == AOBJ_ANIM_CHANGED)
        {
            mobj->anim_wait = -mobj->anim_frame;
        }
        else
        {
            mobj->anim_wait -= mobj->anim_speed;
            mobj->anim_frame += mobj->anim_speed;

            if (mobj->anim_wait > 0.0F)
            {
                return;
            }
        }
        for (i = 0; i < ARRAY_COUNT(mat_aobjs); i++)
        {
            mat_aobjs[i] = NULL;
        }
        for (i = 0; i < ARRAY_COUNT(matspecial_aobjs); i++)
        {
            matspecial_aobjs[i] = NULL;
        }
        aobj = mobj->aobj;

        while (aobj != NULL)
        {
            if ((aobj->track >= nGCAnimTrackMaterialStart) && (aobj->track <= nGCAnimTrackMaterialEnd))
            {
                mat_aobjs[aobj->track - nGCAnimTrackMaterialStart] = aobj;
            }
            if ((aobj->track >= nGCAnimTrackMaterialSubStart) && (aobj->track <= nGCAnimTrackMaterialSubEnd))
            {
                matspecial_aobjs[aobj->track - nGCAnimTrackMaterialSubStart] = aobj;
            }
            aobj = aobj->next;
        }
        do
        {
            if (mobj->matanim_joint.event32 == NULL)
            {
                aobj = mobj->aobj;

                while (aobj != NULL)
                {
                    if (aobj->kind != nGCAnimKindNone)
                    {
                        aobj->length += mobj->anim_speed + mobj->anim_wait;
                    }
                    aobj = aobj->next;
                }
                mobj->anim_frame = mobj->anim_wait;
                mobj->anim_wait = AOBJ_ANIM_END;
                break;
            }
            command_kind = mobj->matanim_joint.event32->command.opcode;

            switch (command_kind)
            {
            case nGCAnimEvent32SetVal0RateBlock:
            case nGCAnimEvent32SetVal0Rate:
                payload = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(mat_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (mat_aobjs[i] == NULL)
                        {
                            mat_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialStart);
                        }
                        mat_aobjs[i]->value_base = mat_aobjs[i]->value_target;
                        mat_aobjs[i]->value_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);

                        mat_aobjs[i]->rate_base = mat_aobjs[i]->rate_target;
                        mat_aobjs[i]->rate_target = 0.0F;

                        mat_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            mat_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        mat_aobjs[i]->length = -mobj->anim_wait - mobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetVal0RateBlock)
                {
                    mobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetValBlock:
            case nGCAnimEvent32SetVal:
                payload = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(mat_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (mat_aobjs[i] == NULL)
                        {
                            mat_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialStart);
                        }
                        mat_aobjs[i]->value_base = mat_aobjs[i]->value_target;
                        mat_aobjs[i]->value_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);

                        mat_aobjs[i]->kind = nGCAnimKindLinear;

                        if (payload != 0.0F)
                        {
                            mat_aobjs[i]->rate_base = (mat_aobjs[i]->value_target - mat_aobjs[i]->value_base) / payload;
                        }
                        mat_aobjs[i]->length = -mobj->anim_wait - mobj->anim_speed;
                        mat_aobjs[i]->rate_target = 0.0F;
                    }
                }

                if (command_kind == nGCAnimEvent32SetValBlock)
                {
                    mobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetValRateBlock:
            case nGCAnimEvent32SetValRate:
                payload = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(mat_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (mat_aobjs[i] == NULL)
                        {
                            mat_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialStart);
                        }
                        mat_aobjs[i]->value_base = mat_aobjs[i]->value_target;
                        mat_aobjs[i]->value_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);

                        mat_aobjs[i]->rate_base = mat_aobjs[i]->rate_target;
                        mat_aobjs[i]->rate_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);

                        mat_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            mat_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        mat_aobjs[i]->length = -mobj->anim_wait - mobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValRateBlock)
                {
                    mobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetTargetRate:
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(mat_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (mat_aobjs[i] == NULL)
                        {
                            mat_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialStart);
                        }
                        mat_aobjs[i]->rate_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);
                    }
                }
                break;

            case nGCAnimEvent32Wait:
                mobj->anim_wait += mobj->matanim_joint.event32->command.payload;

                AObjAnimAdvance(mobj->matanim_joint.event32);
                break;

            case nGCAnimEvent32SetValAfterBlock:
            case nGCAnimEvent32SetValAfter:
                payload = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(mat_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (mat_aobjs[i] == NULL)
                        {
                            mat_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialStart);
                        }
                        mat_aobjs[i]->value_base = mat_aobjs[i]->value_target;
                        mat_aobjs[i]->value_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);

                        mat_aobjs[i]->kind = nGCAnimKindStep;

                        mat_aobjs[i]->length_invert = payload;
                        mat_aobjs[i]->length = -mobj->anim_wait - mobj->anim_speed;

                        mat_aobjs[i]->rate_target = 0.0F;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValAfterBlock)
                {
                    mobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetAnim:
                AObjAnimAdvance(mobj->matanim_joint.event32);

                mobj->matanim_joint.event32 = PORT_RESOLVE(mobj->matanim_joint.event32->p);
                mobj->anim_frame = -mobj->anim_wait;
                break;

            case nGCAnimEvent32Jump:
                AObjAnimAdvance(mobj->matanim_joint.event32);

                mobj->matanim_joint.event32 = PORT_RESOLVE(mobj->matanim_joint.event32->p);
                break;

            case ANIM_CMD_12:
                payload = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(mat_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (mat_aobjs[i] == NULL)
                        {
                            mat_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialStart);
                        }
                        mat_aobjs[i]->length += payload;
                    }
                }
                break;

            case nGCAnimEvent32End:
                aobj = mobj->aobj;

                while (aobj != NULL)
                {
                    if (aobj->kind != nGCAnimKindNone)
                    {
                        aobj->length += mobj->anim_speed + mobj->anim_wait;
                    }
                    aobj = aobj->next;
                }
                mobj->anim_frame = mobj->anim_wait;
                mobj->anim_wait = AOBJ_ANIM_END;
                return; // not break

            case nGCAnimEvent32SetExtValAfterBlock:
            case nGCAnimEvent32SetExtValAfter:
                payload = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(matspecial_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (matspecial_aobjs[i] == NULL)
                        {
                            matspecial_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialSubStart);
                        }
                        matspecial_aobjs[i]->value_base = matspecial_aobjs[i]->value_target;
                        matspecial_aobjs[i]->value_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);

                        matspecial_aobjs[i]->kind = nGCAnimKindStep;

                        matspecial_aobjs[i]->length_invert = payload;
                        matspecial_aobjs[i]->length = -mobj->anim_wait - mobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetExtValAfterBlock)
                {
                    mobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetExtValBlock:
            case nGCAnimEvent32SetExtVal:
                payload = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(matspecial_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (matspecial_aobjs[i] == NULL)
                        {
                            matspecial_aobjs[i] = gcAddAObjForMObj(mobj, i + nGCAnimTrackMaterialSubStart);
                        }
                        matspecial_aobjs[i]->value_base = matspecial_aobjs[i]->value_target;
                        matspecial_aobjs[i]->value_target = mobj->matanim_joint.event32->f;

                        AObjAnimAdvance(mobj->matanim_joint.event32);

                        matspecial_aobjs[i]->kind = nGCAnimKindLinear;

                        if (payload != 0.0F)
                        {
                            matspecial_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        matspecial_aobjs[i]->length = -mobj->anim_wait - mobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetExtValBlock)
                {
                    mobj->anim_wait += payload;
                }
                break;

            case ANIM_CMD_22:
                mobj->anim_wait = mobj->matanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(mobj->matanim_joint.event32)->command.flags;

                if (flags & 0x01)
                {
                    mobj->sub.unk4C = mobj->matanim_joint.event32->u;
                    AObjAnimAdvance(mobj->matanim_joint.event32);
                }
                if (flags & 0x02)
                {
                    mobj->sub.unk6C = mobj->matanim_joint.event32->u;
                    AObjAnimAdvance(mobj->matanim_joint.event32);
                }
                if (flags & 0x04)
                {
                    mobj->sub.unk68 = mobj->matanim_joint.event32->u;
                    AObjAnimAdvance(mobj->matanim_joint.event32);
                }
                if (flags & 0x08)
                {
                    mobj->sub.unk74 = mobj->matanim_joint.event32->u;
                    AObjAnimAdvance(mobj->matanim_joint.event32);
                }
                if (flags & 0x10)
                {
                    mobj->sub.unk70 = mobj->matanim_joint.event32->u;
                    AObjAnimAdvance(mobj->matanim_joint.event32);
                }
                break;

            default:
                break;
            }
        } 
        while (mobj->anim_wait <= 0.0F);
    }
}

void gcPlayMObjMatAnim(MObj *mobj)
{
    f32 value;
    AObj *aobj;
    f32 temp_f16;
    f32 temp_f12;
    f32 temp_f18;
    f32 temp_f14;
    f32 temp_f20;
    f32 temp_f22;
#ifdef PORT
    /* PORT: the inner switch in the color-track branch only handles
     * nGCAnimKindLinear and nGCAnimKindStep.  If aobj->kind is any other
     * value (Cubic, None, etc.), `color` is left uninitialized and the
     * subsequent track switch writes garbage stack bytes into the mobj's
     * primcolor/envcolor/etc.  N64 happens to land on a sensible stack
     * residue most frames, but PC stack contents vary across runs and
     * draw calls — observed as fighter colors that change between runs
     * for the same frame.  Initialize to opaque white as a no-op default. */
    SYColorPack color = { { 0xFF, 0xFF, 0xFF, 0xFF } };
#else
    SYColorPack color; // color
#endif
    f32 temp_f24;
    s32 interp;
    SYColorPack sp38; // sp38
    SYColorPack sp34; // sp34

    if (mobj->anim_wait != AOBJ_ANIM_NULL) 
    {
        aobj = mobj->aobj;
        
        while (aobj != NULL)
        {
            if (aobj->kind != nGCAnimKindNone) 
            {
                if (mobj->anim_wait != AOBJ_ANIM_END) 
                { 
                    aobj->length += mobj->anim_speed; 
                }
                if (aobj->track < (nGCAnimTrackMaterialSubStart - 1))
                {
                    switch (aobj->kind) 
                    {
                    case nGCAnimKindLinear: 
                        value = aobj->value_base + (aobj->length * aobj->rate_base); 
                        break;
                        
                    case nGCAnimKindCubic:
                        temp_f16 = SQUARE(aobj->length_invert);
                        temp_f12 = SQUARE(aobj->length);
                        temp_f18 = aobj->length_invert * temp_f12;
                        temp_f14 = aobj->length * temp_f12 * temp_f16;
                        temp_f20 = 2.0F * temp_f14 * aobj->length_invert;
                        temp_f22 = 3.0F * temp_f12 * temp_f16;
                        temp_f24 = temp_f14 - temp_f18;

                        value = (aobj->value_base * ((temp_f20 - temp_f22) + 1.0F)) + 
                                (aobj->value_target * (temp_f22 - temp_f20)) + 
                                (aobj->rate_base * ((temp_f24 - temp_f18) + aobj->length)) + 
                                (aobj->rate_target * temp_f24);
                        break;
                        
                    case nGCAnimKindStep:
                        value = (aobj->length_invert <= aobj->length) ? aobj->value_target : aobj->value_base;
                        break;
                        
                    default: 
                        break;
                    }
                    switch (aobj->track)
                    {
                    case nGCAnimTrackTextureIDCurrent: 
                        mobj->texture_id_curr = value;
                        break;
                        
                    case nGCAnimTrackTraU:
                        mobj->sub.trau = value;
                        break;
                        
                    case nGCAnimTrackTraV:
                        mobj->sub.trav = value;
                        break;
                        
                    case nGCAnimTrackScaU:
                        mobj->sub.scau = value;
                        break;
                        
                    case nGCAnimTrackScaV: 
                        mobj->sub.scav = value;
                        break;
                        
                    case nGCAnimTrackTextureIDNext: 
                        mobj->texture_id_next = value; 
                        break;
                        
                    case nGCAnimTrackScrU:
                        mobj->sub.scrollu = value; 
                        break;
                        
                    case nGCAnimTrackScrV:
                        mobj->sub.scrollv = value; 
                        break;
                        
                    case nGCAnimTrackSetLFrac: 
                        mobj->lfrac = value; 
                        break;
                        
                    case nGCAnimTrackPaletteID: 
                        mobj->palette_id = value;
                        break;
                        
                    default: 
                        break;
                    }
                } 
                else
                {
                    switch(aobj->kind)
                    {
                    case nGCAnimKindLinear:
                        interp = (aobj->length * aobj->length_invert * 256.0F);

                        if (interp < 0)
                        {
                            interp = 0;
                        }
                        if (interp > 256)
                        {
                            interp = 256;
                        }
#ifdef PORT
                        /* PORT: the IDO original packs two color bytes into
                         * non-adjacent positions of an SYColorPack union,
                         * multiplies the pack u32 by interp to lerp both
                         * bytes in parallel, then reads .s.r and .s.b out.
                         * That trick depends on (a) the union field offsets
                         * being in BE order so .s.r reads the high byte of
                         * the pack u32 and (b) `((u8*)&value)[0]` reading
                         * the BE-MSB of the source value.  Both fail on
                         * little-endian PC: the multiplication overflows
                         * lose the high bits, and the byte index reads the
                         * wrong source byte.  Net effect for white input:
                         * the matanim writes 0xFF00FF00 (rgba 0,255,0,255)
                         * to mobj->sub.primcolor instead of 0xFFFFFFFF.
                         *
                         * Replacement does plain per-channel linear interp
                         * with explicit bit shifts so the same code runs
                         * the same way regardless of host endianness.  We
                         * memcpy through u32 to type-pun safely from f32. */
                        {
                            u32 base_bits, targ_bits;
                            s32 inv = 256 - interp;
                            memcpy(&base_bits, &aobj->value_base,   4);
                            memcpy(&targ_bits, &aobj->value_target, 4);
                            color.s.r = (u8)((inv * (u8)(base_bits >> 24) + interp * (u8)(targ_bits >> 24)) >> 8);
                            color.s.g = (u8)((inv * (u8)(base_bits >> 16) + interp * (u8)(targ_bits >> 16)) >> 8);
                            color.s.b = (u8)((inv * (u8)(base_bits >>  8) + interp * (u8)(targ_bits >>  8)) >> 8);
                            color.s.a = (u8)((inv * (u8)(base_bits      ) + interp * (u8)(targ_bits      )) >> 8);
                        }
#else
                        sp34.pack = 0;
                        sp38.pack = 0;

                        sp38.s.g = ((u8*)&aobj->value_base)[0];
                        sp38.s.a = ((u8*)&aobj->value_base)[1];

                        sp34.s.g = ((u8*)&aobj->value_target)[0];
                        sp34.s.a = ((u8*)&aobj->value_target)[1];

                        sp38.pack = (256 - interp) * sp38.pack + sp34.pack * interp;

                        color.s.r = sp38.s.r;
                        color.s.g = sp38.s.b;

                        sp38.pack = 0;

                        sp38.s.g = ((u8*)&aobj->value_base)[2];
                        sp38.s.a = ((u8*)&aobj->value_base)[3];

                        sp34.s.g = ((u8*)&aobj->value_target)[2];
                        sp34.s.a = ((u8*)&aobj->value_target)[3];

                        sp38.pack = (256 - interp) * sp38.pack + sp34.pack * interp;

                        color.s.b = sp38.s.r;
                        color.s.a = sp38.s.b;
#endif
                        break;

                    case nGCAnimKindStep:
#ifdef PORT
                        /* PORT: same byte-order issue as LINEAR.  N64 reads
                         * the f32 bit pattern as SYColorPack via direct
                         * cast, picking up r at memory byte 0 = BE MSB.
                         * On LE PC the same cast picks up the LSB byte for
                         * r, swapping the channel order.  Type-pun via
                         * memcpy and use bit shifts so the channel layout
                         * stays N64-BE regardless of host. */
                        {
                            u32 src_bits;
                            if (aobj->length_invert <= aobj->length)
                                memcpy(&src_bits, &aobj->value_target, 4);
                            else
                                memcpy(&src_bits, &aobj->value_base, 4);
                            color.s.r = (u8)(src_bits >> 24);
                            color.s.g = (u8)(src_bits >> 16);
                            color.s.b = (u8)(src_bits >>  8);
                            color.s.a = (u8)(src_bits      );
                        }
#else
                        color = (aobj->length_invert <= aobj->length) ? *(SYColorPack*)&aobj->value_target : *(SYColorPack*)&aobj->value_base;
#endif
                        break;

#ifdef PORT
                    /* PORT: the IDO original has no case for nGCAnimKindCubic
                     * on color tracks, so `color` falls through with whatever
                     * stack residue happened to be sitting in that slot.  In
                     * practice fighter costume scripts use SetVal0Rate*/
                    /* SetValRate* (which the parser marks Cubic) to select
                     * costume-specific colors — on N64 the stack residue
                     * happened to hold a sensible previous color, so shoes/
                     * hats/tunics came out right; on PC the `color = white`
                     * init makes every such material render solid white.
                     *
                     * Cubic Hermite on a packed RGBA u32 isn't meaningful
                     * per-channel, and when gcPlayMObjMatAnim fires after
                     * the parser has advanced past the keyframe, `length`
                     * is already at or past `1/length_invert`, so the
                     * Hermite basis collapses to (base=0, target=1, rate*=0):
                     * value = value_target.  Treat Cubic as Step-at-boundary
                     * for color tracks. */
                    case nGCAnimKindCubic:
                        {
                            u32 src_bits;
                            if (aobj->length * aobj->length_invert >= 1.0F)
                                memcpy(&src_bits, &aobj->value_target, 4);
                            else
                                memcpy(&src_bits, &aobj->value_base, 4);
                            color.s.r = (u8)(src_bits >> 24);
                            color.s.g = (u8)(src_bits >> 16);
                            color.s.b = (u8)(src_bits >>  8);
                            color.s.a = (u8)(src_bits      );
                        }
                        break;
#endif
                    }
                    switch (aobj->track)
                    {
                    case nGCAnimTrackPrimColor:
                        mobj->sub.primcolor = color;
                        break;

                    case nGCAnimTrackEnvColor:
                        mobj->sub.envcolor = color;
                        break;

                    case nGCAnimTrackBlendColor:
                        mobj->sub.blendcolor = color;
                        break;

                    case nGCAnimTrackLight1Color:
                        mobj->sub.light1color = color;
                        break;

                    case nGCAnimTrackLight2Color:
                        mobj->sub.light2color = color;
                        break;
                    }
                }
            }
            aobj = aobj->next;
        }
        if (mobj->anim_wait == AOBJ_ANIM_END)
        {
            mobj->anim_wait = AOBJ_ANIM_NULL;
        }
    }
}

void gcPlayAnimAll(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj);
    MObj *mobj;

    while (dobj != NULL) 
    {
        gcParseDObjAnimJoint(dobj);
        gcPlayDObjAnimJoint(dobj);

        mobj = dobj->mobj;

        while (mobj != NULL) 
        {
            gcParseMObjMatAnimJoint(mobj);
            gcPlayMObjMatAnim(mobj);

            mobj = mobj->next;
        }
        if (dobj->child != NULL) 
        {
            dobj = dobj->child;
        } 
        else if (dobj->sib_next != NULL) 
        {
            dobj = dobj->sib_next;
        } 
        else while (TRUE) 
        {
            if (dobj->parent == DOBJ_PARENT_NULL)
            {
                dobj = NULL;

                break;
            }
            else if (dobj->parent->sib_next != NULL)
            {
                dobj = dobj->parent->sib_next;

                break;
            }
            else dobj = dobj->parent;
        }
    }
}

AObj* gcGetTrackAObj(AObj *aobj, u8 track)
{
    while (aobj != NULL) 
    {
        if (aobj->track == track) 
        { 
            return aobj; 
        }
        aobj = aobj->next;
    }
    return NULL;
}

void gcSetDObjAnimLength(DObj *dobj, f32 length)
{
    AObj *aobj = dobj->aobj;
    dobj->anim_wait = dobj->anim_speed + length;

    while (aobj != NULL) 
    {
        aobj->length_invert = 1.0F / length;
        aobj = aobj->next;
    }
}

f32 gcGetDObjAxisTrack(DObj *dobj, s32 track)
{
    switch (track) 
    {
    case nGCAnimTrackRotX: 
        return dobj->rotate.vec.f.x;

    case nGCAnimTrackRotY: 
        return dobj->rotate.vec.f.y;

    case nGCAnimTrackRotZ: 
        return dobj->rotate.vec.f.z;

    case nGCAnimTrackTraX: 
        return dobj->translate.vec.f.x;

    case nGCAnimTrackTraY: 
        return dobj->translate.vec.f.y;

    case nGCAnimTrackTraZ: 
        return dobj->translate.vec.f.z;

    case nGCAnimTrackScaX: 
        return dobj->scale.vec.f.x;

    case nGCAnimTrackScaY: 
        return dobj->scale.vec.f.y;

    case nGCAnimTrackScaZ: 
        return dobj->scale.vec.f.z;
    }
#if defined (AVOID_UB)
    // todo: return NaN?
    return 0.0F;
#endif
}

f32 gcGetDObjDescAxisTrack(DObjDesc *dobjdesc, s32 track)
{
    switch (track) 
    {
    case nGCAnimTrackRotX: 
        return dobjdesc->rotate.x;

    case nGCAnimTrackRotY: 
        return dobjdesc->rotate.y;

    case nGCAnimTrackRotZ: 
        return dobjdesc->rotate.z;

    case nGCAnimTrackTraX: 
        return dobjdesc->translate.x;

    case nGCAnimTrackTraY: 
        return dobjdesc->translate.y;

    case nGCAnimTrackTraZ: 
        return dobjdesc->translate.z;

    case nGCAnimTrackScaX: 
        return dobjdesc->scale.x;

    case nGCAnimTrackScaY: 
        return dobjdesc->scale.y;

    case nGCAnimTrackScaZ: 
        return dobjdesc->scale.z;
    }
#if defined (AVOID_UB)
    // todo: return NaN?
    return 0.0F;
#endif
}

sb32 gcCheckGetDObjNoAxisTrack
(
    sb32 is_desc_or_dobj,
    DObj *dobj,
    f32 *axis_value,
    f32 *rate,
    AObj *seek_aobj,
    DObjDesc *dobjdesc,
    s32 track,
    s32 rate_kind,
    Vec3f *translate,
    sb32 *is_axis_ready
)
{
    AObj *aobj = gcGetTrackAObj(seek_aobj, track);

    if ((aobj != NULL) && (aobj->kind != nGCAnimKindNone))
    {
        if ((is_desc_or_dobj == 0) && (dobj->anim_wait != AOBJ_ANIM_END))
        {
            aobj->length += dobj->anim_speed;
        }
        *axis_value = gcGetAObjValue(aobj);

        if (rate_kind != 0)
        {
            *rate = gcGetAObjRate(aobj);
        }
    }
    else if ((track == nGCAnimTrackTraX) || (track == nGCAnimTrackTraY) || (track == nGCAnimTrackTraZ))
    {
        if (*is_axis_ready != FALSE)
        {
            switch (track)
            {
            case nGCAnimTrackTraX:
                *axis_value = translate->x;
                break;

            case nGCAnimTrackTraY:
                *axis_value = translate->y;
                break;

            case nGCAnimTrackTraZ:
                *axis_value = translate->z;
                break;
            }
        }
        else
        {
            aobj = gcGetTrackAObj(seek_aobj, nGCAnimTrackTraI);

            if ((aobj != NULL) && (aobj->kind != nGCAnimKindNone))
            {
                if ((is_desc_or_dobj == 0) && (dobj->anim_wait != AOBJ_ANIM_END))
                {
                    aobj->length += dobj->anim_speed;
                }
                *axis_value = gcGetAObjValue(aobj);

                if (*axis_value < 0.0F)
                {
                    *axis_value = 0.0F;
                }
                else if (*axis_value > 1.0F)
                {
                    *axis_value = 1.0F;
                }
                syInterpCubic(translate, aobj->interpolate, *axis_value);

                switch (track)
                {
                case nGCAnimTrackTraX:
                    *axis_value = translate->x;
                    break;

                case nGCAnimTrackTraY:
                    *axis_value = translate->y;
                    break;

                case nGCAnimTrackTraZ:
                    *axis_value = translate->z;
                    break;
                }
                *is_axis_ready = TRUE;
            }
            else if (is_desc_or_dobj == 0)
            {
                if (dobjdesc == NULL)
                {
                    return TRUE;
                }
                else *axis_value = gcGetDObjDescAxisTrack(dobjdesc, track);
            }
            else *axis_value = gcGetDObjAxisTrack(dobj, track);
        }
    }
    else if (is_desc_or_dobj == 0)
    {
        if (dobjdesc == NULL)
        {
            return TRUE;
        }
        *axis_value = gcGetDObjDescAxisTrack(dobjdesc, track);
    }
    else *axis_value = gcGetDObjAxisTrack(dobj, track);

    return FALSE;
}

void gcGetAObjTrackAnimTimeMax(s32 track, f32 translate, f32 rotate, f32 scale, f32 *anim_time_max, AObj *aobj)
{
    f32 rate_combine;
    f32 value_diff;
    f32 temp;
    f32 unused;
    f32 value;
    
    switch (track)
    {
    case nGCAnimTrackRotX:
    case nGCAnimTrackRotY:
    case nGCAnimTrackRotZ:
        value = rotate;
        break;

    case nGCAnimTrackTraX:
    case nGCAnimTrackTraY:
    case nGCAnimTrackTraZ:
        value = translate;
        break;

    case nGCAnimTrackScaX:
    case nGCAnimTrackScaY:
    case nGCAnimTrackScaZ:
        value = scale;
        break;
    }
    if (value != 0.0F)
    {
        if (TRUE);

        rate_combine = (2.0F * aobj->rate_base) + aobj->rate_target;
        value_diff = (-6.0F * value) * (aobj->value_target - aobj->value_base);
        
        if (value_diff < SQUARE(rate_combine))
        {
            temp = (sqrtf(SQUARE(rate_combine) - value_diff) + -rate_combine) / value;
            
            TAKE_MAX(*anim_time_max, temp);
            
            temp = (-rate_combine - sqrtf(SQUARE(rate_combine) - value_diff)) / value;
            
            TAKE_MAX(*anim_time_max, temp);
        }
        else if ((SQUARE(rate_combine) + -value_diff) == (0, 0.0F))
        {
            temp = -rate_combine / value;
            
            TAKE_MAX(*anim_time_max, temp);
        }
        if ((SQUARE(rate_combine) + value_diff) > 0.0F)
        {
            temp = (sqrtf(SQUARE(rate_combine) + value_diff) + -rate_combine) / -value;

            TAKE_MAX(*anim_time_max, temp);
            
            temp = (-rate_combine - sqrtf(SQUARE(rate_combine) + value_diff)) / -value;

            TAKE_MAX(*anim_time_max, temp);
        }
        else if ((SQUARE(rate_combine) + value_diff) == (0, 0.0F))
        {
            temp = -rate_combine / -value;
            
            TAKE_MAX(*anim_time_max, temp);
        }
        rate_combine = -(aobj->rate_base + (2.0F * aobj->rate_target));
        value_diff = (-6.0F * value) * (aobj->value_base - aobj->value_target);
        
        if (value_diff < SQUARE(rate_combine))
        {
            temp = (sqrtf(SQUARE(rate_combine) - value_diff) + -rate_combine) / value;
            
            TAKE_MAX(*anim_time_max, temp);
            
            temp = (-rate_combine - sqrtf(SQUARE(rate_combine) - value_diff)) / value;
            
            TAKE_MAX(*anim_time_max, temp);
        }
        else if ((SQUARE(rate_combine) + -value_diff) == (0, 0.0F))
        {
            temp = -rate_combine / value;
            
            TAKE_MAX(*anim_time_max, temp);
        }
        if ((SQUARE(rate_combine) + value_diff) > 0.0F)
        {
            temp = (sqrtf(SQUARE(rate_combine) + value_diff) + -rate_combine) / -value;
            
            TAKE_MAX(*anim_time_max, temp);
            
            temp = (-rate_combine - sqrtf(value_diff + SQUARE(rate_combine))) / -value;
            
            TAKE_MAX(*anim_time_max, temp);
        }
        else if ((SQUARE(rate_combine) + value_diff) == (0, 0.0F))
        {
            temp = -rate_combine / -value;
            
            TAKE_MAX(*anim_time_max, temp);
        }
    }
}

f32 gcGetDObjTempAnimTimeMax
(
    DObj *dobj,
    AObjEvent32 **anim_joints,
    f32 anim_frame,
    DObjDesc *dobjdesc,
    s32 rate_kind,
    f32 length,
    f32 translate,
    f32 rotate,
    f32 scale
)
{
    AObj *root_aobj;
    AObj *parse_aobj;
    AObj *new_aobj;
    AObj *origin_aobj;
    f32 value_old;
    f32 value_new;
    f32 rate_old;
    f32 rate_new;
    f32 time_max;
    s32 i;
    sb32 is_dobj_axis_ready;
    sb32 is_desc_axis_ready;
    Vec3f dobj_translate;
    Vec3f desc_translate;

    parse_aobj = NULL;

    is_dobj_axis_ready = FALSE;
    is_desc_axis_ready = FALSE;

    time_max = 0.0F;

 #ifdef PORT
    if ((anim_joints == NULL) || (*(u32*)anim_joints == 0))
 #else
    if ((anim_joints == NULL) || (*anim_joints == NULL))
 #endif
    {
        if (dobjdesc == NULL)
        {
            gcRemoveAObjFromDObj(dobj);

            return 0;
        }
    }
    root_aobj = dobj->aobj;
    dobj->aobj = NULL;

 #ifdef PORT
    if ((anim_joints != NULL) && (*(u32*)anim_joints != 0))
    {
        dobj->anim_joint.event32 = (AObjEvent32*)PORT_RESOLVE(*(u32*)anim_joints);

        if (dobj->anim_joint.event32 != NULL)
        {
            dobj->anim_wait = AOBJ_ANIM_CHANGED;
            dobj->anim_frame = anim_frame;

            gcParseDObjAnimJoint(dobj);

            parse_aobj = dobj->aobj;
            dobj->aobj = NULL;
        }
    }
 #else
    if ((anim_joints != NULL) && (*anim_joints != NULL))
    {
        dobj->anim_joint.event32 = *anim_joints;
        dobj->anim_wait = AOBJ_ANIM_CHANGED;
        dobj->anim_frame = anim_frame;

        gcParseDObjAnimJoint(dobj);

        parse_aobj = dobj->aobj;
        dobj->aobj = NULL;
    }
 #endif
    for (i = nGCAnimTrackJointStart; i <= nGCAnimTrackJointEnd; i++)
    {
        if (i == nGCAnimTrackTraI)
        {
            continue;
        }
        rate_new = 0.0F;
        rate_old = 0.0F;

        if (gcCheckGetDObjNoAxisTrack(0, dobj, &value_new, &rate_new, parse_aobj, dobjdesc, i, rate_kind, &desc_translate, &is_desc_axis_ready))
        {
            continue;
        }
        gcCheckGetDObjNoAxisTrack(1, dobj, &value_old, &rate_old, root_aobj, dobjdesc, i, rate_kind, &dobj_translate, &is_dobj_axis_ready);

        if ((value_new != value_old) || (rate_new != rate_old))
        {
            new_aobj = gcAddAObjForDObj(dobj, i);

            if ((i == nGCAnimTrackRotX) || (i == nGCAnimTrackRotY) || (i == nGCAnimTrackRotZ))
            {
                if (ABS(value_new - value_old) > ABS(value_new - (value_old + F_CST_DTOR32(360.0F))))
                {
                    value_old += F_CST_DTOR32(360.0F);
                }
                if (ABS(value_new - value_old) > ABS(value_new - (value_old - F_CST_DTOR32(360.0F))))
                {
                    value_old -= F_CST_DTOR32(360.0F);
                }
            }
            switch (rate_kind)
            {
            case 0:
                new_aobj->value_base = value_old;
                new_aobj->value_target = value_new;

                new_aobj->kind = nGCAnimKindLinear;

                new_aobj->length_invert = 1.0F / length;
                new_aobj->length = -dobj->anim_speed;

                new_aobj->rate_base = (new_aobj->value_target - new_aobj->value_base) / length;
                new_aobj->rate_target = 0.0F;
                break;

            case 1:
            case 2:
                new_aobj->value_base = value_old;
                new_aobj->value_target = value_new;

                new_aobj->kind = nGCAnimKindCubic;

                new_aobj->length_invert = 1.0F / length;
                new_aobj->length = -dobj->anim_speed;

                new_aobj->rate_base = rate_old;
                new_aobj->rate_target = rate_new;

                if (rate_kind == nGCAnimKindLinear)
                {
                    gcGetAObjTrackAnimTimeMax(i, translate, rotate, scale, &time_max, new_aobj);
                }
                break;

            default:
                break;
            }
        }
    }
    origin_aobj = dobj->aobj;
    dobj->aobj = root_aobj;

    gcRemoveAObjFromDObj(dobj);

    dobj->aobj = parse_aobj;

    gcRemoveAObjFromDObj(dobj);

    dobj->aobj = origin_aobj;
    dobj->anim_joint.event32 = NULL;

    dobj->anim_wait = dobj->anim_speed + length;
    dobj->anim_frame = -dobj->anim_speed;

    return time_max;
}

// Thanks, inspect!
f32 func_8000EC64_F864
(
    GObj *gobj,
    AObjEvent32 **anim_joints,
    f32 anim_frame,
    DObjDesc *dobjdesc,
    s32 rate_kind,
    f32 length_max,
    f32 length_min,
    f32 translate,
    f32 rotate,
    f32 scale
) 
{
    DObj *dobj;
    f32 temp;
    f32 length_curr;
    f32 length_max_old;

    dobj = DObjGetStruct(gobj);
    gobj->anim_frame = anim_frame;
    
    if (rate_kind == 2)
    {
        length_max_old = length_max;
        
        length_max = 0.0F;
        
        while (dobj != NULL)
        {
            temp = gcGetDObjTempAnimTimeMax(dobj, anim_joints, anim_frame, dobjdesc, rate_kind, length_max, translate, rotate, scale);
            
            if (length_max < temp) 
            {
                length_max = temp;
            }
            if (anim_joints != NULL)
            {
#ifdef PORT
                anim_joints = (AObjEvent32**)(void*)(((u32*)anim_joints) + 1);
#else
                anim_joints++;
#endif
            }
            if (dobjdesc != NULL)
            {
                dobjdesc++;
            }
            dobj = gcGetTreeDObjNext(dobj);
        }
        dobj = DObjGetStruct(gobj);

        if (length_max < length_max_old)
        {
            length_max = length_max_old;
        } 
        else if (length_max > length_min)
        {
            length_max = length_min;
        }
        while (dobj != NULL)
        {
            gcSetDObjAnimLength(dobj, length_max);

            dobj = gcGetTreeDObjNext(dobj);
        }
    } 
    else while (dobj != NULL)
    {
        gcGetDObjTempAnimTimeMax(dobj, anim_joints, anim_frame, dobjdesc, rate_kind, length_max, translate, rotate, scale);

        if (anim_joints != NULL)
        {
#ifdef PORT
            anim_joints = (AObjEvent32**)(void*)(((u32*)anim_joints) + 1);
#else
            anim_joints++;
#endif
        }
        if (dobjdesc != NULL)
        {
            dobjdesc++;
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
    gobj->anim_frame = 0.0F;
    
    return length_max;
}

void func_8000EE40_FA40(GObj *gobj, AObjEvent32 **anim_joints, f32 anim_frame, DObjDesc *dobjdesc)
{
    s32 i;
    DObj *dobj;
    f32 axis_value;
    sb32 unused;
    sb32 is_axis_ready;
    Vec3f translate;
    sb32 is_anim_root;

    dobj = DObjGetStruct(gobj);
    
    is_axis_ready = FALSE;
    is_anim_root = TRUE;
    
    gobj->anim_frame = anim_frame;

    while (dobj != NULL) 
    {
#ifdef PORT
        u32 anim_joint_token = *(u32*)anim_joints;

        if (anim_joint_token != 0)
        {
            AObjEvent32 *anim_joint = (AObjEvent32*)PORT_RESOLVE(anim_joint_token);

            if (anim_joint != NULL)
            {
                gcAddDObjAnimJoint(dobj, anim_joint, anim_frame);
            
                dobj->is_anim_root = is_anim_root;
                is_anim_root = FALSE;

                for (i = nGCAnimTrackJointStart; i <= nGCAnimTrackJointEnd; i++)
                {
                    if (i != nGCAnimTrackTraI)
                    {
                        gcCheckGetDObjNoAxisTrack
                        (
                            0, 
                            dobj,
                            &axis_value, 
                            NULL, 
                            dobj->aobj, 
                            dobjdesc, 
                            i, 
                            0, 
                            &translate, 
                            &is_axis_ready
                        );
                        switch (i) 
                        {
                        case nGCAnimTrackRotX:
                            dobj->rotate.vec.f.x = axis_value;
                            break;
                        
                        case nGCAnimTrackRotY:
                            dobj->rotate.vec.f.y = axis_value; 
                            break;
                        
                        case nGCAnimTrackRotZ:
                            dobj->rotate.vec.f.z = axis_value; 
                            break;
                        
                        case nGCAnimTrackTraX:
                            dobj->translate.vec.f.x = axis_value; 
                            break;
                        
                        case nGCAnimTrackTraY:
                            dobj->translate.vec.f.y = axis_value; 
                            break;
                        
                        case nGCAnimTrackTraZ:
                            dobj->translate.vec.f.z = axis_value; 
                            break;
                        
                        case nGCAnimTrackScaX:
                            dobj->scale.vec.f.x = axis_value; 
                            break;
                        
                        case nGCAnimTrackScaY:
                            dobj->scale.vec.f.y = axis_value; 
                            break;
                        
                        case nGCAnimTrackScaZ:
                            dobj->scale.vec.f.z = axis_value; 
                            break;
                        }
                    }
                }
            }
        }
#else
        if (*anim_joints != NULL)
        {
            gcAddDObjAnimJoint(dobj, *anim_joints, anim_frame);
            
            dobj->is_anim_root = is_anim_root;
            is_anim_root = FALSE;

            for (i = nGCAnimTrackJointStart; i <= nGCAnimTrackJointEnd; i++)
            {
                if (i != nGCAnimTrackTraI)
                {
                    gcCheckGetDObjNoAxisTrack
                    (
                        0, 
                        dobj,
                        &axis_value, 
                        NULL, 
                        dobj->aobj, 
                        dobjdesc, 
                        i, 
                        0, 
                        &translate, 
                        &is_axis_ready
                    );
                    switch (i) 
                    {
                    case nGCAnimTrackRotX:
                        dobj->rotate.vec.f.x = axis_value;
                        break;
                    
                    case nGCAnimTrackRotY:
                        dobj->rotate.vec.f.y = axis_value; 
                        break;
                    
                    case nGCAnimTrackRotZ:
                        dobj->rotate.vec.f.z = axis_value; 
                        break;
                    
                    case nGCAnimTrackTraX:
                        dobj->translate.vec.f.x = axis_value; 
                        break;
                    
                    case nGCAnimTrackTraY:
                        dobj->translate.vec.f.y = axis_value; 
                        break;
                    
                    case nGCAnimTrackTraZ:
                        dobj->translate.vec.f.z = axis_value; 
                        break;
                    
                    case nGCAnimTrackScaX:
                        dobj->scale.vec.f.x = axis_value; 
                        break;
                    
                    case nGCAnimTrackScaY:
                        dobj->scale.vec.f.y = axis_value; 
                        break;
                    
                    case nGCAnimTrackScaZ:
                        dobj->scale.vec.f.z = axis_value; 
                        break;
                    }
                }
            }
        }
#endif
        else
        {
            dobj->anim_wait = AOBJ_ANIM_NULL;
            dobj->is_anim_root = FALSE;
            
            if (dobjdesc != NULL)
            {
                dobj->translate.vec.f = dobjdesc->translate;
                dobj->rotate.vec.f = dobjdesc->rotate;
                dobj->scale.vec.f = dobjdesc->scale;
            }
        }
#ifdef PORT
        anim_joints = (AObjEvent32**)(void*)(((u32*)anim_joints) + 1);
#else
        anim_joints++;
#endif

        if (dobjdesc != NULL)
        {
            dobjdesc++;
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcAddDObjTransformTraRotSca(DObj *dobj)
{
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindSca, 0);
}

DObj* gcAddDObjForGObjTraRotSca(GObj *gobj, void *dvar)
{
    DObj *dobj = gcAddDObjForGObj(gobj, dvar);

    gcAddDObjTransformTraRotSca(dobj);

    return dobj;
}

DObj* gcAddSiblingForDObjTraRotSca(DObj *dobj, void *dvar)
{
    DObj *sibling_dobj = gcAddSiblingForDObj(dobj, dvar);
    gcAddDObjTransformTraRotSca(sibling_dobj);

    return sibling_dobj;
}

DObj* gcAddChildForDObjTraRotSca(DObj *dobj, void *dvar)
{
    DObj *child_dobj = gcAddChildForDObj(dobj, dvar);
    gcAddDObjTransformTraRotSca(child_dobj);

    return child_dobj;
}

void gcSetupCommonDObjs(GObj *gobj, DObjDesc *dobjdesc, DObj **dobjs)
{
    s32 i;
    DObj* dobj;
    s32 id;
    DObj* array_dobjs[DOBJ_ARRAY_MAX];

    for (i = 0; i < ARRAY_COUNT(array_dobjs); i++)
    {
        array_dobjs[i] = NULL;
    }
    while (dobjdesc->id != ARRAY_COUNT(array_dobjs))
    {
        id = dobjdesc->id & 0xFFF;

        if (id != 0)
        {
            dobj = array_dobjs[id] = gcAddChildForDObj(array_dobjs[id - 1], PORT_RESOLVE(dobjdesc->dl));
        }
        else dobj = array_dobjs[0] = gcAddDObjForGObj(gobj, PORT_RESOLVE(dobjdesc->dl));

        if (dobjdesc->id & 0xF000)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKindTra, 0);
        }
        if (dobjdesc->id & 0x8000)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKindRecalcRotRpyRSca, 0);
        } 
        else if (dobjdesc->id & 0x4000)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKind46, 0);
        }
        else if (dobjdesc->id & 0x2000)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKind48, 0);
        }
        else if (dobjdesc->id & 0x1000)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKind50, 0);
        }
        else gcAddDObjTransformTraRotSca(dobj);
        
        dobj->translate.vec.f = dobjdesc->translate;
        dobj->rotate.vec.f = dobjdesc->rotate;
        dobj->scale.vec.f = dobjdesc->scale;

        if (dobjs != NULL)
        {
            *dobjs++ = dobj;
        }
        dobjdesc++;
    }
}

void gcAddDObj3TransformsKind(DObj *dobj, u8 tk1, u8 tk2, u8 tk3)
{
    if (tk1 != nGCMatrixKindNull) 
    {
        gcAddXObjForDObjFixed(dobj, tk1, 0);
    }
    if (tk2 != nGCMatrixKindNull) 
    {
        gcAddXObjForDObjFixed(dobj, tk2, 0);
    }
    if (tk3 != nGCMatrixKindNull) 
    {
        gcAddXObjForDObjFixed(dobj, tk3, 0);
    }
}

void gcDecideDObj3TransformsKind(DObj *dobj, u8 tk1, u8 tk2, u8 tk3, s32 flags)
{
    s32 tra_mode = 0;
    s32 sca_mode = 0;
    s32 rot_mode = 0;

    switch (tk1)
    {
    case nGCMatrixKindTra:
        tra_mode = 1;
        break;

    case nGCMatrixKindRotRpyR: 
        rot_mode = 1;
        break;

    case nGCMatrixKindTraRotRpyR:
        rot_mode = 1;
        tra_mode = 1;
        break;
            
    case nGCMatrixKindTraRotRpyRSca:
        sca_mode = 1;
        rot_mode = 1;
        tra_mode = 1;
        break;

    case nGCMatrixKindRotPyrR: 
        rot_mode = 2; 
        break;

    case nGCMatrixKindTraRotPyrR:
        rot_mode = 2;
        tra_mode = 1;
        break;
            
    case nGCMatrixKindTraRotPyrRSca:
        rot_mode = 2;
        sca_mode = 1;
        tra_mode = 1;
        break;

    case nGCMatrixKindSca: 
        sca_mode = 1;
        break;
    }
    switch (tk2)
    {
    case nGCMatrixKindRotRpyR:
        rot_mode = 1; 
        break;

    case nGCMatrixKindRotPyrR:
        rot_mode = 2;
        break;

    case nGCMatrixKindSca:
        sca_mode = 1;
        break;
    }
    if (tk3 == nGCMatrixKindSca)
    {
        sca_mode = 1;
    }
    if (tra_mode != 0)
    {
        gcAddXObjForDObjFixed(dobj, nGCMatrixKindTra, 0);
    }
    if (flags & 0x4000)
    {
        if (rot_mode == 1)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKind46, 0);
        } 
        else gcAddXObjForDObjFixed(dobj, nGCMatrixKind45, 0);
    }
    else if (flags & 0x2000)
    {
        if (rot_mode == 1)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKind48, 0);
        }
        else gcAddXObjForDObjFixed(dobj, nGCMatrixKind47, 0);
    } 
    else if (flags & 0x1000)
    {
        if (rot_mode == 1)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKind50, 0);
        }
        else gcAddXObjForDObjFixed(dobj, nGCMatrixKind49, 0);
    } 
    else if (sca_mode != 0)
    {
        if (rot_mode == 1)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKindRecalcRotRpyRSca, 0);
        }
        else gcAddXObjForDObjFixed(dobj, nGCMatrixKindRecalcRotPyrRSca, 0);
    }
    else if (rot_mode == 1)
    {
        gcAddXObjForDObjFixed(dobj, nGCMatrixKindRecalcRotRpyR, 0);
    }
    else gcAddXObjForDObjFixed(dobj, nGCMatrixKindRecalcRotPyrR, 0);
}

// 0x8000F590
void gcSetupCustomDObjs(GObj *gobj, DObjDesc *dobjdesc, DObj **dobjs, u8 tk1, u8 tk2, u8 tk3)
{
    s32 i;
    DObj *dobj;
    s32 id;
    DObj *array_dobjs[DOBJ_ARRAY_MAX];

    for (i = 0; i < ARRAY_COUNT(array_dobjs); i++)
    {
        array_dobjs[i] = NULL;
    }
    while (dobjdesc->id != ARRAY_COUNT(array_dobjs)) 
    {
        id = dobjdesc->id & 0xFFF;

        if (id != 0)
        {
            dobj = array_dobjs[id] = gcAddChildForDObj(array_dobjs[id - 1], PORT_RESOLVE(dobjdesc->dl));
        }
        else dobj = array_dobjs[0] = gcAddDObjForGObj(gobj, PORT_RESOLVE(dobjdesc->dl));

        if (dobjdesc->id & 0xF000)
        {
            gcDecideDObj3TransformsKind(dobj, tk1, tk2, tk3, dobjdesc->id & 0xF000);
        }
        else gcAddDObj3TransformsKind(dobj, tk1, tk2, tk3);

        dobj->translate.vec.f = dobjdesc->translate;
        dobj->rotate.vec.f = dobjdesc->rotate;
        dobj->scale.vec.f = dobjdesc->scale;

        if (dobjs != NULL)
        {
            *dobjs++ = dobj;
        }
        dobjdesc++;
    }
}

// 0x8000F720
void gcSetupCustomDObjsWithMObj(GObj *gobj, DObjDesc *dobjdesc, MObjSub ***p_mobjsubs, DObj **dobjs, u8 tk1, u8 tk2, u8 tk3)
{
    s32 i;
    DObj *dobj;
    s32 id;
    DObj *array_dobjs[DOBJ_ARRAY_MAX];

    for (i = 0; i < ARRAY_COUNT(array_dobjs); i++)
    {
        array_dobjs[i] = NULL;
    }
    while (dobjdesc->id != ARRAY_COUNT(array_dobjs))
    {
        id = dobjdesc->id & 0xFFF;

        if (id != 0)
        {
            dobj = array_dobjs[id] = gcAddChildForDObj(array_dobjs[id - 1], PORT_RESOLVE(dobjdesc->dl));
        }
        else dobj = array_dobjs[0] = gcAddDObjForGObj(gobj, PORT_RESOLVE(dobjdesc->dl));
        
        if (dobjdesc->id & 0xF000) 
        {
            gcDecideDObj3TransformsKind(dobj, tk1, tk2, tk3, dobjdesc->id & 0xF000);
        } 
        else gcAddDObj3TransformsKind(dobj, tk1, tk2, tk3);
        
        dobj->translate.vec.f = dobjdesc->translate;
        dobj->rotate.vec.f = dobjdesc->rotate;
        dobj->scale.vec.f = dobjdesc->scale;

        if (p_mobjsubs != NULL)
        {
#ifdef PORT
            u32 mobjsubs_token = *(u32*)p_mobjsubs;
            if (mobjsubs_token != 0)
            {
                u32 *mobjsubs = (u32*)PORT_RESOLVE(mobjsubs_token);

                if (mobjsubs != NULL)
                {
                    u32 mobjsub_token = *mobjsubs;

                    while (mobjsub_token != 0)
                    {
                        MObjSub *mobjsub = (MObjSub*)PORT_RESOLVE(mobjsub_token);

                        if (mobjsub == NULL)
                        {
                            break;
                        }
                        portFixupMObjSub(mobjsub);
                        gcAddMObjForDObj(dobj, mobjsub);

                        mobjsubs++;

                        mobjsub_token = *mobjsubs;
                    }
                }
            }
#else
            if (*p_mobjsubs != NULL)
            {
                MObjSub **mobjsubs = *p_mobjsubs;
                MObjSub *mobjsub = *mobjsubs;

                while (mobjsub != NULL)
                {
                    gcAddMObjForDObj(dobj, mobjsub);

                    mobjsubs++;

                    mobjsub = *mobjsubs;
                }
            }
            p_mobjsubs++;
#endif
#ifdef PORT
            p_mobjsubs = (MObjSub***)(void*)(((u32*)p_mobjsubs) + 1);
#endif
        }
        if (dobjs != NULL)
        {
            *dobjs++ = dobj;
        }
        dobjdesc++;
    }
}

void gcAddMObjAll(GObj *gobj, MObjSub ***p_mobjsubs)
{
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        if (p_mobjsubs != NULL)
        {
#ifdef PORT
            u32 mobjsubs_token = *(u32*)p_mobjsubs;
            if (mobjsubs_token != 0)
            {
                u32 *mobjsubs = (u32*)PORT_RESOLVE(mobjsubs_token);

                if (mobjsubs != NULL)
                {
                    u32 mobjsub_token = *mobjsubs;

                    while (mobjsub_token != 0)
                    {
                        MObjSub *mobjsub = (MObjSub*)PORT_RESOLVE(mobjsub_token);

                        if (mobjsub == NULL)
                        {
                            break;
                        }
                        portFixupMObjSub(mobjsub);
                        gcAddMObjForDObj(dobj, mobjsub);

                        mobjsubs++;

                        mobjsub_token = *mobjsubs;
                    }
                }
            }
#else
            if (*p_mobjsubs != NULL)
            {
                MObjSub **mobjsubs = *p_mobjsubs;
                MObjSub *mobjsub = *mobjsubs;

                while (mobjsub != NULL)
                {
                    gcAddMObjForDObj(dobj, mobjsub);

                    mobjsubs++;

                    mobjsub = *mobjsubs;
                }
            }
            p_mobjsubs++;
#endif
#ifdef PORT
            p_mobjsubs = (MObjSub***)(void*)(((u32*)p_mobjsubs) + 1);
#endif
        }
        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcSetDObjTransformsForGObj(GObj *gobj, DObjDesc *dobjdesc)
{
    DObj *dobj = DObjGetStruct(gobj);

    while ((dobj != NULL) && (dobjdesc->id != DOBJ_ARRAY_MAX))
    {
        dobj->translate.vec.f = dobjdesc->translate;
        dobj->rotate.vec.f = dobjdesc->rotate;
        dobj->scale.vec.f = dobjdesc->scale;

        dobjdesc++;

        dobj = gcGetTreeDObjNext(dobj);
    }
}

void gcAddCObjCamAnimJoint(CObj *cobj, AObjEvent32 *camanim_joint, f32 anim_frame)
{
    AObj *aobj = cobj->aobj;

    while (aobj != NULL)
    {
        aobj->kind = nGCAnimKindNone;
        aobj = aobj->next;
    }
    cobj->camanim_joint.event32 = camanim_joint;
    
    cobj->anim_wait = AOBJ_ANIM_CHANGED;
    cobj->anim_frame = anim_frame;
}

void gcParseCObjCamAnimJoint(CObj *cobj)
{
    AObj *track_aobjs[nGCAnimTrackCameraEnd - nGCAnimTrackCameraStart + 1];
    AObj *aobj;
    s32 i;
    u32 command_kind;
    u32 flags;
    f32 payload;

    if (cobj->anim_wait != AOBJ_ANIM_NULL)
    {
        if (cobj->anim_wait == AOBJ_ANIM_CHANGED)
        {
            cobj->anim_wait = -cobj->anim_frame;
        }
        else
        {
            cobj->anim_wait -= cobj->anim_speed;
            cobj->anim_frame += cobj->anim_speed;
            cobj->parent_gobj->anim_frame = cobj->anim_frame;

            if (cobj->anim_wait > 0.0F)
            {
                return;
            }
        }
        for (i = 0; i < ARRAY_COUNT(track_aobjs); i++)
        {
            track_aobjs[i] = NULL;
        }
        aobj = cobj->aobj;

        while (aobj != NULL)
        {
            if ((aobj->track >= nGCAnimTrackCameraStart) && (aobj->track <= nGCAnimTrackCameraEnd))
            {
                track_aobjs[aobj->track - nGCAnimTrackCameraStart] = aobj;
            }
            aobj = aobj->next;
        }
        do
        {
            if (cobj->camanim_joint.event32 == NULL)
            {
                aobj = cobj->aobj;

                while (aobj != NULL)
                {
                    if (aobj->kind)
                    {
                        aobj->length += cobj->anim_speed + cobj->anim_wait;
                    }
                    aobj = aobj->next;
                }
                cobj->anim_frame = cobj->anim_wait;
                cobj->anim_wait = AOBJ_ANIM_END;

                return;
            }
            command_kind = cobj->camanim_joint.event32->command.opcode;

            switch (command_kind)
            {
            case nGCAnimEvent32SetVal0RateBlock:
            case nGCAnimEvent32SetVal0Rate:
                payload = cobj->camanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(cobj->camanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForCamera(cobj, i + nGCAnimTrackCameraStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = cobj->camanim_joint.event32->f;

                        AObjAnimAdvance(cobj->camanim_joint.event32);

                        track_aobjs[i]->rate_base = track_aobjs[i]->rate_target;
                        track_aobjs[i]->rate_target = 0.0F;
                        track_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        track_aobjs[i]->length = -cobj->anim_wait - cobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetVal0RateBlock)
                {
                    cobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetValBlock:
            case nGCAnimEvent32SetVal:
                payload = cobj->camanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(cobj->camanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForCamera(cobj, i + nGCAnimTrackCameraStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = cobj->camanim_joint.event32->f;

                        AObjAnimAdvance(cobj->camanim_joint.event32);

                        track_aobjs[i]->kind = nGCAnimKindLinear;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->rate_base = (track_aobjs[i]->value_target - track_aobjs[i]->value_base) / payload;
                        }
                        track_aobjs[i]->length = -cobj->anim_wait - cobj->anim_speed;
                        track_aobjs[i]->rate_target = 0.0F;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValBlock)
                {
                    cobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetValRateBlock:
            case nGCAnimEvent32SetValRate:
                payload = cobj->camanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(cobj->camanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForCamera(cobj, i + nGCAnimTrackCameraStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = cobj->camanim_joint.event32->f;

                        AObjAnimAdvance(cobj->camanim_joint.event32);

                        track_aobjs[i]->rate_base = track_aobjs[i]->rate_target;
                        track_aobjs[i]->rate_target = cobj->camanim_joint.event32->f;

                        AObjAnimAdvance(cobj->camanim_joint.event32);

                        track_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        track_aobjs[i]->length = -cobj->anim_wait - cobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValRateBlock)
                {
                    cobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetTargetRate:
                flags = AObjAnimAdvance(cobj->camanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForCamera(cobj, i + nGCAnimTrackCameraStart);
                        }
                        track_aobjs[i]->rate_target = cobj->camanim_joint.event32->f;

                        AObjAnimAdvance(cobj->camanim_joint.event32);
                    }
                }
                break;

            case nGCAnimEvent32Wait:
                cobj->anim_wait += AObjAnimAdvance(cobj->camanim_joint.event32)->command.payload;
                break;

            case nGCAnimEvent32SetValAfterBlock:
            case nGCAnimEvent32SetValAfter:
                payload = cobj->camanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(cobj->camanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForCamera(cobj, i + nGCAnimTrackCameraStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = cobj->camanim_joint.event32->f;

                        AObjAnimAdvance(cobj->camanim_joint.event32);

                        track_aobjs[i]->kind = nGCAnimKindStep;
                        track_aobjs[i]->length_invert = payload;
                        track_aobjs[i]->length = -cobj->anim_wait - cobj->anim_speed;
                        track_aobjs[i]->rate_target = 0.0F;
                    }
                }
                if (command_kind == nGCAnimEvent32SetValAfterBlock)
                {
                    cobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent32SetAnim:
                AObjAnimAdvance(cobj->camanim_joint.event32);
                cobj->camanim_joint.event32 = PORT_RESOLVE(cobj->camanim_joint.event32->p);
                cobj->anim_frame = -cobj->anim_wait;
                cobj->parent_gobj->anim_frame = -cobj->anim_wait;
                break;

            case nGCAnimEvent32Jump:
                AObjAnimAdvance(cobj->camanim_joint.event32);
                cobj->camanim_joint.event32 = PORT_RESOLVE(cobj->camanim_joint.event32->p);
                break;

            case ANIM_CMD_12:
                payload = cobj->camanim_joint.event32->command.payload;
                flags = AObjAnimAdvance(cobj->camanim_joint.event32)->command.flags;

                for (i = 0; i < ARRAY_COUNT(track_aobjs); i++, flags = flags >> 1)
                {
                    if (!(flags))
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        if (track_aobjs[i] == NULL)
                        {
                            track_aobjs[i] = gcAddAObjForCamera(cobj, i + nGCAnimTrackCameraStart);
                        }
                        track_aobjs[i]->length += payload;
                    }
                }
                break;

            case nGCAnimEvent32SetInterp:
                flags = AObjAnimAdvance(cobj->camanim_joint.event32)->command.flags;

                if (flags & 0x08)
                {
                    if (track_aobjs[nGCAnimTrackEyeI - nGCAnimTrackCameraStart] == NULL)
                    {
                        track_aobjs[nGCAnimTrackEyeI - nGCAnimTrackCameraStart] = gcAddAObjForCamera
                        (
                            cobj, 
                            nGCAnimTrackEyeI
                        );
                    }
                    track_aobjs[nGCAnimTrackEyeI - nGCAnimTrackCameraStart]->interpolate = PORT_RESOLVE(cobj->camanim_joint.event32->p);

                    AObjAnimAdvance(cobj->camanim_joint.event32);
                }
                if (flags & 0x80)
                {
                    if (track_aobjs[nGCAnimTrackAtI - nGCAnimTrackCameraStart] == NULL)
                    {
                        track_aobjs[nGCAnimTrackAtI - nGCAnimTrackCameraStart] = gcAddAObjForCamera
                        (
                            cobj, 
                            nGCAnimTrackAtI
                        );
                    }
                    track_aobjs[nGCAnimTrackAtI - nGCAnimTrackCameraStart]->interpolate = PORT_RESOLVE(cobj->camanim_joint.event32->p);

                    AObjAnimAdvance(cobj->camanim_joint.event32);
                }
                break;

            case nGCAnimEvent32End:
                aobj = cobj->aobj;

                while (aobj != NULL)
                {
                    if (aobj->kind != nGCAnimKindNone)
                    {
                        aobj->length += cobj->anim_speed + cobj->anim_wait;
                    }
                    aobj = aobj->next;
                }
                cobj->anim_frame = cobj->anim_wait;
                cobj->anim_wait = AOBJ_ANIM_END;
                return;

            case ANIM_CMD_23:
                cobj->anim_wait += AObjAnimAdvance(cobj->camanim_joint.event32)->command.payload;
                cobj->camanim_joint.event32 += 2;
                break;

                // empty, but necessary
            default:
                break;
            }
        }
        while (cobj->anim_wait <= 0.0F);
    }
}

void gcPlayCObjCamAnim(CObj *cobj)
{
    if (cobj->anim_wait != AOBJ_ANIM_NULL)
    {
        AObj *aobj = cobj->aobj;
        f32 value;

        while (aobj != NULL)
        {
            if (aobj->kind != nGCAnimKindNone)
            {
                if (cobj->anim_wait != AOBJ_ANIM_END)
                {
                    aobj->length += cobj->anim_speed;
                }
                if (!(cobj->parent_gobj->flags & GOBJ_FLAG_NOANIM))
                {
                    switch (aobj->track)
                    {
                    case nGCAnimTrackEyeX:
                        cobj->vec.eye.x = gcGetAObjValue(aobj);
                        break;

                    case nGCAnimTrackEyeY:
                        cobj->vec.eye.y = gcGetAObjValue(aobj);
                        break;

                    case nGCAnimTrackEyeZ:
                        cobj->vec.eye.z = gcGetAObjValue(aobj);
                        break;

                    case nGCAnimTrackEyeI:
                        value = gcGetAObjValue(aobj);

                        if (value < 0.0F)
                        {
                            value = 0.0F;
                        }
                        else if (value > 1.0F)
                        {
                            value = 1.0F;
                        }
                        syInterpCubic(&cobj->vec.eye, aobj->interpolate, value);
                        break;
                    
                    case nGCAnimTrackAtX:
                        cobj->vec.at.x = gcGetAObjValue(aobj);
                        break;

                    case nGCAnimTrackAtY:
                        cobj->vec.at.y = gcGetAObjValue(aobj);
                        break;

                    case nGCAnimTrackAtZ:
                        cobj->vec.at.z = gcGetAObjValue(aobj);
                        break;

                    case nGCAnimTrackAtI:
                        value = gcGetAObjValue(aobj);

                        if (value < 0.0F)
                        {
                            value = 0.0F;
                        }
                        else if (value > 1.0F)
                        {
                            value = 1.0F;
                        }
                        syInterpCubic(&cobj->vec.at, aobj->interpolate, value);
                        break;

                    case nGCAnimTrackUpX:
                        cobj->vec.up.x = gcGetAObjValue(aobj);
                        break;

                    case nGCAnimTrackFovY:
                        cobj->projection.persp.fovy = gcGetAObjValue(aobj);
                        break;
                    }
                }
            }
            aobj = aobj->next;
        }
        if (cobj->anim_wait == AOBJ_ANIM_END)
        {
            cobj->anim_wait = AOBJ_ANIM_NULL;
        }
    }
}

void gcPlayCamAnim(GObj *gobj)
{
    CObj *cobj = CObjGetStruct(gobj);

    gcParseCObjCamAnimJoint(cobj);
    gcPlayCObjCamAnim(cobj);
}

s32 gcGetAnimTotalLength(AObjEvent32 **anim_joints)
{
    AObjEvent32 *anim_joint;
    u32 flags;
    s32 total = 0;
    s32 i;

#ifdef PORT
    while (*(u32*)anim_joints == 0)
    {
        anim_joints = (AObjEvent32**)(void*)(((u32*)anim_joints) + 1);
    }
    anim_joint = (AObjEvent32*)PORT_RESOLVE(*(u32*)anim_joints);
#else
    while (*anim_joints == NULL)
    {
        anim_joints++;
    }
    anim_joint = *anim_joints;
#endif

    while (TRUE)
    {
        switch (anim_joint->command.opcode)
        {
            case nGCAnimEvent32SetValBlock:
            case nGCAnimEvent32SetVal0RateBlock:
            case nGCAnimEvent32SetValAfterBlock:
                total += anim_joint->command.payload;

                /* fallthrough */

            case nGCAnimEvent32SetVal:
            case nGCAnimEvent32SetTargetRate:
            case nGCAnimEvent32SetVal0Rate:
            case nGCAnimEvent32SetValAfter:
                flags = AObjAnimAdvance(anim_joint)->command.flags;

                for (i = 0; i < 10; i++, flags = flags >> 1)
                {
                    if (flags == 0)
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        AObjAnimAdvance(anim_joint);
                    }
                }
                break;

            case nGCAnimEvent32SetValRateBlock:
                total += anim_joint->command.payload;

                /* fallthrough */

            case nGCAnimEvent32SetValRate:
                flags = AObjAnimAdvance(anim_joint)->command.flags;

                for (i = 0; i < 10; i++, flags = flags >> 1)
                {
                    if (flags == 0)
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        anim_joint += 2;
                    }
                }
                break;

            case nGCAnimEvent32Wait:
            case nGCAnimEvent32SetFlags:
                total += AObjAnimAdvance(anim_joint)->command.payload;
                break;

            case ANIM_CMD_12:
                AObjAnimAdvance(anim_joint);
                break;

            case nGCAnimEvent32SetInterp:
                anim_joint += 2;
                break;

            case ANIM_CMD_17:
                total += anim_joint->command.payload;
                flags = AObjAnimAdvance(anim_joint)->command.flags;

                for (i = 4; i < 14; i++, flags = flags >> 1)
                {
                    if (flags == 0)
                    {
                        break;
                    }
                    if (flags & 1)
                    {
                        AObjAnimAdvance(anim_joint);
                    }
                }
                break;

            case nGCAnimEvent32End:
                return total;

            case nGCAnimEvent32Jump:
            case nGCAnimEvent32SetAnim:
                return -total;

            case ANIM_CMD_16:
                break;
        }
    }
}
