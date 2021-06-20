#ifndef Constants_HEADER
#define Constants_HEADER

// #include "vector2.h";

class Constants
{
public:
    //screen
    static const int SCREEN_WIDTH = 960;
    static const int SCREEN_HEIGHT = 540;

    //pool
    static const int GAME_OBJECT_POOL_SIZE = 500;
    static const int GAME_OBJECT_POOL_LIMIT = 1000;

    //game object
    static constexpr float GAME_OBJECT_MIN_SPEED = 1;
    static constexpr float GAME_OBJECT_MAX_SPEED = 10;

    static constexpr float GAME_OBJECT_GENERATION_MARGIN = 20;
};

#endif