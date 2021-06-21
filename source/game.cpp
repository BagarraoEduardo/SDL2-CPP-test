#include <array>
#include <algorithm>

#include "../include/game.h"
#include "../include/error.h"
#include "../include/constants.h"

using namespace std;

Game::Game()
{
    this->isRunning = false;
    this->isPlaying = false;
    this->credits = Constants::INITIAL_CREDITS_NUMBER;

    this->frameTicks = 0;
    this->frameEndTicks = 0;

    this->window = nullptr;
    this->windowSurface = nullptr;

    this->creditsTitleFontSurface = nullptr;
    this->gameStateFontSurface = nullptr;
    this->gameObjectsQuantityFontSurface = nullptr;
    this->outOfCreditsFontSurface = nullptr;

    this->titleFont = nullptr;
    this->messageFont = nullptr;

    this->outOfCreditsMessageTime = Constants::MESSAGE_OUT_OF_CREDITS_DISABLED;

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
                SDL_Log("Could not load .bmp image: %s\n", SDL_GetError());
                break;
            case Error::SDL_INIT_TTF_ERROR:
                SDL_Log("Could not load .ttf library: %s\n", SDL_GetError());
                break;
            case Error::SDL_LOAD_FONT_ERROR:
                SDL_Log("Could not load .ttf font: %s\n", SDL_GetError());
                break;
            default:
                cout << "Generic Error Appeared" << endl;
        }
    }
}

Game::~Game()
{
    if(titleFont != NULL)
    {
        TTF_CloseFont(titleFont);
    }

    if(messageFont != NULL)
    {
        TTF_CloseFont(messageFont);
    }

    if(windowSurface != NULL)
    {
        SDL_FreeSurface(windowSurface);
    }

    if(creditsTitleFontSurface != NULL)
    {
        SDL_FreeSurface(creditsTitleFontSurface);
    }

    if(gameStateFontSurface != NULL)
    {
        SDL_FreeSurface(gameStateFontSurface);
    }

    if(gameObjectsQuantityFontSurface != NULL)
    {
        SDL_FreeSurface(gameObjectsQuantityFontSurface);
    }

    if(outOfCreditsFontSurface != NULL)
    {
        SDL_FreeSurface(outOfCreditsFontSurface);
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

    if(TTF_Init())
    {
        throw Error::SDL_INIT_TTF_ERROR;
    }

    titleFont = TTF_OpenFont("./../resources/fonts/abyssinca.ttf", Constants::TITLE_FONT_SIZE);
    if(titleFont == NULL)
    {
        throw Error::SDL_LOAD_FONT_ERROR;
    }

    messageFont = TTF_OpenFont("./../resources/fonts/abyssinca.ttf", Constants::MESSAGE_FONT_SIZE);
    if(messageFont == NULL)
    {
        throw Error::SDL_LOAD_FONT_ERROR;
    }

    creditsTitleFontSurface = SDL_GetWindowSurface(window);
    if(creditsTitleFontSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }

    this->creditsTitleFontRect.x = (Constants::TITLE_START_RELATIVE_X * Constants::SCREEN_WIDTH) - ( creditsTitleFontSurface->w / 2);
    this->creditsTitleFontRect.y = (Constants::TITLE_START_RELATIVE_Y * Constants::SCREEN_HEIGHT) - ( creditsTitleFontSurface->h / 2);

    gameStateFontSurface = SDL_GetWindowSurface(window);
    if(gameStateFontSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }

    this->gameStateFontRect.x = (Constants::TITLE_START_RELATIVE_X * Constants::SCREEN_WIDTH) - ( gameStateFontSurface->w / 2);
    this->gameStateFontRect.y = creditsTitleFontRect.y + Constants::TITLE_START_MARGIN;

    gameObjectsQuantityFontSurface = SDL_GetWindowSurface(window);
    if(gameObjectsQuantityFontSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }

    this->gameObjectQuantityFontRect.x = (Constants::TITLE_START_RELATIVE_X * Constants::SCREEN_WIDTH) - ( gameObjectsQuantityFontSurface->w / 2);
    this->gameObjectQuantityFontRect.y = gameStateFontRect.y + Constants::TITLE_START_MARGIN;


    outOfCreditsFontSurface = SDL_GetWindowSurface(window);
    if(outOfCreditsFontSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }


    int fontWidth = 0, fontHeight = 0;

    TTF_SizeText(messageFont, Constants::MESSAGE_OUT_OF_CREDITS.c_str(), &fontWidth, &fontHeight);

    this->outOfCreditsFontRect.w = fontWidth;
    this->outOfCreditsFontRect.h = fontHeight;

    this->outOfCreditsFontRect.x = (Constants::SCREEN_WIDTH / 2) - (outOfCreditsFontRect.w / 2);
    this->outOfCreditsFontRect.y = (Constants::SCREEN_HEIGHT / 2) - (outOfCreditsFontRect.h / 2);
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
                InsertCreditAction();
                break;
            case Action::REMOVE_ALL_CREDITS:
                RemoveAllCreditsAction();
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
        outOfCreditsMessageTime = Constants::MESSAGE_OUT_OF_CREDITS_DISABLED;

        for (size_t i = 0; i < gameObjectPointerVector.size(); i++)
        {
            GameObject *pointedGameObject = gameObjectPointerVector[i];

            pointedGameObject->Update(deltaTime);
        }
    }
    else
    {
        if(outOfCreditsMessageTime > 0)
        {
            float timeToSubtract = ((SDL_GetTicks() - frameEndTicks) / 1000.0);

            SDL_Log("%s", to_string(timeToSubtract).c_str());

            this->outOfCreditsMessageTime -= timeToSubtract;
        }
    }
}

