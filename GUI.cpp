#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Board.cpp"


namespace GUI {

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_PADDING = 75;

    int gridSize = 3;
    int rawSize = (WINDOW_WIDTH - 5*WINDOW_PADDING) / gridSize;
    int TILE_PADDING = 5;
    int TILE_SIZE = rawSize - 2*TILE_PADDING;

    //The window we'll render to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;
    
    //Global font
    TTF_Font *gFont = NULL;



    bool init() {

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
            return false;
        }
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

        // Create window
        gWindow = SDL_CreateWindow("Sliding Puzzle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			return false;
		}  

        // Create renderer
        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        if (gRenderer == NULL) {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            return false;
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        
        // Init image loading
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if ( (IMG_Init(imgFlags) & imgFlags) < imgFlags) {
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
			return false;
        }

        //Init SDL_ttf
        if (TTF_Init() == -1) { 
            printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
			return false;
        }
        return true;
    }
    void destroy() {

        // Destroy window
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
        gRenderer = NULL;

        // Quit het cho tao
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void setGridSize(const int& n) {
        int gridSize = 3;
        int rawSize = (WINDOW_WIDTH - 2*WINDOW_PADDING) / gridSize;
        int TILE_PADDING = 5;
        int TILE_SIZE = rawSize - 2*TILE_PADDING;
    }

    void drawBoard(const std::vector<Tile> board, const bool& drawNumber) {

        SDL_Rect backgroundRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_SetRenderDrawColor(gRenderer, 148, 247, 239, 1);
        SDL_RenderFillRect(gRenderer, &backgroundRect);
        for (auto tile : board) {
            int x = tile.x;
            int y = tile.y;
            SDL_Rect tileRect = {x, y, rawSize, rawSize};

            if (tile.id == 0)
                SDL_SetRenderDrawColor(gRenderer, 214, 211, 184, 1); // empty tile 
            else 
                SDL_SetRenderDrawColor(gRenderer, 57, 107, 56, 1); // padding tile
            
            SDL_RenderFillRect(gRenderer, &tileRect);

            /// NOT COMPLETE
            // if (drawNumber) {
            //     if (tile.id == 0) continue;
            //     std::string number = std::to_string(tile.id);
            //     SDL_Texture* numberTexture = renderText(number, )
            
            // }
        }
        SDL_RenderPresent(gRenderer);
    }
    void renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* clip = NULL) {

        SDL_Rect renderQuad;
        renderQuad.x = x;
        renderQuad.y = y;
        if (clip != NULL) {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }
        SDL_RenderCopy(gRenderer, texture, clip, &renderQuad);
    }

    SDL_Texture* renderText(const std::string& s, const std::string& fontpath, SDL_Color color, int fontsize) {

        TTF_Font *font = TTF_OpenFont(fontpath.c_str(), fontsize);
        if (font == NULL) {
            printf("Font could not be loaded! Error: %s", TTF_GetError());
            return NULL;
        }

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, s.c_str(), color);
        if (textSurface == NULL) {
            printf("TTF_RenderText got error! Error: %s", TTF_GetError());
            return NULL;
        }
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (textTexture == NULL) {
            printf("Create Texture From Surface! Error: %s", SDL_GetError());
        }
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return textTexture;
    }
}