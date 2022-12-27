#include "laps.h"
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600

#define BACKGROUND_COLOR 0xFFFFFFFF
#define FOREGROUND_COLOR 0xFFFF00FF

int main(int argc, char* argv[]) {
    
    uint32_t pixels[WIDTH*HEIGHT];

    // available functions in readme file
    laps_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);
    laps_fill_rectange(pixels, WIDTH, HEIGHT, WIDTH/2, HEIGHT/2, 100, 100, 0xFFFF00FF);
    laps_draw_triangle(pixels, WIDTH, HEIGHT, 0, 0, 20, 100, 200, 200, 0xFFFF00FF);
    laps_save_to_ppm(pixels, WIDTH, HEIGHT, "test.ppm");

    return 0;
}