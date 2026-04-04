#ifndef _OBJTYPES_H_
#define _OBJTYPES_H_

#include <macros.h>
#include <config.h>
#include <ssb_types.h>
#include <PR/ultratypes.h>
#include <PR/mbi.h>
#include <PR/os.h>
#include <PR/sp.h>
#include <PR/gu.h>

#include <sys/utils.h>
#include <sys/vector.h>
#include <sys/taskman.h>

#include <sys/objdef.h>

// General OM defines
#define GC_COMMON_MAX_LINKS         33
#define GC_COMMON_MAX_DLLINKS       65

// GObj defines
#define GOBJ_FLAG_NONE              (0)
#define GOBJ_FLAG_HIDDEN            (1 << 0)
#define GOBJ_FLAG_NOANIM            (1 << 1)            // Skip applying aninmation values?
#define GOBJ_FLAG_NORUN            (1 << 6)            // I actually don't know what this really does

#define GOBJ_PRIORITY_DEFAULT       S32_MIN

// DObj defines
#define DOBJ_PARENT_NULL            ((DObj*)1)

#define DOBJ_FLAG_NONE              (0)
#define DOBJ_FLAG_NOTEXTURE         (1 << 0)
#define DOBJ_FLAG_HIDDEN            (1 << 1)

#define DOBJ_ARRAY_MAX              18

// AObj defines
#define AOBJ_ANIM_NULL              F32_MIN             // Used to mark the lack of frames remaining in an animation
#define AOBJ_ANIM_CHANGED           (F32_MIN / 2.0F)    // ???
#define AOBJ_ANIM_END               (F32_MIN / 3.0F)    // Used to mark the end of an animation?

// MObj defines - some of these are wild guesses...
#define MOBJ_FLAG_NONE              (0)
#define MOBJ_FLAG_ALPHA             (1 << 0)
#define MOBJ_FLAG_SPLIT             (1 << 1)    // Uses texture_id_next as texture image...?
#define MOBJ_FLAG_PALETTE           (1 << 2)
#define MOBJ_FLAG_FRAC              (1 << 4)
#define MOBJ_FLAG_TEXTURE           (1 << 7)
#define MOBJ_FLAG_PRIMCOLOR         (1 << 9)
#define MOBJ_FLAG_ENVCOLOR          (1 << 10)
#define MOBJ_FLAG_BLENDCOLOR        (1 << 11)
#define MOBJ_FLAG_LIGHT1            (1 << 12)
#define MOBJ_FLAG_LIGHT2            (1 << 13)

// Camera defines

// Create mask of draw layer (known in Smash Remix lingo as "room") to render
#define COBJ_MASK_DLLINK(r)         (1ULL << (r))

#define COBJ_FLAG_NONE              (0)
#define COBJ_FLAG_ZBUFFER           (1 << 0)    // Use Z-Buffer depth image
#define COBJ_FLAG_FILLCOLOR         (1 << 1)    // Use camera's packed RRGGBBAA `color` variable
#define COBJ_FLAG_DLBUFFERS         (1 << 2)    // Update DLBuffers and DisplayList branches?
#define COBJ_FLAG_IDENTIFIER        (1 << 3)    // If the 0x8 bit is 0, camera_tag is treated as a mask; if 1, it is treated as an ID
#define COBJ_FLAG_GFXEND            (1 << 4)    // Run SYTaskGfxEnd task type?
#define COBJ_FLAG_BRANCHSYNC        (1 << 6)    // Sync all Branch DLs with main DL buffers?

union GCUserData
{
    s32 s;
    u32 u;
    void *p;
};

// Half-sized AObj event struct for bigger animation scripts (used by fighters only)
union AObjEvent16
{
    s16 s;
    u16 u;

    struct
    {
        u16 opcode : 5;
        u16 flags : 10;
        u16 toggle : 1;

    } command;
};

// Normal-sized AObj event struct for common animation scripts
union AObjEvent32
{
    f32 f;
    s32 s;
    u32 u;
    void *p;

    struct
    {
        u32 opcode : 7;
        u32 flags : 10;
        u32 payload : 15;
        
    } command;
};

/*
 * This houses both types of AObj scripts; event32 is the standard type of script.
 * 
 * Only DObjs (and on top of that, only fighters) use the event16 script type, but
 * might as well have a union of both for the sake of convenience.
 * 
 * Note: later games (or Melee, at least) refer to the event16 scripts as "figatrees"
 */ 
union AObjScript
{
    AObjEvent16 *event16;
    AObjEvent32 *event32;
};

struct AObj
{
    AObj *next;
    u8 track;
    u8 kind;
    f32 length_invert;
    f32 length;
    f32 value_base;
    f32 value_target;
    f32 rate_base;
    f32 rate_target;
    void *interpolate;
};

struct GObjThread
{
    GObjThread *next;
    OSThread thread;
    u64 *stack;
    size_t stack_size;
};

struct GObjStack
{
    GObjStack *next;
    size_t stack_size;
    u64 stack[1];
}; // size == 0x08 + VLA

