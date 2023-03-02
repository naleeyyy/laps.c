#include "../../laps.h"
#define PLATFORM_WASM
#include "../../utils/linear_algebra.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818

static uint32_t pixels[WIDTH * HEIGHT];

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);
float tanf(float x);

#define PI 3.14159265359
float fTheta = 0;

#define POINT_COUNT 500

vec3d points[POINT_COUNT];

int initialised = 0;
Laps_Canvas render(float dt)
{
    if (initialised = 0)
    {
        for (size_t i = 0; i < POINT_COUNT; ++i)
        {
            points[i] = make_vec3d(rand(), rand(), rand());
        }
        initialised = 1;
    }
    Laps_Canvas c = laps_canvas(pixels, WIDTH, HEIGHT);

    laps_fill(c, BACKGROUND_COLOR);

    float fNear = 0.1f;
    float fFar = 100.0f;
    float fFov = 50.0f;
    float fAspectRatio = ((float)HEIGHT / (float)WIDTH);
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * PI);

    mat4x4 matProj = init_mat4x4();

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    for (size_t i = 0; i < POINT_COUNT; ++i)
    {
        vec3d point = points[i], pointProjected;
        point.x += (float)(10.0f * (point.y - point.x) * dt);
        point.y += (float)((point.x * (30.0f - point.z) - point.y) * dt);
        point.z += (float)((point.x * point.y - 8 / 3 * point.z) * dt);

        MultiplyMatrixVector(&point, &pointProjected, &matProj);
        // scale_vec3d(&pointProjected, (float)WIDTH, (float)HEIGHT);

        printf("x: %f, y: %f \n", point.x, point.y);

        laps_blend_color(&LAPS_PIXEL(c, (uint16_t)pointProjected.x, (uint16_t)pointProjected.y), 0xFFFFFFFF);
    }

    return c;
}

int main()
{

    render(1.0f);

    return 0;
}