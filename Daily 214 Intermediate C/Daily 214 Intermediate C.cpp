// Daily 214 Intermediate C.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

struct sheet {
	unsigned short c;
	unsigned long x, y, w, h;

};

static bool
sheet_read(struct sheet *s) {
	return scanf("%hu %lu %lu %lu %lu", &s->c, &s->x, &s->y, &s->w, &s->h) == 5;
}

static bool
sheet_inside(const struct sheet *s, unsigned long x, unsigned long y) {
	return x >= s->x && y >= s->y && x < s->x + s->w && y < s->y + s->h;
}


int main(void)
{

	/* Read all sheets. */
	unsigned long width, height;
	scanf("%lu %lu", &width, &height);
	struct sheet sheets[USHRT_MAX] = { {0,0,0, width, height} };
	unsigned short sheet_count = 1;
	while (sheet_read(sheets + sheet_count)) {
		sheet_count++;
	}

	/* Sum areas */
	unsigned long sums[USHRT_MAX] = { 0 };
#pragma omp parallel for schedule(dynamic, 1)

	for (unsigned long iy = 0; iy < height; iy++) {
		unsigned long tmp_sums[USHRT_MAX] = { 0 };
		for (unsigned long ix = 0; ix < width; ix++) {
			for (int i = sheet_count - 1; i < sheet_count; i--) {
				if (sheet_inside(sheets + i, ix, iy)) {
					tmp_sums[sheets[i].c]++;
					break;
				}
			}
		}

		/* Copy to main totals. */
		for (int i = 0; i < UCHAR_MAX; i++) {
			if (tmp_sums[i] > 0)
			{
#pragma omp atomic
				sums[i] += tmp_sums[i];
			}
		}
	}
	/* Print results */
	for (int i = 0; i < USHRT_MAX; i++)
	{
		if (sums[i] > 0)
		{
			printf("%d %lu\n", i, sums[i]);
		}
	}
    return 0;
}

