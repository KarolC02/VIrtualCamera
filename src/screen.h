#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <stdio.h>
#include "define.h"
#include "tools.h"
#include<iostream>

/*
https://www.youtube.com/watch?v=kdRJgYO1BJM
*/

class Screen{

    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<vec3> points;
    std::vector<line> lines;
    std::vector<cube> cubes;
    vec3 cameraPosition;
    float fov;
public:
    Screen(){
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window,&renderer);
        cameraPosition = {0, 0, 0};
        fov = 90.0f;
    }

    
    void addPixel(vec3 point){
        points.emplace_back(point);
    }

    void addLine(line line){
        lines.emplace_back(line);
    }

    void addCube(cube cube){

        std::vector<vec3> verts = cube.vertices;

        if(verts.size() != 8 ){
            return;
        }

        cubes.emplace_back(cube);
        for( int i = 0; i < 3; i++ ){
            line line1 = {verts.at(i), verts.at(i+1)};
            line line2 = {verts.at(i+4), verts.at(i+5)};
            lines.emplace_back(line1);
            lines.emplace_back(line2);
        }

        line line1 = {verts.at(3), verts.at(0)};
        line line2 = {verts.at(4), verts.at(7)};

        lines.emplace_back(line1);
        lines.emplace_back(line2);
        for( int i = 0; i < 4; i++ ){
            line line1 = {verts.at(i), verts.at(i+4)};
            lines.emplace_back(line1);
        }

    }
    void show(){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer,255,255,255,255);

        // Render single points
        for(auto& point : points){
            vec2 TPoint = WorldToScreen(point);
            std::cout << TPoint.x << " " << TPoint.y << std::endl;

            vec2 CanvasPoint = toCanvas(TPoint);
            SDL_RenderDrawPointF(renderer, CanvasPoint.x , CanvasPoint.y);
        }

        // Render lines:
        for(auto& line : lines){
            vec3 p1 = line.p1;
            vec3 p2 = line.p2;
            vec2 Tp1 = WorldToScreen(p1);
            vec2 Tp2 = WorldToScreen(p2);
            vec2 canvasPoint1 = toCanvas(Tp1);
            vec2 canvasPoint2 = toCanvas(Tp2);
            SDL_RenderDrawLine(renderer, canvasPoint1.x, canvasPoint1.y, canvasPoint2.x, canvasPoint2.y);
        }

        // render Cubes:

        SDL_RenderPresent(renderer);
    }

    void clear(){
        points.clear();
    }
    void input() {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        cameraPosition.y -= 1;
                        break;
                    case SDLK_DOWN:
                        cameraPosition.y += 1;
                        break;
                    case SDLK_LEFT:
                        cameraPosition.x -= 1;
                        break;
                    case SDLK_RIGHT:
                        cameraPosition.x += 1;
                        break;
                    case SDLK_w:
                        cameraPosition.z += 1;
                        break;
                    case SDLK_s:
                        cameraPosition.z -= 1;
                        break;
                    case SDLK_z: // Zoom in
                        fov -= 1.0f; 
                        if (fov < 10.0f) fov = 10.0f; 
                        break;
                    case SDLK_x:  // Zoom out
                        fov += 1.0f;
                        if (fov > 160.0f) fov = 160.0f; 
                        break;
                }
            }
        }
    }
    

    vec2 WorldToScreen( vec3 point ){
        float relativeX = point.x - cameraPosition.x;
        float relativeY = point.y - cameraPosition.y;
        float relativeZ = point.z - cameraPosition.z;
        // float relativeX = cameraPosition.x - point.x;
        // float relativeY = cameraPosition.y - point.y;  
        // float relativeZ = cameraPosition.z - point.z;

        vec3 relative = vec3({relativeX, relativeY, relativeZ});
        vec2 projected = ProjectVertex(relative);    
        return projected;
    }

    vec2 ProjectVertex(vec3 v){
        float d = WINDOW_PLANE;
        if( v.z == 0 ) v.z = 0.0001f;
        float newX = v.x * d / v.z;
        float newY = v.y * d / v.z;
        return vec2({newX, newY});
    }

    vec2 toCanvas(vec2 v){
        float fovRad = fov * (M_PI / 180.0f);  
        float Vw = 2.0f * WINDOW_PLANE * tan(fovRad / 2.0f);  
        float Vh = Vw * (float)HEIGHT / (float)WIDTH;   
    
        vec2 toCanvasVec;
        toCanvasVec.x = v.x * WIDTH / Vw + WIDTH / 2.0f;
        toCanvasVec.y = v.y * HEIGHT / Vh + HEIGHT / 2.0f;
        return toCanvasVec;
    }
};