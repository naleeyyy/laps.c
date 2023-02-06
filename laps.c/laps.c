#include "laps.h"

#define WIDTH 800
#define HEIGHT 600

#define WHITE_COLOR 0xFFFFFFFF
#define BLACK_COLOR 0xFF000000
#define RED_COLOR 0xFF0000FF
#define GREEN_COLOR 0xFF00FF00
#define BLUE_COLOR 0xFFFF0000
#define PURPLE_COLOR 0xFFFF00FF

#define BACKGROUND_COLOR WHITE_COLOR
#define FOREGROUND_COLOR PURPLE_COLOR

int main(void)
{
    uint32_t pixels[WIDTH * HEIGHT];

    // available functions in readme file
    laps_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);
    laps_fill_triangle(pixels, WIDTH, HEIGHT, WIDTH / 2, 0, 0, HEIGHT, WIDTH, HEIGHT / 2, FOREGROUND_COLOR);
    laps_save_to_ppm(pixels, WIDTH, HEIGHT, "test.ppm");

    return 0;
}