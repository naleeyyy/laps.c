#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "../laps.h"

typedef struct
{
    float x, y;
} vec2d;

vec2d make_vec2d(float x, float y)
{
    vec2d v2;
    v2.x = x;
    v2.y = y;
    return v2;
}

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

typedef struct
{
    triangle3d tris[12];
} cubeMesh3d;

cubeMesh3d make_cubeMesh3d(triangle3d t1, triangle3d t2, triangle3d t3,
                           triangle3d t4, triangle3d t5, triangle3d t6,
                           triangle3d t7, triangle3d t8, triangle3d t9,
                           triangle3d t10, triangle3d t11, triangle3d t12)
{
    cubeMesh3d c;
    c.tris[0] = t1;
    c.tris[1] = t2;
    c.tris[2] = t3;
    c.tris[3] = t4;
    c.tris[4] = t5;
    c.tris[5] = t6;
    c.tris[6] = t7;
    c.tris[7] = t8;
    c.tris[8] = t9;
    c.tris[9] = t10;
    c.tris[10] = t11;
    c.tris[11] = t12;
    return c;
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
    o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
    o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
    o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
    float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

    if (w != 0.0f)
    {
        o->x /= w;
        o->y /= w;
        o->z /= w;
    }
}

#endif // LINEAR_ALGEBRA_H
