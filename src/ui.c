#include "../include/ui.h"

#include <math.h>
#include <raymath.h>

#include "../include/assets.h"

//Washed out, warm palette
#define UI_OUTLINE      (Color){62, 39, 35, 255}
#define UI_SHADOW       (Color){62, 39, 35, 120}
#define UI_WOOD_DARK    (Color){133, 86, 62, 255}
#define UI_WOOD         (Color){178, 122, 84, 255}
#define UI_WOOD_LIGHT   (Color){212, 163, 115, 255}
#define UI_PAPER        (Color){246, 228, 190, 255}
#define UI_PAPER_DARK   (Color){228, 201, 157, 255}
#define UI_TEXT         (Color){92, 58, 44, 255}
#define UI_TEXT_LIGHT   (Color){250, 240, 215, 255}
#define UI_ACCENT       (Color){176, 96, 72, 255}
#define UI_GOLD         (Color){240, 204, 120, 255}
#define UI_GOLD_LIGHT   (Color){252, 230, 168, 255}
#define UI_GOLD_DARK    (Color){206, 160, 84, 255}
#define UI_HP           (Color){132, 184, 104, 255}
#define UI_HP_LIGHT     (Color){178, 216, 136, 255}
#define UI_HP_LOW       (Color){214, 104, 88, 255}
#define UI_HP_LOW_LIGHT (Color){236, 152, 128, 255}
#define UI_BAR_EMPTY    (Color){96, 70, 62, 255}
#define UI_HAZE         (Color){244, 228, 196, 70}
#define UI_DIM          (Color){46, 34, 44, 130}

#define LOW_HP_FRACTION .3f

//Pixel icons: '#' = outline, 'x' = fill, 'o' = highlight
static const char* const ICON_HEART[] = {
    ".##...##.",
    "#oo#.#xx#",
    "#oxx#xxx#",
    "#xxxxxxx#",
    ".#xxxxx#.",
    "..#xxx#..",
    "...#x#...",
    "....#....",
};

static const char* const ICON_ARROW[] = {
    "#....",
    "##...",
    "#o#..",
    "#xo#.",
    "#xxx#",
    "#xx#.",
    "#x#..",
    "##...",
    "#....",
};

//Size of one UI "pixel" in screen pixels. Everything is a multiple of this to keep the pixel art look.
static int Px(void)
{
    const int scale = GetScreenHeight() / 480;
    return scale < 1 ? 1 : scale;
}

//Default raylib font is 10px high, multiples of that stay crisp
static int FontSmall(void) { return 10 * Px(); }
static int FontBig(void) { return 20 * Px(); }

static void DrawNotchedRect(const int x, const int y, const int w, const int h, const int notch, const Color color)
{
    DrawRectangle(x + notch, y, w - 2 * notch, h, color);
    DrawRectangle(x, y + notch, w, h - 2 * notch, color);
}

static void DrawPixelIcon(const char* const* rows, const int row_count, const int x, const int y, const int scale,
                          const Color fill, const Color highlight)
{
    for (int row = 0; row < row_count; row++)
    {
        for (int col = 0; rows[row][col] != '\0'; col++)
        {
            Color color;
            switch (rows[row][col])
            {
            case '#': color = UI_OUTLINE;
                break;
            case 'x': color = fill;
                break;
            case 'o': color = highlight;
                break;
            default: continue;
            }
            DrawRectangle(x + col * scale, y + row * scale, scale, scale, color);
        }
    }
}

static void DrawTextShadowed(const char* text, const int x, const int y, const int size, const Color color)
{
    const int p = Px();
    DrawText(text, x + p, y + p, size, UI_SHADOW);
    DrawText(text, x, y, size, color);
}

static void DrawTextOutlined(const char* text, const int x, const int y, const int size, const Color color)
{
    const int p = Px();
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 2; dy++)
        {
            if (dx != 0 || dy != 0) DrawText(text, x + dx * p, y + dy * p, size, UI_OUTLINE);
        }
    }
    DrawText(text, x, y, size, color);
}

static void DrawTextCentered(const char* text, const int center_x, const int y, const int size, const Color color)
{
    DrawText(text, center_x - MeasureText(text, size) / 2, y, size, color);
}

