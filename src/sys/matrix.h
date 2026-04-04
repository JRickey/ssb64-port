#ifndef _SYMATRIX_H_
#define _SYMATRIX_H_

#include <ssb_types.h>
#include <PR/mbi.h>

extern u16 gSYSinTable[0x800];

typedef union SYMatrixHub
{
    Mtx *gbi;
    Mtx44f *f;
    void **p;

} SYMatrixHub;

extern void syMatrixF2L(Mtx44f *src, Mtx *dst);
extern void syMatrixF2LFixedW(Mtx44f *src, Mtx *dst);
extern void syMatrixLookAtF
(
    Mtx44f *mf,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixLookAt
(
    Mtx *m,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixModLookAtF
(
    Mtx44f *mf,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 roll,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixModLookAt
(
    Mtx *m,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 roll,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixLookAtReflectF
(
    Mtx44f *mf,
    LookAt *l,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixLookAtReflect
(
    Mtx *m,
    LookAt *l,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixModLookAtReflectF
(
    Mtx44f *mf,
    LookAt *l,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 roll,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixModLookAtReflect
(
    Mtx *m,
    LookAt *l,
    f32 eye_x,
    f32 eye_y,
    f32 eye_z,
    f32 at_x,
    f32 at_y,
    f32 at_z,
    f32 roll,
    f32 up_x,
    f32 up_y,
    f32 up_z
);
extern void syMatrixOrthoF(Mtx44f *mf, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale);
extern void syMatrixOrtho(Mtx *m, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale);
extern void syMatrixPerspFastF(Mtx44f mf, u16 *persp_norm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
extern void syMatrixPerspFast(Mtx *m, u16 *persp_norm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
extern void syMatrixPerspF(Mtx44f mf, u16 *persp_norm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
extern void syMatrixPersp(Mtx *m, u16 *persp_norm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
extern void syMatrixScaF(Mtx44f *mf, f32 x, f32 y, f32 z);
extern void syMatrixSca(Mtx *m, f32 x, f32 y, f32 z);
extern void syMatrixTraF(Mtx44f *mf, f32 x, f32 y, f32 z);
extern void syMatrixTra(Mtx *m, f32 x, f32 y, f32 z);
extern void syMatrixRotRF(Mtx44f *mf, f32 a, f32 x, f32 y, f32 z);
extern void syMatrixRotR(Mtx *m, f32 a, f32 x, f32 y, f32 z);
extern void syMatrixTraRotRF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 angle, f32 rx, f32 ry, f32 rz);
extern void syMatrixTraRotR(Mtx *m, f32 tx, f32 ty, f32 tz, f32 angle, f32 rx, f32 ry, f32 rz);
extern void syMatrixTraRotRScaF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 angle, f32 rx, f32 ry, f32 rz, f32 sx, f32 sy, f32 sz);
extern void syMatrixTraRotRSca(Mtx *m, f32 tx, f32 ty, f32 tz, f32 angle, f32 rx, f32 ry, f32 rz, f32 sx, f32 sy, f32 sz);
extern void syMatrixRotRpyRF(Mtx44f *mf, f32 r, f32 p, f32 y);
extern void syMatrixRotRpyR(Mtx *m, f32 r, f32 p, f32 y);
extern void syMatrixTraRotRpyRF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y);
extern void syMatrixTraRotRpyR(Mtx *m, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y);
extern void syMatrixTraRotRpyRScaF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y, f32 sx, f32 sy, f32 sz);
extern void syMatrixTraRotRpyRSca(Mtx *m, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y, f32 sx, f32 sy, f32 sz);
extern void syMatrixRotPyrRF(Mtx44f *mf, f32 r, f32 p, f32 y);
extern void syMatrixRotPyrR(Mtx *m, f32 r, f32 p, f32 y);
extern void syMatrixTraRotPyrRF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y);
extern void syMatrixTraRotPyrR(Mtx *m, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y);
extern void syMatrixTraRotPyrRScaF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y, f32 sx, f32 sy, f32 sz);
extern void syMatrixTraRotPyrRSca(Mtx *m, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y, f32 sx, f32 sy, f32 sz);
extern void syMatrixRotPyRF(Mtx44f *mf, f32 p, f32 y);
extern void syMatrixRotPyR(Mtx *m, f32 p, f32 y);
extern void syMatrixTraRotPyRF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 p, f32 y);
extern void syMatrixTraRotPyR(Mtx *m, f32 tx, f32 ty, f32 tz, f32 p, f32 y);
extern void syMatrixRotRpRF(Mtx44f *mf, f32 r, f32 p);
extern void syMatrixRotRpR(Mtx *m, f32 p, f32 y);
extern void syMatrixTraRotRpRF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 r, f32 p);
extern void syMatrixTraRotRpR(Mtx *m, f32 tx, f32 ty, f32 tz, f32 r, f32 p);
extern void syMatrixRotYawRF(Mtx44f *mf, f32 y);
extern void syMatrixRotYawR(Mtx *m, f32 y);
extern void syMatrixTraRotYawRF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 y);
extern void syMatrixTraRotYawR(Mtx *m, f32 tx, f32 ty, f32 tz, f32 y);
extern void syMatrixRotPitchRF(Mtx44f *mf, f32 p);
extern void syMatrixRotPitchR(Mtx *m, f32 p);
extern void syMatrixTraRotPitchRF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 p);
extern void syMatrixTraRotPitchR(Mtx *m, f32 tx, f32 ty, f32 tz, f32 p);
extern void syMatrixRotDF(Mtx44f *mf, f32 a, f32 x, f32 y, f32 z);
extern void syMatrixRotD(Mtx *m, f32 a, f32 x, f32 y, f32 z);
extern void syMatrixTraRotDF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 a, f32 rx, f32 ry, f32 rz);
extern void syMatrixTraRotD(Mtx *m, f32 tx, f32 ty, f32 tz, f32 a, f32 rx, f32 ry, f32 rz);
extern void syMatrixRotRpyDF(Mtx44f *mf, f32 r, f32 p, f32 y);
extern void syMatrixRotRpyD(Mtx *m, f32 r, f32 p, f32 y);
extern void syMatrixTraRotRpyDF(Mtx44f *mf, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y);
extern void syMatrixTraRotRpyD(Mtx *m, f32 tx, f32 ty, f32 tz, f32 r, f32 p, f32 y);

#endif
