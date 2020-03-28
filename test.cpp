#include <bits/stdc++.h>

#include "GUI.cpp"

int main() {
    
    if (GUI::init() == false) {
        printf("Deo gi ko init duoc");
        return 0;
    }
    if (GUI::loadMedia() == false) {
        printf("Deo load duoc font");
        return 0;
    }
    GUI::setGridSize(3);
    
    Tile x1 = {0, 10, 10, 100};
    Tile x2 = {1, 300, 300, 100};
    Tile x3 = {2, 10, 300, 100};
    std::vector<Tile> a;
    a.push_back(x1);
    a.push_back(x2);
    a.push_back(x3);
    bool quit = false;
    SDL_Event e;

    while (!quit) {

        while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
            }
        GUI::drawBoard(a, 1);
    
    }

    GUI::destroy();
}