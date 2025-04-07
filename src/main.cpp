#include "drawer.h"
#include "tools.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Screen screen;

    float size = 10.0f;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                float x = 30.0f * i;
                float y = 30.0f * j;
                float z = 100.0f + 30.0f * k;
                vec3 center = {x, y, z};
                cube cube1 = makeCube(size, center);
                screen.addCube(cube1);
            }
        }
    }

    while (true) {
        screen.input();   
        screen.handleMovement();   
        screen.show();  
        screen.clear();   
    }

    return EXIT_SUCCESS;
}
