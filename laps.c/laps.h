#ifndef LAPS_H
#define LAPS_H

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// macros
#define SWAP(a, b) do { typeof(a) temp = a; a = b; b = temp; } while (0)

void laps_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color) {
    for (size_t i = 0; i < width*height; ++i) {
        pixels[i] = color;
    }
}

void laps_draw_rectange(uint32_t *pixels, uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch, uint16_t width, uint16_t height, uint32_t color) {
    for (int y = cy-(cw/2); y < cy+(cw/2); ++y) {
        for (int x = cx-(ch/2); x < cx+(ch/2); ++x) {
            pixels[y*width + x] = color;
        }
    }
}

void laps_draw_circle(uint32_t *pixels, uint16_t cx, uint16_t cy, uint16_t r, size_t width, size_t height, uint32_t color) {
    for (int y = cy-r; y < cy+r; ++y) {
        for (int x = cx-r; x < cx+r; ++x) {
            
            uint16_t dy = abs(cy - y);
            uint16_t dx = abs(cx - x);

            uint32_t d = dx*dx + dy*dy;

            if (r*r >= d) {
                pixels[y*width + x] = color;
            }
        }
    }
}

void laps_draw_line(uint32_t *pixels, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, size_t width, size_t height, uint32_t color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    if (dx != 0) {
        uint16_t c = y1 - dy*x1/dx;

        if (x2 < x1) SWAP(x1, x2);
        for (uint16_t x = x1; x <= x2; ++x) {
            if (0 <= x && x < width) {
                uint16_t sy1 = dy*x/dx + c;
                uint16_t sy2 = dy*(x + 1)/dx + c;
                for (uint16_t y = sy1; y <= sy2; ++y) {
                    if (0 <= y && y < height) {
                        pixels[y*width + x] = color;
                        printf("x = %d, y = %d", x, y);
                    }
                }
            }
        }
    }
    else {
        uint16_t x = x1;
        if (0 <= x && x < height) {
            if (y1 < y2) SWAP(y1, y2);
            for (uint16_t y = y1; y < y2; ++y) {
                if (0 <= y1 && y1 <= width) {
                    pixels[y*width + x] = color;
                }
            }
        }
    }
    
}

int laps_save_to_ppm(uint32_t *pixels, size_t width, size_t height, const char *filePath) {
    FILE *fp = fopen(filePath, "wb"); // wb - write binary
    
    assert(fp != NULL && "File Could Not Open"); 

    assert(fprintf(fp, "P6\n%zu %zu 255\n", width, height) > 0 && "Could not add file header!");

    for (size_t i = 0; i < width*height; ++i) {
        uint32_t pixel = pixels[i];
        uint8_t bytes[3] = {
            (pixel>>(8*0))&0xFF,
            (pixel>>(8*1))&0xFF,
            (pixel>>(8*2))&0xFF
        };
        fwrite(bytes, sizeof(bytes), 1, fp);
    };
    return 1;
}

#endif // LAPS_H