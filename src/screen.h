#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <set>
#include <limits>
#include <cmath>
#include <iostream>
#include "define.h"
#include "tools.h"

class Screen {
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<face> faces;
    vec3 cameraPosition;
    vec3 forward, right, up;
    float fov;
    std::set<SDL_Keycode> keysPressed;
    std::vector<int> faceBuffer;
    bool needsRedraw = true;

public:
    Screen() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
        cameraPosition = {0, 0, 0};
        fov = 90.f;
        forward = {0, 0, 1};
        right = {1, 0, 0};
        up = {0, -1, 0};
        faceBuffer.resize(WIDTH * HEIGHT, -1);
    }

    void addCube(const cube& cube) {
        for (auto& face : cube.faces) {
            faces.emplace_back(face);
        }
    }

    void show() {
        if (!needsRedraw) return;
    
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    
        float aspect = (float)WIDTH / HEIGHT;
        float fovRad = fov * M_PI / 180.0f;
        float planeDist = 1.0f / tanf(fovRad / 2.0f);
    
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                float px = (2.0f * x / WIDTH - 1.0f) * aspect;
                float py = (1.0f - 2.0f * y / HEIGHT);
    
                vec3 rayDir = (right * px + up * py + forward * planeDist).normalize();
                float closestT = std::numeric_limits<float>::max();
                int hitFace = -1;
    
                for (int f = 0; f < faces.size(); ++f) {
                    const face& face = faces[f];
                    vec3 normal = face.normal;
    

                    float denom = normal.dot(rayDir);
                    if (denom >= 0) continue; 
    
        
                    float t = (face.center - cameraPosition).dot(normal) / denom;
                    if (t <= 0.0f || t >= closestT) continue;
    
                    vec3 p = cameraPosition + rayDir * t;
    
                    if (!pointInFace(p, face)) continue;
    
                    closestT = t;
                    hitFace = f;
                }
    
                faceBuffer[y * WIDTH + x] = hitFace;
            }
        }
    
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int y = 1; y < HEIGHT - 1; y++) {
            for (int x = 1; x < WIDTH - 1; x++) {
                int id = faceBuffer[y * WIDTH + x];
                if (id == -1) continue;

                bool edge = false;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue; 
                        if (faceBuffer[(y + dy) * WIDTH + (x + dx)] != id) {
                            edge = true;
                            break;
                        }
                    }
                    if (edge) break;
                }
                if (edge) SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    
        SDL_RenderPresent(renderer);
        needsRedraw = false;
    }    

    void input() {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            }
            if (e.type == SDL_KEYDOWN) keysPressed.insert(e.key.keysym.sym);
            if (e.type == SDL_KEYUP) keysPressed.erase(e.key.keysym.sym);
        }
    }

    void handleMovement() {
        const float moveSpeed = DELTA;
        const float rotSpeed = DELTA_THETA;
        const float zoomSpeed = ZOOM_SPEED;
        bool moved = false;

        for (auto key : keysPressed) {
            switch (key) {
                case SDLK_w: cameraPosition = cameraPosition + forward * moveSpeed; moved = true; break;
                case SDLK_s: cameraPosition = cameraPosition - forward * moveSpeed; moved = true; break;
                case SDLK_a: cameraPosition = cameraPosition - right * moveSpeed; moved = true; break;
                case SDLK_d: cameraPosition = cameraPosition + right * moveSpeed; moved = true; break;
                case SDLK_SPACE: cameraPosition = cameraPosition + up * moveSpeed; moved = true; break;
                case SDLK_LSHIFT:
                case SDLK_RSHIFT: cameraPosition = cameraPosition - up * moveSpeed; moved = true; break;
                case SDLK_UP: rotateAround(right, rotSpeed); moved = true; break;
                case SDLK_DOWN: rotateAround(right, -rotSpeed); moved = true; break;
                case SDLK_LEFT: rotateAround(up, rotSpeed); moved = true; break;
                case SDLK_RIGHT: rotateAround(up, -rotSpeed); moved = true; break;
                case SDLK_g: rotateAround(forward, -rotSpeed); moved = true; break;
                case SDLK_h: rotateAround(forward, rotSpeed); moved = true; break;
                case SDLK_z: fov = std::max(10.0f, fov - zoomSpeed); moved = true; break;
                case SDLK_x: fov = std::min(150.0f, fov + zoomSpeed); moved = true; break;
            }
        }

        if (moved) needsRedraw = true;
    }

private:
    void rotateAround(vec3 axis, float angle) {
        float c = cos(angle), s = sin(angle);
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

    bool pointInFace(const vec3& p, const face& f) {
        int n = f.vertices.size();
        for (int i = 0; i < n; i++) {
            vec3 a = f.vertices[i];
            vec3 b = f.vertices[(i + 1) % n];
            vec3 edge = b - a;
            vec3 toPoint = p - a;
            if ((edge.cross(toPoint)).dot(f.normal) < 0) return false;
        }
        return true;
    }
};