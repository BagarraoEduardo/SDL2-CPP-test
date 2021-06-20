#include <cstdlib>

#include "../include/game.h"
#include "../include/error.h"
#include "../include/constants.h"

#include<array>

using namespace std;

Game::Game()
{
    this->isRunning = false;

    this->frameTicks = 0;

    this->window = nullptr;
    this->windowSurface = nullptr;

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
        if(Pooler::GetInstance()->HasNext())
        {
            GameObject * gameObjectPointerToStore = Pooler::GetInstance()->GetNext(GameObject::Color::RED);
            
            Vector2 randomPosition = GenerateRandomPosition();
            Vector2 randomMovement = GenerateRandomMovement();
            GameObject::Color randomColor = GenerateRandomColor();
            float randomSpeed = 
                static_cast<float>(rand() % static_cast<int>(Constants::GAME_OBJECT_MAX_SPEED - Constants::GAME_OBJECT_MIN_SPEED) + static_cast<int>(Constants::GAME_OBJECT_MIN_SPEED)); 

            gameObjectPointerToStore->SetColor(randomColor);
            gameObjectPointerToStore->SetPosition(randomPosition);
            gameObjectPointerToStore->SetMovement(randomMovement);
            gameObjectPointerToStore->SetSpeed(randomSpeed);

            if(!gameObjectPointerToStore->IsActive())
            {
                gameObjectPointerToStore->SetActive(true);
            }
            gameObjectVector.push_back(gameObjectPointerToStore);
        }
        else
        {
            break;
        }
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
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else  if (event.type == SDL_KEYDOWN)
        {

        }
    }
}

void Game::HandleLogic(float deltaTime)
{
    for(size_t i = 0; i < gameObjectVector.size(); i++)
    {
        GameObject * pointedGameObject = gameObjectVector[i];

        pointedGameObject->Update(deltaTime); //TODO: mudar o deltatime
    }
}

void Game::HandleRendering()
{
    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0));

    for(size_t i = 0; i < gameObjectVector.size(); i++)
    {
        GameObject * gameObjectPointerToRender = gameObjectVector[i];

        if(gameObjectPointerToRender->IsActive())
        {
            SDL_Surface * surfaceToRender = gameObjectPointerToRender->GetSurface();
            
            SDL_Rect* surfaceRect = gameObjectPointerToRender->GetPosition(); 
            SDL_Rect parameterRect = SDL_Rect(*surfaceRect);
            
            SDL_BlitSurface(surfaceToRender, NULL, windowSurface, &parameterRect);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void Game::Update()
{
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

    minWidth = maxHeight = 0 + Constants::GAME_OBJECT_GENERATION_MARGIN;
    maxWidth = minHeight = Constants::SCREEN_WIDTH - Constants::GAME_OBJECT_GENERATION_MARGIN;

    float randomWidth = static_cast<float>(rand() % static_cast<int>(maxWidth));
    float randomHeight = static_cast<float>(rand() % static_cast<int>(maxHeight));;

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

    int chosenIndex = rand() % possibleMovementsArray.size();

    return possibleMovementsArray[chosenIndex];
}

GameObject::Color Game::GenerateRandomColor()
{
    GameObject::Color color = static_cast<GameObject::Color>(rand() % GameObject::Color::GREEN);

    return color;
}