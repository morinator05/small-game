cmake_minimum_required(VERSION 4.1)
project(testgame2 C)

set(CMAKE_C_STANDARD 23)

add_executable(testgame2 main.c
        tilemap.h
        tilemap.c
        sprite.h
        sprite.c
        player.h
        player.c
        assets.h
        assets.c)

target_link_libraries(testgame2 raylib)

file(COPY asset DESTINATION ${CMAKE_BINARY_DIR})