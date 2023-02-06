#include "laps.h"

#define WIDTH 300
#define HEIGHT 200

#define WHITE_COLOR 0xFFFFFFFF
#define PURPLE_COLOR 0xFFFF00FF
#define BACKGROUND_COLOR WHITE_COLOR
#define FOREGROUND_COLOR PURPLE_COLOR

uint32_t *main(void)
{
    uint32_t pixels[WIDTH * HEIGHT];

    laps_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);
    laps_fill_triangle(pixels, WIDTH, HEIGHT, WIDTH / 2, 0, 0, HEIGHT, WIDTH, HEIGHT, FOREGROUND_COLOR);

    return pixels;
}