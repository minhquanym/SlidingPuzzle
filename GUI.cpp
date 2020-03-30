#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Board.cpp"


namespace GUI {
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_PADDING = 50;

    int gridSize = 3;
    int rawSize = (WINDOW_HEIGHT - 2*WINDOW_PADDING) / gridSize;
    int TILE_PADDING = rawSize/20;
    int TILE_SIZE = rawSize - 2*TILE_PADDING;

    SDL_Colour fontcolour = {112, 20, 82};
    //The window we'll render to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;


    //Global font
    TTF_Font *gFont = NULL;
    TTF_Font *gFontMedium = NULL;
    TTF_Font *gFontSmall = NULL;

    Mix_Chunk *gClick = NULL;
    Mix_Music *gMusic = NULL;


    //Texture wrapper class
    class LTexture
    {
        public:
            //Initializes variables
            LTexture() {
                mTexture = NULL;
                mWidth = 0;
                mHeight = 0;
            }

            //Deallocates memory
            ~LTexture() {
                free();
            }

            //Loads image at specified path
            bool loadFromFile( std::string path ) {
                //Get rid of preexisting texture
                free();

                //The final texture
                SDL_Texture* newTexture = NULL;

                //Load image at specified path
                SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
                if( loadedSurface == NULL )
                {
                    printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
                }
                else
                {
                    //Color key image
                    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

                    //Create texture from surface pixels
                    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
                    if( newTexture == NULL )
                    {
                        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
                    }
                    else
                    {
                        //Get image dimensions
                        mWidth = loadedSurface->w;
                        mHeight = loadedSurface->h;
                    }

                    //Get rid of old loaded surface
                    SDL_FreeSurface( loadedSurface );
                }

                //Return success
                mTexture = newTexture;
                return mTexture != NULL;
            }
            
            //Creates image from font string
            bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font) {
                free();

                //Render text surface
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor );
                if( textSurface == NULL )
                {
                    printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
                }
                else
                {
                    //Create texture from surface pixels
                    mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
                    if( mTexture == NULL )
                    {
                        printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
                    }
                    else
                    {
                        //Get image dimensions
                        mWidth = textSurface->w;
                        mHeight = textSurface->h;
                    }

                    //Get rid of old surface
                    SDL_FreeSurface( textSurface );
                }
                
