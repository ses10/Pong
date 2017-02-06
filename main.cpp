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

// Resource variables
SDL_Surface *backbuffer = NULL;
SDL_Surface *backgroundImage = NULL;
SDL_Surface *ballImage = NULL;
SDL_Surface *player1PaddleImage = NULL;
SDL_Surface *player2PaddleImage = NULL;

int main(int argc, char *argv[])
{
    if(!initGame())
    {
        closeGame();
        return 1;
    }

    if(isRunning())
    {

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
    SDL_WM_SetCaption("Paddle Game!",NULL);

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
    backgroundImage = SDL_LoadBMP("graphics/background.bmp");
    player1PaddleImage = SDL_LoadBMP("graphics/player.bmp");
    player2PaddleImage = SDL_LoadBMP("graphics/player.bmp");
    ballImage = SDL_LoadBMP("graphics/ball.bmp");

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

