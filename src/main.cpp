#include "drawer.h"

int main(int argc, char* argv[]){
    Screen screen;
    Drawer drawer = Drawer(screen);
    // for(int i = 0; i < 100; i++){
    //     screen.line(screen, rand()%640, rand()%480, rand()%640, rand()%480);
    // }

    while(true){
        for(int i = 0; i < 100; i++){
            drawer.line(rand()%640, rand()%480, rand()%640, rand()%480);
        }
        screen.show();
        screen.clear();
        screen.input();
    }

    return EXIT_SUCCESS;
}