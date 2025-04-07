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
    vec3 forward, right, up;
    std::set<SDL_Keycode> keysPressed; 
    float fov;

public:
    Screen() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
        cameraPosition = {0, 0, 0};
        fov = 90.0f;

        forward = {0, 0, 1};  
        right = {1, 0, 0};    
        up = {0, -1, 0};    
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

            if (p1.z <= 0.01f || p2.z <= 0.01f) continue;

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
        vec3 relative = point - cameraPosition;

        return {
            relative.x * right.x + relative.y * right.y + relative.z * right.z,
            relative.x * up.x + relative.y * up.y + relative.z * up.z,
            relative.x * forward.x + relative.y * forward.y + relative.z * forward.z
        };
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
        const float moveSpeed = DELTA;
        const float rotSpeed = DELTA_THETA;
        const float zoomSpeed = ZOOM_SPEED;

        for (auto key : keysPressed) {
            switch (key) {
                case SDLK_w:
                    cameraPosition = cameraPosition + forward * moveSpeed;
                    break;
                case SDLK_s:
                    cameraPosition = cameraPosition - forward * moveSpeed;
                    break;
                case SDLK_a:
                    cameraPosition = cameraPosition - right * moveSpeed;
                    break;
                case SDLK_d:
                    cameraPosition = cameraPosition + right * moveSpeed;
                    break;
                case SDLK_SPACE:
                    cameraPosition = cameraPosition - up * moveSpeed;
                    break;
                case SDLK_LSHIFT:
                case SDLK_RSHIFT:
                    cameraPosition = cameraPosition + up * moveSpeed;
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

    void rotateAround(vec3 axis, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        forward = rotateVec(forward, axis, c, s).normalize();
        right = rotateVec(right, axis, c, s).normalize();
        up = rotateVec(up, axis, c, s).normalize();
    }

    vec3 rotateVec(vec3 v, vec3 axis, float c, float s) {
        return v * c + axis.cross(v) * s + axis * (dot(axis, v)) * (1.0f - c);
    }

    float dot(vec3 a, vec3 b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};
