#include <array>

#include <SDL2/SDL.h>

#include "../include/game.h"
#include "../include/error.h"
#include "../include/constants.h"

using namespace std;

Game::Game()
{
    this->isRunning = false;
    this->isPlaying = false;

    this->frameTicks = 0;

    this->window = nullptr;
    this->windowSurface = nullptr;

    this->keyActionToTake = Action::NONE;
    this->lastActionToken = Action::NONE;

    this->mersenneTwisterPseudoRandomGenerator = mt19937(this->randomDevice());
}

void Game::Start()
{
    try{
        Init();

        this->isRunning = true;

        while(isRunning) 
        {    
            this->Update();
        }
    }
    catch(int errorCode)
    {
        switch(errorCode)
        {
            case Error::SDL_INIT_ERROR: 
                cout << "Failed to initialise SDL" << endl;
                break;
            case Error::SDL_WINDOW_ERROR: 
                SDL_Log("Could not create window: %s\n", SDL_GetError());
                break;
            case Error::SDL_WINDOW_SURFACE_ERROR: 
                SDL_Log("Could not create window surface: %s\n", SDL_GetError());
                break;
            case Error::SDL_LOAD_BMP_ERROR: 
                SDL_Log("Could not create load .bmp image: %s\n", SDL_GetError());
                break;
            default:
                cout << "Generic Error Appeared" << endl;
        }
    }
}

void Game::Quit()
{
    if(windowSurface != NULL)
    {
        SDL_FreeSurface(windowSurface);
    }

    if(window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

void Game::Init()
{
    Pooler::GetInstance()->Init(Constants::GAME_OBJECT_POOL_SIZE, Constants::GAME_OBJECT_POOL_LIMIT);

    for(size_t i = 0; i < Constants::GAME_OBJECT_POOL_SIZE; i++)
    {
        AddGameObjectAction(true);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw Error::SDL_INIT_ERROR;
    }

    window = SDL_CreateWindow(
        "A Lot of Balls!",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        Constants::SCREEN_WIDTH,
        Constants::SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (window == NULL)
    {
        throw Error::SDL_WINDOW_ERROR;
    }

    windowSurface = SDL_GetWindowSurface(window);
    if(windowSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }
}

void Game::HandleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        keyActionToTake = Action::NONE;

        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else  if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_1:
                keyActionToTake = Action::INSERT_CREDIT;
                break;
            case SDLK_2:
                keyActionToTake = Action::REMOVE_ALL_CREDITS;
                break;
            case SDLK_z:
                keyActionToTake = Action::PLAY_GAME;
                break;
            case SDLK_x:
                keyActionToTake = Action::STOP_GAME;
                break;
            case SDLK_DOWN:
                keyActionToTake = Action::RETURN_GAME_OBJECT;
                break;
            case SDLK_UP:
                keyActionToTake = Action::ADD_GAME_OBJECT;
                break;
            default:
                keyActionToTake = Action::NONE;
                break;
            }
        }
    }
}

void Game::HandleLogic(float deltaTime)
{
    if(this->keyActionToTake != this->lastActionToken)
    {
        switch(keyActionToTake)
        {
            case Action::STOP_GAME:
                StopAction();
                break;
            case Action::PLAY_GAME:
                PlayAction();
                break;
            case Action::INSERT_CREDIT:
                break;
            case Action::REMOVE_ALL_CREDITS:
                break;
            case Action::RETURN_GAME_OBJECT:
                ReturnGameObjectAction();
                break;
            case Action::ADD_GAME_OBJECT:
                AddGameObjectAction(false);
                break;
            case Action::NONE:
            default:
                break;
        }
    }

    this->lastActionToken = this->keyActionToTake;
    
    if(this->isPlaying)
    {
        for (size_t i = 0; i < gameObjectPointerVector.size(); i++)
        {
            GameObject *pointedGameObject = gameObjectPointerVector[i];

            pointedGameObject->Update(deltaTime);
        }
    }
}

void Game::HandleRendering()
{
    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0));

    if(this->isPlaying)
    {
        for(size_t i = 0; i < gameObjectPointerVector.size(); i++)
        {
            GameObject * gameObjectPointerToRender = gameObjectPointerVector[i];

            if(gameObjectPointerToRender->IsActive())
            {
                SDL_Surface * surfaceToRender = gameObjectPointerToRender->GetSurface();
                
                SDL_Rect* surfaceRect = gameObjectPointerToRender->GetPosition(); 
                SDL_Rect parameterRect = SDL_Rect(*surfaceRect);
                
                SDL_BlitSurface(surfaceToRender, NULL, windowSurface, &parameterRect);
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

void Game::Update()
{
    if(this->isPlaying)
    {
        int currentGameObjects = 0;
        for(size_t i = 0; i < gameObjectPointerVector.size(); i++)
        {
            if(gameObjectPointerVector[i]->IsActive())
            {
                ++currentGameObjects;
            }
        }
        int currentActiveGameObjects = this->gameObjectPointerVector.size();
    }

    float deltaTime = GenerateDeltaTime();

    this->frameTicks = SDL_GetTicks();

    this->HandleEvents();
    this->HandleLogic(deltaTime);
    this->HandleRendering();
}

float Game::GenerateDeltaTime()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), frameTicks + static_cast<Uint32>(16.666f)))
    {
        continue;
    }

    float deltaTime = static_cast<float>((SDL_GetTicks() - frameTicks)) / 1000.0;

    if (deltaTime < 0.5)
    {
        deltaTime = 0.5;
    }

    return deltaTime;
}

