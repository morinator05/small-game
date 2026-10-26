#include "../include/save.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define SAVE_FILE SAVE_DIR "/save.txt"
#define PLAYER_FILE SAVE_DIR "/player.txt"
#define SAVE_WORLDS_DIR SAVE_DIR "/worlds"

#define PATH_LENGTH 256
#define LINE_LENGTH 256
#define KEY_LENGTH 32

/*
 * File helpers
 */

static void GetSavedWorldPath(const World* world, char* out, const size_t size)
{
    snprintf(out, size, SAVE_WORLDS_DIR "/%s.txt", world->name);
}

//Writes go to a .tmp file which replaces the real one in EndWrite, so a crash can't leave half a file behind
static FILE* BeginWrite(const char* path)
{
    char tmp[PATH_LENGTH];
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);

    FILE* file = fopen(tmp, "w");
    if (file == NULL)
    {
        printf("Can't open save file for writing: %s: %s\n", tmp, strerror(errno));
        return NULL;
    }
    fprintf(file, "version %d\n", SAVE_VERSION);
    return file;
}

static bool EndWrite(FILE* file, const char* path)
{
    char tmp[PATH_LENGTH];
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);

    const bool write_failed = ferror(file);
    if (fclose(file) != 0 || write_failed)
    {
        printf("Error writing save file: %s\n", tmp);
        remove(tmp);
        return false;
    }
    if (rename(tmp, path) != 0)
    {
        printf("Error replacing save file: %s: %s\n", path, strerror(errno));
        return false;
    }
    return true;
}

//Reads the next "keyword arguments" line, skipping empty ones. args points into line.
static bool ReadLine(FILE* file, char* line, char* key, const char** args)
{
    while (fgets(line, LINE_LENGTH, file) != NULL)
    {
        int key_end = 0;
        if (sscanf(line, "%31s%n", key, &key_end) != 1) continue;
        *args = line + key_end;
        return true;
    }
    return false;
}

//Files from a newer version of the game might mean something else, so don't guess
static bool IsSupportedVersion(const char* args, const char* path)
{
    int version;
    if (sscanf(args, "%d", &version) == 1 && version <= SAVE_VERSION) return true;

    printf("Unsupported save version in %s\n", path);
    return false;
}

static int FindWorldIndex(const Game* game, const char* name)
{
    for (int i = 0; i < MAX_WORLDS; i++)
    {
        if (game->worlds[i].name[0] != '\0' && strcmp(game->worlds[i].name, name) == 0) return i;
    }
    return -1;
}

/*
 * Saving
 */

static bool SaveGameFile(const Game* game)
{
    FILE* file = BeginWrite(SAVE_FILE);
    if (file == NULL) return false;

    fprintf(file, "world %s\n", game->worlds[game->current_world_idx].name);
    return EndWrite(file, SAVE_FILE);
}

static bool SavePlayer(const Player* player)
{
    FILE* file = BeginWrite(PLAYER_FILE);
    if (file == NULL) return false;

    fprintf(file, "position %.2f %.2f\n", player->position.x, player->position.y);
    fprintf(file, "hp %d\n", player->hp);
    fprintf(file, "weapon %d\n", player->weapon.type);
    return EndWrite(file, PLAYER_FILE);
}

//Dead enemies are left out, the file existing at all tells that the world was visited
static bool SaveWorld(const World* world)
{
    char path[PATH_LENGTH];
    GetSavedWorldPath(world, path, sizeof(path));

    FILE* file = BeginWrite(path);
    if (file == NULL) return false;

    for (int i = 0; i < world->enemy_count; i++)
    {
        const Enemy* enemy = &world->enemies[i];
        if (!enemy->alive) continue;
        fprintf(file, "enemy %d %.2f %.2f %d\n", enemy->type, enemy->position.x, enemy->position.y, enemy->hp);
    }
    return EndWrite(file, path);
}

bool SaveExists(void)
{
    return FileExists(SAVE_FILE);
}

