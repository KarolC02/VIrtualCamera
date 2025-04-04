#pragma once
#include "screen.h"
#include "tools.h"
#include <iostream>

class Drawer{
private:
    Screen &screen;
    

public:
    Drawer(Screen& existingScreen) : screen(existingScreen) {
    }
    
    // void line(vec2 p1, vec2 p2){

    //     float x1 = p1.x;
    //     float y1 = p1.y;
    //     float x2 = p2.x;
    //     float y2 = p2.y;

    //     std::cout << x1 << " " << y1 << std::endl;
    //     std::cout << x2 << " " << y2 << std::endl;
    //     float dx = x2 - x1;
    //     float dy = y2 - y1;

    //     float length = std::sqrt(dx * dx + dy * dy); 
    //     float angle = std::atan2(dy, dx); 

    //     for(float i = 0; i < length; i++){
    //         screen.pixel(
    //             x1 + std::cos(angle) * i,
    //             y1 + std::sin(angle) * i
    //         );
    //     }
    // }


};