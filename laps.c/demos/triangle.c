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
    laps_triangle(c, WIDTH / 2, 0, 0, HEIGHT, WIDTH, HEIGHT / 4, 0x55FF0000);
    laps_triangle(c, WIDTH / 2, 0, 0, HEIGHT, WIDTH, HEIGHT / 2, 0x55AA00FF);
    laps_triangle(c, WIDTH / 2, 0, 0, HEIGHT, WIDTH, HEIGHT * 3 / 4, 0x550000FF);
    laps_save_to_ppm(c, "./images/triangle.ppm");

    return 0;
}