//Wooden frame with a paper inside, the border is 4px thick
static void DrawPanel(const int x, const int y, const int w, const int h)
{
    const int p = Px();
    DrawNotchedRect(x, y, w, h, p, UI_OUTLINE);
    DrawNotchedRect(x + p, y + p, w - 2 * p, h - 2 * p, p, UI_WOOD);
    DrawRectangle(x + 2 * p, y + p, w - 4 * p, p, UI_WOOD_LIGHT);
    DrawRectangle(x + 2 * p, y + h - 2 * p, w - 4 * p, p, UI_WOOD_DARK);
    DrawNotchedRect(x + 3 * p, y + 3 * p, w - 6 * p, h - 6 * p, p, UI_WOOD_DARK);
    DrawNotchedRect(x + 4 * p, y + 4 * p, w - 8 * p, h - 8 * p, p, UI_PAPER);
    DrawRectangle(x + 5 * p, y + h - 5 * p, w - 10 * p, p, UI_PAPER_DARK);
}

static void DrawBar(const int x, const int y, const int w, const int h, float fraction, const Color fill,
                    const Color highlight)
{
    const int p = Px();
    fraction = Clamp(fraction, 0.f, 1.f);

    DrawNotchedRect(x, y, w, h, p, UI_OUTLINE);
    DrawRectangle(x + p, y + p, w - 2 * p, h - 2 * p, UI_BAR_EMPTY);

    const int fill_width = (int)((float)(w - 2 * p) * fraction);
    if (fill_width > 0)
    {
        DrawRectangle(x + p, y + p, fill_width, h - 2 * p, fill);
        DrawRectangle(x + p, y + p, fill_width, p, highlight);
    }
}

static void DrawDimOverlay(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), UI_DIM);
}

static const char* WeaponName(const WeaponTypes type)
{
    switch (type)
    {
    case SWORD: return "Sword";
    case BOW: return "Bow";
    default: return "Fists";
    }
}

/*
 * Menu
 */

#define MENU_BUTTON_HEIGHT 15
#define MENU_BUTTON_GAP 4
#define MENU_PADDING 10

static int MenuHeaderHeight(const Menu* menu)
{
    return menu->title != NULL ? FontBig() + 6 * Px() : 0;
}

static Rectangle MenuPanelRect(const Menu* menu)
{
    const int p = Px();

    int width = 100 * p;
    if (menu->title != NULL)
    {
        width = (int)fmaxf((float)width, (float)(MeasureText(menu->title, FontBig()) + 2 * MENU_PADDING * p));
    }
    for (int i = 0; i < menu->count; i++)
    {
        width = (int)fmaxf((float)width, (float)(MeasureText(menu->items[i], FontSmall()) + 4 * MENU_PADDING * p));
    }

    const int height = 2 * MENU_PADDING * p + MenuHeaderHeight(menu)
        + menu->count * MENU_BUTTON_HEIGHT * p + (menu->count - 1) * MENU_BUTTON_GAP * p;

    return (Rectangle){
        (float)((int)((float)GetScreenWidth() * menu->center_x) - width / 2),
        (float)((int)((float)GetScreenHeight() * menu->center_y) - height / 2),
        (float)width,
        (float)height
    };
}

static Rectangle MenuButtonRect(const Menu* menu, const int idx)
{
    const int p = Px();
    const Rectangle panel = MenuPanelRect(menu);
    return (Rectangle){
        panel.x + (float)(MENU_PADDING * p),
        panel.y + (float)(MENU_PADDING * p + MenuHeaderHeight(menu) + idx * (MENU_BUTTON_HEIGHT + MENU_BUTTON_GAP) * p),
        panel.width - (float)(2 * MENU_PADDING * p),
        (float)(MENU_BUTTON_HEIGHT * p)
    };
}

int UpdateMenu(const Menu* menu, int* selected)
{
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) *selected = (*selected - 1 + menu->count) % menu->count;
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) *selected = (*selected + 1) % menu->count;

    const Vector2 mouse = GetMousePosition();
    for (int i = 0; i < menu->count; i++)
    {
        if (!CheckCollisionPointRec(mouse, MenuButtonRect(menu, i))) continue;

        //Only follow the mouse when it moves, so it doesn't fight the keyboard
        if (Vector2Length(GetMouseDelta()) > 0) *selected = i;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            *selected = i;
            return i;
        }
    }

    if (IsKeyPressed(KEY_ENTER)) return *selected;
    return -1;
}

