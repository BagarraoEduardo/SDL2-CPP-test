#ifndef Game_HEADER
#define Game_HEADER

#include <vector>
#include<random>

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
    void HandleLogic(float deltaTime);
    void HandleRendering();

    void Update();

    float GenerateDeltaTime();

    Vector2 GenerateRandomPosition();
    Vector2 GenerateRandomMovement();
    GameObject::Color GenerateRandomColor();

	int GenerateRandomNumber(int maximum, int minimum = 0);
	float GenerateRandomNumber(float maximum, float minimum = 0.0);
    size_t GenerateRandomNumber(size_t maximum, size_t minimum = 0);

    SDL_Window * window;
    SDL_Surface *windowSurface;

    bool isRunning;
    
    Uint32 frameTicks;

    random_device randomDevice;
    mt19937 mersenneTwisterPseudoRandomGenerator;

    vector <GameObject*> gameObjectVector;
};

#endif