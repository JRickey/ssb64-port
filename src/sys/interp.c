#include <sys/interp.h>
#include <PR/gu.h>

// Biquadrate; easier to make a symbol than quartic (QT looks familiar to me)
#define BIQUAD(x) ((x) * (x) * (x) * (x))

// Catmull-Rom cubic spline
void syInterpCatromCubicSpline(Vec3f *out, Vec3f *ctrl, f32 s, f32 t)
{
    Vec3f *lctrl = ctrl;
    f32 sqt = SQUARE(t);
    f32 w0, w1, w2, w3;
    f32 cbt = sqt * t;

    w0 = (2.0F * sqt - cbt - t) * s;
    w1 = (2.0F - s) * cbt + (s - 3.0F) * sqt + 1.0F;
    w2 = (s - 2.0F) * cbt + (3.0F - 2.0F * s) * sqt + s * t;
    w3 = (cbt - sqt) * s;

    out->x = lctrl[0].x * w0 + lctrl[1].x * w1 + lctrl[2].x * w2 + lctrl[3].x * w3;
    out->y = lctrl[0].y * w0 + lctrl[1].y * w1 + lctrl[2].y * w2 + lctrl[3].y * w3;
    out->z = lctrl[0].z * w0 + lctrl[1].z * w1 + lctrl[2].z * w2 + lctrl[3].z * w3;
}

// quadratic spline?
void syInterpQuadSpline(Vec3f *out, Vec3f *ctrl, f32 s, f32 t)
{
    f32 sqt;
    f32 w1;
    f32 w2;
    f32 w0;
    f32 temp;
    f32 w3;

    sqt = t * t;
    w0 = ((((-3.0F) * sqt) + (4.0F * t)) - 1.0F) * s;
    temp = s - 3.0F;
    w3 = s;
    w1 = (((2.0F - w3) * 3.0F) * sqt) + ((2.0F * temp) * t);
    temp = 3.0F - (2.0F * w3);
    w2 = ((((w3 - 2.0F) * 3.0F) * sqt) + ((2.0F * temp) * t)) + w3;
    w3 = ((3.0F * sqt) - (2.0F * t)) * w3;

    out->x = (ctrl[0].x * w0) + (ctrl[1].x * w1) + (ctrl[2].x * w2) + (ctrl[3].x * w3);
    out->y = (ctrl[0].y * w0) + (ctrl[1].y * w1) + (ctrl[2].y * w2) + (ctrl[3].y * w3);
    out->z = (ctrl[0].z * w0) + (ctrl[1].z * w1) + (ctrl[2].z * w2) + (ctrl[3].z * w3);
}

// some sort of bezier interpolation
void syInterpBezier3Points(Vec3f *out, Vec3f *ctrl, f32 t)
{
    Vec3f *lctrl = ctrl;
    f32 subt;
    f32 cbt;
    f32 w0, w1, w2, w3;
    f32 sqt;

    subt = 1.0F - t;
    sqt = SQUARE(t);
    cbt = sqt * t;

    w0 = (1.0F / 6.0F) * subt * subt * subt;
    w1 = (1.0F / 6.0F) * (3.0F * cbt - 6.0F * sqt + 4.0F);
    w2 = (1.0F / 6.0F) * (3.0F * (sqt - cbt + t) + 1.0F);
    w3 = (1.0F / 6.0F) * cbt;

    out->x = lctrl[0].x * w0 + lctrl[1].x * w1 + lctrl[2].x * w2 + lctrl[3].x * w3;
    out->y = lctrl[0].y * w0 + lctrl[1].y * w1 + lctrl[2].y * w2 + lctrl[3].y * w3;
    out->z = lctrl[0].z * w0 + lctrl[1].z * w1 + lctrl[2].z * w2 + lctrl[3].z * w3;
}

// quadratic bezier with four control points (not three?)
void syInterpBezier4Points(Vec3f* out, Vec3f* ctrl, f32 t)
{
    s32 unused[2];
    f32 sqt;
    f32 mt;
    f32 w1;
    f32 w2;
    f32 w3;
    f32 w0;

    sqt = t * t;
    w0 = 1.0F - t;
    w3 = -0.5F * w0 * w0;
    mt = ((3.0F * sqt) - (4.0F * t)) * 0.5F;
    w1 = ((-3.0F * sqt) + (2.0F * t) + 1.0F) * 0.5F;
    w2 = 0.5F * sqt;

    out->x = (ctrl[0].x * w3) + (ctrl[1].x * mt) + (ctrl[2].x * w1) + (ctrl[3].x * w2);
    out->y = (ctrl[0].y * w3) + (ctrl[1].y * mt) + (ctrl[2].y * w1) + (ctrl[3].y * w2);
    out->z = (ctrl[0].z * w3) + (ctrl[1].z * mt) + (ctrl[2].z * w1) + (ctrl[3].z * w2);
}

