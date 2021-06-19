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
    const Pooler * POOLER_INSTANCE = Pooler::GetInstance();
    
    static const int SCREEN_WIDTH = 960;
    static const int SCREEN_HEIGHT = 540;

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