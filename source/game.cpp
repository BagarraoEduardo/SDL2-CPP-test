#include <array>
#include <algorithm>
#include <string>
#include <random>
#include <filesystem>


#include "../include/game.h"
#include "../include/error.h"
#include "../include/constants.h"

using namespace std;

using std::string;
using std::filesystem::current_path;

/// <summary>
/// Constructor
/// </summary>
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

/// <summary>
/// Method that makes the game start and also
/// that makes the game run until we exit it.
/// 
/// Basically it inits all the variables and objects
/// that are needed and while the variable that
/// defines if the game is running it's ture, the game will 
/// loop and it will always call the update function
/// </summary>
void Game::Start()
{
    try{
        Init();

        this->isRunning = true;

        while(isRunning) 
        {    
            Update();
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

/// <summary>
/// I make sure that I delete everything here when the program finishes
/// </summary>
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

/// <summary>
/// Everything related with SDL and the pooling system is initialized here
/// </summary>
void Game::Init()
{
    //needs to be before because in the pooler  
    //there are called SDL_Logs
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw Error::SDL_INIT_ERROR;
    }

    //sets the pooler to have its starting size, however it can expand if needed until a defined limit
    Pooler::GetInstance()->Init(Constants::GAME_OBJECT_POOL_SIZE, Constants::GAME_OBJECT_POOL_LIMIT);

    for(size_t i = 0; i < Constants::GAME_OBJECT_POOL_SIZE; i++)
    {
        AddGameObjectAction(true); //Adds a gameobject to the vector that stores every gameobject that is being showed on the screen
    }


    //sdl stuff
    this->window = SDL_CreateWindow(
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

    this->windowSurface = SDL_GetWindowSurface(window);
    if(windowSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }

    if(TTF_Init())
    {
        throw Error::SDL_INIT_TTF_ERROR;
    }

    string folder = current_path().string();

    #ifdef WIN32
	    folder = +".\\resources\\fonts\\";
    #else
	    folder = +"./resources/fonts/";
    #endif

    this->titleFont = TTF_OpenFont((folder + "abyssinca.ttf").c_str(), Constants::TITLE_FONT_SIZE);
    if(titleFont == NULL)
    {
        throw Error::SDL_LOAD_FONT_ERROR;
    }

    this->messageFont = TTF_OpenFont((folder + "abyssinca.ttf").c_str(), Constants::MESSAGE_FONT_SIZE);
    if(messageFont == NULL)
    {
        throw Error::SDL_LOAD_FONT_ERROR;
    }

    this->creditsTitleFontSurface = SDL_GetWindowSurface(window);
    if(creditsTitleFontSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }

    this->creditsTitleFontRect.x = (Constants::TITLE_START_RELATIVE_X * Constants::SCREEN_WIDTH) - ( creditsTitleFontSurface->w / 2);
    this->creditsTitleFontRect.y = (Constants::TITLE_START_RELATIVE_Y * Constants::SCREEN_HEIGHT) - ( creditsTitleFontSurface->h / 2);

    this->gameStateFontSurface = SDL_GetWindowSurface(window);
    if(gameStateFontSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }

    this->gameStateFontRect.x = (Constants::TITLE_START_RELATIVE_X * Constants::SCREEN_WIDTH) - ( gameStateFontSurface->w / 2);
    this->gameStateFontRect.y = creditsTitleFontRect.y + Constants::TITLE_START_MARGIN;

    this->gameObjectsQuantityFontSurface = SDL_GetWindowSurface(window);
    if(gameObjectsQuantityFontSurface == NULL)
    {
        throw Error::SDL_WINDOW_SURFACE_ERROR;
    }

    this->gameObjectQuantityFontRect.x = (Constants::TITLE_START_RELATIVE_X * Constants::SCREEN_WIDTH) - ( gameObjectsQuantityFontSurface->w / 2);
    this->gameObjectQuantityFontRect.y = gameStateFontRect.y + Constants::TITLE_START_MARGIN;


    this->outOfCreditsFontSurface = SDL_GetWindowSurface(window);
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

/// <summary>
/// Method that handles all the events 
/// </summary>
void Game::HandleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        this->keyActionToTake = Action::NONE;

        if (event.type == SDL_QUIT)
        {
            this->isRunning = false; //this will make the game exit
        }
        else  if (event.type == SDL_KEYDOWN)
        {
            //this will set the action that will be handled in the HandleLogic method. Only 
            // variables are set here, none of the logic is implemented here

            switch (event.key.keysym.sym)
            {
            case SDLK_1:
                this->keyActionToTake = Action::INSERT_CREDIT;
                break;
            case SDLK_2:
                this->keyActionToTake = Action::REMOVE_ALL_CREDITS;
                break;
            case SDLK_z:
                this->keyActionToTake = Action::PLAY_GAME;
                break;
            case SDLK_x:
                this->keyActionToTake = Action::STOP_GAME;
                break;
            case SDLK_DOWN:
                this->keyActionToTake = Action::RETURN_GAME_OBJECT;
                break;
            case SDLK_UP:
                this->keyActionToTake = Action::ADD_GAME_OBJECT;
                break;
            default:
                this->keyActionToTake = Action::NONE;
                break;
            }
        }
    }
}

/// <summary>
/// Methos that handles all the logic that happens in the game
/// </summary>
/// <param name="deltaTime">the gameobjects move based on a deltatime that is defined at the start of each frame</param>
void Game::HandleLogic(float deltaTime)
{
    //now the actions will be handled
    //it will ignore if the last action was the same
    //to avoid repeated clicks. It's better to see the things happen here
    if(keyActionToTake != lastActionToken)
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

    this->lastActionToken = keyActionToTake;
    
    if(isPlaying)
    {
        //disables the message that shows that you're out of credits
        this->outOfCreditsMessageTime = Constants::MESSAGE_OUT_OF_CREDITS_DISABLED;

        //this will make every gameObject that was active in the game 
        //update their stats
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
            //this manages how much time the message that shows you're out of credits lasts on screen
            //the code only goes here when the message is showing

            float timeToSubtract = ((SDL_GetTicks() - frameEndTicks) / 1000.0);
            this->outOfCreditsMessageTime -= timeToSubtract;
        }
    }
}

/// <summary>
/// Method that renders all into the screen.
/// It runs once every frame
/// </summary>
void Game::HandleRendering()
{
    SDL_Color white = { 255, 255, 255 }; //color to draw the fonts

    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0)); //simple black background

    //this is the code to render the balls, it will only run while playing
    //as the balls are not intended be rendered when not playing
    if(this->isPlaying)
    {
        int activeGameObjectsNumber = 0; //counter to display in the screen the number of active gameobjects

        for(size_t i = 0; i < gameObjectPointerVector.size(); i++)
        {
            GameObject * gameObjectPointerToRender = gameObjectPointerVector[i];

            if(gameObjectPointerToRender->IsActive()) //for safety will only draw an active gameobject
            {
                SDL_Surface * surfaceToRender = gameObjectPointerToRender->GetSurface();
                
                SDL_Rect* surfaceRect = gameObjectPointerToRender->GetPosition(); 
                SDL_Rect parameterRect = SDL_Rect(*surfaceRect);
                
                SDL_BlitSurface(surfaceToRender, NULL, windowSurface, &parameterRect);

                ++activeGameObjectsNumber; //increases the counter of active gameobjects
            }
        }

        //this will print the number of active gameobjects. when not playing this isn't showed
        string gameObjectQuantityMessage = Constants::TITLE_GAME_OBJECTS_QUANTITY + to_string(activeGameObjectsNumber);
        this->gameObjectsQuantityFontSurface = TTF_RenderText_Solid(titleFont, gameObjectQuantityMessage.c_str() , white);
        SDL_BlitSurface(gameObjectsQuantityFontSurface, NULL, windowSurface, &gameObjectQuantityFontRect);
    }
    else
    {
        if (outOfCreditsMessageTime > 0)
        {
            //if the time to show the message that says that you're out 
            //of credits is bigger than zero then it will be showed
            this->outOfCreditsFontSurface = TTF_RenderText_Solid(messageFont, Constants::MESSAGE_OUT_OF_CREDITS.c_str(), white);
            SDL_BlitSurface(outOfCreditsFontSurface, NULL, windowSurface, &outOfCreditsFontRect);
        }
    }

    string creditsTitleMessage = Constants::TITLE_CREDITS + to_string(credits);
    string gameStateMessage = (isPlaying) ? Constants::TITLE_PLAYING : Constants::TITLE_STOPPED;
    
    creditsTitleFontSurface = TTF_RenderText_Solid(titleFont, creditsTitleMessage.c_str() , white);
    gameStateFontSurface = TTF_RenderText_Solid(titleFont, gameStateMessage.c_str() , white);
    
    SDL_BlitSurface(creditsTitleFontSurface, NULL, windowSurface, &creditsTitleFontRect); //the title that shows the number of credits
    SDL_BlitSurface(gameStateFontSurface, NULL, windowSurface, &gameStateFontRect); //this shows the current state of the game

    SDL_UpdateWindowSurface(window);
}