                //Return success
                return mTexture != NULL;
            }

            //Deallocates texture
            void free() {
                //Free texture if it exists
                if( mTexture != NULL )
                {
                    SDL_DestroyTexture( mTexture );
                    mTexture = NULL;
                    mWidth = 0;
                    mHeight = 0;
                }
            }

            //Set color modulation
            void setColor( Uint8 red, Uint8 green, Uint8 blue ) {
                //Modulate texture rgb
                SDL_SetTextureColorMod( mTexture, red, green, blue );
            }

            //Set blending
            void setBlendMode( SDL_BlendMode blending ) {
                //Set blending function
                SDL_SetTextureBlendMode( mTexture, blending );
            }

            //Set alpha modulation
            void setAlpha( Uint8 alpha ) {
                //Modulate texture alpha
                SDL_SetTextureAlphaMod( mTexture, alpha );
            }
            
            //Renders texture at given point
            void render( int x, int y, SDL_Rect* clip = NULL) {

                SDL_Rect renderQuad = { x, y, mWidth, mHeight };

                if (clip != NULL) {
                    renderQuad.w = clip->w;
                    renderQuad.h = clip->h;
                }

                SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
            }

            //Gets image dimensions
            int getWidth()      {return mWidth;}
            int getHeight()     {return mHeight;}

        private:
            //The actual hardware texture
            SDL_Texture* mTexture;

            //Image dimensions
            int mWidth;
            int mHeight;
    };

    LTexture gTextTexture, gTextTextureSmall;
    LTexture gImageTexture;
    std::vector<SDL_Rect> gTileClips;
        
    void drawRectangle(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_Rect rect = {x, y, w, h};
        SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
        SDL_RenderFillRect(gRenderer, &rect);
    }
    
    

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
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            return false;
        }
        //Init SDL_ttf
        if (TTF_Init() < 0) { 
            printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
			return false;
        }
        return true;
    }

    bool loadMedia() {
        
        // Load image
        if (!gImageTexture.loadFromFile("assets/cat600.jpg")) {
            printf("Failed to load picture");
            return false;
        }
        gTileClips.resize(gridSize*gridSize);
        int startWidth = (gImageTexture.getHeight() - TILE_SIZE*gridSize)/2;
        std::cerr<<"startWidth: "<<startWidth<<'\n';
        for (int i = 0; i < gridSize; i++)
            for (int j = 0; j < gridSize; j++) {
                int id = i*gridSize + j + 1;
                if (id == gridSize*gridSize) continue;
                gTileClips[id].x = startWidth + j*TILE_SIZE;
                gTileClips[id].y = startWidth + i*TILE_SIZE;
                gTileClips[id].w = TILE_SIZE;
                gTileClips[id].h = TILE_SIZE;
            }
        // Load audio
        
        gClick = Mix_LoadWAV("assets/click.wav");
        if (gClick == NULL) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            return false;
        }
        gMusic = Mix_LoadMUS("assets/music.wav");
        if (gMusic == NULL) {
            printf("Failed to load background music! SDL_Mixer Error: %s\n", Mix_GetError());
            return false;
        }
        Mix_PlayMusic(gMusic, -1);
        // Load font 
        gFont = TTF_OpenFont("assets/neuropol.ttf", 50);
        gFontMedium = TTF_OpenFont("assets/neuropol.ttf", 25);
        gFontSmall = TTF_OpenFont("assets/neuropol.ttf", 20);

        if (gFont == NULL) {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
            return false;
        }
        if (!gTextTexture.loadFromRenderedText("The world needs you", fontcolour, gFont)) {
            printf( "Failed to render text texture!\n" );
			return false;
        }
        return true;
    }
    void destroy() {

        Mix_FreeChunk(gClick);
        gClick = NULL;
        // Destroy window
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
        gRenderer = NULL;

        // Quit het cho tao
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void setGridSize(const int& n) {
        gridSize = n;
        rawSize = (WINDOW_HEIGHT - 2*WINDOW_PADDING) / gridSize;
        TILE_PADDING = rawSize/20;
        TILE_SIZE = rawSize - 2*TILE_PADDING;

        std::cerr<<"gridSize: "<<gridSize<<'\n';
        std::cerr<<"rawSize: "<<rawSize<<'\n';
        std::cerr<<"TILE_SIZE: "<<TILE_SIZE<<'\n';
        std::cerr<<"TILE_PADDING: "<<TILE_PADDING<<'\n';
    }

    void drawBoard(const std::vector<Tile> board, const bool& drawNumber) {

        // Render background
        drawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
                        88, 55, 128, 1);
        for (auto tile : board) {
            if (tile.id == 0) continue;
            int x = tile.x + WINDOW_PADDING + TILE_PADDING;
            int y = tile.y + WINDOW_PADDING + TILE_PADDING;

            // Draw shadow
            drawRectangle(x+TILE_PADDING, y+TILE_PADDING, TILE_SIZE, TILE_SIZE,
                            42, 45, 46, 1);

            gImageTexture.render(x, y, &gTileClips[tile.id]);

                
            if (drawNumber == 1) {
                std::string number = std::to_string(tile.id);
                gTextTexture.loadFromRenderedText(number, fontcolour, gFont);
                gTextTexture.render(x + (rawSize/2 - gTextTexture.getWidth()/2),
                                    y + (rawSize/2 - gTextTexture.getHeight()/2));
            }
        }
        gTextTextureSmall.loadFromRenderedText("Press S to solve", fontcolour, gFontMedium);
        gTextTextureSmall.render(150, 20);

    }
    void clearRender() {
        SDL_RenderClear(gRenderer);
    }
    void startRender() {
        SDL_RenderPresent(gRenderer);

    }

    void renderWinner() {
        SDL_RenderClear(gRenderer);
        drawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
                        88, 55, 128, 1);
        gImageTexture.render(WINDOW_PADDING, WINDOW_PADDING);
        //gWinnerTexture.render()
        SDL_RenderPresent(gRenderer);
    }

    void renderLoser() {
        SDL_RenderClear(gRenderer);
        drawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
                        88, 55, 128, 1);
        gImageTexture.render(WINDOW_PADDING, WINDOW_PADDING);
        SDL_RenderPresent(gRenderer);
    }
}
