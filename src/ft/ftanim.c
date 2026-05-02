#include <ft/fighter.h>
#include <sys/objanim.h>

#ifdef PORT
extern void port_log(const char *fmt, ...);
#endif

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800EC160
f32 ftAnimGetTargetValue(s16 arg, s32 track, sb32 value_or_step)
{
    f32 unused;
    s32 id;
    f32 ret;

    // 0x8012B910
    f32 fracs[/* */] =
    {
        1.0F / 512.0F,
        1.0F / 4.0F,
        1.0F / 4096.0F,
        1.0F / 16384.0F - (3.0F / 1000000000000.0F), // why tho
        1.0F / 512.0F,
        1.0F / 32.0F,
        1.0F / 8192.0F,
        1.0F / 16384.0F - (3.0F / 1000000000000.0F)  // ???
    };

    switch (track)
    {
    case nGCAnimTrackRotX:
    case nGCAnimTrackRotY:
    case nGCAnimTrackRotZ:
        id = 0;
        break;

    case nGCAnimTrackTraX:
    case nGCAnimTrackTraY:
    case nGCAnimTrackTraZ:
        id = 1;
        break;

    case nGCAnimTrackScaX:
    case nGCAnimTrackScaY:
    case nGCAnimTrackScaZ:
        id = 2;
        break;

    case nGCAnimTrackTraI:
        id = 3;
        break;
    }
    if (value_or_step != 0)
    {
        id += 4;
    }
    ret = arg;

    ret *= fracs[id];

    return ret;
}

