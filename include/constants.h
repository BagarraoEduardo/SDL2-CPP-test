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
    static const int GAME_OBJECT_POOL_SIZE = 1;
    static const int GAME_OBJECT_POOL_LIMIT = 1;

    //game object
    static constexpr float GAME_OBJECT_MIN_SPEED = 3;
    static constexpr float GAME_OBJECT_MAX_SPEED = 6;

    static constexpr float GAME_OBJECT_GENERATION_MARGIN = 10;
};

#endif