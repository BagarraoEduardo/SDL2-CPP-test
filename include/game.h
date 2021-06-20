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
    void Init();

    void HandleEvents();
    void HandleLogic();
    void HandleRendering();

    static Vector2 GenerateRandomPosition();
    static Vector2 GenerateRandomMovement();
    static GameObject::Color GenerateRandomColor();
	
    SDL_Window * window;
    SDL_Surface *windowSurface;

    bool isRunning;

    vector <GameObject*> gameObjectVector;
};

#endif