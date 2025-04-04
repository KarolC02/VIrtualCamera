#include "drawer.h"
#include "tools.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Screen screen;

    float size = 10.0f;

    for( int i = 0; i < 3; i ++ ){
        for( int j = 0; j < 3; j++ ){
            for( int k = 0; k < 3; k++ ){
                float x = 30.0 * i;
                float y = 30.0 * j;
                float z = 100 + 30.0 * k;
                vec3 center = {x,y,z};
                cube cube1 = makeCube(size,center);
                screen.addCube(cube1);
            }
        }
    }

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