Vector2 Game::GenerateRandomPosition()
{
    float minWidth, maxWidth, minHeight, maxHeight;

    minWidth = maxHeight = Constants::GAME_OBJECT_GENERATION_MARGIN;

    maxWidth = Constants::SCREEN_WIDTH - Constants::GAME_OBJECT_GENERATION_MARGIN;
    minHeight = Constants::SCREEN_HEIGHT - Constants::GAME_OBJECT_GENERATION_MARGIN;

    float randomWidth = GenerateRandomNumber(minWidth, maxWidth);
    float randomHeight = GenerateRandomNumber(minHeight, maxHeight);

    return Vector2(randomWidth, randomHeight);
}

Vector2 Game::GenerateRandomMovement()
{
    const size_t length = 4;
    array<Vector2, length> possibleMovementsArray = 
    {{
        Vector2(1,0),
        Vector2(-1,0),
        Vector2(0,1),
        Vector2(0,-1)
    }};

    int chosenIndex = GenerateRandomNumber(possibleMovementsArray.size());

    return possibleMovementsArray[chosenIndex];
}

GameObject::Color Game::GenerateRandomColor()
{
    int chosenIndex = GenerateRandomNumber(GameObject::Color::GREEN);

    GameObject::Color color = static_cast<GameObject::Color>(chosenIndex);

    return color;
}

int Game::GenerateRandomNumber(int maximum, int minimum /* = 0 */) 
{
    uniform_int_distribution<int> distribution(minimum, maximum); 

    int number_generated = distribution(this->mersenneTwisterPseudoRandomGenerator);

    return number_generated;
}

float Game::GenerateRandomNumber(float maximum, float minimum /* = 0.0 */) 
{
    uniform_real_distribution<float> distribution(minimum, maximum); 

    int number_generated = distribution(this->mersenneTwisterPseudoRandomGenerator);

    return number_generated;
}

size_t Game::GenerateRandomNumber(size_t maximum, size_t minimum /* = 0 */) 
{
    uniform_int_distribution<size_t> distribution(minimum, maximum); 

    size_t number_generated = distribution(this->mersenneTwisterPseudoRandomGenerator);
    
    return number_generated;
}

void Game::ReturnGameObjectAction()
{
    if(this->isPlaying)
    {
        if(gameObjectPointerVector.size() > 0)
        {
            int chosenIndexToReturn; 
            int numberOfTries = 0;
            bool didTimeout = false;

            GameObject * gameObjectToBeReturnedPointer = nullptr;

            do
            {
                int chosenIndexToReturn = GenerateRandomNumber(gameObjectPointerVector.size() - 1);

                if(++numberOfTries > Constants::GENERIC_LOOP_TIMEOUT)
                {
                    didTimeout = true;
                    break;
                }

                gameObjectToBeReturnedPointer = gameObjectPointerVector[chosenIndexToReturn];

            } while(gameObjectToBeReturnedPointer == nullptr);
        
            if(didTimeout)
            {
                SDL_Log("Timeout reached while searching for an available gameobject to avoid the possibility of having infinite loops.");
            }
            else
            {
               Pooler::GetInstance()->Return(gameObjectToBeReturnedPointer);

               gameObjectPointerVector.erase(
                   std::remove_if(
                       gameObjectPointerVector.begin(),
                       gameObjectPointerVector.end(),
                       [gameObjectToBeReturnedPointer](GameObject *comparingGameObject)
                       {
                           return (comparingGameObject == gameObjectToBeReturnedPointer);
                       }),
                   gameObjectPointerVector.end());
            }
        }
        else
        {
            SDL_Log("Currently there's no active gameobjects in the pool.");
        }
    }
}

void Game::AddGameObjectAction(bool isInitializing)
{
    if(isInitializing || this->isPlaying)
    {
        if (!Pooler::GetInstance()->HasNext())
        {
            SDL_Log("The pooling system is being used to its full capacity at this moment.");
        }
        else
        {
            GameObject *gameObjectPointerToStore = Pooler::GetInstance()->GetNext(GameObject::Color::RED);

            Vector2 randomPosition = GenerateRandomPosition();
            Vector2 randomMovement = GenerateRandomMovement();
            GameObject::Color randomColor = GenerateRandomColor();

            string x = "new position: " + to_string(randomPosition.GetX()) + "|" + to_string(randomPosition.GetY());

            SDL_Log(x.c_str());

            float randomSpeed = GenerateRandomNumber(Constants::GAME_OBJECT_MAX_SPEED, Constants::GAME_OBJECT_MIN_SPEED);

            gameObjectPointerToStore->SetColor(randomColor);
            gameObjectPointerToStore->SetPosition(randomPosition);
            gameObjectPointerToStore->SetMovement(randomMovement);
            gameObjectPointerToStore->SetSpeed(randomSpeed);

            if (!gameObjectPointerToStore->IsActive())
            {
                gameObjectPointerToStore->SetActive(true);
            }
            this->gameObjectPointerVector.push_back(gameObjectPointerToStore);
        }
    }
}

void Game::PlayAction()
{
    if(!this->isPlaying)
    {
         Pooler::GetInstance()->Freezed(false, gameObjectPointerVector);
         SDL_Log("The game has started.");
    }
    else
    {
        SDL_Log("The game has already started.");
    }
    this->isPlaying = true;
}

void Game::StopAction()
{
    while(gameObjectPointerVector.size() > Constants::GAME_OBJECT_POOL_SIZE)
    {
        ReturnGameObjectAction();
    }

    if(this->isPlaying)
    {
         Pooler::GetInstance()->Freezed(true, gameObjectPointerVector);
         SDL_Log("The game stopped.");
    }
    else
    {
         SDL_Log("The game has already stopped.");
    }
    this->isPlaying = false;
}