// cubic bezier with scale factor of 3?
void syInterpCubicBezierScale(Vec3f *out, Vec3f *ctrl, f32 t)
{
    f32 sqt;
    f32 w1;
    f32 w2;
    f32 w3;
    f32 w0;
    f32 subt;
    f32 sqsubt;

    subt = 1.0F - t;
    sqt = SQUARE(t);
    sqsubt = SQUARE(subt);

    w0 = sqsubt * subt;
    w1 = 3.0F * t * sqsubt;
    w2 = 3.0F * sqt * subt;
    w3 = sqt * t;

    out->x = ctrl[0].x * w0 + ctrl[1].x * w1 + ctrl[2].x * w2 + ctrl[3].x * w3;
    out->y = ctrl[0].y * w0 + ctrl[1].y * w1 + ctrl[2].y * w2 + ctrl[3].y * w3;
    out->z = ctrl[0].z * w0 + ctrl[1].z * w1 + ctrl[2].z * w2 + ctrl[3].z * w3;
}

// four point quadratic bezier with scale of 3?
void syInterpQuadBezier4Points(Vec3f* out, Vec3f* ctrl, f32 t)
{
    f32 mt;
    f32 w0;
    f32 w1;
    f32 w2;
    f32 w3;

    mt = t - 1.0F;
    w3 = -3.0F * mt * mt;
    w0 = SQUARE(t);
    w0 = 3.0F * w0;
    w1 = ((1.0F - (4.0F * t)) + w0) * 3.0F;
    w2 = ((2.0F * t) - w0) * 3.0F;

    out->x = (ctrl[0].x * w3) + (ctrl[1].x * w1) + (ctrl[2].x * w2) + (ctrl[3].x * w0);
    out->y = (ctrl[0].y * w3) + (ctrl[1].y * w1) + (ctrl[2].y * w2) + (ctrl[3].y * w0);
    out->z = (ctrl[0].z * w3) + (ctrl[1].z * w1) + (ctrl[2].z * w2) + (ctrl[3].z * w0);
}


// arg1->points_num is total frames? elapsed frames?
// delta time cubic interpolation?
void syInterpCubicSplineTimeFrac(Vec3f *out, SYInterpDesc *desc, f32 t)
{
    s16 target_frame; // f10
    Vec3f *point;

    if ((t < 0.0F) || (t > 1.0F))
    {
        return;
    }
    else if (t < 1.0F)
    {
        // convert interval from [0,1] to [0,totalFrames]
        t *= (f32) (desc->points_num - 1);

        target_frame = t;

        // get only the rounding difference for interpolation?
        t -= target_frame;

        switch (desc->kind)
        {
        case nSYInterpKindLinear:
            point = &desc->points[target_frame];
            out->x = (point[1].x - point[0].x) * t + point[0].x;
            out->y = (point[1].y - point[0].y) * t + point[0].y;
            out->z = (point[1].z - point[0].z) * t + point[0].z;
            break;

        case nSYInterpKindBezierS3:
            syInterpCubicBezierScale(out, &desc->points[target_frame * 3], t);
            break;
                
        case nSYInterpKindBezier:
            syInterpBezier3Points(out, &desc->points[target_frame], t);
            break;

        case nSYInterpKindCatrom:
            syInterpCatromCubicSpline(out, &desc->points[target_frame], desc->unk04, t);
            break;
        }
    }
    else
    {
        target_frame = desc->points_num - 1;

        switch (desc->kind)
        {
        case nSYInterpKindLinear:
            point  = &desc->points[target_frame];
            *out = *point;
            break;

        case nSYInterpKindBezierS3:
            point  = &desc->points[target_frame * 3];
            *out = *point;
            break;
                
        case nSYInterpKindBezier:
            syInterpBezier3Points(out, &desc->points[target_frame - 1], 1.0F);
            break;
        
        case nSYInterpKindCatrom:
            point  = &desc->points[target_frame + 1];
            *out = *point;
            break;
        }
    }
}

