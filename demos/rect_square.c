#include "../laps.h"
#include "../utils/save_ppm.h"

#define WIDTH 800
#define HEIGHT 600

#define BACKGROUND_COLOR 0xFFFFFFFF

uint32_t pixels[WIDTH * HEIGHT];
int main(void)
{
    Laps_Canvas c = laps_canvas(pixels, WIDTH, HEIGHT);
    laps_fill(c, BACKGROUND_COLOR);
    laps_rectange(c, 0, 0, WIDTH * 3 / 5, HEIGHT * 3 / 5, 0x5500FF00);
    laps_rectange(c, WIDTH * 2 / 5, HEIGHT * 2 / 5, WIDTH, HEIGHT, 0x55AA00FF);
    laps_circle(c, WIDTH / 2, HEIGHT / 2, 50, 0x33AA0000);
    laps_save_to_ppm(c, "./images/rect_square.ppm");

    return 0;
}