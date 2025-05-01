// renderer.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "tools.h"
#include "define.h"
#include "camera.h"

class Renderer {
    SDL_Renderer* sdlRenderer = nullptr;
    Camera* camera = nullptr;

public:
    void setSDLRenderer(SDL_Renderer* r) {
        sdlRenderer = r;
    }

    void setCamera(Camera* cam) {
        camera = cam;
    }

    void render(const std::vector<face>& faces) {
        if (!sdlRenderer || !camera) return;

        SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
        SDL_RenderClear(sdlRenderer);
        SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

        for (auto& face : faces) {
            vec3 relativeNormal = camera->toCameraSpaceNormal(face.normal);
            vec3 relativeCenter = camera->toCameraSpace(face.center);

            float facing = relativeNormal.dot(relativeCenter);
            if (facing >= 0.0f) continue;

            for (int i = 0; i < face.vertices.size(); i++) {
                vec3 currVer = face.vertices.at(i);
                vec3 nextVer = face.vertices.at((i + 1) % face.vertices.size());

                vec3 p1 = camera->toCameraSpace(currVer);
                vec3 p2 = camera->toCameraSpace(nextVer);

                if (p1.z <= 0.01f || p2.z <= 0.01f) continue;

                vec2 sp1 = worldToScreen(p1, camera->fov);
                vec2 sp2 = worldToScreen(p2, camera->fov);
                vec2 c1 = toCanvas(sp1, camera->fov);
                vec2 c2 = toCanvas(sp2, camera->fov);
                SDL_RenderDrawLineF(sdlRenderer, c1.x, c1.y, c2.x, c2.y);
            }
        }

        SDL_RenderPresent(sdlRenderer);
    }

private:
    vec2 worldToScreen(vec3 relativeV, float fov) {
        float d = WINDOW_PLANE;
        float newX = relativeV.x * d / relativeV.z;
        float newY = relativeV.y * d / relativeV.z;
        return vec2{newX, newY};
    }

    vec2 toCanvas(vec2 v, float fov) {
        float fovRad = fov * (M_PI / 180.0f);
        float Vw = 2.0f * WINDOW_PLANE * tan(fovRad / 2.0f);
        float Vh = Vw * (float)HEIGHT / (float)WIDTH;

        return {
            v.x * WIDTH / Vw + WIDTH / 2.0f,
            v.y * HEIGHT / Vh + HEIGHT / 2.0f
        };
    }
};
