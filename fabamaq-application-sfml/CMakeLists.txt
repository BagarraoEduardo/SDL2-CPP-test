# CMakeList.txt : CMake project for fabamaq-application-sfml, include source and define
# project specific logic here.
#
#set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required (VERSION 3.8)


# Add source to this project's executable.
add_executable (fabamaq-application-sfml "fabamaq-application-sfml.cpp" "fabamaq-application-sfml.h" "include/game_object.h" "include/game.h" "include/pooler.h"  "source/vector2.cpp"  "source/game_object.cpp" "source/pooler.cpp")


# TODO: Add tests and install targets if needed.
