#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "../laps.h"

#ifndef PLATFORM_WASM
#include <malloc.h>
#else
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
#endif

typedef struct
{
    float x, y, z;
} vec3d;

vec3d make_vec3d(float x, float y, float z)
{
    vec3d v3;
    v3.x = x;
    v3.y = y;
    v3.z = z;
    return v3;
}

typedef struct
{
    vec3d p[3];
} triangle3d;

triangle3d make_triangle3d(vec3d v1, vec3d v2, vec3d v3)
{
    triangle3d t;
    t.p[0] = v1;
    t.p[1] = v2;
    t.p[2] = v3;
    return t;
}

void translate_triangle3d(triangle3d *i, triangle3d *o, float x, float y, float z)
{
    o->p[0].x = i->p[0].x + x;
    o->p[0].y = i->p[0].y + y;
    o->p[0].z = i->p[0].z + z;
    o->p[1].x = i->p[1].x + x;
    o->p[1].y = i->p[1].y + y;
    o->p[1].z = i->p[1].z + z;
    o->p[2].x = i->p[2].x + x;
    o->p[2].y = i->p[2].y + y;
    o->p[2].z = i->p[2].z + z;
}

void scale_triangle3d(triangle3d *t, float width, float height)
{
    t->p[0].x += 1.0f;
    t->p[0].y += 1.0f;
    t->p[1].x += 1.0f;
    t->p[1].y += 1.0f;
    t->p[2].x += 1.0f;
    t->p[2].y += 1.0f;

    t->p[0].x *= 0.5F * width;
    t->p[0].y *= 0.5F * height;
    t->p[1].x *= 0.5F * width;
    t->p[1].y *= 0.5F * height;
    t->p[2].x *= 0.5F * width;
    t->p[2].y *= 0.5F * height;
}

typedef struct
{
    triangle3d *tris;
    size_t used;
    size_t size;
} mesh3d;

void init_mesh3d(mesh3d *mesh, size_t initial_size)
{
    mesh->tris = (triangle3d *)malloc(initial_size * sizeof(triangle3d));
    mesh->used = 0;
    mesh->size = initial_size;
}

void insert_mesh3d(mesh3d *mesh, triangle3d tri)
{
    if (mesh->used == mesh->size)
    {
        mesh->size *= 2;
        mesh->tris = (triangle3d *)realloc(mesh->tris, mesh->size * sizeof(triangle3d));
    }
    mesh->tris[mesh->used++] = tri;
}

void free_mesh3d(mesh3d *mesh)
{
    free(mesh->tris);
    mesh->tris = NULL;
    mesh->used = mesh->size = 0;
}

typedef struct
{
    float m[4][4];
} mat4x4;

mat4x4 init_mat4x4(void)
{
    mat4x4 m;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m.m[i][j] = 0;
        }
    }
    return m;
}

void MultiplyMatrixVector(vec3d *i, vec3d *o, mat4x4 *m)
{
    float w;
    o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
    o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
    o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
    w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

    if (w != 0.0f)
    {
        o->x /= w;
        o->y /= w;
        o->z /= w;
    }
}

void MultiplyMatrixTriangle(triangle3d *i, triangle3d *o, mat4x4 *m)
{
    MultiplyMatrixVector(&i->p[0], &o->p[0], m);
    MultiplyMatrixVector(&i->p[1], &o->p[1], m);
    MultiplyMatrixVector(&i->p[2], &o->p[2], m);
}

#endif // LINEAR_ALGEBRA_H
