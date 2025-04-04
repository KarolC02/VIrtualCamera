#include "drawer.h"
#include "tools.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Screen screen;

    float size = 1.0f;
    float centerZ = 10.0f; 

    std::vector<vec3> cubeVertices = {
        {-size, -size, centerZ - size},
        { size, -size, centerZ - size},
        { size,  size, centerZ - size},
        {-size,  size, centerZ - size},
        {-size, -size, centerZ + size},
        { size, -size, centerZ + size},
        { size,  size, centerZ + size},
        {-size,  size, centerZ + size},
    };

    // Add the vertices to screen
    for (auto& vertex : cubeVertices) {
        screen.pixel(vertex);
    }

    while (true) {
        screen.input();   

        screen.pixel({0, 0, 20});
        for (auto& vertex : cubeVertices) {
            screen.pixel(vertex);
        }
        screen.show();  
        screen.clear();   

        
    }

    return EXIT_SUCCESS;
}