// 0x800EC238
void ftAnimParseDObjFigatree(DObj *root_dobj)
{
    AObj *track_aobjs[nGCAnimTrackJointEnd - nGCAnimTrackJointStart + 1];
    AObj *current_aobj;
    f32 payload;
    s32 i;
    u16 command_kind;
    u16 flags;
#ifdef PORT
    s32 watchdog = 0;
    static s32 sFTAnimTranslateInterpLogCount = 0;
    static s32 sFTAnimTraICommandLogCount = 0;
#endif

    if (root_dobj->anim_wait != AOBJ_ANIM_NULL)
    {
#ifdef PORT
        if (root_dobj->parent_gobj == NULL)
        {
            root_dobj->anim_wait = AOBJ_ANIM_NULL;
            return;
        }
#endif
        if (root_dobj->anim_wait == AOBJ_ANIM_CHANGED)
        {
            root_dobj->anim_wait = -root_dobj->anim_frame;
        }
        else
        {
            root_dobj->anim_wait -= root_dobj->anim_speed;
            root_dobj->anim_frame += root_dobj->anim_speed;
            root_dobj->parent_gobj->anim_frame = root_dobj->anim_frame;

            if (root_dobj->anim_wait > 0.0F)
            {
                return;
            }
        }
        for (i = 0; i < ARRAY_COUNT(track_aobjs); i++)
        {
            track_aobjs[i] = NULL;
        }
        current_aobj = root_dobj->aobj;

        while (current_aobj != NULL)
        {
            if ((current_aobj->track >= nGCAnimTrackJointStart) && (current_aobj->track <= ARRAY_COUNT(track_aobjs)))
            {
                track_aobjs[current_aobj->track - nGCAnimTrackJointStart] = current_aobj;
            }
            current_aobj = current_aobj->next;
        }
        do
        {
            if (root_dobj->anim_joint.event16 == NULL)
            {
                current_aobj = root_dobj->aobj;

                while (current_aobj != NULL)
                {
                    if (current_aobj->kind != nGCAnimKindNone)
                    {
                        current_aobj->length += root_dobj->anim_speed + root_dobj->anim_wait;
                    }
                    current_aobj = current_aobj->next;
                }
                root_dobj->anim_frame = root_dobj->anim_wait;
                root_dobj->parent_gobj->anim_frame = root_dobj->anim_wait;
                root_dobj->anim_wait = AOBJ_ANIM_END;

                return;
            }
            command_kind = root_dobj->anim_joint.event16->command.opcode;
#ifdef PORT
            if (++watchdog >= 256)
            {
                u16 *raw16 = (u16*)root_dobj->anim_joint.event16;
                u32 raw32 = *(u32*)root_dobj->anim_joint.event16;

                port_log("SSB64: ftAnimParseDObjFigatree - watchdog dobj=%p event16=%p opcode=%u wait=%f frame=%f speed=%f flags=0x%x raw16=[0x%04x 0x%04x] raw32=0x%08x\n",
                    root_dobj, root_dobj->anim_joint.event16, command_kind, root_dobj->anim_wait, root_dobj->anim_frame, root_dobj->anim_speed, root_dobj->flags,
                    raw16[0], raw16[1], raw32);
                root_dobj->anim_wait = AOBJ_ANIM_END;
                return;
            }
#endif

            switch (command_kind)
            {
            case nGCAnimEvent16SetVal0RateBlock:
            case nGCAnimEvent16SetVal0Rate:
                flags = root_dobj->anim_joint.event16->command.flags;
#ifdef PORT
                if ((flags & (1 << (nGCAnimTrackTraI - nGCAnimTrackJointStart))) && (sFTAnimTraICommandLogCount < 32))
                {
                    u16 *raw16 = (u16*)root_dobj->anim_joint.event16;

                    sFTAnimTraICommandLogCount++;
                    port_log("SSB64: ftAnim TraI command - dobj=%p cmd=%u flags=0x%x event16=%p raw16=[0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x]\n",
                        root_dobj, command_kind, flags, root_dobj->anim_joint.event16, raw16[0], raw16[1], raw16[2], raw16[3], raw16[4], raw16[5], raw16[6], raw16[7]);
                }
#endif
                payload = (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle) ? AObjAnimAdvance(root_dobj->anim_joint.event16)->u : 0.0F;

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
                            track_aobjs[i] = gcAddAObjForDObj(root_dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = ftAnimGetTargetValue(AObjAnimAdvance(root_dobj->anim_joint.event16)->s, i + nGCAnimTrackJointStart, 0);

                        track_aobjs[i]->rate_base = track_aobjs[i]->rate_target;
                        track_aobjs[i]->rate_target = 0.0F;

                        track_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        track_aobjs[i]->length = -root_dobj->anim_wait - root_dobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent16SetVal0RateBlock)
                {
                    root_dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent16SetValBlock:
            case nGCAnimEvent16SetVal:
                flags = root_dobj->anim_joint.event16->command.flags;
#ifdef PORT
                if ((flags & (1 << (nGCAnimTrackTraI - nGCAnimTrackJointStart))) && (sFTAnimTraICommandLogCount < 32))
                {
                    u16 *raw16 = (u16*)root_dobj->anim_joint.event16;

                    sFTAnimTraICommandLogCount++;
                    port_log("SSB64: ftAnim TraI command - dobj=%p cmd=%u flags=0x%x event16=%p raw16=[0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x]\n",
                        root_dobj, command_kind, flags, root_dobj->anim_joint.event16, raw16[0], raw16[1], raw16[2], raw16[3], raw16[4], raw16[5], raw16[6], raw16[7]);
                }
#endif
                payload = (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle) ? AObjAnimAdvance(root_dobj->anim_joint.event16)->u : 0.0F;

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
                            track_aobjs[i] = gcAddAObjForDObj(root_dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = ftAnimGetTargetValue(AObjAnimAdvance(root_dobj->anim_joint.event16)->s, i + nGCAnimTrackJointStart, 0);

                        track_aobjs[i]->kind = nGCAnimKindLinear;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->rate_base = (track_aobjs[i]->value_target - track_aobjs[i]->value_base) / payload;
                        }
                        track_aobjs[i]->length = -root_dobj->anim_wait - root_dobj->anim_speed;
                        track_aobjs[i]->rate_target = 0.0F;
                    }
                }
                if (command_kind == nGCAnimEvent16SetValBlock)
                {
                    root_dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent16SetValRateBlock:
            case nGCAnimEvent16SetValRate:
                flags = root_dobj->anim_joint.event16->command.flags;
#ifdef PORT
                if ((flags & (1 << (nGCAnimTrackTraI - nGCAnimTrackJointStart))) && (sFTAnimTraICommandLogCount < 32))
                {
                    u16 *raw16 = (u16*)root_dobj->anim_joint.event16;

                    sFTAnimTraICommandLogCount++;
                    port_log("SSB64: ftAnim TraI command - dobj=%p cmd=%u flags=0x%x event16=%p raw16=[0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x]\n",
                        root_dobj, command_kind, flags, root_dobj->anim_joint.event16, raw16[0], raw16[1], raw16[2], raw16[3], raw16[4], raw16[5], raw16[6], raw16[7]);
                }
#endif
                payload = (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle) ? AObjAnimAdvance(root_dobj->anim_joint.event16)->u : 0.0F;

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
                            track_aobjs[i] = gcAddAObjForDObj(root_dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = ftAnimGetTargetValue(AObjAnimAdvance(root_dobj->anim_joint.event16)->s, i + nGCAnimTrackJointStart, 0);

                        track_aobjs[i]->rate_base = track_aobjs[i]->rate_target;
                        track_aobjs[i]->rate_target = ftAnimGetTargetValue(AObjAnimAdvance(root_dobj->anim_joint.event16)->s, i + nGCAnimTrackJointStart, 1);

                        track_aobjs[i]->kind = nGCAnimKindCubic;

                        if (payload != 0.0F)
                        {
                            track_aobjs[i]->length_invert = 1.0F / payload;
                        }
                        track_aobjs[i]->length = -root_dobj->anim_wait - root_dobj->anim_speed;
                    }
                }
                if (command_kind == nGCAnimEvent16SetValRateBlock)
                {
                    root_dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent16SetTargetRate:
                flags = root_dobj->anim_joint.event16->command.flags;
#ifdef PORT
                if ((flags & (1 << (nGCAnimTrackTraI - nGCAnimTrackJointStart))) && (sFTAnimTraICommandLogCount < 32))
                {
                    u16 *raw16 = (u16*)root_dobj->anim_joint.event16;

                    sFTAnimTraICommandLogCount++;
                    port_log("SSB64: ftAnim TraI command - dobj=%p cmd=%u flags=0x%x event16=%p raw16=[0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x]\n",
                        root_dobj, command_kind, flags, root_dobj->anim_joint.event16, raw16[0], raw16[1], raw16[2], raw16[3], raw16[4], raw16[5], raw16[6], raw16[7]);
                }
#endif

                payload = (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle) ? AObjAnimAdvance(root_dobj->anim_joint.event16)->u : 0.0F;

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
                            track_aobjs[i] = gcAddAObjForDObj(root_dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->rate_target = ftAnimGetTargetValue(AObjAnimAdvance(root_dobj->anim_joint.event16)->s, i + nGCAnimTrackJointStart, 1);
                    }
                }
                break;

            case nGCAnimEvent16Block:
                if (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle)
                {
                    root_dobj->anim_wait += AObjAnimAdvance(root_dobj->anim_joint.event16)->u;
                }
                break;

            case nGCAnimEvent16SetValAfterBlock:
            case nGCAnimEvent16SetValAfter:
                flags = root_dobj->anim_joint.event16->command.flags;
#ifdef PORT
                if ((flags & (1 << (nGCAnimTrackTraI - nGCAnimTrackJointStart))) && (sFTAnimTraICommandLogCount < 32))
                {
                    u16 *raw16 = (u16*)root_dobj->anim_joint.event16;

                    sFTAnimTraICommandLogCount++;
                    port_log("SSB64: ftAnim TraI command - dobj=%p cmd=%u flags=0x%x event16=%p raw16=[0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x]\n",
                        root_dobj, command_kind, flags, root_dobj->anim_joint.event16, raw16[0], raw16[1], raw16[2], raw16[3], raw16[4], raw16[5], raw16[6], raw16[7]);
                }
#endif
                payload = (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle) ? AObjAnimAdvance(root_dobj->anim_joint.event16)->u : 0.0F;

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
                            track_aobjs[i] = gcAddAObjForDObj(root_dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->value_base = track_aobjs[i]->value_target;
                        track_aobjs[i]->value_target = ftAnimGetTargetValue(AObjAnimAdvance(root_dobj->anim_joint.event16)->s, i + nGCAnimTrackJointStart, 0);

                        track_aobjs[i]->kind = nGCAnimKindStep;

                        track_aobjs[i]->length_invert = payload;
                        track_aobjs[i]->length = -root_dobj->anim_wait - root_dobj->anim_speed;

                        track_aobjs[i]->rate_target = 0.0F;
                    }
                }
                if (command_kind == nGCAnimEvent16SetValAfterBlock)
                {
                    root_dobj->anim_wait += payload;
                }
                break;

            case nGCAnimEvent16Loop:
                AObjAnimAdvance(root_dobj->anim_joint.event16);

                root_dobj->anim_joint.event16 += root_dobj->anim_joint.event16->s / 2;

                root_dobj->anim_frame = -root_dobj->anim_wait;
                root_dobj->parent_gobj->anim_frame = -root_dobj->anim_wait;

                if (root_dobj->is_anim_root != FALSE)
                {
                    if (root_dobj->parent_gobj->func_anim != NULL)
                    {
                        root_dobj->parent_gobj->func_anim(root_dobj, -2, 0);
                    }
                }
                break;

            case nGCAnimEvent1611:
                flags = root_dobj->anim_joint.event16->command.flags;
#ifdef PORT
                if ((flags & (1 << (nGCAnimTrackTraI - nGCAnimTrackJointStart))) && (sFTAnimTraICommandLogCount < 32))
                {
                    u16 *raw16 = (u16*)root_dobj->anim_joint.event16;

                    sFTAnimTraICommandLogCount++;
                    port_log("SSB64: ftAnim TraI command - dobj=%p cmd=%u flags=0x%x event16=%p raw16=[0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x]\n",
                        root_dobj, command_kind, flags, root_dobj->anim_joint.event16, raw16[0], raw16[1], raw16[2], raw16[3], raw16[4], raw16[5], raw16[6], raw16[7]);
                }
#endif

                payload = (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle) ? AObjAnimAdvance(root_dobj->anim_joint.event16)->u : 0.0F;

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
                            track_aobjs[i] = gcAddAObjForDObj(root_dobj, i + nGCAnimTrackJointStart);
                        }
                        track_aobjs[i]->length += payload;
                    }
                }
                break;

            case nGCAnimEvent16SetTranslateInterp:
                AObjAnimAdvance(root_dobj->anim_joint.event16);

                if (track_aobjs[nGCAnimTrackTraI - nGCAnimTrackJointStart] == NULL)
                {
                    track_aobjs[nGCAnimTrackTraI - nGCAnimTrackJointStart] = gcAddAObjForDObj(root_dobj, nGCAnimTrackTraI);
                }