void Game::HandleRendering()
{
    SDL_Color white = { 255, 255, 255 };

    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0));

    if(this->isPlaying)
    {
        int activeGameObjectsNumber = 0;

        for(size_t i = 0; i < gameObjectPointerVector.size(); i++)
        {
            GameObject * gameObjectPointerToRender = gameObjectPointerVector[i];

            if(gameObjectPointerToRender->IsActive())
            {
                SDL_Surface * surfaceToRender = gameObjectPointerToRender->GetSurface();
                
                SDL_Rect* surfaceRect = gameObjectPointerToRender->GetPosition(); 
                SDL_Rect parameterRect = SDL_Rect(*surfaceRect);
                
                SDL_BlitSurface(surfaceToRender, NULL, windowSurface, &parameterRect);

                ++activeGameObjectsNumber;
            }
        }

        string gameObjectQuantityMessage = Constants::TITLE_GAME_OBJECTS_QUANTITY + to_string(activeGameObjectsNumber);
        gameObjectsQuantityFontSurface = TTF_RenderText_Solid(titleFont, gameObjectQuantityMessage.c_str() , white);
        SDL_BlitSurface(gameObjectsQuantityFontSurface, NULL, windowSurface, &gameObjectQuantityFontRect);
    }
    else
    {
        if (outOfCreditsMessageTime > 0)
        {
            outOfCreditsFontSurface = TTF_RenderText_Solid(messageFont, Constants::MESSAGE_OUT_OF_CREDITS.c_str(), white);
            SDL_BlitSurface(outOfCreditsFontSurface, NULL, windowSurface, &outOfCreditsFontRect);
        }
    }

    string creditsTitleMessage = Constants::TITLE_CREDITS + to_string(credits);
    string gameStateMessage = (isPlaying) ? Constants::TITLE_PLAYING : Constants::TITLE_STOPPED;
    
    creditsTitleFontSurface = TTF_RenderText_Solid(titleFont, creditsTitleMessage.c_str() , white);
    gameStateFontSurface = TTF_RenderText_Solid(titleFont, gameStateMessage.c_str() , white);
    
    SDL_BlitSurface(creditsTitleFontSurface, NULL, windowSurface, &creditsTitleFontRect);
    SDL_BlitSurface(gameStateFontSurface, NULL, windowSurface, &gameStateFontRect);

    SDL_UpdateWindowSurface(window);
}

void Game::Update()
{
    float deltaTime = GenerateDeltaTime();

    this->frameTicks = SDL_GetTicks();

    this->HandleEvents();
    this->HandleLogic(deltaTime);
    this->HandleRendering();

    this->frameEndTicks = SDL_GetTicks();
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
    if(isPlaying)
    {
        SDL_Log("The game has already started.");
    }
    else
    {
        if(credits > 0)
        {
            --credits;
            
            Pooler::GetInstance()->Freezed(false, gameObjectPointerVector);
            this->isPlaying = true;

            string message = "The game has started. Credits Left: " + to_string(credits);
            SDL_Log("%s", message.c_str());
        }
        else
        {
            SDL_Log("You're ran out of credits. Please insert more if you want to play.");
        
            outOfCreditsMessageTime = Constants::MESSAGE_OUT_OF_CREDITS_TIME;
        }
    }
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

void Game::InsertCreditAction()
{
    ++this->credits;

    string message = "Credit successfully inserted! Current Credits: " + to_string(credits);

    SDL_Log("%s", message.c_str());
}

void Game::RemoveAllCreditsAction()
{
    this->credits = 0;

    SDL_Log("All the credits were removed. Please don't forget to grab them all.");
}