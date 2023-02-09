#ifndef LAPS_H
#define LAPS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// utils
#define return_defer(val) \
    do                    \
    {                     \
        result = (val);   \
        goto defer;       \
    } while (0)

typedef int Errno;

#define LAPS_SWAP(T, a, b) \
    do                     \
    {                      \
        T temp = a;        \
        a = b;             \
        b = temp;          \
    } while (0)

#define LAPS_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define LAPS_ABS(T, x) (LAPS_SIGN(T, x) * (x))
#define LAPS_PIXEL(c, x, y) (c).pixels[(y) * (c).stride + (x)]

typedef struct
{
    uint32_t *pixels;
    size_t width;
    size_t height;
    size_t stride;
} Laps_Canvas;

#define LAPS_CANVAS_NULL ((Laps_Canvas){0})

Laps_Canvas laps_canvas(uint32_t *pixels, size_t width, size_t height)
{
    Laps_Canvas c = {
        .pixels = pixels,
        .width = width,
        .height = height,
        .stride = width,
    };
    return c;
}

typedef struct
{
    // Safe ranges to iterate over.
    int x1, x2;
    int y1, y2;

    // Original uncut ranges some parts of which may be outside of the canvas boundaries.
    int ox1, ox2;
    int oy1, oy2;
} Laps_Normalized_Rect;

