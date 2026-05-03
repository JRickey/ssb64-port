#include <sys/obj.h>

#include <sys/taskman.h>
#include <sys/matrix.h>
#include <sys/video.h>

#include <config.h>
#include "libc/math.h"
#ifdef PORT
#include "port_log.h"
#include <stdbool.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <excpt.h>
#endif
#endif

/* These should no longer be required as they're included in obj.h
#include <macros.h>
#include <ssb_types.h>
#include <PR/mbi.h>
#include <PR/os.h>
#include <PR/sp.h>
#include <PR/ultratypes.h>
*/

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80046FA0 - gbi Mtx* ? pointer to some sort of matrix
Mtx *sGCMatrixProjectL;

// 0x80046FA4
f32 gGCScaleX; // Sprite scale / depth? Appears to overlap objects in its own DLLink, so maybe depth?

// 0x80046FA8
Mtx44f gGCMatrixPerspF;

// 0x80046FE8
Mtx44f sGCMatrixMvpF;

// 0x80047028
Mtx44f sGCMatrixMod1F;

// 0x80047068
Mtx44f sGCMatrixMod2F;

// 0x800470A8
s32 sGCCameraMatrixMode;

// 0x800470AC
syMtxProcess *sGCMatrixFuncList;

// 0x800470B0
Gfx *sGCCurrentDL;

// 0x800470B8 - not sure what this is, but the lists in this array are copied from 800470B0 if they are further ahead than it
Gfx *sGCForwardDLs[4];

// 0x800470C8
Gfx sGCLocalDLs[60];

// 0x800472A8
s32 sGCDetailLevel;

// 0x800472B0 - the first pointer in the set of four doesn't seem to be used too much
Gfx *sGCBufferDLs[4];

// 0x800472C0
Gfx *sGCCameraDL;

#ifdef PORT
static s32 sGCMObjResolveWarningCount;
static s32 sGCDLPointerWarningCount;
static s32 sGCMObjRenderDiagCount;

extern bool portRelocDescribePointer(const void *ptr, uintptr_t *out_base, size_t *out_size, u32 *out_file_id, const char **out_path);
extern char *getenv(const char *name);
extern int atoi(const char *nptr);
extern unsigned long strtoul(const char *nptr, char **endptr, int base);

static s32 gcRenderDiagLimit(void)
{
    const char *value = getenv("SSB64_RENDER_DIAG_LIMIT");
    s32 limit = (value != NULL && value[0] != '\0') ? atoi(value) : 400;

    return (limit > 0) ? limit : 400;
}

static bool gcRenderDiagParseUlongEnv(const char *name, unsigned long *out)
{
    const char *value = getenv(name);
    char *end = NULL;

    if (value == NULL || value[0] == '\0')
    {
        return FALSE;
    }
    *out = strtoul(value, &end, 0);

    return (end != value) ? TRUE : FALSE;
}

static bool gcRenderDiagFileIdMatches(u32 file_id, const char *list)
{
    const char *cursor;

    if (list == NULL || list[0] == '\0')
    {
        return FALSE;
    }
    cursor = list;
    while (*cursor != '\0')
    {
        char *end = NULL;
        unsigned long parsed = strtoul(cursor, &end, 0);

        if ((end != cursor) && ((u32)parsed == file_id))
        {
            return TRUE;
        }
        cursor = (end != cursor) ? end : cursor + 1;
        while ((*cursor == ',') || (*cursor == ' ') || (*cursor == ';') || (*cursor == ':'))
        {
            cursor++;
        }
    }
    return FALSE;
}

static bool gcRenderDiagPointerMatches(const void *ptr)
{
    uintptr_t base = 0;
    size_t size = 0;
    u32 file_id = 0;
    unsigned long min_off = 0;
    unsigned long max_off = 0;
    bool has_min_off;
    bool has_max_off;
    uintptr_t offset;

    if ((ptr == NULL) || !portRelocDescribePointer(ptr, &base, &size, &file_id, NULL))
    {
        return FALSE;
    }
    if (!gcRenderDiagFileIdMatches(file_id, getenv("SSB64_RENDER_DIAG_FILE_ID")))
    {
        return FALSE;
    }

    has_min_off = gcRenderDiagParseUlongEnv("SSB64_RENDER_DIAG_MIN_OFF", &min_off);
    has_max_off = gcRenderDiagParseUlongEnv("SSB64_RENDER_DIAG_MAX_OFF", &max_off);
    offset = (uintptr_t)ptr - base;

    if (has_min_off && offset < min_off)
    {
        return FALSE;
    }
    if (has_max_off && offset > max_off)
    {
        return FALSE;
    }
    return TRUE;
}

static void gcRenderDiagDescribePointer(const void *ptr, u32 *file_id, uintptr_t *offset, const char **path)
{
    uintptr_t base = 0;
    size_t size = 0;

    *file_id = 0;
    *offset = 0;
    *path = "(raw)";
    if ((ptr != NULL) && portRelocDescribePointer(ptr, &base, &size, file_id, path))
    {
        *offset = (uintptr_t)ptr - base;
    }
}

/* SSB64_RENDER_DIAG is a one-shot diagnostic, never toggled mid-run. Cache the
 * enabled state so the per-MObj fast path doesn't take the macOS getenv
 * unfair-lock + linear __environ walk on every call. With 4 fighters in a busy
 * scene that's hundreds of locked env walks per frame for an opt-out check
 * that nearly always returns "off". */
static bool gcRenderDiagEnabled(void)
{
    static int sCached = -1;
    if (sCached == -1)
    {
        const char *enabled = getenv("SSB64_RENDER_DIAG");
        sCached = (enabled != NULL && enabled[0] != '\0' && enabled[0] != '0') ? 1 : 0;
    }
    return sCached != 0;
}

static void gcRenderDiagLogMObj(DObj *dobj, MObj *mobj, u16 flags, void *current_sprite, u32 current_sprite_token,
                                void *next_sprite, u32 next_sprite_token, void *palette_data, u32 palette_token)
{
    bool matches;
    u32 cur_file;
    u32 next_file;
    u32 pal_file;
    u32 dl_file;
    uintptr_t cur_off;
    uintptr_t next_off;
    uintptr_t pal_off;
    uintptr_t dl_off;
    const char *cur_path;
    const char *next_path;
    const char *pal_path;
    const char *dl_path;

    if (!gcRenderDiagEnabled() || (sGCMObjRenderDiagCount >= gcRenderDiagLimit()))
    {
        return;
    }

    matches = gcRenderDiagPointerMatches(current_sprite) || gcRenderDiagPointerMatches(next_sprite) ||
              gcRenderDiagPointerMatches(palette_data) || gcRenderDiagPointerMatches(dobj->dl);
    if (!matches)
    {
        return;
    }

    gcRenderDiagDescribePointer(current_sprite, &cur_file, &cur_off, &cur_path);
    gcRenderDiagDescribePointer(next_sprite, &next_file, &next_off, &next_path);
    gcRenderDiagDescribePointer(palette_data, &pal_file, &pal_off, &pal_path);
    gcRenderDiagDescribePointer(dobj->dl, &dl_file, &dl_off, &dl_path);

    sGCMObjRenderDiagCount++;
    port_log(
        "SSB64_RENDER_DIAG mobj dobj=%p mobj=%p flags=0x%04x fmt=%u siz=%u block_fmt=%u block_siz=%u tex_curr=%d tex_next=%d "
        "cur=%p cur_token=0x%08x cur_file=%u cur_off=0x%lx cur_path=%s next=%p next_token=0x%08x next_file=%u next_off=0x%lx next_path=%s "
        "pal=%p pal_token=0x%08x pal_file=%u pal_off=0x%lx pal_path=%s dl=%p dl_file=%u dl_off=0x%lx dl_path=%s scau=%f scav=%f trau=%f trav=%f\n",
        dobj, mobj, flags, mobj->sub.fmt, mobj->sub.siz, mobj->sub.block_fmt, mobj->sub.block_siz, mobj->texture_id_curr,
        mobj->texture_id_next, current_sprite, current_sprite_token, cur_file, (unsigned long)cur_off, cur_path,
        next_sprite, next_sprite_token, next_file, (unsigned long)next_off, next_path, palette_data, palette_token,
        pal_file, (unsigned long)pal_off, pal_path, dobj->dl, dl_file, (unsigned long)dl_off, dl_path, mobj->sub.scau,
        mobj->sub.scav, mobj->sub.trau, mobj->sub.trav);
}

static void gcLogMObjResolveWarning(const char *issue, DObj *dobj, MObj *mobj, u32 array_token, void *array_ptr, s32 index, u32 value_token)
{
    if (sGCMObjResolveWarningCount < 64)
    {
        port_log
        (
            "SSB64: gcDrawMObjForDObj - %s dobj=%p dl=%p mobj=%p flags=0x%04x array_token=0x%08x array=%p index=%d value_token=0x%08x curr=%u next=%u lfrac=%f palette=%f fmt=%u siz=%u block_fmt=%u block_siz=%u unk10=%d\n",
            issue,
            dobj,
            dobj->dl,
            mobj,
            mobj->sub.flags,
            array_token,
            array_ptr,
            index,
            value_token,
            mobj->texture_id_curr,
            mobj->texture_id_next,
            mobj->lfrac,
            mobj->palette_id,
            mobj->sub.fmt,
            mobj->sub.siz,
            mobj->sub.block_fmt,
            mobj->sub.block_siz,
            mobj->sub.unk10
        );
    }
    sGCMObjResolveWarningCount++;
}

static sb32 gcTryReadTokenSlot(u32 *tokens, s32 index, u32 *out_token)
{
#ifdef _MSC_VER
    __try
    {
        *out_token = tokens[index];
        return TRUE;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        *out_token = 0;
        return FALSE;
    }
#else
    *out_token = tokens[index];
    return TRUE;
#endif
}

static void* gcTryResolveTokenArrayEntry(const char *issue, DObj *dobj, MObj *mobj, u32 array_token, u32 *tokens, s32 index, u32 *out_token)
{
    void *resolved;

    *out_token = 0;

    if (tokens == NULL)
    {
        gcLogMObjResolveWarning(issue, dobj, mobj, array_token, NULL, index, 0);
        return NULL;
    }
    if (gcTryReadTokenSlot(tokens, index, out_token) == FALSE)
    {
        gcLogMObjResolveWarning("token-read-av", dobj, mobj, array_token, tokens, index, 0);
        return NULL;
    }

    resolved = PORT_RESOLVE(*out_token);

    if (resolved == NULL)
    {
        gcLogMObjResolveWarning(issue, dobj, mobj, array_token, tokens, index, *out_token);
    }
    return resolved;
}

static void gcLogSuspiciousDLPointer(const char *issue, DObj *dobj, unsigned long long draw_dl_raw, s32 list_id, DObjDLLink *dl_link)
{
    void *draw_dl = (void*)draw_dl_raw;
    void *resolved_dl = NULL;

    if ((draw_dl == NULL) || (draw_dl_raw >= 0x10000ULL))
    {
        return;
    }
    resolved_dl = PORT_RESOLVE((u32)draw_dl_raw);
    if (sGCDLPointerWarningCount < 64)
    {
        port_log
        (
            "SSB64: gcDrawDObj - suspicious-dl issue=%s dobj=%p dobj_dl=%p draw_dl=%p resolved_dl=%p draw_dl_raw=0x%llx dl_link=%p list_id=%d flags=0x%04x mobj=%p child=%p sib=%p\n",
            issue,
            dobj,
            dobj->dl,
            draw_dl,
            resolved_dl,
            draw_dl_raw,
            dl_link,
            list_id,
            dobj->flags,
            dobj->mobj,
            dobj->child,
            dobj->sib_next
        );
    }
    sGCDLPointerWarningCount++;
}
#endif

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8003B930
s32 dGCCameraScissorTop = 10;

// 0x8003B934
s32 dGCCameraScissorBottom = 10;

// 0x8003B938
s32 dGCCameraScissorLeft = 10;

// 0x8003B93C
s32 dGCCameraScissorRight = 10;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// New file here?
void gcSetCameraScissor(s32 top, s32 bottom, s32 left, s32 right)
{
    dGCCameraScissorTop = top;
    dGCCameraScissorBottom = bottom;
    dGCCameraScissorLeft = left;
    dGCCameraScissorRight = right;
}

void gcSetMatrixFuncList(syMtxProcess *proc_mtx)
{
    sGCMatrixFuncList = proc_mtx;
}

void unref_80010740(void)
{
    return;
}

void func_80010748(Mtx *mtx_l, DObj *dobj, sb32 is_translate)
{
    Mtx44f mtx_f;
    f32 distx, disty, distz;

    CObj *cobj;
    f32 res;

    cobj = CObjGetStruct(gGCCurrentCamera);

    distx = dobj->translate.vec.f.x - cobj->vec.eye.x;
    disty = dobj->translate.vec.f.y - cobj->vec.eye.y;
    distz = dobj->translate.vec.f.z - cobj->vec.eye.z;

    res = 1.0F / sqrtf(SQUARE(distx) + SQUARE(disty) + SQUARE(distz));

    distx *= res;
    disty *= res;
    distz *= res;

    res = sqrtf(SQUARE(distx) + SQUARE(disty));

    mtx_f[0][3] = mtx_f[1][3] = mtx_f[2][3] = mtx_f[1][2] = 0.0F;
    mtx_f[3][3] = 1.0F;

    if (res != 0.0F)
    {
        f32 inv = (1.0F / res);

        mtx_f[2][2] = res;

        mtx_f[0][0] = -distx;

        mtx_f[1][0] = disty * inv;

        mtx_f[2][0] = -distx * distz * inv;
        mtx_f[0][1] = -disty;

        mtx_f[1][1] = -distx * inv;
        mtx_f[2][1] = -disty * distz * inv;

        mtx_f[0][2] = -distz;
    }
    else
    {
        mtx_f[1][0] = mtx_f[2][0] = mtx_f[0][1] = mtx_f[2][1] = mtx_f[0][2] = 0.0F;
        mtx_f[0][0] = mtx_f[1][1] = mtx_f[2][2] = 1.0F;
    }
    if (is_translate != FALSE)
    {
        mtx_f[3][0] = dobj->translate.vec.f.x;
        mtx_f[3][1] = dobj->translate.vec.f.y;
        mtx_f[3][2] = dobj->translate.vec.f.z;
    }
    else mtx_f[3][0] = mtx_f[3][1] = mtx_f[3][2] = 0.0F;

    syMatrixF2LFixedW(&mtx_f, mtx_l);
}

