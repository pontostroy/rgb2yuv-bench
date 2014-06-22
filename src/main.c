#include <time.h>

#include "rgb2yuv.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define PIXELS_COUNT  1080

static uint8_t pixels_wiki[4 * PIXELS_COUNT];
static uint8_t pixels_novel_ch[4 * PIXELS_COUNT];
static uint8_t pixels_tables[4 * PIXELS_COUNT];
static uint8_t pixels_tables64[4 * PIXELS_COUNT];
static uint8_t pixels_gstreamer[4 * PIXELS_COUNT];
static uint8_t pixels_see[4 * PIXELS_COUNT];
static uint8_t pixels_see2[4 * PIXELS_COUNT];

enum {KB = 1024, MB = KB * 1024, GB = MB * 1024};
#define Mbench(name, calls, data_len, call) ({\
  uint64_t i = calls;\
  double start = omp_get_wtime();\
  do {\
    call;\
  } while(--i);\
  double time = omp_get_wtime() - start;\
  fprintf(stderr, "%s: (%lu)%luKB: total %luGB: %fGB/s\n", name, (uint64_t)calls, (uint64_t)data_len/KB, (data_len * calls)/GB, ((double)(data_len * calls)/time)/GB);\
})

int main(int argc, char * argv[]) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  srand(now.tv_nsec);
  // prepare input buffers

  for(uint32_t i = 0; i < 4 * PIXELS_COUNT; i++) {
    uint8_t x = ((i % 4) * rand());
    pixels_wiki[i] = x;
    pixels_novel_ch[i] = x;
    pixels_tables[i] = x;
    pixels_tables64[i] = x;
    pixels_gstreamer[i] = x;
    pixels_see[i] = x;
    pixels_see2[i] = x;
  }

  // run conversations
  Mbench("rgb2yuv_wiki", (1024 * 1024), sizeof(pixels_wiki), rgb2yuv_wiki(pixels_wiki, PIXELS_COUNT));
  Mbench("rgb2yuv_novell_ch", (1024 * 1024), sizeof(pixels_novel_ch), rgb2yuv_novell_ch(pixels_novel_ch, PIXELS_COUNT));
  Mbench("rgb2yuv_tables", (1024 * 1024), sizeof(pixels_tables), rgb2yuv_tables(pixels_tables, PIXELS_COUNT));
  Mbench("rgb2yuv_tables64", (1024 * 1024), sizeof(pixels_tables), rgb2yuv_tables64(pixels_tables, PIXELS_COUNT));
  Mbench("rgb2yuv_gstreamer", (1024 * 1024), sizeof(pixels_gstreamer), rgb2yuv_gstreamer(pixels_gstreamer, PIXELS_COUNT));
  Mbench("rgb2yuv_sse", (1024 * 1024), sizeof(pixels_see), rgb2yuv_sse(pixels_see, PIXELS_COUNT));
  Mbench("rgb2yuv_sse2", (1024 * 1024), sizeof(pixels_see2), rgb2yuv_sse2(pixels_see2, PIXELS_COUNT));


  for(uint32_t i = 0; i < 15; i++) {
    printf("%d: %d %d %d %d %d %d %d\n", i, pixels_wiki[i], pixels_novel_ch[i], pixels_tables[i], pixels_tables64[i], pixels_gstreamer[i], pixels_see[i], pixels_see2[i]);
  }

  return 0;
}
