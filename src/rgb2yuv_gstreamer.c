/**
 * @file rgb2yuv_wiki.c
 *
 * RGB2YUV from gstreamer
 */


#include "rgb2yuv.h"

#define RGB2Y(R, G, B) (  47 * (R) + 157 * (G) +  16 * (B) + 4096) >> 8
#define RGB2U(R, G, B) ( -26 * (R) +  -87 * (G) + 112 * (B) + 32768) >> 8
#define RGB2V(R, G, B) ( 112 * (R) +  -102 * (G) +  -10 * (B) + 32768) >> 8
void rgb2yuv_gstreamer(uint8_t *pixels, int count)
{
        int i, t;
        int y, u, v;
        int r, g, b;
        uint8_t *p = pixels;

        for (i = 0; i < count; i++)
        {
                t = i * 4;
                y = t + 1;
                u = t + 2;
                v = t + 3;
                r = p[y];
                g = p[u];
                b = p[v];
                p[y] = RGB2Y(r,g,b);
                p[u] = RGB2U(r,g,b);
                p[v] = RGB2V(r,g,b);
        } 
}