void DrawMenu(const Menu* menu, const int selected)
{
    const int p = Px();
    const Rectangle panel = MenuPanelRect(menu);
    DrawPanel((int)panel.x, (int)panel.y, (int)panel.width, (int)panel.height);

    if (menu->title != NULL)
    {
        DrawTextCentered(menu->title, (int)(panel.x + panel.width / 2), (int)panel.y + MENU_PADDING * p - p,
                         FontBig(), UI_TEXT);
    }

    for (int i = 0; i < menu->count; i++)
    {
        const Rectangle button = MenuButtonRect(menu, i);
        const int x = (int)button.x;
        const int y = (int)button.y;
        const int w = (int)button.width;
        const int h = (int)button.height;
        const bool is_selected = i == selected;

        DrawNotchedRect(x, y, w, h, p, is_selected ? UI_OUTLINE : UI_WOOD_DARK);
        DrawRectangle(x + p, y + p, w - 2 * p, h - 2 * p, is_selected ? UI_GOLD : UI_PAPER_DARK);
        DrawRectangle(x + p, y + p, w - 2 * p, p, is_selected ? UI_GOLD_LIGHT : UI_PAPER);
        DrawRectangle(x + p, y + h - 2 * p, w - 2 * p, p, is_selected ? UI_GOLD_DARK : UI_WOOD_LIGHT);

        DrawTextCentered(menu->items[i], x + w / 2, y + 4 * p, FontSmall(), UI_TEXT);

        if (is_selected)
        {
            const int bob = (int)(sinf((float)GetTime() * 6.f) * 1.5f) * p;
            DrawPixelIcon(ICON_ARROW, 9, x - (MENU_PADDING + 12) * p + bob, y - 2 * p, 2 * p, UI_GOLD_DARK,
                          UI_GOLD_LIGHT);
        }
    }
}

/*
 * Screens
 */

void DrawTitleScreen(void)
{
    const int p = Px();
    const int center_x = GetScreenWidth() / 2;
    const int bob = (int)(sinf((float)GetTime() * 1.5f) * 2.f) * p;
    const int title_y = GetScreenHeight() / 5 + bob;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), UI_HAZE);

    const char* line1 = "Yet Another";
    const char* line2 = "2D Adventure";
    DrawTextOutlined(line1, center_x - MeasureText(line1, FontBig()) / 2, title_y, FontBig(), UI_GOLD_LIGHT);
    DrawTextOutlined(line2, center_x - MeasureText(line2, 40 * p) / 2, title_y + 24 * p, 40 * p, UI_PAPER);

    const char* hint = "W/S to choose   Enter to confirm";
    DrawTextOutlined(hint, center_x - MeasureText(hint, FontSmall()) / 2, GetScreenHeight() - 24 * p, FontSmall(),
                     UI_PAPER);
}