void func_80010918(Mtx *mtx_l, DObj *dobj, sb32 is_translate)
{
    Mtx44f mtx_f;
    f32 distx, disty, distz;

    CObj *cobj;
    f32 res;

    cobj = CObjGetStruct(gGCCurrentCamera);

    distx = dobj->translate.vec.f.x - cobj->vec.eye.x;
    disty = dobj->translate.vec.f.y - cobj->vec.eye.y;
    distz = dobj->translate.vec.f.z - cobj->vec.eye.z;

    res = 1.0F / sqrtf(SQUARE(distx) + SQUARE(disty) + SQUARE(distz));

    distx *= res;
    disty *= res;
    distz *= res;

    res = sqrtf(SQUARE(distx) + SQUARE(distz));

    mtx_f[0][3] = mtx_f[1][3] = mtx_f[2][3] = mtx_f[0][1] = 0.0F;
    mtx_f[3][3] = 1.0F;

    if (res != 0.0F)
    {
        f32 inv = (1.0F / res);

        mtx_f[0][0] = -distz * inv;
        mtx_f[1][0] = -disty * distx * inv;
        mtx_f[2][0] = -distx;

        mtx_f[1][1] = res;
        mtx_f[2][1] = -disty;

        mtx_f[0][2] = distx * inv;
        mtx_f[1][2] = -disty * distz * inv;
        mtx_f[2][2] = -distz;
    }
    else
    {
        mtx_f[1][0] = mtx_f[2][0] = mtx_f[2][1] = mtx_f[0][2] = mtx_f[1][2] = 0.0F;
        mtx_f[0][0] = mtx_f[1][1] = mtx_f[2][2] = 1.0F;
    }
    if (is_translate != FALSE)
    {
        mtx_f[3][0] = dobj->translate.vec.f.x;
        mtx_f[3][1] = dobj->translate.vec.f.y;
        mtx_f[3][2] = dobj->translate.vec.f.z;
    }
    else mtx_f[3][0] = mtx_f[3][1] = mtx_f[3][2] = 0.0F;

    syMatrixF2LFixedW(&mtx_f, mtx_l);
}

void func_80010AE8(Mtx *mtx_l, DObj *dobj, sb32 is_translate)
{
    Mtx44f mtx_f;
    f32 distx;
    f32 disty;

    CObj *cobj;
    f32 res;

    cobj = CObjGetStruct(gGCCurrentCamera);

    distx = dobj->translate.vec.f.x - cobj->vec.eye.x;
    disty = dobj->translate.vec.f.y - cobj->vec.eye.y;

    res = sqrtf(SQUARE(distx) + SQUARE(disty));

    mtx_f[0][3] = mtx_f[1][3] = mtx_f[2][3] = mtx_f[2][0] = mtx_f[2][1] = mtx_f[0][2] = mtx_f[1][2] = 0.0F;
    mtx_f[2][2] = mtx_f[3][3] = 1.0F;

    if (res != 0.0F)
    {
        f32 inv = 1.0F / res;

        distx *= inv;
        disty *= inv;

        mtx_f[0][0] = -distx;
        mtx_f[0][1] = -disty;
        mtx_f[1][0] = disty;
        mtx_f[1][1] = -distx;
    }
    else
    {
        mtx_f[1][0] = mtx_f[0][1] = 0.0F;
        mtx_f[0][0] = mtx_f[1][1] = 1.0F;
    }

    if (is_translate != FALSE)
    {
        mtx_f[3][0] = dobj->translate.vec.f.x;
        mtx_f[3][1] = dobj->translate.vec.f.y;
        mtx_f[3][2] = dobj->translate.vec.f.z;
    }
    else mtx_f[3][0] = mtx_f[3][1] = mtx_f[3][2] = 0;

    syMatrixF2LFixedW(&mtx_f, mtx_l);
}

void func_80010C2C(Mtx *mtx_l, DObj *dobj, sb32 is_translate)
{
    Mtx44f mtx_f;
    f32 distx;
    f32 distz;

    CObj *cobj;
    f32 res;

    cobj = CObjGetStruct(gGCCurrentCamera);

    distx = dobj->translate.vec.f.x - cobj->vec.eye.x;
    distz = dobj->translate.vec.f.z - cobj->vec.eye.z;

    res = sqrtf(SQUARE(distx) + SQUARE(distz));

    mtx_f[0][3] = mtx_f[1][3] = mtx_f[2][3] = mtx_f[1][0] = mtx_f[0][1] = mtx_f[1][2] = mtx_f[2][1] = 0.0F;
    mtx_f[1][1] = mtx_f[3][3] = 1.0F;

    if (res != 0.0F)
    {
        f32 inv = 1.0F / res;

        distx *= inv;
        distz *= inv;

        mtx_f[0][2] = distx;
        mtx_f[2][0] = -distx;
        mtx_f[0][0] = -distz;
        mtx_f[2][2] = -distz;
    }
    else
    {
        mtx_f[2][0] = mtx_f[0][2] = 0.0F;
        mtx_f[0][0] = mtx_f[2][2] = 1.0F;
    }

    if (is_translate != FALSE)
    {
        mtx_f[3][0] = dobj->translate.vec.f.x;
        mtx_f[3][1] = dobj->translate.vec.f.y;
        mtx_f[3][2] = dobj->translate.vec.f.z;
    }
    else mtx_f[3][0] = mtx_f[3][1] = mtx_f[3][2] = 0;

    syMatrixF2LFixedW(&mtx_f, mtx_l);
}

// 0x80010D70
/* Scratch: https://decomp.me/scratch/X7YA9
 * Similar function is matched in pokemonsnap: renPrepareModelMatrix (render.c)
 */
