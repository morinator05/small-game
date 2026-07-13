cmake_minimum_required(VERSION 4.1)
project(testgame2 C)

set(CMAKE_C_STANDARD 23)

add_executable(testgame2 src/main.c
        src/tilemap.h
        src/tilemap.c
        src/sprite.h
        src/sprite.c
        src/player.h
        src/player.c
        src/assets.h
        src/assets.c
        src/levels.c
        src/levels.h)

target_link_libraries(testgame2 raylib)

file(COPY asset DESTINATION ${CMAKE_BINARY_DIR})