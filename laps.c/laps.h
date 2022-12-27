#ifndef LAPS_H
#define LAPS_H

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// utils

#define LAPS_SWAP(T, a, b) do { T temp = a; a = b; b = temp; } while (0)

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
    printf("Saved File!");
    return 1;
}

void sort_triangle_points_by_y(uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2, uint16_t *x3, uint16_t *y3) {
    if (*y1 > *y2) {
        LAPS_SWAP(uint16_t, *x1, *x2);
        LAPS_SWAP(uint16_t, *y1, *y2);
    }

    if (*y2 > *y3) {
        LAPS_SWAP(uint16_t, *x2, *x3);
        LAPS_SWAP(uint16_t, *y2, *y3);
    }

    if (*y1 > *y2) {
        LAPS_SWAP(uint16_t, *x1, *x2);
        LAPS_SWAP(uint16_t, *y1, *y2);
    }
}

// end utils

void laps_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color) {
    for (size_t i = 0; i < width*height; ++i) {
        pixels[i] = color;
    }
}

void laps_fill_rectange(uint32_t *pixels, uint16_t width, uint16_t height,
                        uint16_t cx, uint16_t cy, 
                        uint16_t cw, uint16_t ch, 
                    uint32_t color) 
{
    for (int y = cy-(cw/2); y < cy+(cw/2); ++y) {
        for (int x = cx-(ch/2); x < cx+(ch/2); ++x) {
            pixels[y*width + x] = color;
        }
    }
}

void laps_fill_circle(uint32_t *pixels, size_t width, size_t height,
                      uint16_t cx, uint16_t cy, uint16_t r, 
                  uint32_t color) 
{
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

void laps_draw_line(uint32_t *pixels, size_t width, size_t height,
                    uint16_t x1, uint16_t y1, 
                    uint16_t x2, uint16_t y2, 
                uint32_t color) 
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    if (dx != 0) {
        uint16_t c = y1 - dy*x1/dx;

        if (x2 < x1) LAPS_SWAP(uint16_t, x1, x2);
        for (uint16_t x = x1; x <= x2; ++x) {
            if (0 <= x && x < width) {
                uint16_t sy1 = dy*x/dx + c;
                uint16_t sy2 = dy*(x + 1)/dx + c;
                for (uint16_t y = sy1; y <= sy2; ++y) {
                    if (0 <= y && y < height) {
                        pixels[y*width + x] = color;
                    }
                }
            }
        }
    }
    else {
        uint16_t x = x1;
        if (0 <= x && x < height) {
            if (y1 < y2) LAPS_SWAP(uint16_t, y1, y2);
            for (uint16_t y = y1; y < y2; ++y) {
                if (0 <= y1 && y1 <= width) {
                    pixels[y*width + x] = color;
                }
            }
        }
    }
    
}

void laps_draw_triangle(uint32_t *pixels, size_t width, size_t height, 
                        uint16_t x1, uint16_t y1, 
                        uint16_t x2, uint16_t y2, 
                        uint16_t x3, uint16_t y3, 
                    uint32_t color)
{
    laps_draw_line(pixels, width, height, x1, y1, x2, y2, color);
    laps_draw_line(pixels, width, height, x1, y1, x3, y3, color);
    laps_draw_line(pixels, width, height, x2, y2, x3, y3, color);
}

void laps_fill_triangle(uint32_t *pixels, size_t width, size_t height,
                        uint16_t x1, uint16_t y1, 
                        uint16_t x2, uint16_t y2, 
                        uint16_t x3, uint16_t y3, 
                    uint32_t color) 
{
    sort_triangle_points_by_y(&x1, &y1, &x2, &y2, &x3, &y3);
    return;
}


#endif // LAPS_H