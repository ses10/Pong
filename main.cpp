#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PADDLE_WIDTH = 27;
const int PADDLE_HEIGHT = 155;
const int BALL_WIDTH = 25;
const int BALL_HEIGHT = 25;
const int FPS = 30;
const int FRAME_DELAY = 1000;
const int PLAYER_SPEED = 10;

// Core Game functions
bool initGame();
void runGame();
void drawGame();
void closeGame();

// Utility functions
bool isRunning();
bool initSDL();
bool loadFiles();
void freeFiles();
void drawImage(SDL_Surface *image, SDL_Surface *dst, int x, int y);
SDL_Surface* loadImage(char* fileName);
void updatePlayer(SDL_Rect* playerRect, bool isPlayer1);

// Resource variables
SDL_Surface *backbuffer = NULL;
SDL_Surface *backgroundImage = NULL;
SDL_Surface *ballImage = NULL;
SDL_Surface *player1PaddleImage = NULL;
SDL_Surface *player2PaddleImage = NULL;

//Game Variables
SDL_Rect player1Rect;
SDL_Rect player2Rect;


int main(int argc, char *argv[])
{
    if(!initGame())
    {
        closeGame();
        return 1;
    }

    while(isRunning())
    {
        //clear screen
        SDL_FillRect(backbuffer, NULL, 0);

        runGame();
        drawGame();

        SDL_Flip(backbuffer);
    }

    closeGame();

    return 0;
}

/**
    Sets up the initial state of the game
    by starting SDL and loading assets

    @return true if successful, false otherwise
*/
bool initGame()
{
    if(!initSDL())
        return false;

    if(!loadFiles())
        return false;

    // Set the title
    SDL_WM_SetCaption("Pong",NULL);

    return true;
}

/**
    Initializes SDL and libraries

    @return true if successful, false otherwise
*/
bool initSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    //Generate screen
    backbuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE );

    //Error check Backbuffer
    if(backbuffer == NULL)
        return false;

    return true;
}

/**
    Loads the assets for the game

    @return true if successful, false otherwise
*/
bool loadFiles()
{
    // Load images
    backgroundImage = loadImage("graphics/background.bmp");
    player1PaddleImage = loadImage("graphics/player.bmp");
    player2PaddleImage = loadImage("graphics/player.bmp");
    ballImage = loadImage("graphics/ball.bmp");

    if(backgroundImage == NULL)
        return false;
    if(ballImage == NULL)
        return false;
    if(player1PaddleImage == NULL)
        return false;
    if(player2PaddleImage == NULL)
        return false;

    // Load sounds

    // Load music

    // Load font

    return true;
}
/**
    Loads a given BMP image into memory

    @param fileName is the path to the BMP image

    @return pointer to SDL_Surface
*/
SDL_Surface* loadImage(char* fileName)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* processedImage = NULL;

    loadedImage = SDL_LoadBMP(fileName);

    if(loadedImage != NULL)
    {
        //convert to format of video framebuffer
        processedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
    }

    return processedImage;
}

/**
    Updates the players and ball every frame
*/
void runGame()
{
    updatePlayer(&player1Rect, true);
    updatePlayer(&player2Rect, false);
    //update ball
}

/**
    Updates the given player's position every frame

    @param playerRect is pointer to the SDL_Rect representing the player paddle
    @param isPlayer1 is whether the given SDL_Rect represents player1
*/
void updatePlayer(SDL_Rect* playerRect, bool isPlayer1)
{
    Uint8 *keys = SDL_GetKeyState(NULL);

    Uint8 up;
    Uint8 down;

    isPlayer1 == true ? up = keys[SDLK_w] : up = keys[SDLK_UP];
    isPlayer1 == true ? down = keys[SDLK_s] : down = keys[SDLK_DOWN];

    //Move the paddle when the up/down key is pressed
    if(up)
        playerRect->y -= PLAYER_SPEED;

    if(down)
        playerRect->y += PLAYER_SPEED;

    //Make sure the paddle doesn't leave the screen
    if(playerRect->y < 0)
        playerRect->y = 0;

    if(playerRect->y > SCREEN_HEIGHT-playerRect->h)
        playerRect->y = SCREEN_HEIGHT-playerRect->h;
}

/**
    Draws all the sprites to the backbuffer
*/
void drawGame()
{
    drawImage(backgroundImage, backbuffer, 0, 0);
    drawImage(player1PaddleImage, backbuffer, player1Rect.x,player1Rect.y);
    drawImage(player2PaddleImage, backbuffer, player2Rect.x,player2Rect.y);
}

/**
    Draws an image to the destination SDL_Surface

    @param image is the source surface
    @param dst is the destination surface
    @param x is the x coordinate of where the image will be drawn to
    @param y is the y coordinate of where the image will be drawn to
*/
void drawImage(SDL_Surface *image, SDL_Surface *dst, int x, int y)
{
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;

    SDL_BlitSurface(image, NULL, dst, &dstRect);
}

/**
    Closes the program and frees all assets from memory
*/
void closeGame()
{
    freeFiles();
    SDL_Quit();
}

/**
    Frees all loaded assets from memory
*/
void freeFiles()
{
    //free images
    SDL_FreeSurface(backgroundImage);
    SDL_FreeSurface(player1PaddleImage);
    SDL_FreeSurface(player2PaddleImage);
    SDL_FreeSurface(ballImage);
}

/**
    Checks to see if user exited game

    @return false if user closes program, true otherwise
*/
bool isRunning()
{
    SDL_Event event;

    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;

        if( event.type == SDL_KEYDOWN )
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                    running = false;
            }
        }
    }

    return running;
}