// quadratic interpolation
void syInterpQuadSplineTimeFrac(Vec3f *out, SYInterpDesc *desc, f32 t)
{
    s16 target_frame;
    f32 t_origin;
    Vec3f *point;

    if ((t < 0.0F) || (t > 1.0F))
    {
        return;
    }
    else
    {
        t_origin = t;
        t *= (f32) (desc->points_num - 1);

        target_frame = t;
        t = t - (f32) target_frame;

        switch (desc->kind)
        {
        case nSYInterpKindLinear:
            if (t_origin == 1.0F)
            {
                target_frame--;
            }
            point    = desc->points + target_frame;
            out->x = point[1].x - point[0].x;
            out->y = point[1].y - point[0].y;
            out->z = point[1].z - point[0].z;
            break;
           
        case nSYInterpKindBezierS3:
            syInterpQuadBezier4Points(out, &desc->points[target_frame * 3], t);
            break;
            
        case nSYInterpKindBezier:
            syInterpBezier4Points(out, &desc->points[target_frame], t);
            break;

        case nSYInterpKindCatrom:
            syInterpQuadSpline(out, &desc->points[target_frame], desc->unk04, t);
            break;
        }
    }
}

// sqrt of quartic polynomial of x?
f32 syInterpGetQuartSum(f32 x, f32 *cof)
{
    f32 sum = cof[0] * BIQUAD(x) + cof[1] * CUBE(x) + cof[2] * SQUARE(x) + cof[3] * x + cof[4];

    if ((sum < 0.0F) && (sum > -0.001F))
    {
        sum = 0.0F;
    }
    return sqrtf(sum);
}

f32 syInterpGetCubicIntegralApprox(f32 t, f32 f, f32 *cof)
{
    f32 factor = (f - t) / 8;
    f32 sum = 0.0F;
    f32 time_scale = t + factor;
    s32 i;

    for (i = 2; i < 9; i++)
    {
        if (!(i & 1))
        {
            sum += 4.0F * syInterpGetQuartSum(time_scale, cof);
        }
        else sum += 2.0F * syInterpGetQuartSum(time_scale, cof);
        
        time_scale += factor;
    }
    return ((syInterpGetQuartSum(t, cof) + sum + syInterpGetQuartSum(f, cof)) * factor) / 3.0F;
}

// something like get t as fractional frames?
f32 syInterpGetFracFrame(SYInterpDesc *desc, f32 t)
{
    f32 *point; // v0
    s32 id;
    f32 frac_frame; // sp5C
    f32 time_scale;
    f32 min = 0.0F;
    f32 max = 1.0F;
    f32 res;
    f32 diff; // f24

    id = 0;
    point = desc->keyframes;

    while (point[1] < t)
    {
        id++;
        point++;
    }
    switch (desc->kind)
    {
    case nSYInterpKindLinear:
        frac_frame = (t - desc->keyframes[id]) / (desc->keyframes[id + 1] - desc->keyframes[id]);
        break;

    case nSYInterpKindBezierS3:
    case nSYInterpKindBezier:
    case nSYInterpKindCatrom:
        time_scale = (t - desc->keyframes[id]) * desc->length;

        do
        {
            frac_frame = (min + max) / 2.0F;
            res = syInterpGetCubicIntegralApprox(min, frac_frame, desc->quartics + (id * 5));

            if (time_scale < (res + 0.00001F)) 
            {
                max = frac_frame;
            }
            else
            {
                min = frac_frame;
                time_scale -= res;
            }
            diff = (min < max) ? -(min - max) : min - max;

            if (diff < 0.00001F)
            {
                break;
            }
        }
        while ((res + 0.00001F) < time_scale || time_scale < (res - 0.00001F));

        break;
    }
    return ((f32) id + frac_frame) / ((f32) desc->points_num - 1.0F);
}

void syInterpCubic(Vec3f *out, SYInterpDesc *desc, f32 t)
{
    syInterpCubicSplineTimeFrac(out, desc, syInterpGetFracFrame(desc, t));
}

void syInterpQuad(Vec3f *out, SYInterpDesc *desc, f32 t)
{
    syInterpQuadSplineTimeFrac(out, desc, syInterpGetFracFrame(desc, t));
}