// List that connects lists of stack nodes of `size` bytes
struct GObjThreadStack 
{
    GObjThreadStack *next;
    GObjStack *stack;
    size_t size;
};

struct GObjProcess
{
    GObjProcess *link_next;
    GObjProcess *link_prev;
    GObjProcess *priority_next;         // This is more than likely not an array, doing this only to get the correct offsets.
    GObjProcess *priority_prev;
    s32 priority;
    u8 kind;
    ub8 is_paused;
    GObj *parent_gobj;
    
    union GObjProcessExec               // These are based on kind.
    {
        GObjThread *gobjthread;
        void (*func)(GObj*);

    } exec;

    void (*func_id)(GObj*);             // Used to identify GObjProcesses that should be paused by function. So weird.
};

struct GObjScript
{
    GObj *next_gobj;
    s32 id;
};

struct GObj
{
    u32 id;
    GObj *link_next;
    GObj *link_prev;
    u8 link_id;
    u8 dl_link_id;
    u8 frame_draw_last;                 // Last frame drawn?
    u8 obj_kind;                        // Determines kind of *obj: 0 = NULL, 1 = DObj, 2 = SObj, 3 = CObj
    u32 link_priority;
    void (*func_run)(GObj*);
    GObjProcess *gobjproc_head;

    union
    {
        s32 unk_0x1C;
        GObj *unk_gobj_0x1C;
        GObjProcess *gobjproc_tail;
    };
    
    GObj *dl_link_next;
    GObj *dl_link_prev;
    u32 dl_link_priority;
    void (*proc_display)(GObj*);
    u64 camera_mask;
    u32 camera_tag;                       // Usually 0xFFFFFFFF
    u64 buffer_mask;
    GObjScript gobjscripts[5];
    s32 gobjscripts_num;                // Length/number of active members of gobjlinks
    void *obj;                          // Can be: NULL, DObj, SObj or CObj
    f32 anim_frame;                     // Current frame of animation
    u32 flags;                          // GObj logic flags (e.g. 0x1 = skip rendering)
    void (*func_anim)(DObj*, s32, f32);
    GCUserData user_data;
};

struct XObj 
{
    XObj *next;
    u8 kind;
    u8 unk05;
    Mtx mtx;
};

struct GCPersp
{
    XObj *xobj;
    u16 norm;
    f32 fovy;
    f32 aspect;
    f32 near;
    f32 far;
    f32 scale;
};

struct GCFrustum
{
    XObj *xobj;
    f32 l, r, b, t, n, f;
    f32 scale;
};

struct GCOrtho
{
    XObj *xobj;
    f32 l, r, b, t, n, f;
    f32 scale;
};

struct GCTranslate
{
    XObj *xobj;

    union
    {
        Vec3f f;
        Vec3i i;

    } vec;
};

struct GCRotate
{
    XObj *xobj;

    f32 a;          // Rotation angle

    union
    {
        Vec3f f;

    } vec;
};

struct GCScale
{
    XObj *xobj;
    
    union
    {
        Vec3f f;

    } vec;
};

struct DObjVec
{
    u8 kinds[3];
    u8 pad;
    u8 data[1];
};

struct MObjSub
{
    u16 pad00;
    u8 fmt;
    u8 siz;
    void **sprites; // should this be a pointer to an array of images (sprite set)?
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    s32 unk10; // could be f32??
    f32 trau;       // U-Translation?
    f32 trav;       // V-Translation?
    f32 scau;       // U-Scale?
    f32 scav;       // V-Scale?
    f32 unk24;
    f32 unk28;
    void **palettes;  // palette pointers?
    u16 flags;      // command flags?
    u8 block_fmt;   // texture image format?
    u8 block_siz;
    u16 block_dxt;
    u16 unk36;
    u16 unk38;
    u16 unk3A;
    f32 scrollu;
    f32 scrollv;
    f32 unk44;
    f32 unk48;       // Translate V?
    u32 unk4C;
    SYColorPack primcolor;
    u8 prim_l;
    u8 prim_m;
    u8 prim_pad[2];
    SYColorPack envcolor;
    SYColorPack blendcolor;
    SYColorPack light1color;
    SYColorPack light2color;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
};

struct MObj                         // Material Object
{
    MObj *next;
    GObj *parent_gobj;              // Unconfirmed
    MObjSub sub;
    u16 texture_id_curr;
    u16 texture_id_next;
    f32 lfrac;
    f32 palette_id;
    s32 unk_mobj_0x8C;
    AObj *aobj;
    AObjScript matanim_joint;
    f32 anim_wait;                  // Animation frames remaining until next command(s) are parsed
    f32 anim_speed;                 // Animation playback rate / interpolation, multi-purpose?
    f32 anim_frame;                 // Current animation frame, multi-purpose?
	GCUserData user_data;           // Actually just padding?
};

struct DObjTransformTypes
{
    u8 tk1, tk2;
    u8 tk3;
};

