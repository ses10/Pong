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

    return 0;
}

/**
    Sets up the initial state of the game
    by starting SDL and loading assets

    @return true if successful, false otherwise
*/
bool initGame()
{
    if(!SDL_Init(SDL_INIT_EVERYTHING))
        return false;

    if(!loadFiles())
        return false;

    //Set the title
    SDL_WM_SetCaption("Paddle Game!",NULL);

    return true;
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
