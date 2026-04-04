#include "vector.h"
#include "macros.h"
#include <PR/gu.h>
#include <sys/utils.h>

f32 syVectorNorm3D(Vec3f *dst)
{
    f32 mag = sqrtf(SQUARE(dst->x) + SQUARE(dst->y) + SQUARE(dst->z));

    if (mag == 0.0F)
    {
        return 0.0F;
    }
    dst->x *= 1.0F / mag;
    dst->y *= 1.0F / mag;
    dst->z *= 1.0F / mag;

    return mag;
}

f32 syVectorMag3D(Vec3f *src)
{
    return sqrtf(SQUARE(src->x) + SQUARE(src->y) + SQUARE(src->z));
}

Vec3f* syVectorAdd3D(Vec3f *dst, Vec3f *add)
{
    dst->x += add->x;
    dst->y += add->y;
    dst->z += add->z;

    return dst;
}

Vec3f* syVectorSub3D(Vec3f *dst, Vec3f *sub)
{
    dst->x -= sub->x;
    dst->y -= sub->y;
    dst->z -= sub->z;

    return dst;
}

Vec3f* syVectorDiff3D(Vec3f *dst, Vec3f *src, Vec3f *sub)
{
    dst->x = src->x - sub->x;
    dst->y = src->y - sub->y;
    dst->z = src->z - sub->z;

    return dst;
}

f32 syVectorNormDiff3D(Vec3f *dst, Vec3f *src, Vec3f *sub)
{
    dst->x = src->x - sub->x;
    dst->y = src->y - sub->y;
    dst->z = src->z - sub->z;

    return syVectorNorm3D(dst);
}

Vec3f* syVectorScale3D(Vec3f *dst, f32 scale)
{
    dst->x *= scale;
    dst->y *= scale;
    dst->z *= scale;

    return dst;
}

Vec3f* syVectorInv3D(Vec3f *dst, f32 inv)
{
    if (inv != 0.0F)
    {
        dst->x /= inv;
        dst->y /= inv;
        dst->z /= inv;
    }
    return dst;
}

Vec3f* syVectorAddScaled3D(Vec3f *dst, Vec3f *add, f32 scale)
{
    dst->x += (add->x * scale);
    dst->y += (add->y * scale);
    dst->z += (add->z * scale);

    return dst;
}

f32 syVectorNormScaleAddScaled3D(Vec3f *dst, f32 dst_scale, Vec3f *add, f32 add_scale)
{
    dst->x *= dst_scale;
    dst->y *= dst_scale;
    dst->z *= dst_scale;

    dst->x += (add->x * add_scale);
    dst->y += (add->y * add_scale);
    dst->z += (add->z * add_scale);

    return syVectorNorm3D(dst);
}

f32 syVectorAngleDiff3D(Vec3f *a, Vec3f *b)
{
    f32 mag = syVectorMag3D(a) * syVectorMag3D(b);

    if (mag != 0.0F)
    {
        f32 cos_diff = SYVECTOR_DOT_3D(a, b) / mag;

        // limit result to -1 < x < 1
        if (cos_diff > 1.0F)
        {
            cos_diff = 1.0F;
        }
        if (cos_diff < -1.0F)
        {
            cos_diff = -1.0F;
        }
        return syUtilsArcCos(cos_diff);
    }
    // at least one of the vectors is a zero vector
    return 0.0F;
}

Vec3f* syVectorRotate3D(Vec3f *dst, s32 axis, f32 angle)
{
    f32 x, y, z;
    f32 sin = sinf(angle);
    f32 cos = cosf(angle);

    switch (axis)
    {
    case SYVECTOR_AXIS_X:
        x = dst->x;
        y = (dst->y * cos) - (dst->z * sin);
        z = (dst->y * sin) + (dst->z * cos);
        break;

    case SYVECTOR_AXIS_Y:
        x = (dst->x * cos) + (dst->z * sin);
        y = dst->y;
        z = (dst->z * cos) - (dst->x * sin);
        break;

    case SYVECTOR_AXIS_Z:
        x = (dst->x * cos) - (dst->y * sin);
        y = (dst->x * sin) + (dst->y * cos);
        z = dst->z;
        break;
    }
    dst->x = x;
    dst->y = y;
    dst->z = z;

    return dst;
}

