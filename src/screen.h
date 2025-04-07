#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <stdio.h>
#include "define.h"
#include "tools.h"
#include <iostream>
#include <cmath> 
#include <set>

class Screen {

    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<vec3> points;
    std::vector<line> lines;
    std::vector<cube> cubes;
    vec3 cameraPosition;
    std::set<SDL_Keycode> keysPressed; 
    float fov;
    float rotationX, rotationY, rotationZ;

public:
    Screen() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
        cameraPosition = {0, 0, 0};
        fov = 90.0f;
        rotationX = rotationY = rotationZ = 0.0f;
    }

    void addPixel(vec3 point) {
        points.emplace_back(point);
    }

    void addLine(line line) {
        lines.emplace_back(line);
    }

    void addCube(cube cube) {
        std::vector<vec3> verts = cube.vertices;
        if (verts.size() != 8) return;

        cubes.emplace_back(cube);
        for (int i = 0; i < 3; i++) {
            lines.emplace_back(line{verts.at(i), verts.at(i + 1)});
            lines.emplace_back(line{verts.at(i + 4), verts.at(i + 5)});
        }
        lines.emplace_back(line{verts.at(3), verts.at(0)});
        lines.emplace_back(line{verts.at(4), verts.at(7)});
        for (int i = 0; i < 4; i++) {
            lines.emplace_back(line{verts.at(i), verts.at(i + 4)});
        }
    }

    void show() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (auto& point : points) {
            vec3 relative = getRelative(point);
            if (relative.z <= 0.01f) continue;
            vec2 screenPoint = WorldToScreen(relative);
            vec2 canvasPoint = toCanvas(screenPoint);
            SDL_RenderDrawPointF(renderer, canvasPoint.x, canvasPoint.y);
        }

        for (auto& line : lines) {
            vec3 p1 = getRelative(line.p1);
            vec3 p2 = getRelative(line.p2);

            if (p1.z <= 0.01f || p2.z <= 0.01f) continue; // skip lines behind camera

            vec2 sp1 = WorldToScreen(p1);
            vec2 sp2 = WorldToScreen(p2);
            vec2 c1 = toCanvas(sp1);
            vec2 c2 = toCanvas(sp2);
            SDL_RenderDrawLineF(renderer, c1.x, c1.y, c2.x, c2.y);
        }

        SDL_RenderPresent(renderer);
    }

    void clear() {
        points.clear();
    }

    void input() {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            }
            if (e.type == SDL_KEYDOWN) {
                keysPressed.insert(e.key.keysym.sym);
            }
            if (e.type == SDL_KEYUP) {
                keysPressed.erase(e.key.keysym.sym);
            }
        }
    }

    vec3 getRelative(vec3 point) {
        float x = point.x - cameraPosition.x;
        float y = point.y - cameraPosition.y;
        float z = point.z - cameraPosition.z;

        // Rotate around X axis (pitch)
        float cosX = cos(rotationX);
        float sinX = sin(rotationX);
        float y1 = y * cosX - z * sinX;
        float z1 = y * sinX + z * cosX;

        // Rotate around Y axis (yaw)
        float cosY = cos(rotationY);
        float sinY = sin(rotationY);
        float x2 = x * cosY + z1 * sinY;
        float z2 = -x * sinY + z1 * cosY;

        // Rotate around Z axis (roll)
        float cosZ = cos(rotationZ);
        float sinZ = sin(rotationZ);
        float x3 = x2 * cosZ - y1 * sinZ;
        float y3 = x2 * sinZ + y1 * cosZ;

        return vec3{x3, y3, z2};
    }

    vec2 WorldToScreen(vec3 relativeV) {
        float d = WINDOW_PLANE;
        float newX = relativeV.x * d / relativeV.z;
        float newY = relativeV.y * d / relativeV.z;
        return vec2({newX, newY});
    }

    vec2 toCanvas(vec2 v) {
        float fovRad = fov * (M_PI / 180.0f);
        float Vw = 2.0f * WINDOW_PLANE * tan(fovRad / 2.0f);
        float Vh = Vw * (float)HEIGHT / (float)WIDTH;

        vec2 canvasVec;
        canvasVec.x = v.x * WIDTH / Vw + WIDTH / 2.0f;
        canvasVec.y = v.y * HEIGHT / Vh + HEIGHT / 2.0f;
        return canvasVec;
    }

    void handleMovement() {

        float cx = cos(-rotationX);
        float sx = sin(-rotationX);
        float cy = cos(-rotationY);
        float sy = sin(-rotationY);
        float cz = cos(-rotationZ);
        float sz = sin(-rotationZ);

        vec3 right = {
            cy * cz + sx * sy * sz,
            cx * sz,
            -sy * cz + sx * cy * sz
        };

        vec3 down = {
            -cy * sz + sx * sy * cz,
            cx * cz,
            sy * sz + sx * cy * cz
        };

        vec3 forward = {
            cx * sy,
            -sx,
            cx * cy
        };

        for (auto key : keysPressed) {
            switch (key) {
                case SDLK_w:
                    cameraPosition.x += forward.x * DELTA;
                    cameraPosition.y += forward.y * DELTA;
                    cameraPosition.z += forward.z * DELTA;
                    break;
                case SDLK_s:
                    cameraPosition.x -= forward.x * DELTA;
                    cameraPosition.y -= forward.y * DELTA;
                    cameraPosition.z -= forward.z * DELTA;
                    break;
                case SDLK_a:
                    cameraPosition.x -= right.x * DELTA;
                    cameraPosition.y -= right.y * DELTA;
                    cameraPosition.z -= right.z * DELTA;
                    break;
                case SDLK_d:
                    cameraPosition.x += right.x * DELTA;
                    cameraPosition.y += right.y * DELTA;
                    cameraPosition.z += right.z * DELTA;
                    break;
                case SDLK_SPACE:
                    cameraPosition.x -= down.x * DELTA;
                    cameraPosition.y -= down.y * DELTA;
                    cameraPosition.z -= down.z * DELTA;
                    break;
                case SDLK_LSHIFT:
                case SDLK_RSHIFT:
                    cameraPosition.x += down.x * DELTA;
                    cameraPosition.y += down.y * DELTA;
                    cameraPosition.z += down.z * DELTA;
                    break;

                case SDLK_UP:
                    rotationX -= DELTA_THETA;
                    break;
                case SDLK_DOWN:
                    rotationX += DELTA_THETA;
                    break;
                case SDLK_LEFT:
                    rotationY += DELTA_THETA;
                    break;
                case SDLK_RIGHT:
                    rotationY -= DELTA_THETA;
                    break;
                case SDLK_g:
                    rotationZ -= DELTA_THETA;
                    break;
                case SDLK_h:
                    rotationZ += DELTA_THETA;
                    break;
            }
        }
    }
};
