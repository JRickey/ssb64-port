#include <PR/guint.h>

void guMtxCatF(float mf[4][4], float nf[4][4], float res[4][4])
{
	int	i, j, k;
	float	temp[4][4];

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
		temp[i][j] = 0.0;
		for (k=0; k<4; k++) {
			temp[i][j] += mf[i][k] * nf[k][j];
		}
		}
	}

	/* make sure we handle case where result is an input */
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
		res[i][j] = temp[i][j];
		}
	}
}