#ifdef PORT
                {
                    void *interp = root_dobj->anim_joint.event16 + (root_dobj->anim_joint.event16->s / 2);

                    track_aobjs[nGCAnimTrackTraI - nGCAnimTrackJointStart]->interpolate = interp;

                    if (sFTAnimTranslateInterpLogCount < 16)
                    {
                        sFTAnimTranslateInterpLogCount++;

                        if (interp != NULL)
                        {
                            u32 *interp_words = (u32*)interp;

                            port_log("SSB64: ftAnim SetTranslateInterp - dobj=%p rel=%d interp=%p words=[0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x]\n",
                                root_dobj, root_dobj->anim_joint.event16->s, interp,
                                interp_words[0], interp_words[1], interp_words[2], interp_words[3], interp_words[4], interp_words[5]);
                        }
                        else port_log("SSB64: ftAnim SetTranslateInterp - dobj=%p rel=%d interp=NULL\n",
                            root_dobj, root_dobj->anim_joint.event16->s);
                    }
                }

                AObjAnimAdvance(root_dobj->anim_joint.event16);
#else
                track_aobjs[nGCAnimTrackTraI - nGCAnimTrackJointStart]->interpolate = root_dobj->anim_joint.event16 + (root_dobj->anim_joint.event16->s / 2);

                AObjAnimAdvance(root_dobj->anim_joint.event16);
