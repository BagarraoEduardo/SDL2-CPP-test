#ifndef Game_HEADER
#define Game_HEADER

#include <vector>
#include<random>

#ifdef WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif


#include "pooler.h"
#include "action.h"
#include "game_object.h"

/// <summary>
/// Game class. This class contains the game loop.
/// </summary>
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

    template<typename Number>
    void validateDistribution(Number &minimum, Number &maximum);

    //action methods
    void ReturnGameObjectAction();
    void AddGameObjectAction(bool isInitializing);
    void InsertCreditAction();
    void RemoveAllCreditsAction();
    void PlayAction();
    void StopAction();

    SDL_Window * window; // main window
    SDL_Surface *windowSurface; //surface to display the project background

    TTF_Font* titleFont; //font to be used to display titles
    TTF_Font* messageFont; // font to be used to diplay messages

    SDL_Surface *creditsTitleFontSurface; //surface to display credits information
    SDL_Rect creditsTitleFontRect; //credits information title rect

    SDL_Surface *gameStateFontSurface; //surface to display the current state of the game
    SDL_Rect gameStateFontRect; //game state title rect

    SDL_Surface *gameObjectsQuantityFontSurface; //surface to display the number of gameobjects that are active in the game
    SDL_Rect gameObjectQuantityFontRect; //number of gameobjects title rect

    SDL_Surface *outOfCreditsFontSurface; //surface to display the message to show that the player need to insert a credid if he wants to play
    SDL_Rect outOfCreditsFontRect; //out of credits message rect

    float outOfCreditsMessageTime; //time that the message that shows that you're out of credits lasts

    //frame control variables
    Uint32 frameTicks;
    Uint32 frameEndTicks;

    //to generate random numbers
    random_device randomDevice;
    mt19937 mersenneTwisterPseudoRandomGenerator;

    bool isRunning; //this will be true since the project opens until we exit from the game
    bool isPlaying; //this will be false when the game is either stopped or when you start the game(before press play)
    int credits; //current number of credits

    vector <GameObject*> gameObjectPointerVector; //vector that store all the gameobjects that are currently in the screen
    
    //key event handling variables
    Action keyActionToTake;
    Action lastActionToken;
};

#endif