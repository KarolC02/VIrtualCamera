#include "drawer.h"
#include "tools.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Screen screen;

    float size = 10.0f;
    float centerZ = 20.0f; 

    std::vector<vec3> Verts;
    Verts.push_back({-size, -size, centerZ - size});
    Verts.push_back({ size, -size, centerZ - size});
    Verts.push_back({ size,  size, centerZ - size});
    Verts.push_back({-size,  size, centerZ - size});
    Verts.push_back({-size, -size, centerZ + size});
    Verts.push_back({ size, -size, centerZ + size});
    Verts.push_back({ size,  size, centerZ + size});
    Verts.push_back({-size,  size, centerZ + size});


    screen.cube(cube{Verts});

    // line line1 = {A,B};
    // line line2 = {B,C};
    // line line3 = {C,D};
    // line line4 = {D,A};
    // line line5 = {E,F};
    // line line6 = {F,G};
    // line line7 = {G,H};
    // line line8 = {H,E};
    // line line9 = {A,E};
    // line line10 = {B,F};
    // line line11 = {C,G};
    // line line12 = {D,H};

    // screen.line(line1);
    // screen.line(line2);
    // screen.line(line3);
    // screen.line(line4);
    // screen.line(line5);
    // screen.line(line6);
    // screen.line(line7);
    // screen.line(line8);
    // screen.line(line9);
    // screen.line(line10);
    // screen.line(line11);
    // screen.line(line12);



    // Add the vertices to screen
    // for (auto& vertex : cubeVertices) {
    //     screen.pixel(vertex);
    // }

    while (true) {

        screen.input();   
        screen.show();  
        screen.clear();   

    }

    return EXIT_SUCCESS;
}