bool SaveGame(const Game* game)
{
    //Creates the save folder too
    if (MakeDirectory(SAVE_WORLDS_DIR) != 0)
    {
        printf("Can't create save folder: %s\n", SAVE_WORLDS_DIR);
        return false;
    }

    bool ok = SavePlayer(&game->player);
    for (int i = 0; i < MAX_WORLDS; i++)
    {
        if (game->worlds[i].name[0] != '\0') ok = SaveWorld(&game->worlds[i]) && ok;
    }
    //Written last, a save only counts as existing once everything else is on disk
    ok = ok && SaveGameFile(game);

    printf(ok ? "Saved game to %s\n" : "Saving to %s failed\n", SAVE_DIR);
    return ok;
}

/*
 * Loading
 */

static bool LoadGameFile(Game* game)
{
    FILE* file = fopen(SAVE_FILE, "r");
    if (file == NULL)
    {
        printf("Can't open save file: %s: %s\n", SAVE_FILE, strerror(errno));
        return false;
    }

    bool ok = true;
    char line[LINE_LENGTH], key[KEY_LENGTH];
    const char* args;
    while (ok && ReadLine(file, line, key, &args))
    {
        if (strcmp(key, "version") == 0) ok = IsSupportedVersion(args, SAVE_FILE);
        else if (strcmp(key, "world") == 0)
        {
            char name[WORLD_NAME_LENGTH] = "";
            const int index = sscanf(args, "%31s", name) == 1 ? FindWorldIndex(game, name) : -1;
            if (index < 0)
            {
                printf("Saved world does not exist: %s\n", name);
                ok = false;
            }
            else game->current_world_idx = index;
        }
    }
    fclose(file);
    return ok;
}

//Missing lines keep the values from CreatePlayer
static bool LoadPlayer(Player* player)
{
    FILE* file = fopen(PLAYER_FILE, "r");
    if (file == NULL)
    {
        printf("Can't open save file: %s: %s\n", PLAYER_FILE, strerror(errno));
        return false;
    }

    bool ok = true;
    char line[LINE_LENGTH], key[KEY_LENGTH];
    const char* args;
    while (ok && ReadLine(file, line, key, &args))
    {
        if (strcmp(key, "version") == 0) ok = IsSupportedVersion(args, PLAYER_FILE);
        else if (strcmp(key, "position") == 0) sscanf(args, "%f %f", &player->position.x, &player->position.y);
        else if (strcmp(key, "hp") == 0) sscanf(args, "%d", &player->hp);
        else if (strcmp(key, "weapon") == 0)
        {
            int weapon;
            if (sscanf(args, "%d", &weapon) == 1 && weapon >= UNARMED && weapon <= BOW)
            {
                AcquireWeapon(player, (WeaponTypes)weapon);
            }
        }
    }
    fclose(file);
    return ok;
}

//A world without a file was never visited, it keeps what CreateWorld gave it
static bool LoadWorld(World* world)
{
    char path[PATH_LENGTH];
    GetSavedWorldPath(world, path, sizeof(path));
    if (!FileExists(path)) return true;

    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Can't open save file: %s: %s\n", path, strerror(errno));
        return false;
    }

    world->enemy_count = 0;

    bool ok = true;
    char line[LINE_LENGTH], key[KEY_LENGTH];
    const char* args;
    while (ok && ReadLine(file, line, key, &args))
    {
        if (strcmp(key, "version") == 0) ok = IsSupportedVersion(args, path);
        else if (strcmp(key, "enemy") == 0)
        {
            int type, hp;
            Vector2 position;
            if (sscanf(args, "%d %f %f %d", &type, &position.x, &position.y, &hp) != 4) continue;
            if (type < RED_SLIME || type > WIZ) continue;

            Enemy enemy;
            CreateEnemy(&enemy, (Type)type);
            enemy.position = position;
            enemy.hp = hp;
            AddEnemyToWorld(world, enemy);
        }
    }
    fclose(file);
    return ok;
}

bool LoadGame(Game* game)
{
    bool ok = LoadGameFile(game) && LoadPlayer(&game->player);
    for (int i = 0; ok && i < MAX_WORLDS; i++)
    {
        if (game->worlds[i].name[0] != '\0') ok = LoadWorld(&game->worlds[i]);
    }

    printf(ok ? "Loaded game from %s\n" : "Loading from %s failed\n", SAVE_DIR);
    return ok;
}
