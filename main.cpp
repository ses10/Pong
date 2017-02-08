#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PADDLE_WIDTH = 27;
const int PADDLE_HEIGHT = 155;
const int PLAYER1_PADDLE_X = PADDLE_WIDTH;
const int PLAYER2_PADDLE_X = SCREEN_WIDTH - PADDLE_WIDTH*2;
const int BALL_WIDTH = 25;
const int BALL_HEIGHT = 25;
const int BALL_MAX_SPEED = 2;
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
void updateBall();
void resetGame();
bool rectOverlap(SDL_Rect rect1, SDL_Rect rect2);

// Resource variables
SDL_Surface *backbuffer = NULL;
SDL_Surface *backgroundImage = NULL;
SDL_Surface *ballImage = NULL;
SDL_Surface *player1PaddleImage = NULL;
SDL_Surface *player2PaddleImage = NULL;

//Game Variables
int player1Score;
int player2Score;
SDL_Rect player1Rect;
SDL_Rect player2Rect;
SDL_Rect ballRect;
int ballXVel;
int ballYVel;

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

    resetGame();

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
    updateBall();
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
    Updates the ball's position every frame
*/
void updateBall()
{
    ballRect.x += ballXVel;
    ballRect.y += ballYVel;

    //If the ball hits player1, make it bounce
    if(rectOverlap(ballRect, player1Rect))
    {
        ballXVel = rand()%BALL_MAX_SPEED + 1;
    }

    //If the ball hits player2, make it bounce
    if(rectOverlap(ballRect, player2Rect))
    {
        ballXVel = (rand()%BALL_MAX_SPEED +1) * -1;
    }

    //Make sure the ball doesn't leave the screen and make it
    //bounce randomly
    if(ballRect.y < 0)
    {
        ballRect.y = 0;
        ballYVel = rand()%BALL_MAX_SPEED + 1;
    }

    if(ballRect.y > SCREEN_HEIGHT - ballRect.h)
    {
        ballRect.y = SCREEN_HEIGHT - ballRect.h;
        ballYVel = (rand()%BALL_MAX_SPEED + 1)* -1 ;
    }

    //If player1 scores
    if(ballRect.x > SCREEN_WIDTH)
    {
        player1Score++;
        resetGame();
    }

    //If player2 scores
    if(ballRect.x < 0-ballRect.w)
    {
        player2Score++;
        resetGame();
    }
}

/**
    Checks whether the two given SDL_Rect collide

    @param rect1 is the rectangle colliding into rect2
    @param rect2 is the rectangle that is the target of the collision

    @return true if given SDL_Rects overlap, false otherwise
*/
bool rectOverlap(SDL_Rect rect1, SDL_Rect rect2)
{
    //check collision to the right
    if(rect1.x >= rect2.x + rect2.w)
        return false;

    //check collision on bottom
    if(rect1.y >= rect2.y + rect2.h)
        return false;

    //check collision to the left
    if(rect2.x >= rect1.x+rect1.w)
        return false;

    //check collision to the right
    if(rect2.y >= rect1.y+rect1.h)
        return false;

    return true;
}

/**
    Draws all the sprites to the backbuffer
*/
void drawGame()
{
    drawImage(backgroundImage, backbuffer, 0, 0);
    drawImage(player1PaddleImage, backbuffer, player1Rect.x,player1Rect.y);
    drawImage(player2PaddleImage, backbuffer, player2Rect.x,player2Rect.y);
    drawImage(ballImage, backbuffer, ballRect.x, ballRect.y);
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

/**
    Resets the player and ball positions
*/
void resetGame()
{
    player1Rect.x = PLAYER1_PADDLE_X;
    player1Rect.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    player1Rect.w = PADDLE_WIDTH;
    player1Rect.h = PADDLE_HEIGHT;

    player2Rect.x = PLAYER2_PADDLE_X;
    player2Rect.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    player2Rect.w = PADDLE_WIDTH;
    player2Rect.h = PADDLE_HEIGHT;

    ballRect.x = SCREEN_WIDTH/2 - BALL_WIDTH/2;
    ballRect.y = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
    ballRect.w = BALL_HEIGHT;
    ballRect.h = BALL_WIDTH;

    //Make the ball X velocity a random value from 1 to BALL_MAX_SPEED
    ballXVel = rand()%BALL_MAX_SPEED + 1;

    //Make the ball Y velocity a random value from -BALL_MAX_SPEED to BALL_MAX_SPEED
    ballYVel = (rand()%BALL_MAX_SPEED*2 + 1) - BALL_MAX_SPEED;

    //Give it a 50% probability of going toward's the player
    if(rand()%2 == 0)
        ballXVel *= -1;
}
