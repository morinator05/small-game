#include "../include/player.h"

#include "../include/tilemap.h"
#include "raymath.h"
#include "../include/assets.h"
#include "../include/enemy.h"
#include "../include/world.h"

void DrawPlayer(Player* player)
{
    DrawSprite(&player->sprite, player->position);
    DrawText(TextFormat("(%d,%f)", player->hp, player->weapon.cooldown), player->position.x + TILE_SIZE,
             player->position.y + TILE_SIZE, 5, WHITE);
}

void CreatePlayer(Player* player)
{
    player->sprite.texture = assets.spritesheet_player;
    player->sprite.origin = (Vector2){0, 0};
    player->sprite.rotation = 0;
    player->velocity = PLAYER_DEFAULT_VELOCITY;
    player->sprite.frames_per_line = ANIM_FRAMES_PER_LINE;
    player->sprite.anim = (Animation){
        .first = ANIM_FRAME_FIRST,
        .last = ANIM_FRAME_LAST,
        .speed = ANIM_SPEED,
    };
    player->hp = PLAYER_DEFAULT_HP;
}

static TileCodes TileBeneathPlayer(const Player* player, const TileMap* tilemap)
{
    Vector2 pos = player->position;
    int x = (pos.x + TILE_SIZE / 2) / TILE_SIZE;
    int y = pos.y / TILE_SIZE + 1;
    if (x < 0 || x >= tilemap->width || y < 0 || y >= tilemap->height)
    {
        return 0;
    }
    return tilemap->tiles[x + y * tilemap->width];
}

static void PlayWalkingSound(const Player* player, const TileMap* tilemap)
{
    if (!IsSoundPlaying(assets.player_walk_gras) & !IsSoundPlaying(assets.player_walk_dirt))
    {
        switch (TileBeneathPlayer(player, tilemap))
        {
        case TILE_GRASS: PlaySound(assets.player_walk_gras);
            break;
        case TILE_DIRT: PlaySound(assets.player_walk_dirt);
            break;
        }
    }
}

void MovePlayer(Player* player, const TileMap* tilemap)
{
    //Calculate speed
    float distance = player->velocity * GetFrameTime();
    distance = IsKeyDown(KEY_LEFT_SHIFT) ? PLAYER_RUNNING_MULTIPLIER * distance : distance;

    Vector2 new_position = player->position;
    Vector2 move_vector = {0, 0};
    Directions current_direction = IDLE;

    //Get the input
    if (IsKeyDown(KEY_W))
    {
        move_vector.y -= distance;
        current_direction = UP;
    }
    if (IsKeyDown(KEY_S))
    {
        move_vector.y += distance;
        current_direction = DOWN;
    }
    if (IsKeyDown(KEY_A))
    {
        move_vector.x -= distance;
        current_direction = LEFT;
    }
    if (IsKeyDown(KEY_D))
    {
        move_vector.x += distance;
        current_direction = RIGHT;
    }

    //Normalize and scale the vector for propper speed when moving diagonally
    if (current_direction != IDLE)
    {
        move_vector = Vector2Normalize(move_vector);
        move_vector = Vector2Scale(move_vector, distance);
    }
    float length = Vector2Length(move_vector);

    if (length > 0.0f)
    {
        //Check if the x part of the Movement is allowed
        new_position.x += move_vector.x;
        if (!CheckCollisionWithMap(tilemap, (Rectangle){new_position.x, new_position.y,TILE_SIZE,TILE_SIZE}))
        {
            player->position.x += move_vector.x;
            PlayWalkingSound(player, tilemap);
        }
        new_position.x = player->position.x;

        //Check if the y part of the Movement is allowed
        new_position.y += move_vector.y;
        if (!CheckCollisionWithMap(tilemap, (Rectangle){new_position.x, new_position.y,TILE_SIZE,TILE_SIZE}))
        {
            player->position.y += move_vector.y;
            PlayWalkingSound(player, tilemap);
        }
    }
    else
    {
        current_direction = IDLE;
    }

    switch (current_direction)
    {
    case UP: SetRow(&player->sprite.anim, PLAYER_TEXTURE_ROW_UP, player->sprite.frames_per_line);
        break;
    case DOWN: SetRow(&player->sprite.anim, PLAYER_TEXTURE_ROW_DOWN, player->sprite.frames_per_line);
        break;
    case LEFT: SetRow(&player->sprite.anim, PLAYER_TEXTURE_ROW_LEFT, player->sprite.frames_per_line);
        break;
    case RIGHT: SetRow(&player->sprite.anim, PLAYER_TEXTURE_ROW_RIGHT, player->sprite.frames_per_line);
        break;
    case IDLE: SetRow(&player->sprite.anim, PLAYER_TEXTURE_ROW_IDLE, player->sprite.frames_per_line);
        break;
    }

    if (player->current_direction != current_direction)
    {
        Reset(&player->sprite.anim);
        player->current_direction = current_direction;
    }
}

static Vector2 DirectionToVector(int direction)
{
    switch (direction)
    {
    case UP: return (Vector2){0, -1};
    case DOWN: return (Vector2){0, 1};
    case LEFT: return (Vector2){-1, 0};
    case RIGHT: return (Vector2){1, 0};
    case IDLE: return (Vector2){0, 1};
    default: return (Vector2){0, 0};;
    }
}

void UpdatePlayer(Player* player, World* world)
{
    player->weapon.cooldown -= GetFrameTime();
    if (player->weapon.cooldown < 0) player->weapon.cooldown = 0;

    if (!IsKeyDown(KEY_SPACE) || player->weapon.cooldown != 0) return;

    for (int i = 0; i < world->enemy_count; i++)
    {
        Enemy* target = (world->enemies + i);
        float distance_to_target = Vector2Distance(player->position, target->position);

        if (distance_to_target <= player->weapon.reach)
        {
            Vector2 to_target = Vector2Normalize(Vector2Subtract(target->position, player->position));
            float dot_product = Vector2DotProduct(DirectionToVector(player->current_direction), to_target);

            if (dot_product >= 0.0f)
            {
                target->hp -= player->weapon.damage;
            }
        }
    }
    player->weapon.cooldown = player->weapon.hit_rate;
}

void AcquireWeapon(Player* player, WeaponTypes weapon_type)
{
    switch (weapon_type)
    {
    case UNARMED: player->weapon = (Weapon){
            .type = UNARMED,
            .damage = 10,
            .hit_rate = 1.f,
            .reach = 20
        };
        break;
    case SWORD: player->weapon = (Weapon){
            .type = SWORD,
            .damage = 50,
            .hit_rate = 1.5f,
            .reach = 40
        };
    }
}
