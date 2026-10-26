#ifndef TESTGAME2_SAVE_H
#define TESTGAME2_SAVE_H
#include "game.h"

//A save is a folder of small text files, one keyword per line, so it stays readable and easy to extend
#define SAVE_DIR "saves/save1"
#define SAVE_VERSION 1

bool SaveExists(void);
bool SaveGame(const Game* game);
//Expects a game fresh from CreateGame, overwrites the player and the saved worlds' enemies
bool LoadGame(Game* game);

#endif //TESTGAME2_SAVE_H
