#ifndef MESHES_H
#define MESHES_H

#include "./utils/linear_algebra.h"

mesh3d make_cubeMesh3d(vec3d origin, float length)
{
    mesh3d mesh;
    size_t size = 12;

    init_mesh3d(&mesh, size);
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y, origin.z), make_vec3d(origin.x, origin.y + length, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z), make_vec3d(origin.x + length, origin.y, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + length, origin.y, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z + length)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + length, origin.y, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z + length), make_vec3d(origin.x + length, origin.y, origin.z + length)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + length, origin.y, origin.z + length), make_vec3d(origin.x + length, origin.y + length, origin.z + length), make_vec3d(origin.x, origin.y + length, origin.z + length)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + length, origin.y, origin.z + length), make_vec3d(origin.x, origin.y + length, origin.z + length), make_vec3d(origin.x, origin.y, origin.z + length)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y, origin.z + length), make_vec3d(origin.x, origin.y + length, origin.z + length), make_vec3d(origin.x, origin.y + length, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y, origin.z + length), make_vec3d(origin.x, origin.y + length, origin.z), make_vec3d(origin.x, origin.y, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y + length, origin.z), make_vec3d(origin.x, origin.y + length, origin.z + length), make_vec3d(origin.x + length, origin.y + length, origin.z + length)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y + length, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z + length), make_vec3d(origin.x + length, origin.y + length, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + length, origin.y, origin.z + length), make_vec3d(origin.x, origin.y, origin.z + length), make_vec3d(origin.x, origin.y, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + length, origin.y, origin.z + length), make_vec3d(origin.x, origin.y, origin.z), make_vec3d(origin.x + length, origin.y, origin.z)));

    return mesh;
}

mesh3d make_pyramidMesh3d(vec3d origin, float length)
{
    mesh3d mesh;
    size_t size = 6;

    init_mesh3d(&mesh, size);
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + 0.5f, origin.y, origin.z + 0.5f), make_vec3d(origin.x, origin.y + length, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + 0.5f, origin.y, origin.z + 0.5f), make_vec3d(origin.x + length, origin.y + length, origin.z), make_vec3d(origin.x + length, origin.y + length, origin.z + length)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + 0.5f, origin.y, origin.z + 0.5f), make_vec3d(origin.x + length, origin.y + length, origin.z + length), make_vec3d(origin.x, origin.y + length, origin.z + length)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x + 0.5f, origin.y, origin.z + 0.5f), make_vec3d(origin.x, origin.y + length, origin.z + length), make_vec3d(origin.x, origin.y + length, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y + length, origin.z + length), make_vec3d(origin.x + length, origin.y + length, origin.z), make_vec3d(origin.x, origin.y + length, origin.z)));
    insert_mesh3d(&mesh, make_triangle3d(make_vec3d(origin.x, origin.y + length, origin.z + length), make_vec3d(origin.x + length, origin.y + length, origin.z + length), make_vec3d(origin.x + length, origin.y + length, origin.z)));

    return mesh;
}

#endif // MESHES_H
