#include <SDL2/SDL.h>
#include "GUI.cpp"
#include "Solution.cpp"
#include "RandomStart.cpp"


namespace GAME_PLAY {
    const int DRAW_NUMBER = 1;
    bool FREEZE = false;

    const int BUTTON_WIDTH = 200;
    const int BUTTON_HEIGHT = 50;
    const int TOTAL_BUTTONS = 3;
    
    enum LButtonState
    {
        BUTTON_MOUSE_OUT = 0,
        BUTTON_MOUSE_OVER_MOTION = 1,
        BUTTON_MOUSE_DOWN = 2,
        BUTTON_MOUSE_UP = 3,
        BUTTON_TOTAL = 4
    };
    Board board;
    int score = 0;

    void moveBoard(Board &board, int addX, int addY);
    void SOLVE() {
        FREEZE = true; score = 0;
        Solution::Solution_A_star(board);
        std::cerr << "AFTER:\n";
        board.debug_board();
        for (std::pair<int, int> foo : Solution::lsTrace) 
            moveBoard(board, foo.first, foo.second);
    }
    //The mouse button
    class LButton
    {
        public:
            //Initializes internal variables
            LButton() {
                mPosition.x = 0;
                mPosition.y = 0;
                mCurrentState = BUTTON_MOUSE_OUT;
            }

            //Sets top left position
            void setPosition( int x, int y ) {
                mPosition.x = x;
	            mPosition.y = y;
            }
            void setName(std::string s) {
                mName = s;
            }
            //Handles mouse event
            void handleEvent( SDL_Event* e ) {
                //If mouse event happened
                if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
                {
                    //Get mouse position
                    int x, y;
                    SDL_GetMouseState( &x, &y );

                    //Check if mouse is in button
                    bool inside = true;

                    //Mouse is left of the button
                    if( x < mPosition.x )
                    {
                        inside = false;
                    }
                    //Mouse is right of the button
                    else if( x > mPosition.x + BUTTON_WIDTH )
                    {
                        inside = false;
                    }
                    //Mouse above the button
                    else if( y < mPosition.y )
                    {
                        inside = false;
                    }
                    //Mouse below the button
                    else if( y > mPosition.y + BUTTON_HEIGHT )
                    {
                        inside = false;
                    }

                    //Mouse is outside button
                    if( !inside )
                    {
                        mCurrentState = BUTTON_MOUSE_OUT;
                    }
                    //Mouse is inside button
                    else
                    {

                        //Set mouse over sprite
                        switch( e->type )
                        {
                            case SDL_MOUSEMOTION:
                            mCurrentState = BUTTON_MOUSE_OVER_MOTION;
                            break;
                        
                            case SDL_MOUSEBUTTONDOWN:
                            mCurrentState = BUTTON_MOUSE_DOWN;
                            break;
                            
                            case SDL_MOUSEBUTTONUP:
                            mCurrentState = BUTTON_MOUSE_UP;
                            break;
                        }
                    }
                }
            }
        
            //Shows button sprite
            void render() {

                int &x = mPosition.x;
                int &y = mPosition.y;
                GUI::drawRectangle(x+GUI::TILE_PADDING, y+GUI::TILE_PADDING, BUTTON_WIDTH, BUTTON_HEIGHT,
                            42, 45, 46, 1);
                
                Uint8 r = 121, g = 130, b = 127;
                switch (mCurrentState) {
                    case BUTTON_MOUSE_DOWN:
                        Mix_PlayChannel(-1, GUI::gClick, 0);
                        break;
                    case BUTTON_MOUSE_OUT:
                        r = 161, g = 181, b = 175;
                        break;
                    default:
                        break;
                }
                GUI::drawRectangle(x, y, BUTTON_WIDTH, BUTTON_HEIGHT,
                                    r, g, b, 1);
                GUI::gTextTextureSmall.loadFromRenderedText(mName, GUI::fontcolour, GUI::gFontSmall);
                GUI::gTextTextureSmall.render(x + (BUTTON_WIDTH-GUI::gTextTextureSmall.getWidth())/2,
                                         y + (BUTTON_HEIGHT-GUI::gTextTextureSmall.getHeight())/2);
                
            }

        private:
            //Top left position
            SDL_Point mPosition;
            std::string mName;
            //Currently used global sprite
            LButtonState mCurrentState;
    };


    LButton gButtons[ TOTAL_BUTTONS ];

    

    void DRAW() {
        GUI::clearRender();
        GUI::drawBoard(board.TilePos, DRAW_NUMBER);
        gButtons[0].setName("SCORE: " + std::to_string(score));
        gButtons[0].render();
        GUI::startRender();
    }
    
    void SETUP() {
        std::cout << ".......Loading game data.......\n";

        /// init GUI
        GUI::init();
        GUI::setGridSize(3);
        gButtons[0].setPosition(GUI::WINDOW_PADDING + GUI::rawSize*GUI::gridSize + 50, GUI::WINDOW_PADDING + 50);
        
        GUI::loadMedia();
        
        /// initalize tile information
        board.destination(3, GUI::rawSize);

        /// show destination board state
        DRAW();
        SDL_Delay(2000);

        /// initalize start board state
        RandomStart::randomStart(board);
        DRAW();

        /// init score 
        score = (int) board.a.size() * 100;
    }

    void moveBoard(Board &board, int addX, int addY) {

        int Finish_x = board.Space_location.first, Finish_y = board.Space_location.second;
        int Start_x = Finish_x + addX, Start_y = Finish_y + addY;
        if ( !board.inBoard(Start_x, Start_y) ) return;

        if (!FREEZE) {
            score -= 5;
            if (score <= 0) return;
        }
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
            DRAW();
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
                if (FREEZE) break;
                if ( event.type != SDL_KEYDOWN ) {
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
                            SOLVE();
                            break;
                        case SDLK_ESCAPE:
                            /// out GAME
                            quit = true;
                            break;
                    }
                }
                if (!FREEZE) {
                    for (int i = 0; i < 1; i++)
                        gButtons[i].handleEvent(&event);
                }

                if (score <= 0) {
                    std::cout << "LOSER!!!!!\n";
                    SDL_Delay(1000);
                    GUI::renderLoser();
                    FREEZE = true;
                }
                else if ( board.winGame() ) {
                    FREEZE = true;
                    SDL_Delay(1000);
                    GUI::renderWinner();
                }
                else DRAW();
            }

            }
        GUI::destroy();
    }
}