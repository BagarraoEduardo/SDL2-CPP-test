#ifndef Action_HEADER
#define Action_HEADER

/// <summary>
/// Different input actions
/// </summary>
enum class Action
{
    NONE = 0,
    INSERT_CREDIT = 1,
    REMOVE_ALL_CREDITS = 2,
    PLAY_GAME = 3,
    STOP_GAME = 4,
    RETURN_GAME_OBJECT = 5,
    ADD_GAME_OBJECT = 6
};

#endif