void DrawHud(const Player* player, const World* world)
{
    const int p = Px();
    const int x = 6 * p;

    //Health
    {
        const int y = 6 * p;
        const int w = 110 * p;
        const int h = 30 * p;
        DrawPanel(x, y, w, h);
        DrawPixelIcon(ICON_HEART, 8, x + 7 * p, y + 7 * p, 2 * p, UI_HP_LOW, UI_HP_LOW_LIGHT);

        const int hp = player->hp < 0 ? 0 : player->hp;
        const float fraction = (float)hp / PLAYER_DEFAULT_HP;
        const bool is_low = fraction <= LOW_HP_FRACTION;
        const int bar_x = x + 28 * p;
        const int bar_y = y + 9 * p;
        const int bar_w = w - 35 * p;
        DrawBar(bar_x, bar_y, bar_w, 12 * p, fraction,
                is_low ? UI_HP_LOW : UI_HP, is_low ? UI_HP_LOW_LIGHT : UI_HP_LIGHT);

        const char* text = TextFormat("%d/%d", hp, PLAYER_DEFAULT_HP);
        DrawTextShadowed(text, bar_x + (bar_w - MeasureText(text, FontSmall())) / 2, bar_y + 2 * p, FontSmall(),
                         UI_TEXT_LIGHT);
    }

    //Weapon
    {
        const int y = 38 * p;
        const int w = 80 * p;
        const int h = 30 * p;
        DrawPanel(x, y, w, h);

        const int slot_x = x + 6 * p;
        const int slot_y = y + 6 * p;
        DrawNotchedRect(slot_x, slot_y, 18 * p, 18 * p, p, UI_WOOD_DARK);
        DrawRectangle(slot_x + p, slot_y + p, 16 * p, 16 * p, UI_PAPER_DARK);

        if (player->weapon.type == SWORD)
        {
            DrawTexturePro(assets.spritesheet_sword,
                           (Rectangle){0, 0, TILE_SIZE, TILE_SIZE},
                           (Rectangle){(float)(slot_x + p), (float)(slot_y + p), (float)(16 * p), (float)(16 * p)},
                           (Vector2){0, 0}, 0, WHITE);
        }

        //Cooldown shade shrinks from the top
        if (player->weapon.hit_rate > 0 && player->weapon.cooldown > 0)
        {
            const float fraction = player->weapon.cooldown / player->weapon.hit_rate;
            const int shade = (int)(16.f * fraction) * p;
            DrawRectangle(slot_x + p, slot_y + p + 16 * p - shade, 16 * p, shade, UI_SHADOW);
        }

        DrawText(WeaponName(player->weapon.type), x + 28 * p, y + 7 * p, FontSmall(), UI_TEXT);
        DrawText(TextFormat("DMG %d", player->weapon.damage), x + 28 * p, y + 17 * p, FontSmall(), UI_ACCENT);
    }

    //Enemies left
    if (world->enemy_count > 0)
    {
        int alive = 0;
        for (int i = 0; i < world->enemy_count; i++)
        {
            if (world->enemies[i].alive) alive++;
        }

        const int w = 70 * p;
        const int h = 30 * p;
        const int panel_x = GetScreenWidth() - 6 * p - w;
        const int y = 6 * p;
        DrawPanel(panel_x, y, w, h);
        //The slime only fills the lower middle of its frame, so crop to it
        DrawTexturePro(assets.spritesheet_red_slime,
                       (Rectangle){4, 8, 8, 8},
                       (Rectangle){(float)(panel_x + 8 * p), (float)(y + 7 * p), (float)(16 * p), (float)(16 * p)},
                       (Vector2){0, 0}, 0, WHITE);
        DrawText(alive > 0 ? TextFormat("x %d", alive) : "Clear!", panel_x + 28 * p, y + 11 * p, FontSmall(),
                 alive > 0 ? UI_TEXT : UI_HP);
    }
}

void DrawPauseScreen(void)
{
    const int p = Px();
    DrawDimOverlay();

    const char* title = "Paused";
    const char* hint = "Press P to resume";
    const int w = (int)fmaxf((float)MeasureText(title, FontBig()), (float)MeasureText(hint, FontSmall())) + 24 * p;
    const int h = 50 * p;
    const int x = (GetScreenWidth() - w) / 2;
    const int y = (GetScreenHeight() - h) / 2;

    DrawPanel(x, y, w, h);
    DrawTextCentered(title, x + w / 2, y + 10 * p, FontBig(), UI_TEXT);
    DrawTextCentered(hint, x + w / 2, y + 32 * p, FontSmall(), UI_WOOD_DARK);
}

void DrawGameMenu(const Menu* menu, const int selected, const Player* player)
{
    const int p = Px();
    DrawDimOverlay();
    DrawMenu(menu, selected);

    //Stats next to the menu
    const Rectangle menu_panel = MenuPanelRect(menu);
    const int x = (int)(menu_panel.x + menu_panel.width) + 6 * p;
    const int y = (int)menu_panel.y;
    const int row_height = 13 * p;
    const int row_count = 5;
    const int w = 120 * p;
    const int h = (int)fmaxf(menu_panel.height,
                             (float)(2 * MENU_PADDING * p + MenuHeaderHeight(menu) + row_count * row_height));
    DrawPanel(x, y, w, h);

    DrawTextCentered("Stats", x + w / 2, y + MENU_PADDING * p - p, FontBig(), UI_TEXT);

    const int hp = player->hp < 0 ? 0 : player->hp;
    const char* labels[] = {"Health", "Weapon", "Damage", "Reach", "Cooldown"};
    const char* values[] = {
        TextFormat("%d/%d", hp, PLAYER_DEFAULT_HP),
        WeaponName(player->weapon.type),
        TextFormat("%d", player->weapon.damage),
        TextFormat("%d", (int)player->weapon.reach),
        TextFormat("%.1fs", player->weapon.hit_rate),
    };

    const int rows_y = y + MENU_PADDING * p + MenuHeaderHeight(menu);
    for (int i = 0; i < row_count; i++)
    {
        const int row_y = rows_y + i * row_height;
        DrawText(labels[i], x + MENU_PADDING * p, row_y, FontSmall(), UI_WOOD_DARK);
        DrawText(values[i], x + w - MENU_PADDING * p - MeasureText(values[i], FontSmall()), row_y, FontSmall(),
                 UI_TEXT);
    }
}

