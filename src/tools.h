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

cube makeCube(float size, vec3 center){
    float x = center.x;
    float y = center.y;
    float z = center.z;
    std::vector<vec3> Verts;
    Verts.push_back({x -size, y-size, z - size});
    Verts.push_back({x +size, y-size, z - size});
    Verts.push_back({x +size, y + size, z - size});
    Verts.push_back({x -size, y + size, z - size});
    Verts.push_back({x -size, y-size, z + size});
    Verts.push_back({x +size, y-size, z + size});
    Verts.push_back({x + size, y+size, z + size});
    Verts.push_back({x -size, y+ size, z + size});
    return cube({Verts});
}