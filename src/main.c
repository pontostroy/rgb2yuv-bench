#include <time.h>

#include "rgb2yuv.h"
#include <stdlib.h>
#include <stdio.h>

#define PIXELS_COUNT	(100 * 1000 * 1000)

static uint8_t pixels_wiki[4 * PIXELS_COUNT];
static uint8_t pixels_novel_ch[4 * PIXELS_COUNT];
static uint8_t pixels_tables[4 * PIXELS_COUNT];
static uint8_t pixels_gstreamer[4 * PIXELS_COUNT];
static uint8_t pixels_see[4 * PIXELS_COUNT];

int main(int argc, char *argv[])
{
	struct timespec now;
	clock_t start, finish;

	clock_gettime(CLOCK_REALTIME, &now);
	srand(now.tv_nsec);

	// prepare input buffers

	int i;
	for (i = 0; i < 4 * PIXELS_COUNT; i++)
	{
		uint8_t x = ((i % 4) * rand());
		pixels_wiki[i] = x;
		pixels_novel_ch[i] = x;
		pixels_tables[i] = x;
                pixels_gstreamer[i] = x;
                pixels_see[i] = x;
	}

	// run conversations

	start = clock();
	rgb2yuv_wiki(pixels_wiki, PIXELS_COUNT);
	finish = clock();
	printf("rgb2yuv_wiki: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);

	start = clock();
	rgb2yuv_novell_ch(pixels_novel_ch, PIXELS_COUNT);
	finish = clock();
	printf("rgb2yuv_novell_ch: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);

	start = clock();
	rgb2yuv_tables_init();
	rgb2yuv_tables(pixels_tables, PIXELS_COUNT);
	finish = clock();
	printf("rgb2yuv_tables: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);
        
        start = clock();
        rgb2yuv_gstreamer(pixels_gstreamer, PIXELS_COUNT);
        finish = clock();
        printf("rgb2yuv_gstreamer: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);
        
        start = clock();
        rgb2yuv_sse(pixels_see, PIXELS_COUNT);
        finish = clock();
        printf("rgb2yuv_sse: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);


	for (i = 0; i < 100; i++)
	{
		printf("%d: %d %d %d %d %d\n", i, pixels_wiki[i], pixels_novel_ch[i], pixels_tables[i], pixels_gstreamer[i], pixels_see[i]);
	}

	return 0;
}
