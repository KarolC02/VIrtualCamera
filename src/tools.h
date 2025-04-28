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

struct face {
    std::vector<vec3> vertices;
    vec3 normal;
};

struct cube {
    std::vector<face> faces;
};

cube makeCube(float size, vec3 center) {
    float x = center.x;
    float y = center.y;
    float z = center.z;

    std::vector<vec3> Verts;
    Verts.push_back({x - size, y - size, z - size}); // 0
    Verts.push_back({x + size, y - size, z - size}); // 1
    Verts.push_back({x + size, y + size, z - size}); // 2
    Verts.push_back({x - size, y + size, z - size}); // 3
    Verts.push_back({x - size, y - size, z + size}); // 4
    Verts.push_back({x + size, y - size, z + size}); // 5
    Verts.push_back({x + size, y + size, z + size}); // 6
    Verts.push_back({x - size, y + size, z + size}); // 7

    std::vector<face> Faces;

    // Bottom face
    Faces.push_back(face{
        {Verts[0], Verts[1], Verts[5], Verts[4]},
        (Verts[1] - Verts[0]).cross(Verts[4] - Verts[0]).normalize()
    });

    // Top face
    Faces.push_back(face{
        {Verts[3], Verts[2], Verts[6], Verts[7]},
        (Verts[2] - Verts[3]).cross(Verts[7] - Verts[3]).normalize()
    });

    // Front face
    Faces.push_back(face{
        {Verts[0], Verts[4], Verts[7], Verts[3]},
        (Verts[4] - Verts[0]).cross(Verts[3] - Verts[0]).normalize()
    });

    // Back face
    Faces.push_back(face{
        {Verts[1], Verts[2], Verts[6], Verts[5]},
        (Verts[2] - Verts[1]).cross(Verts[5] - Verts[1]).normalize()
    });

    // Left face
    Faces.push_back(face{
        {Verts[0], Verts[1], Verts[2], Verts[3]},
        (Verts[1] - Verts[0]).cross(Verts[3] - Verts[0]).normalize()
    });

    // Right face
    Faces.push_back(face{
        {Verts[4], Verts[5], Verts[6], Verts[7]},
        (Verts[5] - Verts[4]).cross(Verts[7] - Verts[4]).normalize()
    });

    return cube{Faces};
}

