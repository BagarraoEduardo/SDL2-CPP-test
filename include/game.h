#ifndef Game_HEADER
#define Game_HEADER

#include <vector>
#include<random>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "pooler.h"
#include "action.h"
#include "game_object.h"


class Game
{

public:
    ~Game();
    Game();
    void Start();

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

    //action methods
    void ReturnGameObjectAction();
    void AddGameObjectAction(bool isInitializing);
    void InsertCreditAction();
    void RemoveAllCreditsAction();
    void PlayAction();
    void StopAction();

    SDL_Window * window;
    SDL_Surface *windowSurface;

    TTF_Font* titleFont;

    SDL_Surface *creditsTitleFontSurface;
    SDL_Rect creditsTitleFontRect;

    SDL_Surface *gameStateFontSurface;
    SDL_Rect gameStateFontRect;

    SDL_Surface *gameObjectsQuantityFontSurface;
    SDL_Rect gameObjectQuantityFontRect;

    Uint32 frameTicks;

    random_device randomDevice;
    mt19937 mersenneTwisterPseudoRandomGenerator;

    bool isRunning;
    bool isPlaying;
    int credits;

    vector <GameObject*> gameObjectPointerVector;
    
    Action keyActionToTake;
    Action lastActionToken;

    int i = 10;
};

#endif