/// <summary>
/// The method were delta time is generated and where all the Handler methods are called
/// </summary>
void Game::Update()
{
    float deltaTime = GenerateDeltaTime();

    this->frameTicks = SDL_GetTicks();

    this->HandleEvents();
    this->HandleLogic(deltaTime);
    this->HandleRendering();

    this->frameEndTicks = SDL_GetTicks();
}

/// <summary>
/// Method that generates the deltatime to apply on the gameobjects movement speed
/// </summary>
/// <returns>deltatime</returns>
float Game::GenerateDeltaTime()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), frameTicks + static_cast<Uint32>(16.666f)))
    {
        continue;
    }

    float deltaTime = static_cast<float>((SDL_GetTicks() - frameTicks)) / 1000.0;

    //as SDL is pixel based and I'm using float variables to calculate deltaTime, 
    // I've tried to not clamp it too small, as I was risking that the value
    //multiplied by the objects speed was less than zero, and rounding the value
    // to the gameobject rect wouldn't move it from the previous position
    if (deltaTime < 0.5)
    {
        deltaTime = 0.5;
    }

    return deltaTime;
}

/// <summary>
/// This generates a random position to place the ball in the game
/// </summary>
/// <returns>randomPosition</returns>
Vector2 Game::GenerateRandomPosition()
{
    float minWidth, maxWidth, minHeight, maxHeight;

    minWidth = maxHeight = Constants::GAME_OBJECT_GENERATION_MARGIN;

    maxWidth = Constants::SCREEN_WIDTH - Constants::GAME_OBJECT_GENERATION_MARGIN;
    minHeight = Constants::SCREEN_HEIGHT - Constants::GAME_OBJECT_GENERATION_MARGIN;

    float randomWidth = GenerateRandomNumber(minWidth, maxWidth);
    float randomHeight = GenerateRandomNumber(maxHeight, minHeight);

    return Vector2(randomWidth, randomHeight);
}

