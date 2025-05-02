#include "screen.h"
#include "tools.h"
#include <iostream>
#include <random>
#include <set>
#include <vector>

void addRandomCubes(Screen& screen, float size) {
    std::vector<vec3> positions;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                float x = 30.0f * i;
                float y = 30.0f * j;
                float z = 100.0f + 30.0f * k;
                positions.push_back({x, y, z});
            }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, positions.size() - 1);

    std::set<int> selectedIndexes;
    while (selectedIndexes.size() < 3) {
        selectedIndexes.insert(dis(gen));  
    }

    for (int idx : selectedIndexes) {
        vec3 center = positions[idx];
        cube cube1 = makeCube(size, center);
        screen.addCube(cube1);
    }
}

int main(int argc, char* argv[]) {
    Screen screen;

    float size = 10.0f;

    addRandomCubes(screen, size);

    while (true) {
        screen.input();   
        screen.handleMovement();   
        screen.show();  
    }

    return EXIT_SUCCESS;
}
