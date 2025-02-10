// gameobject.h
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "struct.h"
#include <vector>

struct Mesh { //moved mesh from struct.h
    std::vector<vec3> points;
    std::vector<triangle> triangles;
};

struct GameObject {
    vec3 position = {0, 0, 0};
    vec3 rotation = {0, 0, 0};
    vec3 scale = {1, 1, 1};  // Add scale
    Mesh mesh;     // Use the Mesh struct
    // Add other properties as needed (e.g., color, velocity, etc.)
};

#endif