static bool MouseTile(const TileMap* tile_map, const Camera2D camera, int* tile_x, int* tile_y)
{
    const Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    *tile_x = (int)floorf(mouse_world_pos.x / TILE_SIZE);
    *tile_y = (int)floorf(mouse_world_pos.y / TILE_SIZE);
    return *tile_x >= 0 && *tile_x < tile_map->width && *tile_y >= 0 && *tile_y < tile_map->height;
}

void DrawEditorUi(const TileMap* tile_map, const Camera2D camera)
{
    const int p = Px();
    const char* keys[] = {"LMB", "RMB", "MMB", "Shift+LMB", "Shift+RMB", "Shift+MMB", "J / K", "F1"};
    const char* actions[] = {"Dirt", "Grass", "Border", "Water", "Sand", "Erase", "Save / Load", "Back to game"};
    const int count = 8;

    const int row_height = 12 * p;
    const int x = 6 * p;
    const int y = 6 * p;
    const int w = 160 * p;
    const int h = 2 * MENU_PADDING * p + FontBig() + 6 * p + (count + 1) * row_height;
    DrawPanel(x, y, w, h);

    DrawText("Level Editor", x + MENU_PADDING * p, y + MENU_PADDING * p - p, FontBig(), UI_TEXT);

    const int rows_y = y + MENU_PADDING * p + FontBig() + 6 * p;
    for (int i = 0; i < count; i++)
    {
        DrawText(keys[i], x + MENU_PADDING * p, rows_y + i * row_height, FontSmall(), UI_ACCENT);
        DrawText(actions[i], x + 78 * p, rows_y + i * row_height, FontSmall(), UI_TEXT);
    }

    int tile_x, tile_y;
    const char* tile_text = MouseTile(tile_map, camera, &tile_x, &tile_y)
                                ? TextFormat("Tile %d, %d", tile_x, tile_y)
                                : "Tile -";
    DrawText(tile_text, x + MENU_PADDING * p, rows_y + count * row_height, FontSmall(), UI_WOOD_DARK);
}

/*
 * World space
 */

void DrawEnemyHealthBars(const World* world)
{
    for (int i = 0; i < world->enemy_count; i++)
    {
        const Enemy* enemy = &world->enemies[i];
        if (!enemy->alive || enemy->max_hp <= 0) continue;
        if (!enemy->angry && enemy->hp >= enemy->max_hp) continue;

        //One world pixel per bar pixel, so it matches the sprites
        const float x = enemy->position.x;
        //Slimes are small and sit at the bottom of their tile
        const float y = enemy->position.y + (enemy->type == RED_SLIME ? 5 : -3);
        const float fraction = Clamp((float)enemy->hp / (float)enemy->max_hp, 0.f, 1.f);

        DrawRectangleRec((Rectangle){x + 2, y, 12, 3}, UI_OUTLINE);
        DrawRectangleRec((Rectangle){x + 1, y + 1, 14, 1}, UI_OUTLINE);
        DrawRectangleRec((Rectangle){x + 2, y + 1, 12, 1}, UI_BAR_EMPTY);
        DrawRectangleRec((Rectangle){x + 2, y + 1, floorf(12 * fraction), 1}, UI_HP_LOW);
    }
}

void DrawEditorCursor(const TileMap* tile_map, const Camera2D camera)
{
    int tile_x, tile_y;
    if (!MouseTile(tile_map, camera, &tile_x, &tile_y)) return;

    const Rectangle tile = {(float)(tile_x * TILE_SIZE), (float)(tile_y * TILE_SIZE), TILE_SIZE, TILE_SIZE};
    DrawRectangleLinesEx(tile, 1, UI_GOLD);
}
