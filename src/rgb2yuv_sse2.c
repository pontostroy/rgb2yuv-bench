/**
 * @file rgb2yuv_sse.c
 *
 * RGB2YUV sse
 */

#include "rgb2yuv.h"
#include <xmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>

typedef struct {
  __m128i r, g, b;
} r8g8b8_t;

typedef struct {
  __m128i y, u, v;
} y8u8v8_t;

typedef struct {
  __m128i r_coef, g_coef, b_coef, x_coef;
} rgb_to_yuv_mat_v8_t;

static inline rgb_to_yuv_mat_v8_t create_vecm(int32_t r_coef, int32_t g_coef, int32_t b_coef, int32_t x_coef) {
  return (rgb_to_yuv_mat_v8_t) {
    _mm_set1_epi16(r_coef),
    _mm_set1_epi16(g_coef),
    _mm_set1_epi16(b_coef),
    _mm_set1_epi16(x_coef)
  };
}

static inline __m128i op(r8g8b8_t v, rgb_to_yuv_mat_v8_t m) {
  __m128i a = _mm_mullo_epi16(v.r, m.r_coef);
  __m128i b = _mm_mullo_epi16(v.g, m.g_coef);
  __m128i c = _mm_mullo_epi16(v.b, m.b_coef);
  __m128i ret = _mm_add_epi16(_mm_add_epi16(a, b), _mm_add_epi16(c, m.x_coef));
  return _mm_srai_epi16(ret, 8);
}

static inline __m128i r8g8b8_to_y8(r8g8b8_t v) {
  return op(v, create_vecm(47, 157, 16, 4096));
}
static inline __m128i r8g8b8_to_u8(r8g8b8_t v) {
  return op(v, create_vecm(-26, -87, 112, 32768));
}
static inline __m128i r8g8b8_to_v8(r8g8b8_t v) {
  return op(v, create_vecm(112, -102, -10, 32768));
}

static inline y8u8v8_t r8g8b8_to_y8u8v8(r8g8b8_t v) {
  return (y8u8v8_t) {r8g8b8_to_y8(v), r8g8b8_to_u8(v), r8g8b8_to_v8(v)};
}

// static inline r8g8b8_t p16x2_to_r8g8b8(__m128i a, __m128i b) {
//   __m128i v_byte1 = _mm_set1_epi32(0x000000ff);
//   __m128i v_byte3 = _mm_set1_epi32(0x00ff0000);
//   __m128i r8 = _mm_or_si128(_mm_and_si128(_mm_srli_si128(a, 1), v_byte1), _mm_and_si128(_mm_slli_si128(b, 1), v_byte3));
//   __m128i g8 = _mm_or_si128(_mm_and_si128(_mm_srli_si128(a, 2), v_byte1), _mm_and_si128(b, v_byte3));
//   __m128i b8 = _mm_or_si128(_mm_and_si128(_mm_srli_si128(a, 3), v_byte1), _mm_and_si128(_mm_srli_si128(b, 1), v_byte3));
//   return (r8g8b8_t) {r8, g8, b8};
// }

static inline r8g8b8_t p16x2_to_r8g8b8(__m128i a, __m128i b) {
  __m128i shuff_ra  = _mm_setr_epi8(1, 255, 255, 255, 5, 255, 255, 255, 9, 255, 255, 255, 13, 255, 255, 255);
  __m128i shuff_ga  = _mm_setr_epi8(2, 255, 255, 255, 6, 255, 255, 255, 10, 255, 255, 255, 14, 255, 255, 255);
  __m128i shuff_ba  = _mm_setr_epi8(3, 255, 255, 255, 7, 255, 255, 255, 11, 255, 255, 255, 15, 255, 255, 255);
  __m128i shuff_rb  = _mm_setr_epi8(255, 255, 1, 255, 255, 255, 5, 255, 255, 255, 9, 255, 255, 255, 13, 255);
  __m128i shuff_gb  = _mm_setr_epi8(255, 255, 2, 255, 255, 255, 6, 255, 255, 255, 10, 255, 255, 255, 14, 255);
  __m128i shuff_bb  = _mm_setr_epi8(255, 255, 3, 255, 255, 255, 7, 255, 255, 255, 11, 255, 255, 255, 15, 255);
  __m128i r8 = _mm_or_si128(_mm_shuffle_epi8(a, shuff_ra), _mm_shuffle_epi8(b, shuff_rb));
  __m128i g8 = _mm_or_si128(_mm_shuffle_epi8(a, shuff_ga), _mm_shuffle_epi8(b, shuff_gb));
  __m128i b8 = _mm_or_si128(_mm_shuffle_epi8(a, shuff_ba), _mm_shuffle_epi8(b, shuff_bb));
  return (r8g8b8_t) {r8, g8, b8};
}

