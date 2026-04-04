#include <PR/guint.h>

void guNormalize(float *x, float *y, float *z)
{
	float	m;

	m = 1/sqrtf((*x)*(*x) + (*y)*(*y) + (*z)*(*z));
	*x *= m;
	*y *= m;
	*z *= m;
}
