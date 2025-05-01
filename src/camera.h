#pragma once
#include "tools.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <set>

class Camera {
public:
    vec3 position;
    vec3 forward, right, up;
    float fov;

    Camera()
        : position{0, 0, 0}, forward{0, 0, 1}, right{1, 0, 0}, up{0, -1, 0}, fov(90.0f) {}

    vec3 toCameraSpace(const vec3& point) const {
        vec3 relative = point - position;
        return {
            relative.x * right.x + relative.y * right.y + relative.z * right.z,
            relative.x * up.x + relative.y * up.y + relative.z * up.z,
            relative.x * forward.x + relative.y * forward.y + relative.z * forward.z
        };
    }

    vec3 toCameraSpaceNormal(const vec3& normal) const {
        return {
            normal.x * right.x + normal.y * right.y + normal.z * right.z,
            normal.x * up.x + normal.y * up.y + normal.z * up.z,
            normal.x * forward.x + normal.y * forward.y + normal.z * forward.z
        };
    }

    void handleMovement(const std::set<SDL_Keycode>& keys) {
        const float moveSpeed = DELTA;
        const float rotSpeed = DELTA_THETA;
        const float zoomSpeed = ZOOM_SPEED;

        for (auto key : keys) {
            switch (key) {
                case SDLK_w:
                    position = position + forward * moveSpeed;
                    break;
                case SDLK_s:
                    position = position - forward * moveSpeed;
                    break;
                case SDLK_a:
                    position = position - right * moveSpeed;
                    break;
                case SDLK_d:
                    position = position + right * moveSpeed;
                    break;
                case SDLK_SPACE:
                    position = position - up * moveSpeed;
                    break;
                case SDLK_LSHIFT:
                case SDLK_RSHIFT:
                    position = position + up * moveSpeed;
                    break;
                case SDLK_UP:
                    rotateAround(right, -rotSpeed);
                    break;
                case SDLK_DOWN:
                    rotateAround(right, rotSpeed);
                    break;
                case SDLK_LEFT:
                    rotateAround(up, rotSpeed);
                    break;
                case SDLK_RIGHT:
                    rotateAround(up, -rotSpeed);
                    break;
                case SDLK_g:
                    rotateAround(forward, -rotSpeed);
                    break;
                case SDLK_h:
                    rotateAround(forward, rotSpeed);
                    break;
                case SDLK_z:
                    fov -= zoomSpeed;
                    if (fov < 10.0f) fov = 10.0f;
                    break;
                case SDLK_x:
                    fov += zoomSpeed;
                    if (fov > 150.0f) fov = 150.0f;
                    break;
            }
        }
    }

private:
    void rotateAround(const vec3& axis, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        forward = rotateVec(forward, axis, c, s).normalize();
        right = rotateVec(right, axis, c, s).normalize();
        up = rotateVec(up, axis, c, s).normalize();
    }

    vec3 rotateVec(const vec3& v, const vec3& axis, float c, float s) {
        return v * c + axis.cross(v) * s + axis * (axis.dot(v)) * (1.0f - c);
    }
};
