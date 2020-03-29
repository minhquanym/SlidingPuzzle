#include <SDL2/SDL.h>
#include "GUI.cpp"

namespace GAME_PLAY {
    const int SPEED = 10;
    Board board;

    void SETUP() {
        std::cout << ".......Loading game data.......\n";

        /// initalize tile information
        board.a.resize(3);
        for (int i = 0; i < 3; ++i) board.a[i].resize(3);

            int cc = 0;
            for (int j = 0; j < 3; ++j) for (int i = 0; i < 3; ++i) board.a[i][j] = (++cc) % 9; 
            board.Space_location = std::make_pair(2, 2);

            board.debug_board();

        int m = (int) board.a.size();
        int n = (int) board.a[0].size();
        board.TilePos.resize(m*n);

        int current_x = 0, current_y = 0, cnt = 0;
        for (int i = 1; i <= m*n; ++i) {
            int id = i % (m*n);
            board.TilePos[id].id = id;
            board.TilePos[id].x = current_x;
            board.TilePos[id].y = current_y;
            board.TilePos[id].current_speed = SPEED;

            current_x += GUI::TILE_SIZE + GUI::TILE_PADDING + 10;
            if (++cnt == n) {
                cnt = 0;
                current_y += GUI::TILE_SIZE + GUI::TILE_PADDING + 10;
                current_x = 0;
            } 
        }

        /// init GUI
        GUI::init();
        GUI::loadMedia();
        GUI::setGridSize(3);

        /// show destination board state
        GUI::drawBoard(board.TilePos, 1);

        /// initalize start board state
        // board = Solution_And_Random_Board::RandomStart();
    }

    void moveBoard(Board &board, int addX, int addY) {
        int Finish_x = board.Space_location.first, Finish_y = board.Space_location.second;
        int Start_x = Finish_x + addX, Start_y = Finish_y + addY;
        if ( !board.inBoard(Start_x, Start_y) ) return;

        int fi = board.a[Finish_x][Finish_y];
        int st = board.a[Start_x][Start_y];
        Tile saveTile = board.TilePos[st];

        addX = -addX; addY = -addY;
        // std::cerr << board.TilePos[st].x << " " << board.TilePos[st].y << '\n';
        // std::cerr << board.TilePos[fi].x << " " << board.TilePos[fi].y << '\n';
        // std::cerr << addX << " " << addY << "\n\n";

        while ( board.TilePos[st] != board.TilePos[fi] ) {    
            board.TilePos[st].x += board.TilePos[st].current_speed * addX;
            board.TilePos[st].y += board.TilePos[st].current_speed * addY;

            if (board.TilePos[st].x * addX > board.TilePos[fi].x * addX) 
                board.TilePos[st].x = board.TilePos[fi].x; 

            if (board.TilePos[st].y * addY > board.TilePos[fi].y * addY) 
                board.TilePos[st].y = board.TilePos[fi].y;

            GUI::drawBoard(board.TilePos, 1);
        }
        
        /// update for new board
        std::swap( board.a[Finish_x][Finish_y], board.a[Start_x][Start_y] );
        board.TilePos[fi].x = saveTile.x;
        board.TilePos[fi].y = saveTile.y;
        board.Space_location = std::make_pair( Start_x, Start_y );

        // board.debug_board();
    }

    void PLAY() {
        SDL_Event event;

        bool quit = false;
        while (!quit) {

            while ( SDL_PollEvent(&event) ) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                if ( event.type != SDL_KEYDOWN ) continue;

                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        moveBoard(board, 0, 1);
                        break;
                    case SDLK_DOWN:
                        moveBoard(board, 0, -1);
                        break;
                    case SDLK_LEFT:
                        moveBoard(board, 1, 0);
                        break;
                    case SDLK_RIGHT:
                        moveBoard(board, -1, 0);
                        break;
                    case SDLK_s:
                        ///solution
                        break;
                    case SDLK_ESCAPE:
                        /// out GAME
                        quit = true;
                        break;
                }

                board.debug_board();

                if ( board.winGame() ) {
                    std::cout << "Accept\n";
                    SDL_Delay(500);
                    break;
                }
            }
        }
        GUI::destroy();
    }
}