int laps_save_to_ppm(Laps_Canvas c, const char *filePath)
{
    FILE *fp = fopen(filePath, "wb"); // wb - write binary

    // assert(fp != NULL && "File Could Not Open");

    // assert(fprintf(fp, "P6\n%d %d 255\n", width, height) > 0 && "Could not add file header!");
    fprintf(fp, "P6\n%d %d 255\n", c.width, c.height);

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

bool laps_normalize_rect(int x, int y, int w, int h,
                         size_t canvas_width, size_t canvas_height,
                         Laps_Normalized_Rect *nr)
{
    // No need to render empty rectangle
    if (w == 0)
        return false;
    if (h == 0)
        return false;

    nr->ox1 = x;
    nr->oy1 = y;

    // Convert the rectangle to 2-points representation
    nr->ox2 = nr->ox1 + LAPS_SIGN(int, w) * (LAPS_ABS(int, w) - 1);
    if (nr->ox1 > nr->ox2)
        LAPS_SWAP(int, nr->ox1, nr->ox2);
    nr->oy2 = nr->oy1 + LAPS_SIGN(int, h) * (LAPS_ABS(int, h) - 1);
    if (nr->oy1 > nr->oy2)
        LAPS_SWAP(int, nr->oy1, nr->oy2);

    // Cull out invisible rectangle
    if (nr->ox1 >= (int)canvas_width)
        return false;
    if (nr->ox2 < 0)
        return false;
    if (nr->oy1 >= (int)canvas_height)
        return false;
    if (nr->oy2 < 0)
        return false;

    nr->x1 = nr->ox1;
    nr->y1 = nr->oy1;
    nr->x2 = nr->ox2;
    nr->y2 = nr->oy2;

    // Clamp the rectangle to the boundaries
    if (nr->x1 < 0)
        nr->x1 = 0;
    if (nr->x2 >= (int)canvas_width)
        nr->x2 = (int)canvas_width - 1;
    if (nr->y1 < 0)
        nr->y1 = 0;
    if (nr->y2 >= (int)canvas_height)
        nr->y2 = (int)canvas_height - 1;

    return true;
}

Laps_Canvas laps_subcanvas(Laps_Canvas c, int x, int y, int w, int h)
{
    Laps_Normalized_Rect nr = {0};
    if (!laps_normalize_rect(x, y, w, h, c.width, c.height, &nr))
        return LAPS_CANVAS_NULL;
    c.pixels = &LAPS_PIXEL(c, nr.x1, nr.y1);
    c.width = nr.x2 - nr.x1 + 1;
    c.height = nr.y2 - nr.y1 + 1;
    return c;
}

#define LAPS_RED(color) (((color)&0x000000FF) >> (8 * 0))
#define LAPS_GREEN(color) (((color)&0x0000FF00) >> (8 * 1))
#define LAPS_BLUE(color) (((color)&0x00FF0000) >> (8 * 2))
#define LAPS_ALPHA(color) (((color)&0xFF000000) >> (8 * 3))
#define LAPS_RGBA(r, g, b, a) ((((r)&0xFF) << (8 * 0)) | (((g)&0xFF) << (8 * 1)) | (((b)&0xFF) << (8 * 2)) | (((a)&0xFF) << (8 * 3)))

void laps_blend_color(uint32_t *c1, uint32_t c2)
{
    uint32_t r1 = LAPS_RED(*c1);
    uint32_t g1 = LAPS_GREEN(*c1);
    uint32_t b1 = LAPS_BLUE(*c1);
    uint32_t a1 = LAPS_ALPHA(*c1);

    uint32_t r2 = LAPS_RED(c2);
    uint32_t g2 = LAPS_GREEN(c2);
    uint32_t b2 = LAPS_BLUE(c2);
    uint32_t a2 = LAPS_ALPHA(c2);

    r1 = (r1 * (255 - a2) + r2 * a2) / 255;
    if (r1 > 255)
        r1 = 255;
    g1 = (g1 * (255 - a2) + g2 * a2) / 255;
    if (g1 > 255)
        g1 = 255;
    b1 = (b1 * (255 - a2) + b2 * a2) / 255;
    if (b1 > 255)
        b1 = 255;

    *c1 = LAPS_RGBA(r1, g1, b1, a1);
}
// end utils

// Fills the canvas with one color
void laps_fill(Laps_Canvas c, uint32_t color)
{
    for (size_t y = 0; y < c.height; ++y)
    {
        for (size_t x = 0; x < c.width; ++x)
        {
            LAPS_PIXEL(c, x, y) = color;
        }
    }
}

// Draws line from two points
void laps_line(Laps_Canvas c, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    // If both of the differences are 0 there will be a division by 0 below.
    if (dx == 0 && dy == 0)
    {
        if (0 <= x1 && x1 < (int)c.width && 0 <= y1 && y1 < (int)c.height)
        {
            laps_blend_color(&LAPS_PIXEL(c, x1, y1), color);
        }
        return;
    }

    if (LAPS_ABS(int, dx) > LAPS_ABS(int, dy))
    {
        if (x1 > x2)
        {
            LAPS_SWAP(int, x1, x2);
            LAPS_SWAP(int, y1, y2);
        }

        // Cull out invisible line
        if (x1 > (int)c.width)
            return;
        if (x2 < 0)
            return;

        // Clamp the line to the boundaries
        if (x1 < 0)
            x1 = 0;
        if (x2 >= (int)c.width)
            x2 = (int)c.width - 1;

        for (int x = x1; x <= x2; ++x)
        {
            int y = dy * (x - x1) / dx + y1;
            // TODO: move boundary checks out side of the loops in LAPS_draw_line
            if (0 <= y && y < (int)c.height)
            {
                laps_blend_color(&LAPS_PIXEL(c, x, y), color);
            }
        }
    }
    else
    {
        if (y1 > y2)
        {
            LAPS_SWAP(int, x1, x2);
            LAPS_SWAP(int, y1, y2);
        }

        // Cull out invisible line
        if (y1 > (int)c.height)
            return;
        if (y2 < 0)
            return;

        // Clamp the line to the boundaries
        if (y1 < 0)
            y1 = 0;
        if (y2 >= (int)c.height)
            y2 = (int)c.height - 1;

        for (int y = y1; y <= y2; ++y)
        {
            int x = dx * (y - y1) / dy + x1;
            // TODO: move boundary checks out side of the loops in LAPS_draw_line
            if (0 <= x && x < (int)c.width)
            {
                laps_blend_color(&LAPS_PIXEL(c, x, y), color);
            }
        }
    }
}

// Fills rectangle with one color
void laps_rectange(Laps_Canvas c,
                   uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h,
                   uint32_t color)
{
    Laps_Normalized_Rect nr = {0};
    if (!laps_normalize_rect(x, y, w, h, c.width, c.height, &nr))
        return;
    for (int x = nr.x1; x <= nr.x2; ++x)
    {
        for (int y = nr.y1; y <= nr.y2; ++y)
        {
            laps_blend_color(&LAPS_PIXEL(c, x, y), color);
        }
    }
}

// Fills circle with one color
void laps_circle(Laps_Canvas c,
                 uint16_t cx, uint16_t cy, uint16_t r,
                 uint32_t color)
{
    for (uint16_t y = cy - r; y < cy + r; ++y)
    {
        if (y < c.height)
        {
            for (uint16_t x = cx - r; x < cx + r; ++x)
            {
                if (x < c.width)
                {
                    uint16_t dy = LAPS_ABS(uint16_t, cy - y);
                    uint16_t dx = LAPS_ABS(uint16_t, cx - x);

                    uint32_t d = dx * dx + dy * dy;

                    if (r * r >= d)
                    {
                        laps_blend_color(&LAPS_PIXEL(c, x, y), color);
                    }
                }
            }
        }
    }
}

void barycentric(int x1, int y1, int x2, int y2, int x3, int y3,
                 int xp, int yp,
                 int *u1, int *u2, int *det)
{
    *det = ((x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3));
    *u1 = ((y2 - y3) * (xp - x3) + (x3 - x2) * (yp - y3));
    *u2 = ((y3 - y1) * (xp - x3) + (x1 - x3) * (yp - y3));
    // u3 = det - u1 - u2
}

bool laps_normalize_triangle(size_t width, size_t height, int x1, int y1, int x2, int y2, int x3, int y3, int *lx, int *hx, int *ly, int *hy)
{
    *lx = x1;
    *hx = x1;
    if (*lx > x2)
        *lx = x2;
    if (*lx > x3)
        *lx = x3;
    if (*hx < x2)
        *hx = x2;
    if (*hx < x3)
        *hx = x3;
    if (*lx < 0)
        *lx = 0;
    if ((size_t)*lx >= width)
        return false;
    ;
    if (*hx < 0)
        return false;
    ;
    if ((size_t)*hx >= width)
        *hx = width - 1;

    *ly = y1;
    *hy = y1;
    if (*ly > y2)
        *ly = y2;
    if (*ly > y3)
        *ly = y3;
    if (*hy < y2)
        *hy = y2;
    if (*hy < y3)
        *hy = y3;
    if (*ly < 0)
        *ly = 0;
    if ((size_t)*ly >= height)
        return false;
    ;
    if (*hy < 0)
        return false;
    ;
    if ((size_t)*hy >= height)
        *hy = height - 1;

    return true;
}

// Fills triangle with one color
void laps_triangle(Laps_Canvas c,
                   uint16_t x1, uint16_t y1,
                   uint16_t x2, uint16_t y2,
                   uint16_t x3, uint16_t y3,
                   uint32_t color)
{
    int lx, hx, ly, hy;
    if (laps_normalize_triangle(c.width, c.height, x1, y1, x2, y2, x3, y3, &lx, &hx, &ly, &hy))
    {
        for (int y = ly; y <= hy; ++y)
        {
            for (int x = lx; x <= hx; ++x)
            {
                int u1, u2, det;
                barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det);
                int u3 = det - u1 - u2;
                if (
                    (LAPS_SIGN(int, u1) == LAPS_SIGN(int, det) || u1 == 0) &&
                    (LAPS_SIGN(int, u2) == LAPS_SIGN(int, det) || u2 == 0) &&
                    (LAPS_SIGN(int, u3) == LAPS_SIGN(int, det) || u3 == 0))
                {
                    laps_blend_color(&LAPS_PIXEL(c, x, y), color);
                }
            }
        }
    }
}

#endif // LAPS_H