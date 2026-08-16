cmake_minimum_required(VERSION 4.1)
project(testgame2 C)

set(CMAKE_C_STANDARD 23)

add_executable(testgame2 src/main.c
        include/tilemap.h
        src/tilemap.c
        include/sprite.h
        src/sprite.c
        include/player.h
        src/player.c
        include/assets.h
        src/assets.c
        src/levels.c
        include/levels.h
        include/animation.h
        src/animation.c
        include/enemy.h
        src/enemy.c
        include/world.h
        src/world.c)

target_link_libraries(testgame2 raylib m)

file(COPY asset DESTINATION ${CMAKE_BINARY_DIR})