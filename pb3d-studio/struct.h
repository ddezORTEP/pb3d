#ifndef STRUCT_H
#define STRUCT_H

struct vec3 {
    float x, y, z;
};

struct color {
    Uint8 r, g, b, a;
};

struct triangle {
    int p1, p2, p3;
    color col;
};

#endif
