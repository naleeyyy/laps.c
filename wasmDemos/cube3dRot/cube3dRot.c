#define PLATFORM_WASM 1

#include "../../utils/linear_algebra.h"

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

Laps_Canvas render(float dt)
{
    Laps_Canvas c = laps_canvas(pixels, WIDTH, HEIGHT);

    laps_fill(c, BACKGROUND_COLOR);

    float fNear = 0.1f;
    float fFar = 100.0f;
    float fFov = 50.0f;
    float fAspectRatio = ((float)HEIGHT / (float)WIDTH);
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * PI);

    mesh3d mesh;
    size_t size = 12;

    init_mesh3d(&mesh, size);
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(0.0f, 0.0f, 0.0f), make_vec3d(0.0f, 1.0f, 0.0f), make_vec3d(1.0f, 1.0f, 0.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(0.0f, 0.0f, 0.0f), make_vec3d(1.0f, 1.0f, 0.0f), make_vec3d(1.0f, 0.0f, 0.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(1.0f, 0.0f, 0.0f), make_vec3d(1.0f, 1.0f, 0.0f), make_vec3d(1.0f, 1.0f, 1.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(1.0f, 0.0f, 0.0f), make_vec3d(1.0f, 1.0f, 1.0f), make_vec3d(1.0f, 0.0f, 1.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(1.0f, 0.0f, 1.0f), make_vec3d(1.0f, 1.0f, 1.0f), make_vec3d(0.0f, 1.0f, 1.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(1.0f, 0.0f, 1.0f), make_vec3d(0.0f, 1.0f, 1.0f), make_vec3d(0.0f, 0.0f, 1.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(0.0f, 0.0f, 1.0f), make_vec3d(0.0f, 1.0f, 1.0f), make_vec3d(0.0f, 1.0f, 0.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(0.0f, 0.0f, 1.0f), make_vec3d(0.0f, 1.0f, 0.0f), make_vec3d(0.0f, 0.0f, 0.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(0.0f, 1.0f, 0.0f), make_vec3d(0.0f, 1.0f, 1.0f), make_vec3d(1.0f, 1.0f, 1.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(0.0f, 1.0f, 0.0f), make_vec3d(1.0f, 1.0f, 1.0f), make_vec3d(1.0f, 1.0f, 0.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(1.0f, 0.0f, 1.0f), make_vec3d(0.0f, 0.0f, 1.0f), make_vec3d(0.0f, 0.0f, 0.0f)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(1.0f, 0.0f, 1.0f), make_vec3d(0.0f, 0.0f, 0.0f), make_vec3d(1.0f, 0.0f, 0.0f)));

    mat4x4 matRotZ = init_mat4x4();
    mat4x4 matRotX = init_mat4x4();
    fTheta += 1.0f * dt;

    // Rotation Z
    matRotZ.m[0][0] = cosf(fTheta);
    matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta);
    matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    // Rotation X
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fTheta * 0.5f);
    matRotX.m[1][2] = sinf(fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(fTheta * 0.5f);
    matRotX.m[2][2] = cosf(fTheta * 0.5f);
    matRotX.m[3][3] = 1;

    mat4x4 matProj = init_mat4x4();

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    for (size_t i = 0; i < mesh.used; ++i)
    {
        triangle3d triProjected, triTranslated, triRotatedZ, triRotatedZX;

        // Rotate in Z-Axis
        MultiplyMatrixTriangle(&mesh.tris[i], &triRotatedZ, &matRotZ);

        // Rotate in X-Axis
        MultiplyMatrixTriangle(&triRotatedZ, &triRotatedZX, &matRotX);

        // Offset into the screen
        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

        // Project triangles from 3D --> 2D
        MultiplyMatrixTriangle(&triTranslated, &triProjected, &matProj);

        // Scale into view
        triProjected.p[0].x += 1.0f;
        triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f;
        triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f;
        triProjected.p[2].y += 1.0f;
        triProjected.p[0].x *= 0.5f * (float)WIDTH;
        triProjected.p[0].y *= 0.5f * (float)HEIGHT;
        triProjected.p[1].x *= 0.5f * (float)WIDTH;
        triProjected.p[1].y *= 0.5f * (float)HEIGHT;
        triProjected.p[2].x *= 0.5f * (float)WIDTH;
        triProjected.p[2].y *= 0.5f * (float)HEIGHT;

        // Rasterize triangle
        laps_hollow_triangle(c, (uint16_t)triProjected.p[0].x, (uint16_t)triProjected.p[0].y,
                             (uint16_t)triProjected.p[1].x, (uint16_t)triProjected.p[1].y,
                             (uint16_t)triProjected.p[2].x, (uint16_t)triProjected.p[2].y,
                             0xFFFFFFFF);
    }
    free_mesh3d(&mesh);
    return c;
}