static inline __m128i y8u8v8_to_p16a(y8u8v8_t v) {
  __m128i shuff_ya  = _mm_setr_epi8(255, 0, 255, 255, 255, 4, 255, 255, 255, 8, 255, 255, 255, 12, 255, 255);
  __m128i shuff_ua  = _mm_setr_epi8(255, 255, 0, 255, 255, 255, 4, 255, 255, 255, 8, 255, 255, 255, 12, 255);
  __m128i shuff_va  = _mm_setr_epi8(255, 255, 255, 0, 255, 255, 255, 4, 255, 255, 255, 8, 255, 255, 255, 12);
  return _mm_or_si128(_mm_or_si128(_mm_shuffle_epi8(v.y, shuff_ya), _mm_shuffle_epi8(v.u, shuff_ua)), _mm_shuffle_epi8(v.v, shuff_va));
}

static inline __m128i y8u8v8_to_p16b(y8u8v8_t v) {
  __m128i shuff_yb  = _mm_setr_epi8(255, 2, 255, 255, 255, 6, 255, 255, 255, 10, 255, 255, 255, 14, 255, 255);
  __m128i shuff_ub  = _mm_setr_epi8(255, 255, 2, 255, 255, 255, 6, 255, 255, 255, 10, 255, 255, 255, 14, 255);
  __m128i shuff_vb  = _mm_setr_epi8(255, 255, 255, 2, 255, 255, 255, 6, 255, 255, 255, 10, 255, 255, 255, 14);
  return _mm_or_si128(_mm_or_si128(_mm_shuffle_epi8(v.y, shuff_yb), _mm_shuffle_epi8(v.u, shuff_ub)), _mm_shuffle_epi8(v.v, shuff_vb));
}

void rgb2yuv_sse2(void * pixels, uint64_t len) {
  __m128i * it = pixels, * end = (__m128i *)((int32_t *)pixels + len);
  do {
    {
    __m128i a = *(it + 0), b = *(it + 1);
    y8u8v8_t v = r8g8b8_to_y8u8v8(p16x2_to_r8g8b8(a, b));
    a = y8u8v8_to_p16a(v), b = y8u8v8_to_p16b(v);
    *(it + 0) = a, *(it + 1) = b;
    it += 2;
    }
//     {
//     __m128i a = *(it + 0), b = *(it + 1);
//     y8u8v8_t v = r8g8b8_to_y8u8v8(p16x2_to_r8g8b8(a, b));
//     a = y8u8v8_to_p16a(v), b = y8u8v8_to_p16b(v);
//     *(it + 0) = a, *(it + 1) = b;
//     it += 2;
//     }
//     {
//     __m128i a = *(it + 0), b = *(it + 1);
//     y8u8v8_t v = r8g8b8_to_y8u8v8(p16x2_to_r8g8b8(a, b));
//     a = y8u8v8_to_p16a(v), b = y8u8v8_to_p16b(v);
//     *(it + 0) = a, *(it + 1) = b;
//     it += 2;
//     }
//     {
//     __m128i a = *(it + 0), b = *(it + 1);
//     y8u8v8_t v = r8g8b8_to_y8u8v8(p16x2_to_r8g8b8(a, b));
//     a = y8u8v8_to_p16a(v), b = y8u8v8_to_p16b(v);
//     *(it + 0) = a, *(it + 1) = b;
//     it += 2;
//     }
//     {
//     __m128i a = *(it + 0), b = *(it + 1);
//     y8u8v8_t v = r8g8b8_to_y8u8v8(p16x2_to_r8g8b8(a, b));
//     a = y8u8v8_to_p16a(v), b = y8u8v8_to_p16b(v);
//     *(it + 0) = a, *(it + 1) = b;
//     it += 2;
//     }
  } while(it < end);
}