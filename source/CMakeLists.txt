cmake_minimum_required(VERSION 3.0.0)
project(SDL2-CPP-test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable (SDL2-CPP-test "SDL2-CPP-test.cpp" "../include/SDL2-CPP-test.h" "../include/action.h" "../include/game_object.h" "../include/game.h" "../include/pooler.h" "../include/error.h" "../include/constants.h" "game.cpp" "vector2.cpp"  "game_object.cpp" "pooler.cpp")

#INCLUDE(FindPkgConfig)

#PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

target_link_libraries(SDL2-CPP-test SDL2_ttf SDL2 )
target_link_libraries(SDL2-CPP-test SDL2main SDL2 )
target_include_directories(SDL2-CPP-test PRIVATE ${YOUR_DIRECTORY})