struct DObjDesc
{
    s32 id;
    void *dl;
    Vec3f translate;
    Vec3f rotate;
    Vec3f scale;
};

struct DObjTraDesc
{
    s32 id;
    void *dl;
    Vec3f translate;
};

struct DObjMultiList
{
    s32 id;
    Gfx *dl1, *dl2;
};

struct DObjDLLink
{
    s32 list_id;
    Gfx *dl;
};

struct DObjDistDL
{
    f32 target_dist;
    Gfx *dl;
};

struct DObjDistDLLink
{
    f32 target_dist;
    DObjDLLink *dl_link;
};

struct GCGfxLink
{
    u8 frame;
    Gfx *dls[4];
};

struct DObj                 // Draw Object
{
    DObj *alloc_free;       // Has to do with memory allocation
    GObj *parent_gobj;      // GObj that this DObj belongs to
    DObj *sib_next;         // Next sibling
    DObj *sib_prev;         // Previous sibling
    DObj *child;            // Child
    DObj *parent;           // Parent

    GCTranslate translate;  // Translation XObj and vector
    GCRotate rotate;        // Rotation XObj and vector
    GCScale scale;          // Scale XObj and vector

    DObjVec *vec;

    union
    {
        void *dv;           // Default void* display list variable
        Gfx *dl;
        Gfx **dls;
        DObjMultiList *multi_list;
        DObjDLLink *dl_link;
        DObjDistDL *dist_dl;
        DObjDistDLLink *dist_dl_link;
    };

    u8 flags;
    
    ub8 is_anim_root;       // TRUE if this DObj's animation script is at the top of the hierarchy?

    u8 xobjs_num;
    XObj *xobjs[5];

    AObj *aobj;

    AObjScript anim_joint;
    
    f32 anim_wait;          // Multi-purpose? Usually frames remaining until next anim command, but used as rotation step in Crate/Barrel smash GFX?
    f32 anim_speed;         // Multi-purpose? Fighters use this as animation playback rate / interpolation, but it is used as rotation step in Crate/Barrel smash GFX?
    f32 anim_frame;         // Multi-purpose? Usually current animation frame, but used as rotation step in Crate/Barrel smash GFX?

    MObj *mobj;

    /* 
     * Can be:
     * Other DObj that this DObj is attached to
     * FTParts
     * YakumonoID
     * Color for 1P Game Master Hand defeat fadeout (?)
     */
    GCUserData user_data;
};

struct SObj                     // Sprite object
{
    SObj *alloc_free;           // Has to do with memory allocation
    GObj *parent_gobj;          // GObj that owns this SObj
    SObj *next;                 // Next SObj in linked list
    SObj *prev;                 // Prev SObj in linked list
    Sprite sprite;              // Sprite data
    GCUserData user_data;       // Custom parameters attached to SObj
    Vec2f pos;                  // Position on screen; ghosting effect if out of camera scissor bounds?
    SYColorRGBA envcolor;       // Color of outline around / under sprite?
    u8 cmt, cms;                // t-axis and s-axis mirror, no-mirror, wrap and clamp flags
    u8 maskt, masks;            // t-axis and s-axis mask
    u16 lrs, lrt;               // lower right s and t - used for wrap/mirror boundary
};

struct CObjVec
{
    XObj *xobj;
    Vec3f eye;
    Vec3f at;
    Vec3f up;
};

struct CObjDesc
{
    Vec3f eye;
    Vec3f at;
    f32 upx;
};

struct CObj
{
    CObj *next;
    GObj *parent_gobj;

    Vp viewport;

    union CameraProjection
    {
        GCPersp persp;
        GCOrtho ortho;
        GCFrustum frustum;

    } projection;

    CObjVec vec;

    s32 xobjs_num;
    XObj *xobjs[2];

    AObj *aobj;
    AObjScript camanim_joint;

    f32 anim_wait;
    f32 anim_speed;
    f32 anim_frame;

    u32 flags;
    u32 color;

    void (*func_camera)(CObj*, s32);

    s32 unk_camera_0x8C;
};

struct GCSetup 
{
    GObjThread *gobjthreads;
    s32 gobjthreads_num;

    size_t gobjthreadstack_size;
    GObjStack *gobjthreadstacks;
    u32 gobjthreadstacks_num;

    s32 unk_gcsetup_0x14;

    GObjProcess *gobjprocs;
    s32 gobjprocs_num;

    GObj *gobjs;
    s32 gobjs_num;
    size_t gobj_size;

    XObj *xobjs;
    s32 xobjs_num;

    void (*func_eject)(DObjVec*);

    AObj *aobjs;
    s32 aobjs_num;

    MObj *mobjs;
    s32 mobjs_num;

    DObj *dobjs;
    s32 dobjs_num;
    size_t dobj_size;

    SObj *sobjs;
    s32 sobjs_num;
    size_t sobj_size;

    CObj *cameras;
    s32 cobjs_num;
    size_t cobj_size;
};

#endif
