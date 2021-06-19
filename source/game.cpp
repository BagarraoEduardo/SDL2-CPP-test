#include "../include/game.h"
#include "../include/error.h"

Game::Game()
{
    this->isRunning = false;

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
            HandleEvents();

            HandleLogic();

            HandleRendering();
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
        }
    }
}

void Game::Quit()
{
    if(windowSurface != nullptr)
    {
        SDL_FreeSurface(windowSurface);
    }

    if(windowSurface != nullptr)
    {
        SDL_DestroyWindow(window);
    }
    
    SDL_Quit();
}

void Game::Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw Error::SDL_INIT_ERROR;
    }
    
    window = SDL_CreateWindow(
        "A Lot of Balls!",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
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
        if (event.type == SDL_KEYDOWN)
        {

        }
        else if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
    }
}

void Game::HandleLogic(){}

void Game::HandleRendering(){}