/// <summary>
/// This generates a random movement Vector to set on a gameobject
/// This is currently set to be either or only horizontal or vertical movement
/// </summary>
/// <returns>randomMovement</returns>
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

    int chosenIndex = GenerateRandomNumber(possibleMovementsArray.size() - 1);

    return possibleMovementsArray[chosenIndex];
}

/// <summary>
/// Method that generates a random color to be set on a gameobject
/// </summary>
/// <returns>gameObject</returns>
GameObject::Color Game::GenerateRandomColor()
{
    int chosenIndex = GenerateRandomNumber(GameObject::Color::GREEN);

    GameObject::Color color = static_cast<GameObject::Color>(chosenIndex);

    return color;
}

/// <summary>
/// This is used to validate the arguments to use on the ransom generator methods
/// because in SDL the higher it is, the lower the Y value is, so when where in that case
/// the value between the minimum and maximum are swapped
/// </summary>
/// <typeparam name="Number">Number</typeparam>
/// <param name="minimum">minimum</param>
/// <param name="maximum">maximum</param>
template<typename Number>
void Game::validateDistribution(Number &minimum, Number &maximum) 
{
    if (minimum > maximum) 
    {
        Number temporaryNumber = minimum;
        minimum = maximum;
        maximum = temporaryNumber;
    }
}

/// <summary>
/// Generates a random number
/// </summary>
/// <param name="maximum">maximum</param>
/// <param name="minimum">minimum</param>
/// <returns>ransomNumber</returns>
int Game::GenerateRandomNumber(int maximum, int minimum /* = 0 */) 
{
    validateDistribution<int>(minimum, maximum);

    if (minimum > maximum) 
    {
        auto temp = maximum;
        maximum = minimum;
        minimum = maximum;
    }

    uniform_int_distribution<int> distribution(minimum, maximum); 

    int number_generated = distribution(this->mersenneTwisterPseudoRandomGenerator);

    return number_generated;
}

/// <summary>
/// Generates a random number
/// </summary>
/// <param name="maximum">maximum</param>
/// <param name="minimum">minimum</param>
/// <returns>ransomNumber</returns>
float Game::GenerateRandomNumber(float maximum, float minimum /* = 0.0 */) 
{
    validateDistribution<float>(minimum, maximum);

    uniform_real_distribution<float> distribution(minimum, maximum); 

    int number_generated = distribution(this->mersenneTwisterPseudoRandomGenerator);

    return number_generated;
}

