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
    vec3 center;
};

struct cube {
    std::vector<face> faces;
};

inline face makeFace(const std::vector<vec3>& verts) {
    vec3 normal = (verts[1] - verts[0]).cross(verts[2] - verts[0]).normalize();

    vec3 center = {0, 0, 0};
    for (auto& v : verts) {
        center = center + v;
    }
    center = center * (1.0f / (float)verts.size());

    return face{verts, normal, center};
}

inline cube makeCube(float size, vec3 center) {
    float x = center.x;
    float y = center.y;
    float z = center.z;

    std::vector<vec3> Verts = {
        {x - size, y - size, z - size}, // 0
        {x + size, y - size, z - size}, // 1
        {x + size, y + size, z - size}, // 2
        {x - size, y + size, z - size}, // 3
        {x - size, y - size, z + size}, // 4
        {x + size, y - size, z + size}, // 5
        {x + size, y + size, z + size}, // 6
        {x - size, y + size, z + size}  // 7
    };

    std::vector<face> Faces;

    // Bottom face (y - size) -> order counter-clockwise from below
    Faces.push_back(makeFace({Verts[0], Verts[1], Verts[5], Verts[4]}));

    // Top face (y + size) -> order counter-clockwise from above
    Faces.push_back(makeFace({Verts[7], Verts[6], Verts[2], Verts[3]}));

    // Front face (z + size) -> counter-clockwise looking towards positive Z
    Faces.push_back(makeFace({Verts[4], Verts[5], Verts[6], Verts[7]}));

    // Back face (z - size) -> counter-clockwise looking towards negative Z
    Faces.push_back(makeFace({Verts[1], Verts[0], Verts[3], Verts[2]}));

    // Left face (x - size) -> counter-clockwise looking towards negative X
    Faces.push_back(makeFace({Verts[0], Verts[4], Verts[7], Verts[3]}));

    // Right face (x + size) -> counter-clockwise looking towards positive X
    Faces.push_back(makeFace({Verts[5], Verts[1], Verts[2], Verts[6]}));

    return cube{Faces};
}