Vec3f* syVectorNeg3D(Vec3f *dst)
{
    dst->x = -dst->x;
    dst->y = -dst->y;
    dst->z = -dst->z;

    return dst;
}

Vec3f* syVectorRotateAbout3D(Vec3f *dst, Vec3f *dir, f32 angle)
{
    f32 mag_yz;
    f32 ratio_z;
    f32 ratio_y;
    f32 rot_x;
    f32 sin;
    f32 cos;
    f32 rot_y;
    f32 intermediate_x;
    f32 intermediate_z;
    f32 rot_z;

    mag_yz = sqrtf(SQUARE(dir->y) + SQUARE(dir->z));
    sin = sinf(angle);
    cos = cosf(angle);

    if (mag_yz != 0.0F)
    {
        ratio_z = dir->z / mag_yz;
        ratio_y = dir->y / mag_yz;
        rot_x = dst->x;
        rot_y = dst->y * ratio_z - dst->z * ratio_y;
        rot_z = dst->y * ratio_y + dst->z * ratio_z;
    }
    else
    {
        rot_x = rot_z = dst->x;
        rot_y = dst->y;
        rot_z = dst->z;
    }
    intermediate_z = rot_x * mag_yz - rot_z * dir->x;
    intermediate_x = rot_x * dir->x + rot_z * mag_yz;
    rot_x = intermediate_z * cos - rot_y * sin;
    
    rot_y = intermediate_z * sin + rot_y * cos;
    intermediate_z = rot_x * mag_yz + intermediate_x * dir->x;
    intermediate_x = -rot_x * dir->x + intermediate_x * mag_yz;

    if (mag_yz != 0.0F)
    {
        dst->x = intermediate_z;
        dst->y = rot_y * ratio_z + intermediate_x * ratio_y;
        dst->z = -rot_y * ratio_y + intermediate_x * ratio_z;
    }
    else
    {
        dst->x = intermediate_z;
        dst->y = rot_y;
        dst->z = intermediate_x;
    }
    return dst;
}

Vec3f* syVectorAxisNeg3D(Vec3f *dst, u32 flags)
{
    if (flags & SYVECTOR_AXIS_X)
    {
        dst->x = -dst->x;
    }
    if (flags & SYVECTOR_AXIS_Y)
    {
        dst->y = -dst->y;
    }
    if (flags & SYVECTOR_AXIS_Z)
    {
        dst->z = -dst->z;
    }
    return dst;
}

// Granularly negates components of a vector
Vec3f* syVectorGranularNeg3D(Vec3f *dst, u32 flags)
{
    if (flags & SYVECTOR_INV_SWAP)
    {
        if ((flags & SYVECTOR_AXIS_X) && (dst->x < 0.0F))
        {
            dst->x = -dst->x;
        }
        else if ((flags & SYVECTOR_INV_X) && (dst->x > 0.0F))
        {
            dst->x = -dst->x;
        }
        if ((flags & SYVECTOR_AXIS_Y) && (dst->y < 0.0F))
        {
            dst->y = -dst->y;
        }
        else if ((flags & SYVECTOR_INV_Y) && (dst->y > 0.0F))
        {
            dst->y = -dst->y;
        }
        if ((flags & SYVECTOR_AXIS_Z) && (dst->z < 0.0F))
        {
            dst->z = -dst->z;
        }
        else if ((flags & SYVECTOR_INV_Z) && (dst->z > 0.0F))
        {
            dst->z = -dst->z;
        }
    }
    else
    {
        if ((flags & SYVECTOR_AXIS_X) && (dst->x > 0.0F))
        {
            dst->x = -dst->x;
        }
        else if ((flags & SYVECTOR_INV_X) && (dst->x < 0.0F))
        {
            dst->x = -dst->x;
        }
        if ((flags & SYVECTOR_AXIS_Y) && (dst->y > 0.0F))
        {
            dst->y = -dst->y;
        }
        else if ((flags & SYVECTOR_INV_Y) && (dst->y < 0.0F))
        {
            dst->y = -dst->y;
        }
        if ((flags & SYVECTOR_AXIS_Z) && (dst->z > 0.0F))
        {
            dst->z = -dst->z;
        }
        else if ((flags & SYVECTOR_INV_Z) && (dst->z < 0.0F))
        {
            dst->z = -dst->z;
        }
    }
    return dst;
}