/// <summary>
/// Generates a random number
/// </summary>
/// <param name="maximum">maximum</param>
/// <param name="minimum">minimum</param>
/// <returns>ransomNumber</returns>
size_t Game::GenerateRandomNumber(size_t maximum, size_t minimum /* = 0 */) 
{

    validateDistribution<size_t>(minimum, maximum);

    uniform_int_distribution<size_t> distribution(minimum, maximum); 

    size_t number_generated = distribution(this->mersenneTwisterPseudoRandomGenerator);
    
    return number_generated;
}

/// <summary>
/// Action that is called to release a gameobject to the pooling system
/// </summary>
void Game::ReturnGameObjectAction()
{
    if(this->isPlaying)
    {
        //it only where when there are active gameobject in the screen
        if(gameObjectPointerVector.size() > 0)
        {
            int chosenIndexToReturn; 
            int numberOfTries = 0;
            bool didTimeout = false;

            GameObject * gameObjectToBeReturnedPointer = nullptr;

            do
            {
                //it will choose a gameobject that doesn't point to null from the ones that are in the screen

                int chosenIndexToReturn = GenerateRandomNumber(gameObjectPointerVector.size() - 1);

                if(++numberOfTries > Constants::GENERIC_LOOP_TIMEOUT)
                {
                    didTimeout = true; //to make sure an infinite loop is avoided here
                    break;
                }

                gameObjectToBeReturnedPointer = gameObjectPointerVector[chosenIndexToReturn];

            } while(gameObjectToBeReturnedPointer == nullptr);
        
            //just to make sure that I avout an infinite loop
            if(didTimeout)
            {
                SDL_Log("Timeout reached while searching for an available gameobject to avoid the possibility of having infinite loops.");
            }
            else
            {
                //it will call the method to release the gameobject and will remove it from 
                //the vector that stores all the active gameobjects in the game

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

/// <summary>
/// Action to add the gameobject. It's used on players action but also at the game start
/// </summary>
/// <param name="isInitializing">isInitializing</param>
void Game::AddGameObjectAction(bool isInitializing)
{
    if(isInitializing || isPlaying)
    {
        //it will not return nothing when reaching the limit
        if (!Pooler::GetInstance()->HasNext())
        {
            SDL_Log("The pooling system is being used to its full capacity at this moment.");
        }
        else
        {
            //it will retrieve the next available gameobject and it will set it with the random variables

            GameObject::Color randomColor = GenerateRandomColor();

            GameObject *gameObjectPointerToStore = Pooler::GetInstance()->GetNext(randomColor);

            Vector2 randomPosition = GenerateRandomPosition();
            Vector2 randomMovement = GenerateRandomMovement();

            float randomSpeed = GenerateRandomNumber(Constants::GAME_OBJECT_MAX_SPEED, Constants::GAME_OBJECT_MIN_SPEED);

            //gameObjectPointerToStore->SetColor(randomColor);
            gameObjectPointerToStore->SetPosition(randomPosition);
            gameObjectPointerToStore->SetMovement(randomMovement);
            gameObjectPointerToStore->SetSpeed(randomSpeed);

            //it will make the object ready to be rendered on the screen
            if (!gameObjectPointerToStore->IsActive())
            {
                gameObjectPointerToStore->SetActive(true);
            }
            this->gameObjectPointerVector.push_back(gameObjectPointerToStore);
        }
    }
}

/// <summary>
/// Action to start a new game
/// </summary>
void Game::PlayAction()
{
    if(isPlaying)
    {
        SDL_Log("The game has already started.");
    }
    else
    {
        //it only will start in case you have enough credits to play

        if(credits > 0)
        {
            --credits;
            
            //unfreeze the gameobjects
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

/// <summary>
/// Action that will stop the current game
/// </summary>
void Game::StopAction()
{
    //returns all the gameobjects to the pool that are above the normal pool size
    while(gameObjectPointerVector.size() > Constants::GAME_OBJECT_POOL_SIZE)
    {
        ReturnGameObjectAction();
    }

    if(this->isPlaying)
    {
        //the other ones will be all freezed
         Pooler::GetInstance()->Freezed(true, gameObjectPointerVector);
         SDL_Log("The game stopped.");
    }
    else
    {
         SDL_Log("The game has already stopped.");
    }
    this->isPlaying = false;
}

/// <summary>
/// Action to insert credits
/// </summary>
void Game::InsertCreditAction()
{
    ++this->credits;

    string message = "Credit successfully inserted! Current Credits: " + to_string(credits);

    SDL_Log("%s", message.c_str());
}

/// <summary>
/// Action to remove all credits
/// </summary>
void Game::RemoveAllCreditsAction()
{
    this->credits = 0;

    SDL_Log("All the credits were removed. Please don't forget to grab them all.");
}