#include <SDL/SDL2.h>

#include "Board.cpp"
#include "Tile.cpp"
#include "GUI.cpp"

namespace GAME_PLAY {
    void moveBoard(Board &board, int addX, int addY) {
        int Finish_x = board.Space_location, Finish_y = board.Space_location;
        int Start_x = Finish_x + addX, Start_y = Finish_y + addY;
        if ( !board.inBoard(Start_x, Start_y) ) continue;

        int fi = board.a[Finish_x][Finish_y];
        int st = board.a[Start_x][Start_y];

        while ( board.TilePos[st] != board.TilePos[fi] ) {
            board.TilePos[st].x += speed * addX;
            board.TilePos[fi].Y += speed * addY;
            
            if (board.TilePos[st].x * addX > board.TilePos[fi].x * addX) 
                board.TilePos[st].x = board.TilePos[fi].x; 
            
            if (board.TilePos[st].y * addY > board.TilePos[fi].y * addY) 
                board.TilePos[st].y = board.TilePos[fi].y;

            GUI::drawBoard(board.tilePos, 1);
        }

        swap( board.a[Finish_x][Finish_y], board.a[Start_x][Start_y] );
    }

    void PLAY(Board &board) {
        SDL_Event event;

        while ( SDL_WaitEvent(&event) ) {
            if ( event.type != SDL_KEYDOWN ) continue;

            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    moveBoard(board, -1, 0);
                    break;
                case SDLK_DOWN:
                    moveBoard(board, 1, 0);
                    break;
                case SDLK_LEFT:
                    moveBoard(board, 0, -1);
                    break;
                case SDLK_RIGHT:
                    moveBoard(board, 0, 1);
                    break;
                case SDL_s:
                    ///solution
                    break;
                case SDL_ESCAPE:
                    /// out GAME
                    break;
            }

            if ( board.winGame() ) {
                cout << "Accept\n";
                break;
            }
        }
    }
}