#endif
                break;

            case nGCAnimEvent32End:
                current_aobj = root_dobj->aobj;

                while (current_aobj != NULL)
                {
                    if (current_aobj->kind != nGCAnimKindNone)
                    {
                        current_aobj->length += root_dobj->anim_speed + root_dobj->anim_wait;
                    }
                    current_aobj = current_aobj->next;
                }
                root_dobj->anim_frame = root_dobj->anim_wait;
                root_dobj->parent_gobj->anim_frame = root_dobj->anim_wait;
                root_dobj->anim_wait = AOBJ_ANIM_END;

                if (root_dobj->is_anim_root != FALSE)
                {
                    if (root_dobj->parent_gobj->func_anim != NULL)
                    {
                        root_dobj->parent_gobj->func_anim(root_dobj, -1, 0);
                    }
                }
                return;

            case nGCAnimEvent16SetFlags:
                root_dobj->flags = root_dobj->anim_joint.event16->command.flags;

                if (AObjAnimAdvance(root_dobj->anim_joint.event16)->command.toggle)
                {
                    root_dobj->anim_wait += AObjAnimAdvance(root_dobj->anim_joint.event16)->u;
                }
                break;

            default:
                break;
            }
        } 
        while (root_dobj->anim_wait <= 0.0F);
    }
}

// 0x800ECCA4 - Unused?
void func_ovl2_800ECCA4(GObj *gobj)
{
    DObj *main_dobj = DObjGetStruct(gobj);
    MObj *mobj;

    while (main_dobj != NULL)
    {
        ftAnimParseDObjFigatree(main_dobj);
        gcPlayDObjAnimJoint(main_dobj);

        mobj = main_dobj->mobj;

        while (mobj != NULL)
        {
            gcParseMObjMatAnimJoint(mobj);
            gcPlayMObjMatAnim(mobj);

            mobj = mobj->next;
        }
        if (main_dobj->child != NULL)
        {
            main_dobj = main_dobj->child;
        }
        else if (main_dobj->sib_next != NULL)
        {
            main_dobj = main_dobj->sib_next;
        }
        else while (TRUE)
        {
            if (main_dobj->parent == DOBJ_PARENT_NULL)
            {
                main_dobj = NULL;

                break;
            }
            else if (main_dobj->parent->sib_next != NULL)
            {
                main_dobj = main_dobj->parent->sib_next;

                break;
            }
            else main_dobj = main_dobj->parent;
        }
    }
}
