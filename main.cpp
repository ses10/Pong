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
void drawGame():
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

    if(isRunning)
    {

    }

    return 0;
}
