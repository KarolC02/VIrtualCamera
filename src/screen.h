#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <set>
#include <cmath>
#include <iostream>
#include "define.h"
#include "tools.h"
#include "camera.h"
#include "renderer.h"

class Screen {
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    std::vector<face> faces;
    std::set<SDL_Keycode> keysPressed; 
    Camera camera;
    Renderer renderer;

public:
    Screen() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &sdlRenderer);
        renderer.setSDLRenderer(sdlRenderer);
        renderer.setCamera(&camera);
    }

    void addCube(const cube& cube) {
        for (auto& face : cube.faces) {
            faces.emplace_back(face);
        }
    }

    void show() {
        renderer.render(faces);
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

    void handleMovement() {
        camera.handleMovement(keysPressed);
    }
};
