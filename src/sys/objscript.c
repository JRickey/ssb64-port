#include <sys/obj.h>
#include <stddef.h>

s32 gcSetupGObjScript(GObj *this_gobj, s32 id, GObj *next_gobj)
{
    if (this_gobj == NULL)
    {
        this_gobj = gGCCurrentCommon;
    }
    if (next_gobj == NULL)
    {
        next_gobj = gGCCurrentCommon;
    }
    if (this_gobj->gobjscripts_num == ARRAY_COUNT(this_gobj->gobjscripts))
    {
        return -1;
    }
    this_gobj->gobjscripts[this_gobj->gobjscripts_num].id = id;
    this_gobj->gobjscripts[this_gobj->gobjscripts_num].next_gobj = next_gobj;
    
    this_gobj->gobjscripts_num++;

    return 0;
}

GObj* gcAddGObjScript(GObj *gobj, GObjScript *gobjscript)
{
    return gcSetupGObjScript(gobj, gobjscript->id, gobjscript->next_gobj);
}

void gcAddGObjScriptByLink(s32 link, s32 id, GObj *gobj)
{
    GObjScript gobjscript;
    
    gobjscript.next_gobj = (gobj != NULL) ? gobj : gGCCurrentCommon;
    gobjscript.id = id;

    gcFuncGObjByLinkEx(link, gcAddGObjScript, &gobjscript, FALSE);
}

sb32 gcParseGObjScript(void (*func)(GObjScript))
{
    GObj *gobj = gGCCurrentCommon;

#if defined (AVOID_UB) || defined (NON_MATCHING)
    GObjScript *gobjscript = gobj->gobjscripts;

#else 
    GObjScript *gobjscript = gobj = gobj->gobjscripts;

#endif
    s32 i;

    /* FAKE MATCH: gobj is transformed into the same pointer as gobjscript (base + 0x48), 
     * but then it reads the len at gobjscript + 0x28 (aka base + 0x70) for the loop;
     * match involves hacks, please use the solution inside the AVOID_UB guard for real applications.
     */
#if defined (AVOID_UB) || defined (NON_MATCHING)
    for (i = 0; i < gobj->gobjscripts_num; i++)

#else
    for (i = 0; i < *(s32*)((uintptr_t)gobj + (offsetof(GObj, gobjscripts_num) - offsetof(GObj, gobjscripts))); i++)

#endif
    {
        switch (gobjscript[i].id + ARRAY_COUNT(gobj->gobjscripts)) 
        {
        case nGCCommonEventEnd:
            gcEjectGObj(NULL); 
            return TRUE;
            
        case nGCCommonEventPause:
            gcPauseGObjProcessAll(NULL);
            break;
            
        case nGCCommonEventResume:
            gcResumeGObjProcessAll(NULL);
            break;
            
        case nGCCommonEventHide:
            gGCCurrentCommon->flags |= GOBJ_FLAG_HIDDEN;
            break;
            
        case nGCCommonEventShow:
            gGCCurrentCommon->flags &= ~GOBJ_FLAG_HIDDEN;
            break;
            
        default:
            if (func != NULL) 
            {
                func(gobjscript[i]);
            }
            break;
        }
    }
#if defined (AVOID_UB) || defined (NON_MATCHING)
    gobj->gobjscripts_num = 0;

#else
    *(s32*)((uintptr_t)gobj + (offsetof(GObj, gobjscripts_num) - offsetof(GObj, gobjscripts))) = 0;

#endif
    return FALSE;
}
