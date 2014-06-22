/**
 * @file rgb2yuv_sse.c
 *
 * RGB2YUV sse
 */

#include "rgb2yuv.h"
#include <xmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>

void rgb2yuv_sse(uint8_t *pixels, int count)
{
        int i;
        uint8_t *p = pixels;
 
        __m128i v_byte1 = _mm_set1_epi32(0x000000ff);
        __m128i v_byte3 = _mm_set1_epi32(0x00ff0000);
        __m128i v_mat_00 = _mm_set1_epi16((short int)47);
        __m128i v_mat_01 = _mm_set1_epi16((short int)157);
        __m128i v_mat_02 = _mm_set1_epi16((short int)16);
        __m128i v_mat_03 = _mm_set1_epi16((short int)4096);
        __m128i v_mat_04 = _mm_set1_epi16((short int)-26);
        __m128i v_mat_05 = _mm_set1_epi16((short int)-87);
        __m128i v_mat_06 = _mm_set1_epi16((short int)112);
        __m128i v_mat_07 = _mm_set1_epi16((short int)32768);
        __m128i v_mat_08 = _mm_set1_epi16((short int)112);
        __m128i v_mat_09 = _mm_set1_epi16((short int)-102);
        __m128i v_mat_10 = _mm_set1_epi16((short int)-10);
        __m128i v_mat_11 = _mm_set1_epi16((short int)32768);
        
        __m128i mask2   = _mm_set1_epi32(0x00ff00ff);
 
        __m128i mask_y1 = _mm_set_epi8((char)128, (char)128, 12, (char)128,   (char)128, (char)128, 8, (char)128,
                                        (char)128, (char)128, 4, (char)128,   (char)128, (char)128, 0, (char)128);
 
        __m128i mask_y2 = _mm_set_epi8((char)128, (char)128, 14,  (char)128,  (char)128, (char)128, 10, (char)128,
                                        (char)128, (char)128, 6, (char)128,   (char)128, (char)128, 2, (char)128);
 
        __m128i mask_u1 = _mm_set_epi8((char)128, 12, (char)128, (char)128,   (char)128, 8, (char)128, (char)128,
                                        (char)128, 4, (char)128, (char)128,   (char)128, 0, (char)128, (char)128);
 
        __m128i mask_u2 = _mm_set_epi8((char)128, 14, (char)128, (char)128,   (char)128, 10, (char)128, (char)128,
                                        (char)128, 6, (char)128, (char)128,   (char)128, 2, (char)128, (char)128);
 
        __m128i mask_v1 = _mm_set_epi8(12, (char)128, (char)128, (char)128,   8, (char)128, (char)128, (char)128,
                                        4, (char)128, (char)128, (char)128,   0, (char)128, (char)128, (char)128);
 
        __m128i mask_v2 = _mm_set_epi8(14, (char)128, (char)128, (char)128,   10, (char)128, (char)128, (char)128,
                                        6, (char)128, (char)128, (char)128,   2, (char)128, (char)128, (char)128);
 
//         #pragma omp parallel for
        for (i=0; i<count / 8; i++) {
                __m128i a1, a2, r, g, b, y, u, v, res;
 
                a1 = _mm_loadu_si128((__m128i *)&p[i*32]);
                a2 = _mm_loadu_si128((__m128i *)&p[i*32 + 16]);
 
                r = _mm_or_si128(_mm_and_si128(_mm_srli_si128(a1, 1), v_byte1), _mm_and_si128(_mm_slli_si128(a2, 1), v_byte3));
                g = _mm_or_si128(_mm_and_si128(_mm_srli_si128(a1, 2), v_byte1), _mm_and_si128(a2, v_byte3));
                b = _mm_or_si128(_mm_and_si128(_mm_srli_si128(a1, 3), v_byte1), _mm_and_si128(_mm_srli_si128(a2, 1), v_byte3));
 
 
                y = _mm_add_epi16(
                        _mm_add_epi16(
                                _mm_mullo_epi16(r, v_mat_00),
                                _mm_mullo_epi16(g, v_mat_01)),
                        _mm_add_epi16(
                                _mm_mullo_epi16(b, v_mat_02),
                                v_mat_03));
 
                y = _mm_and_si128(_mm_srai_epi16(y, 8), mask2);
 
                u = _mm_add_epi16(
                        _mm_add_epi16(
                                _mm_mullo_epi16(r, v_mat_04),
                                _mm_mullo_epi16(g, v_mat_05)),
                        _mm_add_epi16(
                                _mm_mullo_epi16(b, v_mat_06),
                                v_mat_07));
 
                u  = _mm_and_si128(_mm_srai_epi16(u, 8), mask2);
 
                v = _mm_add_epi16(
                        _mm_add_epi16(
                                _mm_mullo_epi16(r, v_mat_08),
                                _mm_mullo_epi16(g, v_mat_09)),
                        _mm_add_epi16(
                                _mm_mullo_epi16(b, v_mat_10),
                                v_mat_11));
 
                v = _mm_and_si128(_mm_srai_epi16(v, 8), mask2);
 
 
                res = _mm_or_si128(_mm_shuffle_epi8(y, mask_y1), _mm_shuffle_epi8(u, mask_u1));
                res = _mm_or_si128(res, _mm_shuffle_epi8(v, mask_v1));
 
                _mm_storeu_si128((__m128i *)&p[i*32], res);
 
                res = _mm_or_si128(_mm_shuffle_epi8(y, mask_y2), _mm_shuffle_epi8(u, mask_u2));
               res = _mm_or_si128(res, _mm_shuffle_epi8(v, mask_v2));
 
                _mm_storeu_si128((__m128i *)&p[i*32 + 16], res);
        }
}