Vec3f* syVectorReflectAcross3D(Vec3f *dst, Vec3f *src)
{
    f32 dot = SYVECTOR_DOT_3D(src, dst) * -2.0F;

    dst->x += (src->x * dot);
    dst->y += (src->y * dot);
    dst->z += (src->z * dot);

    return dst;
}

Vec3f* syVectorReflectFacing3D(Vec3f *dst, Vec3f *src)
{
    f32 dot = SYVECTOR_DOT_3D(src, dst);

    if (dot < 0.0F)
    {
        dot *= -2.0F;

        dst->x += (src->x * dot);
        dst->y += (src->y * dot);
        dst->z += (src->z * dot);
    }
    return dst;
}

// Returns 1 if the two vectors are less than 180 degrees apart,
// -1 if they are more than 180 degrees apart,
// and 0 if they are perpendicular.
s32 syVectorDirection3D(Vec3f *a, Vec3f *b)
{
    f32 dot = SYVECTOR_DOT_3D(b, a);

    if (dot != 0.0F)
    {
        return (dot < 0.0F) ? -1 : 1;
    }
    else return 0;
}

Vec3f* syVectorNormCross3D(Vec3f *a, Vec3f *b, Vec3f *dst)
{
    dst->x = (a->y * b->z) - (a->z * b->y);
    dst->y = (a->z * b->x) - (a->x * b->z);
    dst->z = (a->x * b->y) - (a->y * b->x);

    return (syVectorNorm3D(dst) != 0.0F) ? dst : NULL;
}

// Subtracts v0 from b, and runs the same check as syVectorDirection3D on the result and a
s32 syVectorDiffDirection3D(Vec3f *sub, Vec3f *a, Vec3f *b)
{
    f32 dot = (a->x * (b->x - sub->x)) + (a->y * (b->y - sub->y)) + (a->z * (b->z - sub->z));

    if (dot != 0.0F)
    {
        return (dot < 0.0F) ? -1 : 1;
    }
    else return 0;
}

// gets the magnitude of the difference of 2 vectors
f32 syVectorDist3D(Vec3f *sub, Vec3f *src)
{
    Vec3f res;

    res.x = src->x - sub->x;
    res.y = src->y - sub->y;
    res.z = src->z - sub->z;

    return syVectorMag3D(&res);
}

// Takes the cross product of (b - arg0) and (c - arg0), then
// does the difference of cross dot point and cross dot arg0, then
// takes the absolute value of that difference and divides by the magnitude of the cross product
f32 syVectorDistPlaneToPoint3D(Vec3f *plane_a, Vec3f *plane_b, Vec3f *plane_c, Vec3f *point)
{
    Vec3f dist_ab;
    Vec3f dist_ac;
    f32 normal_x;
    f32 normal_y;
    f32 normal_z;
    f32 cross_neg;
    f32 numerator;

    dist_ab.x = (plane_b->x - plane_a->x);
    dist_ab.y = (plane_b->y - plane_a->y);
    dist_ab.z = (plane_b->z - plane_a->z);

    dist_ac.x = (plane_c->x - plane_a->x);
    dist_ac.y = (plane_c->y - plane_a->y);
    dist_ac.z = (plane_c->z - plane_a->z);

    normal_x = (dist_ab.y * dist_ac.z) - (dist_ab.z * dist_ac.y);
    normal_y = (dist_ab.z * dist_ac.x) - (dist_ab.x * dist_ac.z);
    normal_z = (dist_ab.x * dist_ac.y) - (dist_ab.y * dist_ac.x);

    cross_neg = -((normal_x * plane_a->x) + (normal_y * plane_a->y) + (normal_z * plane_a->z));
    numerator = ABSF(((normal_x * point->x) + (normal_y * point->y) + (normal_z * point->z)) + cross_neg);

    return numerator / sqrtf(SQUARE(normal_x) + SQUARE(normal_y) + SQUARE(normal_z));
}
