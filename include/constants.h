#ifndef Constants_HEADER
#define Constants_HEADER

class Constants
{
public:
    //generic
    static const int GENERIC_LOOP_TIMEOUT = 30;

    static const int INITIAL_CREDITS_NUMBER = 3;

    //font
    static const int TITLE_FONT_SIZE = 30;
    static const int MESSAGE_FONT_SIZE = 20;

    //titles
    inline static const string TITLE_CREDITS = "Credits: ";
    inline static const string TITLE_STOPPED = "[ Stopped ]";
    inline static const string TITLE_PLAYING = "[ Playing ]";

    //text position
    static constexpr float TITLE_START_RELATIVE_X = 0.5;
    static constexpr float TITLE_START_RELATIVE_Y = 0.5;
    static constexpr float TITLE_START_MARGIN = 100;

    //messages
    const string MESSAGE_NO_CREDITS = "No Credits available";

    //screen
    static const int SCREEN_WIDTH = 960;
    static const int SCREEN_HEIGHT = 540;

    //pool
    static const int GAME_OBJECT_POOL_SIZE = 2;
    static const int GAME_OBJECT_POOL_LIMIT = 5;

    //game object
    static constexpr float GAME_OBJECT_MIN_SPEED = 1;
    static constexpr float GAME_OBJECT_MAX_SPEED = 10;

    static constexpr float GAME_OBJECT_GENERATION_MARGIN = 20;
};

#endif