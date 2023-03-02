#include <stdio.h>
#include <math.h>

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "../utils/tinyobjloader.h"

#include "../laps.h"
#include "../utils/linear_algebra.h"
#include "../utils/save_ppm.h"

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818
#define PI 3.14159265359
float fTheta = 0;

static uint32_t pixels[WIDTH * HEIGHT];

void loadFile(void *ctx, const char *filename, const int is_mtl, const char *obj_filename, char **buffer, size_t *len)
{
    long string_size = 0, read_size = 0;
    FILE *handler = fopen(filename, "r");

    if (handler)
    {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
        *buffer = (char *)malloc(sizeof(char) * (string_size + 1));
        read_size = fread(*buffer, sizeof(char), (size_t)string_size, handler);
        (*buffer)[string_size] = '\0';
        if (string_size != read_size)
        {
            free(buffer);
            *buffer = NULL;
        }
        fclose(handler);
    }

    *len = read_size;
}

int main(void)
{
    Laps_Canvas c = laps_canvas(pixels, WIDTH, HEIGHT);
    // laps_fill(c, 0xFF202020);

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

    const char *filename = "../models/sphere.obj";
    const char *search_path = NULL;

    tinyobj_shape_t *shape = NULL;
    tinyobj_material_t *material = NULL;

    tinyobj_attrib_t attrib;
    tinyobj_attrib_init(&attrib);

    unsigned int num_shapes;
    unsigned int num_materials;

    tinyobj_parse_obj(&attrib, &shape, &num_shapes, &material, &num_materials, filename, loadFile, NULL, TINYOBJ_FLAG_TRIANGULATE);
    int i = 0;

    while (i < attrib.num_vertices)
    {
        vec3d vec1 = make_vec3d(attrib.vertices[i + 0], attrib.vertices[i + 1], attrib.vertices[i + 2]);
        vec3d vec2 = make_vec3d(attrib.vertices[i + 3], attrib.vertices[i + 4], attrib.vertices[i + 5]);
        vec3d vec3 = make_vec3d(attrib.vertices[i + 6], attrib.vertices[i + 7], attrib.vertices[i + 8]);

        triangle3d tri, triProjected, triTranslated;
        tri = make_triangle3d(vec1, vec2, vec3);

        translate_triangle3d(&tri, &triTranslated, 0.0f, -1.0f, 3.0f);

        // Project triangles from 3D --> 2D
        MultiplyMatrixTriangle(&triTranslated, &triProjected, &matProj);

        // Scale into view
        scale_triangle3d(&triProjected, (float)WIDTH, (float)HEIGHT);

        // Rasterize triangle
        laps_triangle(c, (uint16_t)triProjected.p[0].x, (uint16_t)triProjected.p[0].y,
                      (uint16_t)triProjected.p[1].x, (uint16_t)triProjected.p[1].y,
                      (uint16_t)triProjected.p[2].x, (uint16_t)triProjected.p[2].y,
                      0x55FFFFFF);
        i += 9;
    }

    laps_save_to_ppm(c, "sphere.ppm");

    tinyobj_attrib_free(&attrib);

    return 0;
}
