#pragma once
#include "define.h"
#include <vector>
#include <cmath>

struct vec3 {
    float x, y, z;

    vec3 operator+(const vec3& v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    vec3 operator-(const vec3& v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    vec3 operator*(float s) const {
        return {x * s, y * s, z * s};
    }

    vec3 cross(const vec3& v) const {
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }

    float dot(const vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    vec3 normalize() const {
        float len = std::sqrt(x * x + y * y + z * z);
        if (len == 0.0f) return {0, 0, 0};
        return {x / len, y / len, z / len};
    }
};

struct vec2 {
    float x, y;
};

struct line {
    vec3 p1, p2;
};

struct cube {
    std::vector<vec3> vertices;
};

cube makeCube(float size, vec3 center) {
    float x = center.x;
    float y = center.y;
    float z = center.z;
    std::vector<vec3> Verts;
    Verts.push_back({x - size, y - size, z - size});
    Verts.push_back({x + size, y - size, z - size});
    Verts.push_back({x + size, y + size, z - size});
    Verts.push_back({x - size, y + size, z - size});
    Verts.push_back({x - size, y - size, z + size});
    Verts.push_back({x + size, y - size, z + size});
    Verts.push_back({x + size, y + size, z + size});
    Verts.push_back({x - size, y + size, z + size});
    return cube({Verts});
}
