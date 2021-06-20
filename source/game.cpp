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
    Pooler::GetInstance()->Init(GAME_OBJECT_POOL_SIZE, GAME_OBJECT_POOL_LIMIT);

    for(size_t i = 0; i < GAME_OBJECT_POOL_SIZE; i++)
    {
        if(Pooler::GetInstance()->HasNext())
        {
            GameObject * gameObjectPointerToStore = Pooler::GetInstance()->GetNext(GameObject::Color::RED);
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

void Game::HandleLogic()
{

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