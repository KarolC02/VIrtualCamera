#pragma once
#include "screen.h"

class Drawer{
private:
    Screen &screen;
    

public:
    Drawer(Screen& existingScreen) : screen(existingScreen) {
    }


    
    void line(float x1, float y1, float x2, float y2 ){
        float dx = x2 - x1;
        float dy = y2 - y1;

        float length = std::sqrt(dx * dx + dy * dy); 
        float angle = std::atan2(dy, dx);

        for(float i = 0; i < length; i++){
            screen.pixel(
                x1 + std::cos(angle) * i,
                y1 + std::sin(angle) * i
            );
        }
    }
};