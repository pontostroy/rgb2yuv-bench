/**
 * @file rgb2yuv_tables64.c
 *
 * Modified version RGB2YUV from Novell-ch to use tables instead of multiply
 * and all components in one 64-bit number
 */

#include "rgb2yuv.h"

static int64_t t_r3[256], t_g3[256], t_b3[256];

void rgb2yuv_tables64_init(void)
{
	int i, j;
	int k_r[] = {  66, -38, 112 };
	int k_g[] = { 129, -74, -94 };
	int k_b[] = {  25, 112, -18 };


	for (i = 0; i <= 255; i++)
	{
		int64_t r = 0, g = 0, b = 0;
		for (j = 0; j <= 2; j++)
		{
			r = (r << 16) + k_r[j] * i;
			g = (g << 16) + k_g[j] * i;
			b = (b << 16) + k_b[j] * i;
		}
		t_r3[i] = r;
		t_g3[i] = g;
		t_b3[i] = b;
	}
}

void rgb2yuv_tables64(uint8_t *pixels, int count)
{
	int t;
	uint8_t r, g, b;
	int64_t c1 = (128LL << 32) + (128 << 16) + 128 +
			(16LL << 40) + (128 << 24) + (128 << 8);

	for (t = 0; t < count * 4; t += 4)
	{
		r = pixels[t + 1];
		g = pixels[t + 2];
		b = pixels[t + 3];

		int64_t x3 = t_r3[r] + t_g3[g] + t_b3[b] + c1;

		pixels[t + 1] = x3 >> 40;
		pixels[t + 2] = x3 >> 24;
		pixels[t + 3] = x3 >> 8;
	}
} 
