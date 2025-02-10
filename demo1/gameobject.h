#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "struct.h"
#include <vector>

struct Mesh {
    std::vector<vec3> points;
    std::vector<triangle> triangles;
};

struct GameObject {
    vec3 position = {0, 0, 0};
    vec3 rotation = {0, 0, 0};
    vec3 scale = {1, 1, 1};
    Mesh mesh;
};

#endif
