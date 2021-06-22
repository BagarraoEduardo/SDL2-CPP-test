#ifndef Constants_HEADER
#define Constants_HEADER

/// <summary>
/// Class where the project constants are stored
/// </summary>
class Constants
{
public:
    //generic
    static const int GENERIC_LOOP_TIMEOUT = 30;

    static const int INITIAL_CREDITS_NUMBER = 3;

    //font
    static const int TITLE_FONT_SIZE = 24;
    static const int MESSAGE_FONT_SIZE = 18;

    //titles
    inline static const string TITLE_CREDITS = "Credits: ";
    inline static const string TITLE_STOPPED = "Stopped";
    inline static const string TITLE_PLAYING = "Playing";
    inline static const string TITLE_GAME_OBJECTS_QUANTITY = "Active GameObjects: ";

    //messages
    inline static const string MESSAGE_OUT_OF_CREDITS = "Insert credits.";
    
    //out of credits message properties
    static const int MESSAGE_OUT_OF_CREDITS_TIME = 3;
    static const int MESSAGE_OUT_OF_CREDITS_DISABLED = -1;

    //text position
    static constexpr float TITLE_START_RELATIVE_X = 0.5;
    static constexpr float TITLE_START_RELATIVE_Y = 0.5;
    static constexpr float TITLE_START_MARGIN = 25;

    //messages
    const string MESSAGE_NO_CREDITS = "No Credits available";

    //screen
    static const int SCREEN_WIDTH = 1000;
    static const int SCREEN_HEIGHT = 1000;

    //pool
    static const int GAME_OBJECT_POOL_SIZE = 20;
    static const int GAME_OBJECT_POOL_LIMIT = 1000;

    //game object
    static constexpr float GAME_OBJECT_MIN_SPEED = 1;
    static constexpr float GAME_OBJECT_MAX_SPEED = 10;

    static constexpr float GAME_OBJECT_GENERATION_MARGIN = 20;
};

#endif