#ifdef NON_MATCHING
s32 gcPrepDObjMatrix(Gfx **dl, DObj *dobj)
{
    Gfx *current_dl = *dl;
    XObj *xobj;
    s32 sp2CC;
    s32 ret;
    SYMatrixHub mtx_hub;
    GCTranslate *translate;
    GCRotate *rotate; // fp (s8)
    GCScale *scale;
    f32 f12;
    s32 i;
    s32 j;
    s32 kind;

    sp2CC = 0;

    if (dobj->vec != NULL)
    {
        uintptr_t csr = (uintptr_t)dobj->vec->data;

        for (i = 0; i < ARRAY_COUNT(dobj->vec->kinds); i++)
        {
            switch (dobj->vec->kinds[i])
            {
            case 0:
                break;

            case 1:
                translate = (GCTranslate*)csr;
                csr += sizeof(*translate);
                break;

            case 2:
                rotate = (GCRotate*)csr;
                csr += sizeof(*rotate);
                break;

            case 3:
                scale = (GCScale*)csr;
                csr += sizeof(*scale);
                break;
            }
        }
    }
    for (i = 0; i < dobj->xobjs_num; i++) // Can use "j" here without any consequences
    {
        xobj = dobj->xobjs[i]; // s3

        if (xobj != NULL)
        {
            mtx_hub.gbi = &xobj->mtx;

            /* 
             * Non-matching part begins here. xobj->unk05 gets forced into v1 instead of v0, and xobj->kind into v0 instead of v1.
             * gSYTaskmanGraphicsHeap is also placed in v0 instead of v1; these two v0/v1 swaps are *mostly* unrelated. I have tried for hours,
             * but I cannot find a permutation that satisfies all requirements. The "closest" I got to a real match was by using
             * fabricated inline getters for xobj->kind in the first two >= 66 comparisons, which bloated the stack frame too much,
             * and of course also generated a stub that I reckon will not appear in this TU. I have just about given up on this function,
             * but I do not feel too much remorse for doing so; it is functionally equivalent and all instructions match at the very least.
             * 
             * If a brave volunteer would like to try in the future (so you either get a light bulb above your head or so you can avoid wasting your time), here's what I've tried:
             *     - making a variable for xobj->kind or xobj->unk05 
             *     - a bunch of permutations regarding how gSYTaskmanGraphicsHeap.ptr is advanced (gSYTaskmanGraphicsHeap.ptr++, gSYTaskmanGraphicsHeap.size += sizeof(Mtx44f), etc.)
             *     - the C address hack "*(type*)&" to get xobj->kind and xobj->unk05
             *     - making a u8* variable to xobj->kind and xobj->unk05 and dereferencing that
             *     - various control flow permutations in an attempt to bump regalloc
             *     - more that I am forgetting
             * 
             * If there is one last clue, it should be that the permuter cannot find any solutions. That tells us what the solution *isn't*.
             * Good luck!
             */

            if (xobj->unk05 != 2)
            {
                if (xobj->unk05 == 4)
                {
                    if (dobj->parent_gobj->frame_draw_last != (u8)dSYTaskmanFrameCount)
                    {
                        mtx_hub.f = gSYTaskmanGraphicsHeap.ptr;
                        gSYTaskmanGraphicsHeap.ptr = (mtx_hub.f = gSYTaskmanGraphicsHeap.ptr) + 1;
                    }
                    else
                    {
                        switch (xobj->kind)
                        {
                        case 33:
                        case 34:
                        case 35:
                        case 36:
                        case 37:
                        case 38:
                        case 39:
                        case 40:
                        case 41:
                        case 42:
                        case 43:
                        case 44:
                        case 45:
                        case 46:
                        case 47:
                        case 48:
                        case 49:
                        case 50:
                            gSYTaskmanGraphicsHeap.ptr = (mtx_hub.gbi = gSYTaskmanGraphicsHeap.ptr) + 1;
                            break;

                        default:
                            if (xobj->kind >= 66)
                            {
                                gSYTaskmanGraphicsHeap.ptr = (mtx_hub.f = gSYTaskmanGraphicsHeap.ptr) + 1;
                            }
                            else
                            {
                                mtx_hub.p = *mtx_hub.p;

                                goto check_05;
                            }
                            break;
                        }
                    }
                }
                else if (gSYTaskmanTaskID > 0)
                {
                    gSYTaskmanGraphicsHeap.ptr = (mtx_hub.f = gSYTaskmanGraphicsHeap.ptr) + 1;
                }
                else if (dobj->parent_gobj->frame_draw_last == (u8)dSYTaskmanFrameCount)
                {
                    switch (xobj->kind)
                    {
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                        gSYTaskmanGraphicsHeap.ptr = (mtx_hub.gbi = gSYTaskmanGraphicsHeap.ptr) + 1;
                        break;

                    default:
                        if (xobj->kind >= 66)
                        {
                            gSYTaskmanGraphicsHeap.ptr = (mtx_hub.f = gSYTaskmanGraphicsHeap.ptr) + 1;
                        }
                        else if (xobj->unk05 == 3)
                        {
                            gSYTaskmanGraphicsHeap.ptr = (mtx_hub.f = gSYTaskmanGraphicsHeap.ptr) + 1;
                        }
                        else goto check_05;

                        break;
                    }
                }
                ret = 0;

                switch (xobj->kind)
                {
                case 1:
                {
                    break;
                }
                case 2:
                {
                    break;
                }
                case nGCMatrixKindTra:
                {
                    syMatrixTra(mtx_hub.gbi, dobj->translate.vec.f.x, dobj->translate.vec.f.y, dobj->translate.vec.f.z);
                    break;
                }
                case nGCMatrixKindRotD:
                {
                    syMatrixRotD(mtx_hub.gbi, dobj->rotate.a, dobj->rotate.vec.f.x, dobj->rotate.vec.f.y, dobj->rotate.vec.f.z);
                    break;
                }
                case nGCMatrixKindTraRotD:
                {
                    syMatrixTraRotD
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.a,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindRotRpyD:
                {
                    syMatrixRotRpyD(mtx_hub.gbi, dobj->rotate.vec.f.x, dobj->rotate.vec.f.y, dobj->rotate.vec.f.z);
                    break;
                }
                case nGCMatrixKindTraRotRpyD:
                {
                    syMatrixTraRotRpyD
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindRotR:
                {
                    syMatrixRotR
                    (
                        mtx_hub.gbi,
                        dobj->rotate.a,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindTraRotR:
                {
                    syMatrixTraRotR
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.a,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindTraRotRSca:
                {
                    syMatrixTraRotRSca
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.a,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z,
                        dobj->scale.vec.f.x,
                        dobj->scale.vec.f.y,
                        dobj->scale.vec.f.z
                    );
                    gGCScaleX *= dobj->scale.vec.f.x;
                    break;
                }
                case nGCMatrixKindRotRpyR:
                {
                    syMatrixRotRpyR(mtx_hub.gbi, dobj->rotate.vec.f.x, dobj->rotate.vec.f.y, dobj->rotate.vec.f.z);
                    break;
                }
                case nGCMatrixKindTraRotRpyR:
                {
                    syMatrixTraRotRpyR
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindTraRotRpyRSca:
                {
                    syMatrixTraRotRpyRSca
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z,
                        dobj->scale.vec.f.x,
                        dobj->scale.vec.f.y,
                        dobj->scale.vec.f.z
                    );
                    gGCScaleX *= dobj->scale.vec.f.x;
                    break;
                }
                case nGCMatrixKindRotPyrR:
                {
                    syMatrixRotPyrR(mtx_hub.gbi, dobj->rotate.vec.f.x, dobj->rotate.vec.f.y, dobj->rotate.vec.f.z);
                    break;
                }
                case nGCMatrixKindTraRotPyrR:
                {
                    syMatrixTraRotPyrR
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindTraRotPyrRSca:
                {
                    syMatrixTraRotPyrRSca
                    (
                        mtx_hub.gbi,
                        dobj->translate.vec.f.x,
                        dobj->translate.vec.f.y,
                        dobj->translate.vec.f.z,
                        dobj->rotate.vec.f.x,
                        dobj->rotate.vec.f.y,
                        dobj->rotate.vec.f.z,
                        dobj->scale.vec.f.x,
                        dobj->scale.vec.f.y,
                        dobj->scale.vec.f.z
                    );
                    gGCScaleX *= dobj->scale.vec.f.x;
                    break;
                }
                case nGCMatrixKindSca:
                {
                    syMatrixSca(mtx_hub.gbi, dobj->scale.vec.f.x, dobj->scale.vec.f.y, dobj->scale.vec.f.z);
                    gGCScaleX *= dobj->scale.vec.f.x;
                    break;
                }
                case 33:
                {
                    func_80010AE8(mtx_hub.gbi, dobj, FALSE);
                    break;
                }
                case 34:
                {
                    func_80010AE8(mtx_hub.gbi, dobj, TRUE);
                    break;
                }
                case 35:
                {
                    func_80010748(mtx_hub.gbi, dobj, FALSE);
                    break;
                }
                case 36:
                {
                    func_80010748(mtx_hub.gbi, dobj, TRUE);
                    break;
                }
                case 37:
                {
                    func_80010C2C(mtx_hub.gbi, dobj, FALSE);
                    break;
                }
                case 38:
                {
                    func_80010C2C(mtx_hub.gbi, dobj, TRUE);
                    break;
                }
                case 39:
                {
                    func_80010918(mtx_hub.gbi, dobj, FALSE);
                    break;
                }
                case 40:
                {
                    func_80010918(mtx_hub.gbi, dobj, TRUE);
                    break;
                }
                case nGCMatrixKindVecTra:
                {
                    syMatrixTra(mtx_hub.gbi, translate->vec.f.x, translate->vec.f.y, translate->vec.f.z);
                    break;
                }
                case nGCMatrixKindVecRotR:
                {
                    syMatrixRotR(mtx_hub.gbi, rotate->a, rotate->vec.f.x, rotate->vec.f.y, rotate->vec.f.z);
                    break;
                }
                case nGCMatrixKindVecRotRpyR:
                {
                    syMatrixRotRpyR(mtx_hub.gbi, rotate->vec.f.x, rotate->vec.f.y, rotate->vec.f.z);
                    break;
                }
                case nGCMatrixKindVecSca:
                {
                    syMatrixSca(mtx_hub.gbi, scale->vec.f.x, scale->vec.f.y, scale->vec.f.z);
                    gGCScaleX *= scale->vec.f.x;
                    break;
                }
                case nGCMatrixKindVecTraRotR:
                {
                    syMatrixTraRotR
                    (
                        mtx_hub.gbi,
                        translate->vec.f.x,
                        translate->vec.f.y,
                        translate->vec.f.z,
                        rotate->a,
                        rotate->vec.f.x,
                        rotate->vec.f.y,
                        rotate->vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindVecTraRotRSca:
                {
                    syMatrixTraRotRSca
                    (
                        mtx_hub.gbi,
                        translate->vec.f.x,
                        translate->vec.f.y,
                        translate->vec.f.z,
                        rotate->a,
                        rotate->vec.f.x,
                        rotate->vec.f.y,
                        rotate->vec.f.z,
                        scale->vec.f.x,
                        scale->vec.f.y,
                        scale->vec.f.z
                    );
                    gGCScaleX *= scale->vec.f.x;
                    break;
                }
                case nGCMatrixKindVecTraRotRpyR:
                {
                    syMatrixTraRotRpyR
                    (
                        mtx_hub.gbi,
                        translate->vec.f.x,
                        translate->vec.f.y,
                        translate->vec.f.z,
                        rotate->vec.f.x,
                        rotate->vec.f.y,
                        rotate->vec.f.z
                    );
                    break;
                }
                case nGCMatrixKindVecTraRotRpyRSca:
                {
                    syMatrixTraRotRpyRSca
                    (
                        mtx_hub.gbi,
                        translate->vec.f.x,
                        translate->vec.f.y,
                        translate->vec.f.z,
                        rotate->vec.f.x,
                        rotate->vec.f.y,
                        rotate->vec.f.z,
                        scale->vec.f.x,
                        scale->vec.f.y,
                        scale->vec.f.z
                    );
                    gGCScaleX *= scale->vec.f.x;
                    break;
                }
                case 41:
                {
                    gSPMvpRecalc(current_dl++);
                    // gSPInsertMatrix?
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, sGCMatrixProjectL->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, sGCMatrixProjectL->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, sGCMatrixProjectL->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, sGCMatrixProjectL->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, sGCMatrixProjectL->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, sGCMatrixProjectL->m[1][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, sGCMatrixProjectL->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, sGCMatrixProjectL->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, sGCMatrixProjectL->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, sGCMatrixProjectL->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, sGCMatrixProjectL->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, sGCMatrixProjectL->m[3][1]);
                    // this is different
                    continue;
                }
                case 42:
                {
                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, sGCMatrixProjectL->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, sGCMatrixProjectL->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, sGCMatrixProjectL->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, sGCMatrixProjectL->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, sGCMatrixProjectL->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, sGCMatrixProjectL->m[1][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, sGCMatrixProjectL->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, sGCMatrixProjectL->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, sGCMatrixProjectL->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, sGCMatrixProjectL->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, sGCMatrixProjectL->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, sGCMatrixProjectL->m[3][1]);

                    continue;
                }
                case 43:
                {
                    f12 = dobj->scale.vec.f.y * gGCScaleX;

                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][0] = gGCMatrixPerspF[0][0] * gGCScaleX;
                    sGCMatrixMvpF[1][1] = gGCMatrixPerspF[1][1] * f12;
                    sGCMatrixMvpF[2][2] = gGCMatrixPerspF[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = gGCMatrixPerspF[2][3] * gGCScaleX;

                    sGCMatrixMvpF[0][1] = 0.0F;
                    sGCMatrixMvpF[0][2] = 0.0F;
                    sGCMatrixMvpF[0][3] = 0.0F;
                    sGCMatrixMvpF[1][0] = 0.0F;
                    sGCMatrixMvpF[1][2] = 0.0F;
                    sGCMatrixMvpF[1][3] = 0.0F;
                    sGCMatrixMvpF[2][0] = 0.0F;
                    sGCMatrixMvpF[2][1] = 0.0F;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                case 44:
                {
                    f12 = dobj->scale.vec.f.y * gGCScaleX;

                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][0] = gGCMatrixPerspF[0][0] * gGCScaleX;
                    sGCMatrixMvpF[1][1] = gGCMatrixPerspF[1][1] * f12;
                    sGCMatrixMvpF[2][2] = gGCMatrixPerspF[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = gGCMatrixPerspF[2][3] * gGCScaleX;

                    sGCMatrixMvpF[0][1] = 0.0F;
                    sGCMatrixMvpF[0][2] = 0.0F;
                    sGCMatrixMvpF[0][3] = 0.0F;
                    sGCMatrixMvpF[1][0] = 0.0F;
                    sGCMatrixMvpF[1][2] = 0.0F;
                    sGCMatrixMvpF[1][3] = 0.0F;
                    sGCMatrixMvpF[2][0] = 0.0F;
                    sGCMatrixMvpF[2][1] = 0.0F;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                case 45:
                {
                    f32 cosx, sinx;

                    sinx = __sinf(dobj->rotate.vec.f.x); // sp1CC
                    cosx = cosf(dobj->rotate.vec.f.x); // sp1C8 ?

                    // f2 * f8 -> f12
                    f12 = dobj->scale.vec.f.y * gGCScaleX;
                    // f2 * f10 -> f4 store reload -> f2
                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][2] = 0.0F;
                    sGCMatrixMvpF[1][2] = 0.0F;
                    sGCMatrixMvpF[0][3] = 0.0F;
                    sGCMatrixMvpF[1][3] = 0.0F;
                    sGCMatrixMvpF[2][0] = 0.0F;
                    sGCMatrixMvpF[2][1] = 0.0F;

                    sGCMatrixMvpF[0][0] = gGCMatrixPerspF[0][0] * gGCScaleX * cosx;
                    sGCMatrixMvpF[1][0] = gGCMatrixPerspF[0][0] * gGCScaleX * -sinx;
                    sGCMatrixMvpF[0][1] = gGCMatrixPerspF[1][1] * f12 * sinx;
                    sGCMatrixMvpF[1][1] = gGCMatrixPerspF[1][1] * f12 * cosx;
                    sGCMatrixMvpF[2][2] = gGCMatrixPerspF[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = gGCMatrixPerspF[2][3] * gGCScaleX;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                case 46:
                {
                    f32 cosz, sinz;

                    sinz = __sinf(dobj->rotate.vec.f.z); // sp190
                    cosz = cosf(dobj->rotate.vec.f.z); // sp188 ?

                    f12 = dobj->scale.vec.f.y * gGCScaleX;

                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][2] = 0.0F;
                    sGCMatrixMvpF[1][2] = 0.0F;
                    sGCMatrixMvpF[0][3] = 0.0F;
                    sGCMatrixMvpF[1][3] = 0.0F;
                    sGCMatrixMvpF[2][0] = 0.0F;
                    sGCMatrixMvpF[2][1] = 0.0F;

                    sGCMatrixMvpF[0][0] = gGCMatrixPerspF[0][0] * gGCScaleX * cosz;
                    sGCMatrixMvpF[1][0] = gGCMatrixPerspF[0][0] * gGCScaleX * -sinz;
                    sGCMatrixMvpF[0][1] = gGCMatrixPerspF[1][1] * f12 * sinz;
                    sGCMatrixMvpF[1][1] = gGCMatrixPerspF[1][1] * f12 * cosz;
                    sGCMatrixMvpF[2][2] = gGCMatrixPerspF[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = gGCMatrixPerspF[2][3] * gGCScaleX;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                case 47:
                {
                    f12 = gGCScaleX * dobj->scale.vec.f.y;

                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][0] = sGCMatrixMod1F[0][0] * gGCScaleX;
                    sGCMatrixMvpF[0][1] = sGCMatrixMod1F[0][1] * gGCScaleX;
                    sGCMatrixMvpF[0][2] = sGCMatrixMod1F[0][2] * gGCScaleX;
                    sGCMatrixMvpF[0][3] = sGCMatrixMod1F[0][3] * gGCScaleX;
                    sGCMatrixMvpF[1][0] = sGCMatrixMod1F[1][0] * f12;
                    sGCMatrixMvpF[1][1] = sGCMatrixMod1F[1][1] * f12;
                    sGCMatrixMvpF[1][2] = sGCMatrixMod1F[1][2] * f12;
                    sGCMatrixMvpF[1][3] = sGCMatrixMod1F[1][3] * f12;
                    sGCMatrixMvpF[2][0] = sGCMatrixMod1F[2][0] * gGCScaleX;
                    sGCMatrixMvpF[2][1] = sGCMatrixMod1F[2][1] * gGCScaleX;
                    sGCMatrixMvpF[2][2] = sGCMatrixMod1F[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = sGCMatrixMod1F[2][3] * gGCScaleX;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                case 48:
                {
                    f12 = gGCScaleX * dobj->scale.vec.f.y;

                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][0] = sGCMatrixMod1F[0][0] * gGCScaleX;
                    sGCMatrixMvpF[0][1] = sGCMatrixMod1F[0][1] * gGCScaleX;
                    sGCMatrixMvpF[0][2] = sGCMatrixMod1F[0][2] * gGCScaleX;
                    sGCMatrixMvpF[0][3] = sGCMatrixMod1F[0][3] * gGCScaleX;
                    sGCMatrixMvpF[1][0] = sGCMatrixMod1F[1][0] * f12;
                    sGCMatrixMvpF[1][1] = sGCMatrixMod1F[1][1] * f12;
                    sGCMatrixMvpF[1][2] = sGCMatrixMod1F[1][2] * f12;
                    sGCMatrixMvpF[1][3] = sGCMatrixMod1F[1][3] * f12;
                    sGCMatrixMvpF[2][0] = sGCMatrixMod1F[2][0] * gGCScaleX;
                    sGCMatrixMvpF[2][1] = sGCMatrixMod1F[2][1] * gGCScaleX;
                    sGCMatrixMvpF[2][2] = sGCMatrixMod1F[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = sGCMatrixMod1F[2][3] * gGCScaleX;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                case 49:
                {
                    f12 = gGCScaleX * dobj->scale.vec.f.y;

                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][0] = sGCMatrixMod2F[0][0] * gGCScaleX;
                    sGCMatrixMvpF[0][1] = sGCMatrixMod2F[0][1] * gGCScaleX;
                    sGCMatrixMvpF[0][2] = sGCMatrixMod2F[0][2] * gGCScaleX;
                    sGCMatrixMvpF[0][3] = sGCMatrixMod2F[0][3] * gGCScaleX;
                    sGCMatrixMvpF[1][0] = sGCMatrixMod2F[1][0] * f12;
                    sGCMatrixMvpF[1][1] = sGCMatrixMod2F[1][1] * f12;
                    sGCMatrixMvpF[1][2] = sGCMatrixMod2F[1][2] * f12;
                    sGCMatrixMvpF[1][3] = sGCMatrixMod2F[1][3] * f12;
                    sGCMatrixMvpF[2][0] = sGCMatrixMod2F[2][0] * gGCScaleX;
                    sGCMatrixMvpF[2][1] = sGCMatrixMod2F[2][1] * gGCScaleX;
                    sGCMatrixMvpF[2][2] = sGCMatrixMod2F[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = sGCMatrixMod2F[2][3] * gGCScaleX;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                case 50:
                {
                    f12 = gGCScaleX * dobj->scale.vec.f.y;

                    gGCScaleX *= dobj->scale.vec.f.x;

                    sGCMatrixMvpF[0][0] = sGCMatrixMod2F[0][0] * gGCScaleX;
                    sGCMatrixMvpF[0][1] = sGCMatrixMod2F[0][1] * gGCScaleX;
                    sGCMatrixMvpF[0][2] = sGCMatrixMod2F[0][2] * gGCScaleX;
                    sGCMatrixMvpF[0][3] = sGCMatrixMod2F[0][3] * gGCScaleX;
                    sGCMatrixMvpF[1][0] = sGCMatrixMod2F[1][0] * f12;
                    sGCMatrixMvpF[1][1] = sGCMatrixMod2F[1][1] * f12;
                    sGCMatrixMvpF[1][2] = sGCMatrixMod2F[1][2] * f12;
                    sGCMatrixMvpF[1][3] = sGCMatrixMod2F[1][3] * f12;
                    sGCMatrixMvpF[2][0] = sGCMatrixMod2F[2][0] * gGCScaleX;
                    sGCMatrixMvpF[2][1] = sGCMatrixMod2F[2][1] * gGCScaleX;
                    sGCMatrixMvpF[2][2] = sGCMatrixMod2F[2][2] * gGCScaleX;
                    sGCMatrixMvpF[2][3] = sGCMatrixMod2F[2][3] * gGCScaleX;

                    syMatrixF2L(&sGCMatrixMvpF, mtx_hub.gbi);

                    gSPMvpRecalc(current_dl++);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_I, mtx_hub.gbi->m[0][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_I, mtx_hub.gbi->m[0][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_I, mtx_hub.gbi->m[0][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_I, mtx_hub.gbi->m[0][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_I, mtx_hub.gbi->m[1][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_I, mtx_hub.gbi->m[1][1]);

                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XX_XY_F, mtx_hub.gbi->m[2][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_XZ_XW_F, mtx_hub.gbi->m[2][1]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YX_YY_F, mtx_hub.gbi->m[2][2]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_YZ_YW_F, mtx_hub.gbi->m[2][3]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZX_ZY_F, mtx_hub.gbi->m[3][0]);
                    gMoveWd(current_dl++, G_MW_MATRIX, G_MWO_MATRIX_ZZ_ZW_F, mtx_hub.gbi->m[3][1]);

                    continue;
                }
                default:
                    if (xobj->kind >= 66)
                    {
                        if (sGCMatrixFuncList != NULL)
                        {
                            sb32(*proc)(Mtx*, DObj*, Gfx**) = (dobj->parent_gobj->frame_draw_last != (u8)dSYTaskmanFrameCount) ? sGCMatrixFuncList[xobj->kind - 66].proc_diff : sGCMatrixFuncList[xobj->kind - 66].proc_same;

                            // If proc's return value uses up a GPR and is assigned to a variable, IDO refuses to free up v0 later down.
                            ret = proc(mtx_hub.gbi, dobj, &current_dl);
                        }
                    }
                    if (ret == 1)
                    {
                        continue;
                    }
                    else break;
                }
                // The problem is right here. If we can figure out a way to free up v0 after being assigned to ret from proc, this function will be matched.
            check_05:
                if (xobj->unk05 == 1)
                {
                    if (mtx_hub.gbi == &xobj->mtx)
                    {
                        xobj->unk05 = 2;
                    }
                }
            }
            if (xobj->kind != 2)
            {
                if ((sp2CC == 0) && (dobj->parent == DOBJ_PARENT_NULL || dobj->sib_next != NULL))
                {
                    gSPMatrix(current_dl++, mtx_hub.gbi, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                }
                else gSPMatrix(current_dl++, mtx_hub.gbi, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                sp2CC++;
            }
        }
    }
    *dl = current_dl;

    return sp2CC;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/sys/objdisplay/gcPrepDObjMatrix.s")
#endif /* NON_MATCHING */

// 0x80012D90
void gcDrawMObjForDObj(DObj *dobj, Gfx **dl_head)
{
    s32 mobj_count;
    s32 i;
    MObj *mobj;
    Gfx *new_dl;
    Gfx *branch_dl;
    u32 flags;
    f32 scau;
    f32 scav;
    f32 trau;
    f32 trav;
    f32 scrollu;
    f32 scrollv;
    s32 uls, ult;
    s32 s, t;
#ifdef PORT
    u32 *sprite_tokens;
    u32 *palette_tokens;
    void *palette_data;
    void *current_sprite;
    void *next_sprite;
    u32 palette_token;
    u32 current_sprite_token;
    u32 next_sprite_token;
#endif

    if (dobj->mobj == NULL)
    {
        return;
    }
    gSPSegment(dl_head[0]++, 0xE, gSYTaskmanGraphicsHeap.ptr);

    for (mobj_count = 0, mobj = dobj->mobj; mobj != NULL; mobj_count++)
    {
        mobj = mobj->next;
    }
    mobj = dobj->mobj;
    branch_dl = (Gfx*) (((Gfx*) gSYTaskmanGraphicsHeap.ptr) + mobj_count);
    new_dl = gSYTaskmanGraphicsHeap.ptr;

    for (i = 0; i < mobj_count; i++, mobj = mobj->next)
    {
        flags = mobj->sub.flags;
#ifdef PORT
        sprite_tokens = NULL;
        palette_tokens = NULL;
        palette_data = NULL;
        current_sprite = NULL;
        next_sprite = NULL;
        palette_token = 0;
        current_sprite_token = 0;
        next_sprite_token = 0;
#endif

        if (flags == MOBJ_FLAG_NONE)
        {
            flags = (MOBJ_FLAG_TEXTURE | 0x20 | MOBJ_FLAG_ALPHA);
        }
        if (flags & (MOBJ_FLAG_TEXTURE | 0x40 | 0x20))
        {
            scau = mobj->sub.scau;
            scav = mobj->sub.scav;
            trau = mobj->sub.trau;
            trav = mobj->sub.trav;
            scrollu = mobj->sub.scrollu;
            scrollv = mobj->sub.scrollv;

            if (mobj->sub.unk10 == 1)
            {
                scau *= 0.5F;
                trau = ((trau - mobj->sub.unk24) + 1.0F - (mobj->sub.unk28 * 0.5F)) * 0.5F;
                scrollu = ((scrollu - mobj->sub.unk44) + 1.0F - (mobj->sub.unk28 * 0.5F)) * 0.5F;
            }
        }
        gSPBranchList(&new_dl[i], branch_dl);

        if (flags & MOBJ_FLAG_PALETTE)
        {
#ifdef PORT
            palette_tokens = (u32*)PORT_RESOLVE(mobj->sub.palettes);
            palette_data = gcTryResolveTokenArrayEntry("palette-null", dobj, mobj, mobj->sub.palettes, palette_tokens, (s32)mobj->palette_id, &palette_token);

            if (palette_data != NULL)
            {
                gDPSetTextureImage(branch_dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, palette_data);
            }
#else
            gDPSetTextureImage(branch_dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, mobj->sub.palettes[(s32)mobj->palette_id]);
#endif

            if (
#ifdef PORT
                (palette_data != NULL) &&
#endif
                (flags & (MOBJ_FLAG_SPLIT | MOBJ_FLAG_ALPHA))
            )
            {
                gDPTileSync(branch_dl++);
                gDPSetTile
                (
                    branch_dl++,
                    G_IM_FMT_RGBA,
                    G_IM_SIZ_4b,
                    0,
                    0x0100,
                    5,
                    0,
                    G_TX_NOMIRROR | G_TX_WRAP,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOMIRROR | G_TX_WRAP,
                    G_TX_NOMASK,
                    G_TX_NOLOD
                );
                gDPLoadSync(branch_dl++);
                gDPLoadTLUTCmd(branch_dl++, 5, (mobj->sub.siz == G_IM_SIZ_8b) ? 0xFF : 0xF);
                gDPPipeSync(branch_dl++);
            }
        }
        if (flags & MOBJ_FLAG_LIGHT1)
        {
#ifdef PORT
            /* PORT: SYColorPack.pack is a LE u32 read of the in-memory byte
             * layout [R, G, B, A], which numerically equals
             * (a<<24)|(b<<16)|(g<<8)|r — *not* the N64 packcol format Fast3D
             * expects.  Rebuild an N64-format packcol by hand so
             * (r<<24)|(g<<16)|(b<<8)|a lands in the Gfx command's w1 field. */
            gSPLightColor(branch_dl++, LIGHT_1,
                (((u32)mobj->sub.light1color.s.r) << 24) |
                (((u32)mobj->sub.light1color.s.g) << 16) |
                (((u32)mobj->sub.light1color.s.b) <<  8) |
                 ((u32)mobj->sub.light1color.s.a));
#else
            gSPLightColor(branch_dl++, LIGHT_1, mobj->sub.light1color.pack);
#endif
        }
        if (flags & MOBJ_FLAG_LIGHT2)
        {
#ifdef PORT
            gSPLightColor(branch_dl++, LIGHT_2,
                (((u32)mobj->sub.light2color.s.r) << 24) |
                (((u32)mobj->sub.light2color.s.g) << 16) |
                (((u32)mobj->sub.light2color.s.b) <<  8) |
                 ((u32)mobj->sub.light2color.s.a));
#else
            gSPLightColor(branch_dl++, LIGHT_2, mobj->sub.light2color.pack);
#endif
        }
        if (flags & (MOBJ_FLAG_PRIMCOLOR | MOBJ_FLAG_FRAC | 0x8))
        {
            if (flags & MOBJ_FLAG_FRAC)
            {
                s32 trunc = mobj->lfrac;

                gDPSetPrimColor
                (
                    branch_dl++,
                    mobj->sub.prim_m,
                    (mobj->lfrac - trunc) * 256.0F,
                    mobj->sub.primcolor.s.r,
                    mobj->sub.primcolor.s.g,
                    mobj->sub.primcolor.s.b,
                    mobj->sub.primcolor.s.a
                );
                mobj->texture_id_curr = trunc;
                mobj->texture_id_next = trunc + 1;
            }
            else
            {
                gDPSetPrimColor
                (
                    branch_dl++,
                    mobj->sub.prim_m,
                    mobj->lfrac * 255.0F,
                    mobj->sub.primcolor.s.r,
                    mobj->sub.primcolor.s.g,
                    mobj->sub.primcolor.s.b,
                    mobj->sub.primcolor.s.a
                );
            }
        }
        if (flags & MOBJ_FLAG_ENVCOLOR)
        {
            gDPSetEnvColor
            (
                branch_dl++,
                mobj->sub.envcolor.s.r,
                mobj->sub.envcolor.s.g,
                mobj->sub.envcolor.s.b,
                mobj->sub.envcolor.s.a
            );
        }
        if (flags & MOBJ_FLAG_BLENDCOLOR)
        {
            gDPSetBlendColor
            (
                branch_dl++,
                mobj->sub.blendcolor.s.r,
                mobj->sub.blendcolor.s.g,
                mobj->sub.blendcolor.s.b,
                mobj->sub.blendcolor.s.a
            );
        }
#ifdef PORT
        if (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_SPLIT | MOBJ_FLAG_ALPHA))
        {
            sprite_tokens = (u32*)PORT_RESOLVE(mobj->sub.sprites);

            if (sprite_tokens == NULL)
            {
                // Zero token means no sprite array by design (e.g. S2DEX BG objects use
                // G_BG_COPY/G_BG_1CYC and don't reference a sprite array). Only warn if
                // the token was non-zero, which indicates a genuine resolution failure.
                if (mobj->sub.sprites != 0)
                {
                    gcLogMObjResolveWarning("sprite-array-null", dobj, mobj, mobj->sub.sprites, NULL, -1, 0);
                }
            }
            else
            {
                if (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_SPLIT))
                {
                    next_sprite = gcTryResolveTokenArrayEntry("next-sprite-null", dobj, mobj, mobj->sub.sprites, sprite_tokens, mobj->texture_id_next, &next_sprite_token);
                }
                if (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_ALPHA))
                {
                    current_sprite = gcTryResolveTokenArrayEntry("current-sprite-null", dobj, mobj, mobj->sub.sprites, sprite_tokens, mobj->texture_id_curr, &current_sprite_token);
                }
                if ((next_sprite == NULL) && (current_sprite != NULL) && (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_SPLIT)))
                {
                    gcLogMObjResolveWarning("next-sprite-fallback-current", dobj, mobj, mobj->sub.sprites, sprite_tokens, mobj->texture_id_next, next_sprite_token);
                    next_sprite = current_sprite;
                }
                if ((current_sprite == NULL) && (next_sprite != NULL) && (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_ALPHA)))
                {
                    gcLogMObjResolveWarning("current-sprite-fallback-next", dobj, mobj, mobj->sub.sprites, sprite_tokens, mobj->texture_id_curr, current_sprite_token);
                    current_sprite = next_sprite;
                }
            }
        }
#endif
#ifdef PORT
        gcRenderDiagLogMObj(dobj, mobj, flags, current_sprite, current_sprite_token, next_sprite, next_sprite_token,
                            palette_data, palette_token);
#endif
        if (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_SPLIT))
        {
            s32 block_siz = (mobj->sub.block_siz == G_IM_SIZ_32b) ? G_IM_SIZ_32b : G_IM_SIZ_16b;

#ifdef PORT
            if (next_sprite != NULL)
            {
                gDPSetTextureImage(branch_dl++, mobj->sub.block_fmt, block_siz, 1, next_sprite);
            }
#else
            gDPSetTextureImage
            (
                branch_dl++,
                mobj->sub.block_fmt,
                block_siz,
                1,
                mobj->sub.sprites[mobj->texture_id_next]
            );
#endif
            if (
#ifdef PORT
                (next_sprite != NULL) &&
#endif
                (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_ALPHA))
            )
            {
                gDPLoadSync(branch_dl++);

                switch (mobj->sub.block_siz)
                {
                case G_IM_SIZ_4b:
                    gDPLoadBlock
                    (
                        branch_dl++,
                        6,
                        0,
                        0,
                        (((mobj->sub.block_dxt * mobj->sub.unk36) + 3) >> 2) - 1,
                        (((mobj->sub.block_dxt / 16 <= 0) ? 1 : mobj->sub.block_dxt / 16) + 0x7FF) / ((mobj->sub.block_dxt / 16 <= 0) ? 1 : mobj->sub.block_dxt / 16)
                    );
                    break;

                case G_IM_SIZ_8b:
                    gDPLoadBlock
                    (
                        branch_dl++,
                        6,
                        0,
                        0,
                        (((mobj->sub.block_dxt * mobj->sub.unk36) + 1) >> 1) - 1,
                        (((mobj->sub.block_dxt / 8 <= 0) ? 1 : mobj->sub.block_dxt / 8) + 0x7FF) / ((mobj->sub.block_dxt / 8 <= 0) ? 1 : mobj->sub.block_dxt / 8)
                    );
                    break;

                case G_IM_SIZ_16b:
                    gDPLoadBlock
                    (
                        branch_dl++,
                        6,
                        0,
                        0,
                        (mobj->sub.block_dxt * mobj->sub.unk36) - 1,
                        ((((mobj->sub.block_dxt * 2) / 8 <= 0) ? 1 : (mobj->sub.block_dxt * 2) / 8) + 0x7FF) / (((mobj->sub.block_dxt * 2) / 8 <= 0) ? 1 : (mobj->sub.block_dxt * 2) / 8)
                    );
                    break;

                case G_IM_SIZ_32b:
                    gDPLoadBlock
                    (
                        branch_dl++,
                        6,
                        0,
                        0,
                        (mobj->sub.block_dxt * mobj->sub.unk36) - 1,
                        ((((mobj->sub.block_dxt * 4) / 8 <= 0) ? 1 : (mobj->sub.block_dxt * 4) / 8) + 0x7FF) / (((mobj->sub.block_dxt * 4) / 8 <= 0) ? 1 : (mobj->sub.block_dxt * 4) / 8)
                    );
                    break;
                }
                gDPLoadSync(branch_dl++);
            }
        }
        if (flags & (MOBJ_FLAG_FRAC | MOBJ_FLAG_ALPHA))
        {
#ifdef PORT
            if (current_sprite != NULL)
            {
                gDPSetTextureImage(branch_dl++, mobj->sub.fmt, mobj->sub.siz, 1, current_sprite);
            }
#else
            gDPSetTextureImage
            (
                branch_dl++,
                mobj->sub.fmt,
                mobj->sub.siz,
                1,
                mobj->sub.sprites[mobj->texture_id_curr]
            );
#endif
        }
        if (flags & 0x20)
        {
            if (mobj->sub.unk10 == 2)
            {
                uls = (ABSF(scau) > (1.0F / 65535.0F)) ? ((mobj->sub.unk0C * trau) / scau) * 4.0F : 0.0F;
                ult = (ABSF(scav) > (1.0F / 65535.0F)) ? ((mobj->sub.unk0E * trav) / scav) * 4.0F : 0.0F;

                if (uls < 0)
                {
                    uls = 0;
                }
                if (ult < 0)
                {
                    ult = 0;
                }
            }
            else
            {
                uls = (ABSF(scau) > (1.0F / 65535.0F)) ? (((mobj->sub.unk0C * trau) + mobj->sub.unk0A) / scau) * 4.0F : 0.0F;
                ult = (ABSF(scav) > (1.0F / 65535.0F)) ? (((((1.0F - scav) - trav) * mobj->sub.unk0E) + mobj->sub.unk0A) / scav) * 4.0F : 0.0F;
            }
            gDPSetTileSize
            (
                branch_dl++,
                0,
                uls,
                ult,
                ((mobj->sub.unk0C - 1) << 2) + uls,
                ((mobj->sub.unk0E - 1) << 2) + ult
            );
        }
        if (flags & 0x40)
        {
            uls = (ABSF(scau) > (1.0F / 65535.0F)) ? (((mobj->sub.unk38 * scrollu) + mobj->sub.unk0A) / scau) * 4.0F : 0.0F;
            ult = (ABSF(scav) > (1.0F / 65535.0F)) ? (((((1.0F - scav) - scrollv) * mobj->sub.unk3A) + mobj->sub.unk0A) / scav) * 4.0F : 0.0F;

            gDPSetTileSize
            (
                branch_dl++,
                1,
                uls,
                ult,
                ((mobj->sub.unk38 - 1) << 2) + uls,
                ((mobj->sub.unk3A - 1) << 2) + ult
            );
        }
        if (flags & MOBJ_FLAG_TEXTURE)
        {
            if (mobj->sub.unk10 == 2)
            {
                s = (ABSF(scau) > (1.0F / 65535.0F)) ? (mobj->sub.unk0C * 64) / scau : 0.0F;
                t = (ABSF(scav) > (1.0F / 65535.0F)) ? (mobj->sub.unk0E * 64) / scav : 0.0F;
            }
            else
            {
                s = (ABSF(scau) > (1.0F / 65535.0F)) ? (2097152.0F / mobj->sub.unk08) / scau : 0.0F;
                t = (ABSF(scav) > (1.0F / 65535.0F)) ? (2097152.0F / mobj->sub.unk08) / scav : 0.0F;
            }
            if (s > 0xFFFF)
            {
                s = 0xFFFF;
            }
            if (t > 0xFFFF)
            {
                t = 0xFFFF;
            }
            gSPTexture(branch_dl++, s, t, 0, 0, G_ON);
        }
        gSPEndDisplayList(branch_dl++);
    }
    gSYTaskmanGraphicsHeap.ptr = (void*) branch_dl;
}

// 0x80013D90
void gcDrawDObjForGObj(GObj *gobj, Gfx **dl_head)
{
    s32 num;
    DObj *dobj = DObjGetStruct(gobj);

    gGCScaleX = 1.0F;

    if (dobj->dv != NULL)
    {
        if (dobj->flags == DOBJ_FLAG_NONE)
        {
            num = gcPrepDObjMatrix(dl_head, dobj);
            gcDrawMObjForDObj(dobj, dl_head);
#ifdef PORT
            gcLogSuspiciousDLPointer("dobj", dobj, (unsigned long long)dobj->dl, -1, NULL);
#endif
            gSPDisplayList(dl_head[0]++, dobj->dl);

            if (num != 0)
            {
                if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                {
                    gSPPopMatrix(dl_head[0]++, G_MTX_MODELVIEW);
                }
            }
        }
    }
}

// 0x80013E68
void gcDrawDObjDLHead0(GObj *gobj) 
{
    gcDrawDObjForGObj(gobj, &gSYTaskmanDLHeads[0]);
}

// 0x80013E8C
void gcDrawDObjDLHead1(GObj *gobj)
{
    gcDrawDObjForGObj(gobj, &gSYTaskmanDLHeads[1]);
}

// 0x80013EB0
void gcDrawDObjDLHead2(GObj *gobj)
{
    gcDrawDObjForGObj(gobj, &gSYTaskmanDLHeads[2]);
}

// 0x80013ED4
void gcDrawDObjDLHead3(GObj *gobj)
{
    gcDrawDObjForGObj(gobj, &gSYTaskmanDLHeads[3]);
}

// 0x80013EF8
void gcDrawDObjTree(DObj *this_dobj) 
{
    s32 num;
    DObj *current_dobj;
    f32 bak;

    if (!(this_dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        bak = gGCScaleX;
        num = gcPrepDObjMatrix(gSYTaskmanDLHeads, this_dobj);

        if ((this_dobj->dv != NULL) && !(this_dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            gcDrawMObjForDObj(this_dobj, gSYTaskmanDLHeads);
#ifdef PORT
            gcLogSuspiciousDLPointer("tree-dobj", this_dobj, (unsigned long long)this_dobj->dl, 0, NULL);
#endif
            gSPDisplayList(gSYTaskmanDLHeads[0]++, this_dobj->dl);
        }
        if (this_dobj->child != NULL)
        { 
            gcDrawDObjTree(this_dobj->child);
        }
        if (num != 0)
        {
            if ((this_dobj->parent == DOBJ_PARENT_NULL) || (this_dobj->sib_next != NULL))
            {
                gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
            }
        }
        gGCScaleX = bak;
    }
    if (this_dobj->sib_prev == NULL) 
    {
        current_dobj = this_dobj->sib_next;

        while (current_dobj != NULL)
        {
            gcDrawDObjTree(current_dobj);
            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x80014038
void gcDrawDObjTreeForGObj(GObj *gobj) 
{
    gGCScaleX = 1.0F;
    gcDrawDObjTree(DObjGetStruct(gobj));
}

// 0x80014068
void gcDrawDObjDLLinks(DObj *dobj, DObjDLLink *dl_link)
{
    s32 num;
    s32 list_id;
    Gfx *dl_start; // start (t1)
    Gfx *dl_end; // end
    s32 unused;
    void *ptr;

    list_id = -1;

    if ((dl_link != NULL) && (dobj->flags == DOBJ_FLAG_NONE))
    {
        dl_start = gSYTaskmanDLHeads[dl_link->list_id];
        num = gcPrepDObjMatrix(&gSYTaskmanDLHeads[dl_link->list_id], dobj);
        dl_end = gSYTaskmanDLHeads[dl_link->list_id];

        if (dl_link->dl != NULL)
        {
            ptr = gSYTaskmanGraphicsHeap.ptr;

            gcDrawMObjForDObj(dobj, &gSYTaskmanDLHeads[dl_link->list_id]);
#ifdef PORT
            gcLogSuspiciousDLPointer("dl-link", dobj, (unsigned long long)dl_link->dl, dl_link->list_id, dl_link);
#endif
            gSPDisplayList(gSYTaskmanDLHeads[dl_link->list_id]++, PORT_RESOLVE(dl_link->dl));

            if (num != 0)
            {
                if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                {
                    gSPPopMatrix(gSYTaskmanDLHeads[dl_link->list_id]++, G_MTX_MODELVIEW);
                }
            }
        }
        else list_id = dl_link->list_id;

        while ((++dl_link)->list_id != ARRAY_COUNT(gSYTaskmanDLHeads))
        {
            if (dl_link->dl != NULL)
            {
                Gfx *dl_curr = dl_start;

                while (dl_curr != dl_end)
                {
                    *gSYTaskmanDLHeads[dl_link->list_id]++ = *dl_curr++;
                }
                if (dobj->mobj != NULL)
                {
                    gSPSegment(gSYTaskmanDLHeads[dl_link->list_id]++, 0xE, ptr);
                }
#ifdef PORT
                gcLogSuspiciousDLPointer("dl-link-copy", dobj, (unsigned long long)dl_link->dl, dl_link->list_id, dl_link);
#endif
                gSPDisplayList(gSYTaskmanDLHeads[dl_link->list_id]++, PORT_RESOLVE(dl_link->dl));

                if (num != 0)
                {
                    if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                    {
                        gSPPopMatrix(gSYTaskmanDLHeads[dl_link->list_id]++, G_MTX_MODELVIEW);
                    }
                }
            }
            continue; // Required!
        }
        if (list_id != -1)
        {
            gSYTaskmanDLHeads[list_id] = dl_start;
        }
    }
    else return;
}

// 0x801143FC
void gcDrawDObjDLLinksForGObj(GObj *gobj)
{
    DObj *dobj;

    gGCScaleX = 1.0F;
    dobj = DObjGetStruct(gobj);
    gcDrawDObjDLLinks(dobj, dobj->dl_link);
}

// 0x80014430
void gcInitDLs(void)
{
    s32 i;

    sGCCurrentDL = sGCLocalDLs;

    for (i = 0; i < ARRAY_COUNT(sGCForwardDLs); i++) { sGCForwardDLs[i] = sGCLocalDLs; } // needs one line
}

// 0x8001445C
void gcDrawDObjTreeDLLinks(DObj *dobj)
{
    s32 i;
    s32 num;
    DObjDLLink *dl_link;
    Gfx *dl;
    DObj *current_dobj;
    void *ptr;
    f32 bak;

    ptr = NULL;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        bak = gGCScaleX;
        dl_link = dobj->dl_link;
        dl = sGCCurrentDL;
        num = gcPrepDObjMatrix(&sGCCurrentDL, dobj);

        if ((dl_link != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            while (dl_link->list_id != ARRAY_COUNT(gSYTaskmanDLHeads))
            {
                if (dl_link->dl != NULL)
                {
                    while (sGCCurrentDL != sGCForwardDLs[dl_link->list_id])
                    {
                        *gSYTaskmanDLHeads[dl_link->list_id]++ = *sGCForwardDLs[dl_link->list_id]++;
                    }
                    if (dobj->mobj != NULL)
                    {
                        if (ptr == NULL)
                        {
                            ptr = gSYTaskmanGraphicsHeap.ptr;
                            gcDrawMObjForDObj(dobj, &gSYTaskmanDLHeads[dl_link->list_id]);

                            goto set_display_list; // The goto is required ONLY if we condense the gSYTaskmanDLHeads and sGCForwardDLs increments into a single operation.
                        }
                        else gSPSegment(gSYTaskmanDLHeads[dl_link->list_id]++, 0xE, ptr);
                    }
                set_display_list:
                    gSPDisplayList(gSYTaskmanDLHeads[dl_link->list_id]++, PORT_RESOLVE(dl_link->dl));
                }
                dl_link++;
            }
        }
        if (dobj->child != NULL)
        {
            gcDrawDObjTreeDLLinks(dobj->child);
        }
        sGCCurrentDL = dl;

        for (i = 0; i < ARRAY_COUNT(sGCForwardDLs); i++)
        {
            if (sGCForwardDLs[i] > sGCCurrentDL)
            {
                sGCForwardDLs[i] = sGCCurrentDL;

                if (num != 0)
                {
                    if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                    {
                        gSPPopMatrix(gSYTaskmanDLHeads[i]++, G_MTX_MODELVIEW);
                    }
                }
            }
            continue; // Required!
        }
        gGCScaleX = bak;
    }
    if (dobj->sib_prev == NULL)
    {
        current_dobj = dobj->sib_next;

        while (current_dobj != NULL)
        {
            gcDrawDObjTreeDLLinks(current_dobj);
            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x80014768
void gcDrawDObjTreeDLLinksForGObj(GObj *gobj)
{
    gGCScaleX = 1.0F;
    gcDrawDObjTreeDLLinks(DObjGetStruct(gobj));
}

// 0x80014798
f32 gcGetDObjDistFromEye(DObj *dobj) 
{
    f32 x, y, z;
    CObj *cobj = CObjGetStruct(gGCCurrentCamera);

    x = dobj->translate.vec.f.x - cobj->vec.eye.x;
    y = dobj->translate.vec.f.y - cobj->vec.eye.y;
    z = dobj->translate.vec.f.z - cobj->vec.eye.z;

    return SQUARE(x) + SQUARE(y) + SQUARE(z);
}

// 0x800147E0
void unref_800147E0(GObj *gobj)
{
    DObjDistDL *dist_dl;
    s32 num;
    DObj *dobj;
    f32 dist;

    dobj = DObjGetStruct(gobj);
    dist_dl = dobj->dist_dl;

    if ((dist_dl != NULL) && (dobj->flags == DOBJ_FLAG_NONE)) 
    {
        dist = gcGetDObjDistFromEye(dobj);

        while (dist < dist_dl->target_dist)
        { 
            dist_dl++;
        }
        gGCScaleX = 1.0F;

        if (dist_dl->dl != NULL)
        {
            num = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);
            gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
            gSPDisplayList(gSYTaskmanDLHeads[0]++, PORT_RESOLVE(dist_dl->dl));

            if (num != 0)
            {
                if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                {
                    gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
                }
            }
        }
    }
}

// 0x8001490C
void gcDrawDObjTreeMultiList(DObj *dobj) 
{
    s32 num;
    void *dls;
    Gfx *dl;
    DObj *current_dobj;
    f32 bak;

    dls = dobj->dls;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        bak = gGCScaleX;
        num = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

        dl = PORT_RESOLVE_ARRAY(dls, sGCDetailLevel);

        if (dl != NULL) 
        {
            if (!(dobj->flags & DOBJ_FLAG_NOTEXTURE))
            {
                gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
                gSPDisplayList(gSYTaskmanDLHeads[0]++, dl);
            }
        }
        if (dobj->child != NULL) 
        { 
            gcDrawDObjTreeMultiList(dobj->child);
        }
        if (num != 0)
        {
            if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
            {
                gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
            }
        }
        gGCScaleX = bak;
    }
    if (dobj->sib_prev == NULL) 
    {
        current_dobj = dobj->sib_next;

        while (current_dobj != NULL)
        {
            gcDrawDObjTreeMultiList(current_dobj);

            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x80014A84
void unref_80014A84(GObj *gobj)
{
    DObjDistDL *dist_dl;
    s32 num;
    f32 dist;
    DObj *dobj;
    DObj *current_dobj;

    dobj = DObjGetStruct(gobj);
    gGCScaleX = 1.0F;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        dist_dl = dobj->dist_dl;

        if (dist_dl != NULL)
        {
            sGCDetailLevel = 0;
            dist = gcGetDObjDistFromEye(dobj);
            while (dist < dist_dl->target_dist)
            {
                dist_dl++;
                sGCDetailLevel++;
            }
            num = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

            if ((dist_dl->dl != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
            {
                gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
                gSPDisplayList(gSYTaskmanDLHeads[0]++, PORT_RESOLVE(dist_dl->dl));
            }
            if (dobj->child != NULL)
            {
                gcDrawDObjTreeMultiList(dobj->child);
            }
            if (num != 0)
            {
                if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                {
                    gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
                }
            }
            if (dobj->sib_prev == NULL)
            {
                current_dobj = dobj->sib_next;

                while (current_dobj != NULL)
                {
                    gcDrawDObjTreeMultiList(current_dobj);

                    current_dobj = current_dobj->sib_next;
                }
            }
        }
    }
    else return;
}

// 0x80014C38
void unref_80014C38(GObj *gobj) 
{
    DObjDistDLLink *dist_dl_link;
    f32 dist;
    DObj *dobj;

    dobj = DObjGetStruct(gobj);
    gGCScaleX = 1.0F;

    if (dobj->flags == DOBJ_FLAG_NONE) 
    {
        dist_dl_link = dobj->dist_dl_link;

        if (dist_dl_link != NULL)
        {
            dist = gcGetDObjDistFromEye(dobj);

            while (dist < dist_dl_link->target_dist)
            { 
                dist_dl_link++;
            }
            gcDrawDObjDLLinks(dobj, PORT_RESOLVE(dist_dl_link->dl_link));
        }
    }
}

// 0x80014CD0
void func_80014CD0(DObj *dobj)
{
    void *ptr;
    s32 num;
    DObjDLLink **s0;
    DObjDLLink *dl_link;
    Gfx *dl;
    DObj *current_dobj;
    s32 i;
    f32 bak;

    ptr = NULL;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        bak = gGCScaleX;
        s0 = (DObjDLLink**)dobj->dv;
        if (s0 != NULL)
        {
            dl_link = s0[sGCDetailLevel];
        }
        dl = sGCCurrentDL;
        num = gcPrepDObjMatrix(&sGCCurrentDL, dobj);

        if ((s0 != NULL) && (dl_link != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            while (dl_link->list_id != ARRAY_COUNT(gSYTaskmanDLHeads))
            {
                if (dl_link->dl != NULL)
                {
                    while (sGCCurrentDL != sGCForwardDLs[dl_link->list_id])
                    {
                        *gSYTaskmanDLHeads[dl_link->list_id]++ = *sGCForwardDLs[dl_link->list_id]++;
                    }
                    if (dobj->mobj != NULL)
                    {
                        if (ptr == NULL)
                        {
                            ptr = gSYTaskmanGraphicsHeap.ptr;
                            gcDrawMObjForDObj(dobj, &gSYTaskmanDLHeads[dl_link->list_id]);

                            goto set_display_list; // *sigh* required to match...
                        }
                        else gSPSegment(gSYTaskmanDLHeads[dl_link->list_id]++, 0xE, ptr);
                    }
                set_display_list:
                    gSPDisplayList(gSYTaskmanDLHeads[dl_link->list_id]++, PORT_RESOLVE(dl_link->dl));
                }
                dl_link++;
            }
        }
        if (dobj->child != NULL)
        {
            func_80014CD0(dobj->child);
        }
        sGCCurrentDL = dl;

        for (i = 0; i < ARRAY_COUNT(sGCForwardDLs); i++)
        {
            if (sGCForwardDLs[i] > sGCCurrentDL)
            {
                sGCForwardDLs[i] = sGCCurrentDL;

                if (num != 0)
                {
                    if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                    {
                        gSPPopMatrix(gSYTaskmanDLHeads[i]++, G_MTX_MODELVIEW);
                    }
                }
            }
            else continue; // Required! Both the "else" and the "continue"!
        }
        gGCScaleX = bak;
    }
    if (dobj->sib_prev == NULL)
    {
        current_dobj = dobj->sib_next;

        while (current_dobj != NULL)
        {
            func_80014CD0(current_dobj);
            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x80014FFC
void unref_80014FFC(GObj *gobj)
{
    DObjDistDLLink *dist_dl_link;
    DObj *dobj;
    s32 num;
    void *ptr;
    f32 dist;
    s32 i;
    DObjDLLink *dl_link;
    Gfx *dl;
    DObj *current_dobj;

    dobj = DObjGetStruct(gobj);
    gGCScaleX = 1.0F;
    ptr = NULL;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        dist_dl_link = dobj->dist_dl_link;

        if (dist_dl_link != NULL)
        {
            sGCDetailLevel = 0;

            dist = gcGetDObjDistFromEye(dobj);

            while (dist < dist_dl_link->target_dist)
            {
                sGCDetailLevel++;
                dist_dl_link++;
            }
            dl_link = PORT_RESOLVE(dist_dl_link->dl_link);
            dl = sGCCurrentDL;
            num = gcPrepDObjMatrix(&sGCCurrentDL, dobj);

            if ((dl_link != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
            {
                while (dl_link->list_id != ARRAY_COUNT(gSYTaskmanDLHeads))
                {
                    if (dl_link->dl != NULL)
                    {
                        while (sGCCurrentDL != sGCForwardDLs[dl_link->list_id])
                        {
                            *gSYTaskmanDLHeads[dl_link->list_id]++ = *sGCForwardDLs[dl_link->list_id]++;
                        }
                        if (dobj->mobj != NULL)
                        {
                            if (ptr == NULL)
                            {
                                ptr = gSYTaskmanGraphicsHeap.ptr;
                                gcDrawMObjForDObj(dobj, &gSYTaskmanDLHeads[dl_link->list_id]);

                                goto set_display_list;
                            }
                            else gSPSegment(gSYTaskmanDLHeads[dl_link->list_id]++, 0xE, ptr);
                        }
                    set_display_list:
                        gSPDisplayList(gSYTaskmanDLHeads[dl_link->list_id]++, PORT_RESOLVE(dl_link->dl));
                    }
                    dl_link++;
                }
            }
            if (dobj->child != NULL)
            {
                // Even though this function is unreferenced, this seems wrong. Shouldn't it be calling itself instead of func_80014CD0?
                func_80014CD0(dobj->child);
            }
            sGCCurrentDL = dl;

            for (i = 0; i < ARRAY_COUNT(sGCForwardDLs); i++)
            {
                if (sGCForwardDLs[i] > sGCCurrentDL)
                {
                    sGCForwardDLs[i] = sGCCurrentDL;

                    if (num != 0)
                    {
                        if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                        {
                            gSPPopMatrix(gSYTaskmanDLHeads[i]++, G_MTX_MODELVIEW);
                        }
                    }
                    else continue;
                }
            }
            if (dobj->sib_prev == NULL)
            {
                current_dobj = dobj->sib_next;

                while (current_dobj != NULL)
                {
                    // Same here?
                    func_80014CD0(current_dobj);

                    current_dobj = current_dobj->sib_next;
                }
            }
        }
    }
}

// 0x80015358
void gcDrawDObjTreeDLArray(DObj *dobj) 
{
    s32 num;
    void *dls;
    Gfx *dl0;
    Gfx *dl1;
    f32 bak;
    DObj *current_dobj;

    dls = dobj->dls;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        bak = gGCScaleX;

        dl0 = PORT_RESOLVE_ARRAY(dls, 0);
        dl1 = PORT_RESOLVE_ARRAY(dls, 1);

        if ((dl0 != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dl0);
        }
        num = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

        if ((dl1 != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dl1);
        }
        if (dobj->child != NULL)
        { 
            gcDrawDObjTreeDLArray(dobj->child);
        }
        if (num != 0)
        {
            if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
            {
                gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
            }
        }
        gGCScaleX = bak;
    }
    if (dobj->sib_prev == NULL) 
    {
        current_dobj = dobj->sib_next;

        while (current_dobj != NULL)
        {
            gcDrawDObjTreeDLArray(current_dobj);
            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x800154F0
void unref_800154F0(GObj *gobj) 
{
    gGCScaleX = 1.0F;
    gcDrawDObjTreeDLArray(DObjGetStruct(gobj));
}

// 0x80015520
void func_80015520(DObj *dobj)
{
    s32 unused;
    s32 num;
    DObjMultiList *multi_list;
    Gfx *dl;
    void *ptr;
    s32 i;
    f32 bak;
    DObj *current_dobj;

    ptr = NULL;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        bak = gGCScaleX;
        multi_list = dobj->multi_list;
        dl = sGCCurrentDL;
        num = gcPrepDObjMatrix(&sGCCurrentDL, dobj);

        if ((multi_list != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            while (multi_list->id != ARRAY_COUNT(gSYTaskmanDLHeads))
            {
                if (multi_list->dl2 != NULL)
                {
                    if (multi_list->dl1 != NULL)
                    {
                        gSPDisplayList(gSYTaskmanDLHeads[multi_list->id]++, PORT_RESOLVE(multi_list->dl1));
                    }
                    while (sGCCurrentDL != sGCForwardDLs[multi_list->id])
                    {
                        *gSYTaskmanDLHeads[multi_list->id]++ = *sGCForwardDLs[multi_list->id]++;
                    }
                    if (dobj->mobj != NULL)
                    {
                        if (ptr == NULL)
                        {
                            ptr = gSYTaskmanGraphicsHeap.ptr;
                            gcDrawMObjForDObj(dobj, &gSYTaskmanDLHeads[multi_list->id]);

                            goto set_display_list;
                        }
                        else gSPSegment(gSYTaskmanDLHeads[multi_list->id]++, 0xE, ptr);
                    }
                set_display_list:
                    gSPDisplayList(gSYTaskmanDLHeads[multi_list->id]++, PORT_RESOLVE(multi_list->dl2));
                }
                multi_list++;
            }
        }
        if (dobj->child != NULL)
        {
            func_80015520(dobj->child);
        }
        sGCCurrentDL = dl;

        for (i = 0; i < ARRAY_COUNT(sGCForwardDLs); i++)
        {
            if (sGCForwardDLs[i] > sGCCurrentDL)
            {
                sGCForwardDLs[i] = sGCCurrentDL;

                if (num != 0)
                {
                    if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                    {
                        gSPPopMatrix(gSYTaskmanDLHeads[i]++, G_MTX_MODELVIEW);
                    }
                }
                else continue;
            }
        }
        gGCScaleX = bak;
    }
    if (dobj->sib_prev == NULL)
    {
        current_dobj = dobj->sib_next;

        while (current_dobj != NULL)
        {
            func_80015520(current_dobj);
            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x80015860
void unref_80015860(GObj *gobj) 
{
    gGCScaleX = 1.0F;
    func_80015520(DObjGetStruct(gobj));
}

// 0x80015890
void gcDrawDObjTreeDLDoubleArray(DObj *dobj)
{
    s32 num;
    DObj *current_dobj;
    f32 bak;
    void *dls;
    void *p_dls;
    Gfx *dl0;
    Gfx *dl1;

    p_dls = dobj->dv;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN)) 
    {
        bak = gGCScaleX;

        dls = PORT_RESOLVE_ARRAY(p_dls, sGCDetailLevel);
        dl0 = PORT_RESOLVE_ARRAY(dls, 0);
        dl1 = PORT_RESOLVE_ARRAY(dls, 1);

        if ((dl0 != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dl0);
        }
        num = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

        if ((dl1 != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dl1);
        }
        if (dobj->child != NULL)
        {
            gcDrawDObjTreeDLDoubleArray(dobj->child); 
        }
        if (num != 0)
        {
            if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
            {
                gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
            }
        }
        gGCScaleX = bak;
    }
    if (dobj->sib_prev == NULL)
    {
        current_dobj = dobj->sib_next;

        while (current_dobj != NULL) 
        {
            gcDrawDObjTreeDLDoubleArray(current_dobj);
            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x80015A58
void unref_80015A58(GObj *gobj)
{
    DObjDistDL *dist_dl;
    s32 num;
    f32 dist;
    DObj *dobj;
    DObj *current_dobj;

    dobj = DObjGetStruct(gobj);

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        dist_dl = dobj->dist_dl;

        if (dist_dl != NULL)
        {
            gGCScaleX = 1.0F;
            sGCDetailLevel = 0;

            dist = gcGetDObjDistFromEye(dobj);

            while (dist < dist_dl->target_dist)
            {
                sGCDetailLevel++;
                dist_dl++;
            }
            num = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

            if ((dist_dl->dl != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
            {
                gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
                gSPDisplayList(gSYTaskmanDLHeads[0]++, PORT_RESOLVE(dist_dl->dl));
            }
            if (dobj->child != NULL)
            {
                gcDrawDObjTreeDLDoubleArray(dobj->child);
            }
            if (num != 0)
            {
                if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                {
                    gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
                }
            }
            if (dobj->sib_prev == NULL)
            {
                current_dobj = dobj->sib_next;

                while (current_dobj != NULL)
                {
                    gcDrawDObjTreeDLDoubleArray(current_dobj);

                    current_dobj = current_dobj->sib_next;
                }
            }
        }
    }
    else return;
}

// 0x80015C0C
void func_80015C0C(DObj *dobj) 
{
    void *ptr;
    s32 num;
    DObjMultiList **p_multi_list;
    DObjMultiList *multi_list;
    Gfx *dl;
    s32 i;
    DObj *current_dobj;
    f32 bak;

    ptr = NULL;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        bak = gGCScaleX;
        p_multi_list = (DObjMultiList**)dobj->dv;

        if (p_multi_list != NULL) 
        {
            multi_list = p_multi_list[sGCDetailLevel]; 
        }
        dl = sGCCurrentDL;
        num  = gcPrepDObjMatrix(&sGCCurrentDL, dobj);

        if ((p_multi_list != NULL) && (multi_list != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            while (multi_list->id != ARRAY_COUNT(gSYTaskmanDLHeads))
            {
                if (multi_list->dl2 != NULL) 
                {
                    if (multi_list->dl1 != NULL)
                    { 
                        gSPDisplayList(gSYTaskmanDLHeads[multi_list->id]++, PORT_RESOLVE(multi_list->dl1));
                    }
                    while (sGCCurrentDL != sGCForwardDLs[multi_list->id]) 
                    {
                        *gSYTaskmanDLHeads[multi_list->id]++ = *sGCForwardDLs[multi_list->id]++;
                    }
                    if (dobj->mobj != NULL) 
                    {
                        if (ptr == NULL) 
                        {
                            ptr = gSYTaskmanGraphicsHeap.ptr;
                            gcDrawMObjForDObj(dobj, &gSYTaskmanDLHeads[multi_list->id]);

                            goto set_display_list;
                        }
                        else gSPSegment(gSYTaskmanDLHeads[multi_list->id]++, 0xE, ptr);
                    }
                set_display_list:
                    gSPDisplayList(gSYTaskmanDLHeads[multi_list->id]++, PORT_RESOLVE(multi_list->dl2));
                }
                multi_list++;
            }
        }
        if (dobj->child != NULL) 
        { 
            func_80015C0C(dobj->child);
        }
        sGCCurrentDL = dl;

        for (i = 0; i < ARRAY_COUNT(gSYTaskmanDLHeads); i++) 
        {
            if (sGCForwardDLs[i] > sGCCurrentDL)
            {
                sGCForwardDLs[i] = sGCCurrentDL;

                if (num != 0)
                {
                    if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                    {
                        gSPPopMatrix(gSYTaskmanDLHeads[i]++, G_MTX_MODELVIEW);
                    }
                }
                continue; // Not required this time; this is for the sake of consistency.
            }
        }
        gGCScaleX = bak;
    }
    if (dobj->sib_prev == NULL)
    {
        current_dobj = dobj->sib_next;

        while (current_dobj != NULL) 
        {
            func_80015C0C(current_dobj);

            current_dobj = current_dobj->sib_next;
        }
    }
}

// 0x80015F6C
void unref_80015F6C(GObj *gobj)
{
    f32 dist;
    s32 i;
    s32 num;
    DObj *dobj;
    void *ptr;
    DObjDistDLLink *dist_dl_link;
    DObjDLLink *dl_link;
    Gfx *dl;
    DObj *current_dobj;

    dobj = DObjGetStruct(gobj);
    ptr = NULL;

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        dist_dl_link = dobj->dist_dl_link;

        if (dist_dl_link != NULL)
        {
            gGCScaleX = 1.0F;
            sGCDetailLevel = 0;
            dist = gcGetDObjDistFromEye(dobj);

            while (dist < dist_dl_link->target_dist)
            {
                dist_dl_link++;
                sGCDetailLevel++;
            }
            dl_link = PORT_RESOLVE(dist_dl_link->dl_link);
            dl = sGCCurrentDL;
            num = gcPrepDObjMatrix(&sGCCurrentDL, dobj);

            if ((dl_link != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
            {
                while (dl_link->list_id != ARRAY_COUNT(gSYTaskmanDLHeads))
                {
                    if (dl_link->dl != NULL)
                    {
                        while (sGCCurrentDL != sGCForwardDLs[dl_link->list_id])
                        {
                            *gSYTaskmanDLHeads[dl_link->list_id]++ = *sGCForwardDLs[dl_link->list_id]++;
                        }
                        if (dobj->mobj != NULL)
                        {
                            if (ptr == NULL)
                            {
                                ptr = gSYTaskmanGraphicsHeap.ptr;
                                gcDrawMObjForDObj(dobj, &gSYTaskmanDLHeads[dl_link->list_id]);

                                goto set_display_list;
                            }
                            else gSPSegment(gSYTaskmanDLHeads[dl_link->list_id]++, 0xE, ptr);
                        }
                    set_display_list:
                        gSPDisplayList(gSYTaskmanDLHeads[dl_link->list_id]++, PORT_RESOLVE(dl_link->dl));
                    }
                    dl_link++;
                }
            }
            if (dobj->child != NULL)
            {
                func_80015C0C(dobj->child);
            }
            sGCCurrentDL = dl;

            for (i = 0; i < ARRAY_COUNT(sGCForwardDLs); i++)
            {
                if (sGCForwardDLs[i] > sGCCurrentDL)
                {
                    sGCForwardDLs[i] = sGCCurrentDL;

                    if (num != 0)
                    {
                        if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
                        {
                            gSPPopMatrix(gSYTaskmanDLHeads[i]++, G_MTX_MODELVIEW);
                        }
                    }
                    else continue;
                }
            }
            if (dobj->sib_prev == NULL)
            {
                current_dobj = dobj->sib_next;

                while (current_dobj != NULL)
                {
                    func_80015C0C(current_dobj);
                    current_dobj = current_dobj->sib_next;
                }
            }
        }
    }
}

#ifndef PORT
// 0x800162C8 — Unreferenced N64 sprite draw via spDraw; uses rsp_dl_next
// as a runtime Gfx* which is incompatible with u32 token fields on PORT.
void unref_800162C8(GObj *gobj)
{
    SObj *sobj = SObjGetStruct(gobj);

    while (sobj != NULL)
    {
        if (!(sobj->sprite.attr & SP_HIDDEN))
        {
            sobj->sprite.rsp_dl_next = gSYTaskmanDLHeads[0];

            spDraw(&sobj->sprite);

            gSYTaskmanDLHeads[0] = sobj->sprite.rsp_dl_next - 1;
        }
        sobj = sobj->next;
    }
}
#endif

// 0x80016338
void func_80016338(Gfx **dls, CObj *cobj, s32 buffer_id)
{
    Vp_t *viewport = &cobj->viewport.vp;
    Gfx *dl = dls[0];
    s32 ulx, uly, lrx, lry;

    if ((buffer_id == 0) || (buffer_id == 1))
    {
        if (cobj->flags & 0x20)
        {
            syTaskmanAppendGfxUcodeLoad(dls, D_80046626);
            D_80046628 = 1;

            dl = dls[0];
        }
    }
    gSPViewport(dl++, viewport);

    ulx = (viewport->vtrans[0] / 4) - (viewport->vscale[0] / 4);
    uly = (viewport->vtrans[1] / 4) - (viewport->vscale[1] / 4);

    lrx = (viewport->vtrans[0] / 4) + (viewport->vscale[0] / 4);
    lry = (viewport->vtrans[1] / 4) + (viewport->vscale[1] / 4);

    if (ulx < (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorLeft)
    {
        ulx = (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorLeft;
    }
    if (uly < (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorTop)
    {
        uly = (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorTop;
    }
    if (lrx > gSYVideoResWidth - ((gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorRight))
    {
        lrx = gSYVideoResWidth - ((gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorRight);
    }
    if (lry > gSYVideoResHeight - ((gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorBottom))
    {
        lry = gSYVideoResHeight - ((gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorBottom);
    }
    gDPSetScissor(dl++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    gDPPipeSync(dl++);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, gSYVideoColorDepth, gSYVideoResWidth, (void*)0x0F000000);
    gDPSetCycleType(dl++, G_CYC_1CYCLE);

    if ((buffer_id == 0) || (buffer_id == 2))
    {
        gDPSetRenderMode(dl++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    }
    else gDPSetRenderMode(dl++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    dls[0] = dl;
}

// 0x8001663C
void func_8001663C(Gfx **dls, CObj *cobj, s32 buffer_id)
{
    Gfx *dl = dls[0];
    Vp_t *viewport = &cobj->viewport.vp;
    s32 ulx, uly, lrx, lry;

    if ((buffer_id == 0) || (buffer_id == 1))
    {
        if (cobj->flags & 0x20)
        {
            syTaskmanAppendGfxUcodeLoad(dls, D_80046626);
            D_80046628 = 1;

            dl = dls[0];
        }
    }
    gSPViewport(dl++, viewport);

    ulx = (viewport->vtrans[0] / 4) - (viewport->vscale[0] / 4);
    uly = (viewport->vtrans[1] / 4) - (viewport->vscale[1] / 4);
    lrx = (viewport->vtrans[0] / 4) + (viewport->vscale[0] / 4);
    lry = (viewport->vtrans[1] / 4) + (viewport->vscale[1] / 4);

    if (ulx < (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorLeft)
    {
        ulx = (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorLeft;
    }
    if (uly < (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorTop)
    {
        uly = (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorTop;
    }
    if (lrx > gSYVideoResWidth - ((gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorRight))
    {
        lrx = gSYVideoResWidth - ((gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorRight);
    }
    if (lry > gSYVideoResHeight - ((gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorBottom))
    {
        lry = gSYVideoResHeight - ((gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorBottom);
    }
    gDPSetScissor(dl++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

    lrx--, lry--;

    if (cobj->flags & COBJ_FLAG_ZBUFFER)
    {
        gDPPipeSync(dl++);
        gDPSetCycleType(dl++, G_CYC_FILL);
        gDPSetRenderMode(dl++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gSYVideoResWidth, gSYVideoZBuffer);
        gDPSetFillColor(dl++, GPACK_FILL16(GPACK_ZDZ(G_MAXFBZ, 0)));
        gDPFillRectangle(dl++, ulx, uly, lrx, lry);
    }
    gDPPipeSync(dl++);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, gSYVideoColorDepth, gSYVideoResWidth, (void*)0x0F000000);

    if (cobj->flags & COBJ_FLAG_FILLCOLOR)
    {
        gDPSetCycleType(dl++, G_CYC_FILL);
        gDPSetRenderMode(dl++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(dl++, syVideoGetFillColor(cobj->color));
        gDPFillRectangle(dl++, ulx, uly, lrx, lry);
    }
    gDPPipeSync(dl++);
    gDPSetCycleType(dl++, G_CYC_1CYCLE);

    if ((buffer_id == 0) || (buffer_id == 2))
    {
        gDPSetRenderMode(dl++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    }
    else gDPSetRenderMode(dl++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    dls[0] = dl;
}

// 0x80016AE4
void unref_80016AE4(Gfx **dls, CObj *cobj, s32 arg2, void *image, s32 max_lrx, s32 max_lry, void *depth)
{
    Gfx *dl = dls[0];
    Vp_t *viewport = &cobj->viewport.vp;
    s32 ulx, uly, lrx, lry;

    gSPViewport(dl++, viewport);

    ulx = (viewport->vtrans[0] / 4) - (viewport->vscale[0] / 4);
    uly = (viewport->vtrans[1] / 4) - (viewport->vscale[1] / 4);
    lrx = (viewport->vtrans[0] / 4) + (viewport->vscale[0] / 4);
    lry = (viewport->vtrans[1] / 4) + (viewport->vscale[1] / 4);

    if (ulx < 0)
    {
        ulx = 0;
    }
    if (uly < 0)
    {
        uly = 0;
    }
    if (lrx > max_lrx)
    {
        lrx = max_lrx;
    }
    if (lry > max_lry)
    {
        lry = max_lry;
    }
    gDPSetScissor(dl++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

    lrx--, lry--;

    if (cobj->flags & COBJ_FLAG_ZBUFFER)
    {
        gDPPipeSync(dl++);
        gDPSetCycleType(dl++, G_CYC_FILL);
        gDPSetRenderMode(dl++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, max_lrx, depth);
        gDPSetFillColor(dl++, GPACK_FILL16(GPACK_ZDZ(G_MAXFBZ, 0)));
        gDPFillRectangle(dl++, ulx, uly, lrx, lry);
    }
    gDPPipeSync(dl++);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, gSYVideoColorDepth, max_lrx, image);
    gDPSetDepthImage(dl++, depth);

    if (cobj->flags & COBJ_FLAG_FILLCOLOR)
    {
        gDPSetCycleType(dl++, G_CYC_FILL);
        gDPSetRenderMode(dl++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(dl++, syVideoGetFillColor(cobj->color));
        gDPFillRectangle(dl++, ulx, uly, lrx, lry);
    }
    gDPPipeSync(dl++);
    gDPSetCycleType(dl++, G_CYC_1CYCLE);

    if ((arg2 == 0) || (arg2 == 2))
    {
        gDPSetRenderMode(dl++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    }
    else gDPSetRenderMode(dl++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    dls[0] = dl;
}

// 0x80016EDC
void gcPrepCameraMatrix(Gfx **dls, CObj *cobj)
{
    Gfx *dl;
    s32 i;
    XObj *xobj;
    SYMatrixHub mtx_hub;
    s32 var_s3;
    s32 spC8;
    LookAt *look_at;

    dl = dls[0];
    spC8 = 0;
    var_s3 = 0;

    if (cobj->xobjs_num != 0)
    {
        for (i = 0; i < cobj->xobjs_num; i++)
        {
            xobj = cobj->xobjs[i];

            if (xobj != NULL)
            {
                mtx_hub.gbi = &xobj->mtx;

                if (xobj->unk05 != 2)
                {
                    if (gSYTaskmanTaskID > 0)
                    {
                        mtx_hub.gbi = gSYTaskmanGraphicsHeap.ptr;
                        gSYTaskmanGraphicsHeap.ptr = mtx_hub.gbi + 1;
                    }
                    switch (xobj->kind)
                    {
                    case 1:
                        break;

                    case 2:
                        break;

                    case nGCMatrixKindPerspFastF:
                        syMatrixPerspFastF
                        (
                            gGCMatrixPerspF,
                            &cobj->projection.persp.norm,
                            cobj->projection.persp.fovy,
                            cobj->projection.persp.aspect,
                            cobj->projection.persp.near,
                            cobj->projection.persp.far,
                            cobj->projection.persp.scale
                        );
                        syMatrixF2L(&gGCMatrixPerspF, mtx_hub.gbi);
                        sGCMatrixProjectL = mtx_hub.gbi;
                        break;

                    case nGCMatrixKindPerspF:
                        syMatrixPerspF
                        (
                            gGCMatrixPerspF,
                            &cobj->projection.persp.norm,
                            cobj->projection.persp.fovy,
                            cobj->projection.persp.aspect,
                            cobj->projection.persp.near,
                            cobj->projection.persp.far,
                            cobj->projection.persp.scale
                        );
                        syMatrixF2L(&gGCMatrixPerspF, mtx_hub.gbi);
                        sGCMatrixProjectL = mtx_hub.gbi;
                        break;

                    case nGCMatrixKindOrtho:
                        syMatrixOrtho
                        (
                            mtx_hub.gbi,
                            cobj->projection.ortho.l,
                            cobj->projection.ortho.r,
                            cobj->projection.ortho.b,
                            cobj->projection.ortho.t,
                            cobj->projection.ortho.n,
                            cobj->projection.ortho.f,
                            cobj->projection.ortho.scale
                        );
                        sGCMatrixProjectL = mtx_hub.gbi;
                        break;

                    case 6:
                    case 7:
                        syMatrixLookAt
                        (
                            mtx_hub.gbi,
                            cobj->vec.eye.x,
                            cobj->vec.eye.y,
                            cobj->vec.eye.z,
                            cobj->vec.at.x,
                            cobj->vec.at.y,
                            cobj->vec.at.z,
                            cobj->vec.up.x,
                            cobj->vec.up.y,
                            cobj->vec.up.z
                        );
                        var_s3 = (cobj->vec.up.z < cobj->vec.up.y) ? 1 : 2;
                        break;

                    case 8:
                    case 9:
                        syMatrixModLookAt(mtx_hub.gbi, cobj->vec.eye.x, cobj->vec.eye.y, cobj->vec.eye.z, cobj->vec.at.x, cobj->vec.at.y, cobj->vec.at.z, cobj->vec.up.x, 0.0F, 1.0F, 0.0F);
                        var_s3 = 1;
                        break;

                    case 10:
                    case 11:
                        syMatrixModLookAt(mtx_hub.gbi, cobj->vec.eye.x, cobj->vec.eye.y, cobj->vec.eye.z, cobj->vec.at.x, cobj->vec.at.y, cobj->vec.at.z, cobj->vec.up.x, 0.0F, 0.0F, 1.0F);
                        var_s3 = 2;
                        break;

                    case 12:
                    case 13:
                        look_at = syMallocSet(&gSYTaskmanGraphicsHeap, sizeof(LookAt), 0x8);
                        syMatrixLookAtReflect
                        (
                            mtx_hub.gbi,
                            look_at,
                            cobj->vec.eye.x,
                            cobj->vec.eye.y,
                            cobj->vec.eye.z,
                            cobj->vec.at.x,
                            cobj->vec.at.y,
                            cobj->vec.at.z,
                            cobj->vec.up.x,
                            cobj->vec.up.y,
                            cobj->vec.up.z
                        );
                        var_s3 = (cobj->vec.up.z < cobj->vec.up.y) ? 1 : 2;
                        break;

                    case 14:
                    case 15:
                        look_at = syMallocSet(&gSYTaskmanGraphicsHeap, sizeof(LookAt), 0x8);
                        var_s3 = 1;
                        syMatrixModLookAtReflect(mtx_hub.gbi, look_at, cobj->vec.eye.x, cobj->vec.eye.y, cobj->vec.eye.z, cobj->vec.at.x, cobj->vec.at.y, cobj->vec.at.z, cobj->vec.up.x, 0.0F, 1.0F, 0.0F);
                        break;

                    case 16:
                    case 17:
                        look_at = syMallocSet(&gSYTaskmanGraphicsHeap, sizeof(LookAt), 0x8);
                        var_s3 = 2;
                        syMatrixModLookAtReflect(mtx_hub.gbi, look_at, cobj->vec.eye.x, cobj->vec.eye.y, cobj->vec.eye.z, cobj->vec.at.x, cobj->vec.at.y, cobj->vec.at.z, cobj->vec.up.x, 0.0F, 0.0F, 1.0F);
                        break;

                    default:
                        if ((xobj->kind >= 66) && (sGCMatrixFuncList != NULL))
                        {
                            if (sGCMatrixFuncList[xobj->kind - 66].proc_diff != NULL)
                            {
                                sGCMatrixFuncList[xobj->kind - 66].proc_diff(mtx_hub.gbi, cobj, &dl);
                            }
                        }
                        break;
                    }
                    if ((xobj->unk05 == 1) && (&xobj->mtx == mtx_hub.gbi))
                    {
                        xobj->unk05 = 2;
                    }
                }
                switch (xobj->kind)
                {
                case 1:
                    break;

                case 2:
                    break;

                case nGCMatrixKindPerspFastF:
                case nGCMatrixKindPerspF:
                    gSPMatrix(dl++, mtx_hub.gbi, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
                    gSPPerspNormalize(dl++, cobj->projection.persp.norm);
                    break;

                case nGCMatrixKindOrtho:
                    gSPMatrix(dl++, mtx_hub.gbi, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
                    break;

                case 12:
                case 14:
                case 16:
                    gSPLookAtX(dl++, &look_at->l[0]);
                    gSPLookAtY(dl++, &look_at->l[1]);
                    /* fallthrough */
                case 6:
                case 8:
                case 10:
                    gSPMatrix(dl++, mtx_hub.gbi, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
                    break;

                case 13:
                case 15:
                case 17:
                    gSPLookAtX(dl++, &look_at->l[0]);
                    gSPLookAtY(dl++, &look_at->l[1]);
                    /* fallthrough */
                case 7:
                case 9:
                case 11:
                    gSPMatrix(dl++, mtx_hub.gbi, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    break;

                default:
                    if ((xobj->kind >= 66) && (sGCMatrixFuncList != NULL))
                    {
                        if (sGCMatrixFuncList[xobj->kind - 66].proc_same != NULL)
                        {
                            sGCMatrixFuncList[xobj->kind - 66].proc_same(mtx_hub.gbi, cobj, &dl);
                        }
                    }
                    break;
                }
            }

        }
        switch (sGCCameraMatrixMode)
        {
        case 0:
            spC8 = var_s3;
            break;

        case 1:
            var_s3 = 0;
            break;

        case 2:
            spC8 = 1;
            var_s3 = 1;
            break;

        case 3:
            var_s3 = 1;
            break;

        case 4:
            spC8 = 1;
            var_s3 = 0;
            break;

        case 5:
            spC8 = 2;
            var_s3 = 2;
            break;

        case 6:
            var_s3 = 2;
            break;

        case 7:
            spC8 = 2;
            var_s3 = 0;
            break;
        }
        if (var_s3 != 0)
        {
            f32 eye_z, eye_y, at_y;

            switch (var_s3)
            {
            case 1:
                eye_z = sqrtf(SQUARE(cobj->vec.at.z - cobj->vec.eye.z) + SQUARE(cobj->vec.at.x - cobj->vec.eye.x));
                eye_y = cobj->vec.eye.y;
                at_y = cobj->vec.at.y;
                break;

            case 2:
                eye_z = sqrtf(SQUARE(cobj->vec.at.y - cobj->vec.eye.y) + SQUARE(cobj->vec.at.x - cobj->vec.eye.x));
                eye_y = cobj->vec.eye.z;
                at_y = cobj->vec.at.z;
                break;
            }
            if (eye_z < 0.0001F)
            {
                syMatrixScaF(&sGCMatrixMod1F, 0.0F, 0.0F, 0.0F);
            }
            else
            {
                syMatrixLookAtF(&sGCMatrixMod1F, 0.0F, eye_y, eye_z, 0.0F, at_y, 0.0F, 0.0F, 1.0F, 0.0F);
                guMtxCatF(sGCMatrixMod1F, gGCMatrixPerspF, sGCMatrixMod1F);
            }
        }
        if (spC8 != 0)
        {
            f32 eye_z, eye_x, at_x;

            switch (spC8)
            {
            case 1:
                eye_z = sqrtf(SQUARE(cobj->vec.at.y - cobj->vec.eye.y) + SQUARE(cobj->vec.at.z - cobj->vec.eye.z));
                eye_x = cobj->vec.eye.x;
                at_x = cobj->vec.at.x;
                break;

            case 2:
                eye_z = sqrtf(SQUARE(cobj->vec.at.z - cobj->vec.eye.z) + SQUARE(cobj->vec.at.x - cobj->vec.eye.x));
                eye_x = cobj->vec.eye.y;
                at_x = cobj->vec.at.y;
                break;
            }
            if (eye_z < 0.0001F)
            {
                syMatrixScaF(&sGCMatrixMod2F, 0.0F, 0.0F, 0.0F);
            }
            else
            {
                syMatrixLookAtF(&sGCMatrixMod2F, eye_x, 0.0F, eye_z, at_x, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F);
                guMtxCatF(sGCMatrixMod2F, gGCMatrixPerspF, sGCMatrixMod2F);
            }
        }
        dls[0] = dl;
    }
}

// 0x80017830
void gcSetCameraMatrixMode(s32 val)
{
    sGCCameraMatrixMode = val;
}

// the second arg may just be unused
void gcRunFuncCamera(CObj *cobj, s32 dl_id)
{
    if (cobj->func_camera != NULL)
    { 
        cobj->func_camera(cobj, dl_id);
    }
}

// 0x80017868
void gcCaptureTaggedGObjs(GObj *camera_gobj, s32 link_id, sb32 is_tag_mask_or_id)
{
    GObj *current_gobj = gGCCommonDLLinks[link_id];

    while (current_gobj != NULL)
    {
        if (!(current_gobj->flags & GOBJ_FLAG_HIDDEN))
        {
            if
            (
                ((is_tag_mask_or_id == 0) && (camera_gobj->camera_tag &  current_gobj->camera_tag)) ||
                ((is_tag_mask_or_id == 1) && (camera_gobj->camera_tag == current_gobj->camera_tag))
            )
            {
                dGCCurrentStatus = nGCStatusDisplaying;
                gGCCurrentDisplay = current_gobj;

                current_gobj->proc_display(current_gobj);
                dGCCurrentStatus = nGCStatusCapturing;

                current_gobj->frame_draw_last = dSYTaskmanFrameCount;
            }
        }
        current_gobj = current_gobj->dl_link_next;
    }
}

// 0x80017978
void gcCaptureDoubleBufferGObjs(GObj *camera_gobj, s32 id, sb32 is_tag_mask_or_id)
{
    Gfx *dls[ARRAY_COUNT(gSYTaskmanDLHeads)];
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSYTaskmanDLHeads); i++)
    {
        dls[i] = gSYTaskmanDLHeads[i];

        // Reserve space for 2 commands: gSPDisplayList and gSPBranchList
        gSYTaskmanDLHeads[i] += 2;
    }
    gcCaptureTaggedGObjs(camera_gobj, id, is_tag_mask_or_id);

    for (i = 0; i < ARRAY_COUNT(gSYTaskmanDLHeads); i++)
    {
        if (gSYTaskmanDLHeads[i] == dls[i] + 2)
        {
            // Nothing added to this Display List
            gSYTaskmanDLHeads[i] -= 2;
            gGCFrameQueueGfxLinks[id].dls[i] = NULL;
        }
        else
        {
            gSPEndDisplayList(gSYTaskmanDLHeads[i]++);

            gSPDisplayList(dls[i], dls[i] + 2);
            dls[i]++;
            gSPBranchList(dls[i]++, gSYTaskmanDLHeads[i]);
            gGCFrameQueueGfxLinks[id].dls[i] = dls[i];
        }
    }
    gGCFrameQueueGfxLinks[id].frame = dSYTaskmanFrameCount;
}

// 0x80017AAC
void gcAddLinkedDL(s32 id)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSYTaskmanDLHeads); i++) 
    {
        if (gGCFrameQueueGfxLinks[id].dls[i] != NULL)
        {
            gSPDisplayList(gSYTaskmanDLHeads[i]++, gGCFrameQueueGfxLinks[id].dls[i]);
        }
    }
}

// 0x80017B80
void gcCaptureCameraGObj(GObj *camera_gobj, sb32 is_tag_mask_or_id)
{
    s32 id = 0;
    u64 camera_mask = camera_gobj->camera_mask;
    u64 buffer_mask = camera_gobj->buffer_mask;

    while (camera_mask)
    {
        if (camera_mask & 1)
        {
            if (buffer_mask & 1)
            {
                if ((u8)dSYTaskmanFrameCount == gGCFrameQueueGfxLinks[id].frame)
                {
                    gcAddLinkedDL(id);
                } 
                else gcCaptureDoubleBufferGObjs(camera_gobj, id, is_tag_mask_or_id);
            } 
            else gcCaptureTaggedGObjs(camera_gobj, id, is_tag_mask_or_id);
        }
        camera_mask >>= 1;
        buffer_mask >>= 1;
        id++;
    }
}

// 0x80017CC8
void func_80017CC8(CObj *cobj) 
{
    if (cobj->flags & COBJ_FLAG_DLBUFFERS)
    {
        syTaskmanUpdateDLBuffers(); 
    }
    if (cobj->flags & COBJ_FLAG_GFXEND) 
    {
        func_800053CC();
        func_80004F78();
    }
    if (cobj->flags & COBJ_FLAG_BRANCHSYNC)
    { 
        func_800053CC();
    }
}

// 0x80017D3C
void func_80017D3C(GObj *gobj, Gfx **dls, s32 id)
{
    CObj *cobj = CObjGetStruct(gobj);

    func_8001663C(dls, cobj, id);
    gcPrepCameraMatrix(dls, cobj);
    gcRunFuncCamera(cobj, id);
    gcCaptureCameraGObj(gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? TRUE : FALSE);
    func_80017CC8(cobj);
}

// 0x80017DBC
void func_80017DBC(GObj *gobj) 
{
    func_80017D3C(gobj, &gSYTaskmanDLHeads[0], 0);
}

// 0x80017DE4
void unref_80017DE4(GObj *gobj)
{
    func_80017D3C(gobj, &gSYTaskmanDLHeads[1], 1);
}

// 0x80017E0C
void unref_80017E0C(GObj *gobj) 
{
    func_80017D3C(gobj, &gSYTaskmanDLHeads[2], 2);
}

// 0x80017E34
void unref_80017E34(GObj *gobj)
{
    func_80017D3C(gobj, &gSYTaskmanDLHeads[3], 3);
}

// 0x80017E5C
void unref_80017E5C(void) 
{
    CObj *cobj = CObjGetStruct(gGCCurrentCamera);

    func_800053CC();
    func_80004F78();
    func_8001663C(gSYTaskmanDLHeads, cobj, 0);
    gcPrepCameraMatrix(gSYTaskmanDLHeads, cobj);
    gcRunFuncCamera(cobj, 0);
}

// 0x80017EC0
void func_80017EC0(GObj *gobj)
{
    CObj *cobj = CObjGetStruct(gobj);
    s32 i;

    func_8001663C(gSYTaskmanDLHeads, cobj, 0);
    sGCCameraDL = gSYTaskmanDLHeads[0] + 1;
    gSPDisplayList(gSYTaskmanDLHeads[0], gSYTaskmanDLHeads[0] + 2);
    gSYTaskmanDLHeads[0] += 2;

    gcPrepCameraMatrix(gSYTaskmanDLHeads, cobj);
    gSPEndDisplayList(gSYTaskmanDLHeads[0]++);
    gSPBranchList(sGCCameraDL, gSYTaskmanDLHeads[0]);

    gcRunFuncCamera(cobj, 0);

    if (cobj->flags & 0x20)
    {
        func_80016338(&gSYTaskmanDLHeads[1], cobj, 1);
    }
    for (i = 1; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sGCBufferDLs)) / 2; i++)
    {
        sGCBufferDLs[i] = ++gSYTaskmanDLHeads[i];
    }
    gcCaptureCameraGObj(gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? TRUE : FALSE);

    for (i = 1; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sGCBufferDLs)) / 2; i++)
    {
        if (sGCBufferDLs[i] == gSYTaskmanDLHeads[i])
        {
            gSYTaskmanDLHeads[i]--;
        }
        else
        {
            Gfx *start = gSYTaskmanDLHeads[i]++;

            gSPDisplayList(sGCBufferDLs[i] - 1, gSYTaskmanDLHeads[i]);

            if ((i != 1) || !(cobj->flags & 0x20))
            {
                func_80016338(&gSYTaskmanDLHeads[i], cobj, i);
            }
            gSPDisplayList(gSYTaskmanDLHeads[i]++, sGCCameraDL + 1);
            gcRunFuncCamera(cobj, i);
            gSPEndDisplayList(gSYTaskmanDLHeads[i]++);
            gSPBranchList(start, gSYTaskmanDLHeads[i]);
        }
    }
    func_80017CC8(cobj);
}

// 0x8001810C
void unref_8001810C(void)
{
    CObj *cobj = CObjGetStruct(gGCCurrentCamera);
    s32 i;

    for (i = 1; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sGCBufferDLs)) / 2; i++)
    {
        if (sGCBufferDLs[i] == gSYTaskmanDLHeads[i])
        {
            gSYTaskmanDLHeads[i]--;
        }
        else
        {
            Gfx *start = gSYTaskmanDLHeads[i]++;

            gSPDisplayList(sGCBufferDLs[i] - 1, gSYTaskmanDLHeads[i]);
            func_80016338(&gSYTaskmanDLHeads[i], cobj, i);
            gSPDisplayList(gSYTaskmanDLHeads[i]++, sGCCameraDL + 1);
            gcRunFuncCamera(cobj, i);
            gSPEndDisplayList(gSYTaskmanDLHeads[i]++);
            gSPBranchList(start, gSYTaskmanDLHeads[i]);
        }
    }
    func_800053CC();
    func_80004F78();
    func_8001663C(&gSYTaskmanDLHeads[0], cobj, 0);

    sGCCameraDL = gSYTaskmanDLHeads[0] + 1;

    gSPDisplayList(gSYTaskmanDLHeads[0], gSYTaskmanDLHeads[0] + 2);

    gSYTaskmanDLHeads[0] += 2;

    gcPrepCameraMatrix(gSYTaskmanDLHeads, cobj);
    gSPEndDisplayList(gSYTaskmanDLHeads[0]++);
    gSPBranchList(sGCCameraDL, gSYTaskmanDLHeads[0]);

    gcRunFuncCamera(cobj, 0);

    for (i = 1; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sGCBufferDLs)) / 2; i++)
    {
        sGCBufferDLs[i] = ++gSYTaskmanDLHeads[i];
    }
}

// 0x80018300
void func_80018300(GObj *gobj)
{
    CObj *cobj = CObjGetStruct(gobj);
    Vp_t *viewport = &cobj->viewport.vp;
    s32 xmin = (viewport->vtrans[0] / 4) - (viewport->vscale[0] / 4);
    s32 ymin = (viewport->vtrans[1] / 4) - (viewport->vscale[1] / 4);
    s32 xmax = (viewport->vtrans[0] / 4) + (viewport->vscale[0] / 4);
    s32 ymax = (viewport->vtrans[1] / 4) + (viewport->vscale[1] / 4);

    if (xmin < (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorLeft)
    {
        xmin = (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorLeft;
    }
    if (ymin < (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorTop)
    {
        ymin = (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorTop;
    }
    if (xmax > gSYVideoResWidth - ((gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorRight))
    {
        xmax = gSYVideoResWidth - ((gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT) * dGCCameraScissorRight);
    }
    if (ymax > gSYVideoResHeight - ((gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorBottom))
    {
        ymax = gSYVideoResHeight - ((gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT) * dGCCameraScissorBottom);
    }
    func_8001663C(gSYTaskmanDLHeads, cobj, 0);
    spInit(gSYTaskmanDLHeads);
    spScissor(xmin, xmax, ymin, ymax);
    gcCaptureCameraGObj(gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? TRUE : FALSE);
    spFinish(gSYTaskmanDLHeads);

    gSYTaskmanDLHeads[0]--;

    gDPSetTexturePersp(gSYTaskmanDLHeads[0]++, G_TP_PERSP);
}

#pragma GCC diagnostic pop
