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

uint32_t pixels[WIDTH * HEIGHT];
int main(void)
{
    Laps_Canvas c = laps_canvas(pixels, WIDTH, HEIGHT);
    laps_fill(c, BACKGROUND_COLOR);
    laps_rectange(c, 0, 0, WIDTH * 3 / 5, HEIGHT * 3 / 5, 0x5500FF00);
    laps_rectange(c, WIDTH * 2 / 5, HEIGHT * 2 / 5, WIDTH, HEIGHT, 0x55AA00FF);
    laps_aa_circle(c, WIDTH / 2, HEIGHT / 2, WIDTH / 5, 0x33AA0000);
    laps_save_to_ppm(c, "test.ppm");

    return 0;
}