/**
 * @file rgb2yuv.h
 *
 * Definitions of all RGB2YUV implementations
 */

#ifndef RGB2YUV_H_
#define RGB2YUV_H_

#include <stdint.h>

/*
 * RGB2YUV conversion implementations
 * @param pixels Buffer with pixels in RGB on input and in YUV on output.
 *        RGB pixel - [unused, R, G, B] (4 bytes). YUV pixel - [unused, Y, U, V] (4 bytes).
 * @param count Count of pixels for conversion in buffer
 */

void rgb2yuv_wiki(uint8_t *pixels, int count);
void rgb2yuv_novell_ch(uint8_t *pixels, int count);
void rgb2yuv_tables(uint8_t *pixels, int count);
void rgb2yuv_gstreamer(uint8_t *pixels, int count);
void rgb2yuv_sse(uint8_t *pixels, int count);

void rgb2yuv_tables_init(void);

#endif /* RGB2YUV_H_ */
