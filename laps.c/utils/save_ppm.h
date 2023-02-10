#ifndef SAVE_PPM_H
#define SAVE_PPM_H

#include <stdio.h>
#include "../laps.h"

int laps_save_to_ppm(Laps_Canvas c, const char *filePath)
{
    FILE *fp = fopen(filePath, "wb"); // wb - write binary

    // assert(fp != NULL && "File Could Not Open");

    // assert(fprintf(fp, "P6\n%d %d 255\n", width, height) > 0 && "Could not add file header!");
    fprintf(fp, "P6\n%u %u 255\n", c.width, c.height);

    for (size_t i = 0; i < c.width * c.height; ++i)
    {
        uint32_t pixel = c.pixels[i];
        uint8_t bytes[3] = {
            (pixel >> (8 * 0)) & 0xFF,
            (pixel >> (8 * 1)) & 0xFF,
            (pixel >> (8 * 2)) & 0xFF};
        fwrite(bytes, sizeof(bytes), 1, fp);
    };
    printf("Saved File!");
    return 0;
}

#endif // SAVE_PPM_H