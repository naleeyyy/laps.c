#include "laps.h"
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600

#define BACKGROUND_COLOR 0xFFFFFFFF
#define FOREGROUND_COLOR 0xFFFF00FF

int main(int argc, char* argv[]) {
    uint32_t pixels[WIDTH*HEIGHT];
    
    laps_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);
    // laps_draw_rectange(pixels, WIDTH/2, HEIGHT/2, 100, 100, WIDTH, HEIGHT, 0xFFFF00FF);
    // laps_draw_circle(pixels, WIDTH/2, HEIGHT/2, 25, WIDTH, HEIGHT, 0xFF00FFFF);
    // laps_draw_line(pixels, 0, 0, HEIGHT, WIDTH, WIDTH, HEIGHT, 0xFF00FFFF);
    laps_draw_line(pixels, 0, 0, WIDTH, HEIGHT, WIDTH, HEIGHT, FOREGROUND_COLOR);
    laps_save_to_ppm(pixels, WIDTH, HEIGHT, "test.ppm");

    return 0;
}