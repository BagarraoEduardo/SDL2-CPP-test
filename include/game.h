#ifndef Game_HEADER
#define Game_HEADER

#include <vector>

#include <SDL2/SDL.h>

#include "pooler.h"
#include "game_object.h"


class Game
{

public:
    Game();
    void Start();
    void Quit();

private:
    
    static const int SCREEN_WIDTH = 960;
    static const int SCREEN_HEIGHT = 540;

    static const int GAME_OBJECT_POOL_SIZE = 1;
    static const int GAME_OBJECT_POOL_LIMIT = 1;

    void Init();

    void HandleEvents();
    void HandleLogic();
    void HandleRendering();

    SDL_Window * window;
    SDL_Surface *windowSurface;

    bool isRunning;

    vector <GameObject *> gameObjectVector;
};

#endif