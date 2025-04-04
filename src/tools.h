#pragma once
#include "define.h"
#include <vector>
struct vec3{
    float x,y,z;
};

struct vec2{
    float x,y;
};

struct line{
    vec3 p1, p2;
};

struct cube{
    std::vector<vec3> vertices;
};