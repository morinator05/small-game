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

//All sizes below are in UI pixels
#define UI_REFERENCE_HEIGHT 480

//Default raylib font is 10px high, multiples of that stay crisp
#define FONT_SMALL 10
#define FONT_LARGE 20
#define FONT_HUGE 40

#define SCREEN_MARGIN 6
#define PANEL_GAP 2
#define WINDOW_PADDING 10
#define WINDOW_HEADER_HEIGHT (FONT_LARGE + 6)
#define ROW_HEIGHT 12

#define MENU_MIN_WIDTH 100
#define MENU_BUTTON_HEIGHT 15
#define MENU_BUTTON_SPACING 4
#define MENU_BUTTON_TEXT_Y 4
#define MENU_ARROW_X 4

#define HUD_PANEL_HEIGHT 30
#define HUD_PADDING 6
#define HUD_SLOT_SIZE 18
#define HUD_TEXT_X (HUD_SLOT_SIZE + 4)

#define LOW_HP_FRACTION .3f

typedef enum
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT,
} TextAlign;

//Pixel art drawn from a string: '#' = outline, 'x' = fill, 'o' = highlight, anything else is transparent
typedef struct
{
    int width;
    int height;
    const char* pixels;
} PixelIcon;

static const PixelIcon ICON_HEART = {
    9, 8,
    ".##...##."
    "#oo#.#xx#"
    "#oxx#xxx#"
    "#xxxxxxx#"
    ".#xxxxx#."
    "..#xxx#.."
    "...#x#..."
    "....#....",
};

static const PixelIcon ICON_ARROW = {
    5, 9,
    "#...."
    "##..."
    "#o#.."
    "#xo#."
    "#xxx#"
    "#xx#."
    "#x#.."
    "##..."
    "#....",
};

typedef struct
{
    const char* key;
    const char* action;
} KeyBinding;

static const KeyBinding EDITOR_BINDINGS[] = {
    {"LMB", "Dirt"},
    {"RMB", "Grass"},
    {"MMB", "Border"},
    {"Shift+LMB", "Water"},
    {"Shift+RMB", "Sand"},
    {"Shift+MMB", "Erase"},
    {"J / K", "Save / Load"},
    {"F1", "Back to game"},
};

#define EDITOR_BINDING_COUNT (int)(sizeof(EDITOR_BINDINGS) / sizeof(EDITOR_BINDINGS[0]))

/*
 * Layout
 */

static Camera2D UiCamera(void)
{
    const int scale = GetScreenHeight() / UI_REFERENCE_HEIGHT;
    return (Camera2D){.zoom = scale < 1 ? 1.f : (float)scale};
}

static Vector2 UiScreenSize(void)
{
    const float zoom = UiCamera().zoom;
    return (Vector2){(float)GetScreenWidth() / zoom, (float)GetScreenHeight() / zoom};
}

static Vector2 UiMousePosition(void)
{
    return GetScreenToWorld2D(GetMousePosition(), UiCamera());
}

//Snapped to whole UI pixels to keep the pixel art look
static Rectangle RectCentered(const Vector2 center, const float width, const float height)
{
    return (Rectangle){floorf(center.x - width / 2), floorf(center.y - height / 2), width, height};
}

static Rectangle Inset(const Rectangle rect, const float dx, const float dy)
{
    return (Rectangle){rect.x + dx, rect.y + dy, rect.width - 2 * dx, rect.height - 2 * dy};
}

static Rectangle TopRow(const Rectangle rect)
{
    return (Rectangle){rect.x, rect.y, rect.width, 1};
}

static Rectangle BottomRow(const Rectangle rect)
{
    return (Rectangle){rect.x, rect.y + rect.height - 1, rect.width, 1};
}

/*
 * Primitives
 */

static void DrawNotchedRect(const Rectangle rect, const Color color)
{
    DrawRectangleRec(Inset(rect, 1, 0), color);
    DrawRectangleRec(Inset(rect, 0, 1), color);
}

static void DrawPixelIcon(const PixelIcon* icon, const Vector2 position, const float scale, const Color fill,
                          const Color highlight)
{
    for (int y = 0; y < icon->height; y++)
    {
        for (int x = 0; x < icon->width; x++)
        {
            Color color;
            switch (icon->pixels[y * icon->width + x])
            {
            case '#': color = UI_OUTLINE;
                break;
            case 'x': color = fill;
                break;
            case 'o': color = highlight;
                break;
            default: continue;
            }
            DrawRectangleRec((Rectangle){position.x + x * scale, position.y + y * scale, scale, scale}, color);
        }
    }
}

static int AlignText(const char* text, const float x, const int size, const TextAlign align)
{
    switch (align)
    {
    case ALIGN_CENTER: return (int)x - MeasureText(text, size) / 2;
    case ALIGN_RIGHT: return (int)x - MeasureText(text, size);
    default: return (int)x;
    }
}

static void DrawLabel(const char* text, const float x, const float y, const int size, const TextAlign align,
                      const Color color)
{
    DrawText(text, AlignText(text, x, size, align), (int)y, size, color);
}

static void DrawLabelShadowed(const char* text, const float x, const float y, const int size, const TextAlign align,
                              const Color color)
{
    const int left = AlignText(text, x, size, align);
    DrawText(text, left + 1, (int)y + 1, size, UI_SHADOW);
    DrawText(text, left, (int)y, size, color);
}

static void DrawLabelOutlined(const char* text, const float x, const float y, const int size, const TextAlign align,
                              const Color color)
{
    const int left = AlignText(text, x, size, align);
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 2; dy++)
        {
            if (dx != 0 || dy != 0) DrawText(text, left + dx, (int)y + dy, size, UI_OUTLINE);
        }
    }
    DrawText(text, left, (int)y, size, color);
}

static void DrawDimOverlay(void)
{
    const Vector2 size = UiScreenSize();
    DrawRectangleRec((Rectangle){0, 0, size.x, size.y}, UI_DIM);
}

/*
 * Widgets
 */

//Wooden frame with a paper inside, the frame is 4px thick
static void DrawPanel(const Rectangle rect)
{
    const Rectangle wood = Inset(rect, 1, 1);
    DrawNotchedRect(rect, UI_OUTLINE);
    DrawNotchedRect(wood, UI_WOOD);
    DrawRectangleRec(TopRow(Inset(wood, 1, 0)), UI_WOOD_LIGHT);
    DrawRectangleRec(BottomRow(Inset(wood, 1, 0)), UI_WOOD_DARK);

    DrawNotchedRect(Inset(rect, 3, 3), UI_WOOD_DARK);
    DrawNotchedRect(Inset(rect, 4, 4), UI_PAPER);
    DrawRectangleRec(BottomRow(Inset(rect, 5, 4)), UI_PAPER_DARK);
}

//A window is a panel with an optional title, its content sits below the title
static float WindowHeight(const bool has_title, const float content_height)
{
    return 2 * WINDOW_PADDING + (has_title ? WINDOW_HEADER_HEIGHT : 0) + content_height;
}

static Rectangle WindowContent(const Rectangle rect, const bool has_title)
{
    Rectangle content = Inset(rect, WINDOW_PADDING, WINDOW_PADDING);
    if (has_title)
    {
        content.y += WINDOW_HEADER_HEIGHT;
        content.height -= WINDOW_HEADER_HEIGHT;
    }
    return content;
}

static Rectangle DrawWindow(const Rectangle rect, const char* title)
{
    DrawPanel(rect);
    if (title != NULL)
    {
        DrawLabel(title, rect.x + rect.width / 2, rect.y + WINDOW_PADDING - 1, FONT_LARGE, ALIGN_CENTER, UI_TEXT);
    }
    return WindowContent(rect, title != NULL);
}

//One "label ..... value" line of a list inside a window
static void DrawPropertyRow(const Rectangle content, const int row, const char* label, const Color label_color,
                            const char* value)
{
    const float y = content.y + (float)(row * ROW_HEIGHT);
    DrawLabel(label, content.x, y, FONT_SMALL, ALIGN_LEFT, label_color);
    DrawLabel(value, content.x + content.width, y, FONT_SMALL, ALIGN_RIGHT, UI_TEXT);
}

static void DrawButton(const Rectangle rect, const char* text, const bool selected)
{
    const Rectangle face = Inset(rect, 1, 1);
    DrawNotchedRect(rect, selected ? UI_OUTLINE : UI_WOOD_DARK);
    DrawRectangleRec(face, selected ? UI_GOLD : UI_PAPER_DARK);
    DrawRectangleRec(TopRow(face), selected ? UI_GOLD_LIGHT : UI_PAPER);
    DrawRectangleRec(BottomRow(face), selected ? UI_GOLD_DARK : UI_WOOD_LIGHT);
    DrawLabel(text, rect.x + rect.width / 2, rect.y + MENU_BUTTON_TEXT_Y, FONT_SMALL, ALIGN_CENTER, UI_TEXT);
}

static void DrawBar(const Rectangle rect, const float fraction, const Color fill, const Color highlight)
{
    const Rectangle inner = Inset(rect, 1, 1);
    DrawNotchedRect(rect, UI_OUTLINE);
    DrawRectangleRec(inner, UI_BAR_EMPTY);

    const Rectangle filled = {inner.x, inner.y, floorf(inner.width * Clamp(fraction, 0.f, 1.f)), inner.height};
    if (filled.width <= 0) return;
    DrawRectangleRec(filled, fill);
    DrawRectangleRec(TopRow(filled), highlight);
}

/*
 * Menu
 */

static Rectangle MenuBounds(const Menu* menu)
{
    float width = MENU_MIN_WIDTH;
    if (menu->title != NULL)
    {
        width = fmaxf(width, (float)(MeasureText(menu->title, FONT_LARGE) + 2 * WINDOW_PADDING));
    }
    for (int i = 0; i < menu->item_count; i++)
    {
        width = fmaxf(width, (float)(MeasureText(menu->items[i], FONT_SMALL) + 4 * WINDOW_PADDING));
    }

    const float content_height = (float)(menu->item_count * MENU_BUTTON_HEIGHT
        + (menu->item_count - 1) * MENU_BUTTON_SPACING);
    //Menus sit in the top left corner
    return (Rectangle){SCREEN_MARGIN, SCREEN_MARGIN, width, WindowHeight(menu->title != NULL, content_height)};
}

static Rectangle MenuButtonBounds(const Menu* menu, const int index)
{
    const Rectangle content = WindowContent(MenuBounds(menu), menu->title != NULL);
    return (Rectangle){
        content.x,
        content.y + (float)(index * (MENU_BUTTON_HEIGHT + MENU_BUTTON_SPACING)),
        content.width,
        MENU_BUTTON_HEIGHT
    };
}

int UpdateMenu(const Menu* menu, int* selected)
{
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) *selected = (*selected - 1 + menu->item_count) % menu->item_count;
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) *selected = (*selected + 1) % menu->item_count;

    const Vector2 mouse = UiMousePosition();
    for (int i = 0; i < menu->item_count; i++)
    {
        if (!CheckCollisionPointRec(mouse, MenuButtonBounds(menu, i))) continue;

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
    DrawWindow(MenuBounds(menu), menu->title);

    for (int i = 0; i < menu->item_count; i++)
    {
        const Rectangle button = MenuButtonBounds(menu, i);
        DrawButton(button, menu->items[i], i == selected);

        //The arrow sits inside the button, outside the window it would leave the screen
        if (i == selected)
        {
            const float bob = floorf(sinf((float)GetTime() * 6.f) * 1.5f);
            const Vector2 arrow = {
                button.x + MENU_ARROW_X + bob,
                button.y + floorf((MENU_BUTTON_HEIGHT - (float)ICON_ARROW.height) / 2)
            };
            DrawPixelIcon(&ICON_ARROW, arrow, 1, UI_GOLD_DARK, UI_GOLD_LIGHT);
        }
    }
}

/*
 * Screens
 */

void BeginUiMode(void)
{
    BeginMode2D(UiCamera());
}

void EndUiMode(void)
{
    EndMode2D();
}

void DrawTitleScreen(void)
{
    const Vector2 screen = UiScreenSize();

    DrawRectangleRec((Rectangle){0, 0, screen.x, screen.y}, UI_HAZE);
    DrawLabelOutlined("W/S to choose   Enter to confirm", floorf(screen.x / 2), screen.y - 24, FONT_SMALL,
                      ALIGN_CENTER, UI_PAPER);
}

//Small panel with a square slot on the left and text on the right
static Rectangle DrawHudPanel(const Rectangle rect)
{
    DrawPanel(rect);
    return Inset(rect, HUD_PADDING, HUD_PADDING);
}

static void DrawHealthPanel(const Rectangle rect, const Player* player)
{
    const Rectangle content = DrawHudPanel(rect);
    DrawPixelIcon(&ICON_HEART, (Vector2){content.x, content.y + 1}, 2, UI_HP_LOW, UI_HP_LOW_LIGHT);

    const int hp = player->hp < 0 ? 0 : player->hp;
    const float fraction = (float)hp / PLAYER_DEFAULT_HP;
    const bool is_low = fraction <= LOW_HP_FRACTION;
    const Rectangle bar = {content.x + HUD_TEXT_X, content.y + 3, content.width - HUD_TEXT_X, 12};
    DrawBar(bar, fraction, is_low ? UI_HP_LOW : UI_HP, is_low ? UI_HP_LOW_LIGHT : UI_HP_LIGHT);

    DrawLabelShadowed(TextFormat("%d/%d", hp, PLAYER_DEFAULT_HP), bar.x + bar.width / 2, bar.y + 2, FONT_SMALL,
                      ALIGN_CENTER, UI_TEXT_LIGHT);
}

//Square slot showing the equipped weapon, hidden when unarmed
static void DrawWeaponPanel(const Rectangle rect, const Weapon* weapon)
{
    const Rectangle content = DrawHudPanel(rect);
    const Rectangle icon = Inset(content, 1, 1);
    DrawNotchedRect(content, UI_WOOD_DARK);
    DrawRectangleRec(icon, UI_PAPER_DARK);

    if (weapon->type == SWORD)
    {
        DrawTexturePro(assets.spritesheet_sword, (Rectangle){0, 0, TILE_SIZE, TILE_SIZE}, icon, Vector2Zero(), 0,
                       WHITE);
    }

    //Cooldown shade shrinks from the top
    if (weapon->hit_rate > 0 && weapon->cooldown > 0)
    {
        const float shade = floorf(icon.height * weapon->cooldown / weapon->hit_rate);
        DrawRectangleRec((Rectangle){icon.x, icon.y + icon.height - shade, icon.width, shade}, UI_SHADOW);
    }
}

void DrawHud(const Player* player)
{
    const Rectangle health = {SCREEN_MARGIN, SCREEN_MARGIN, 110, HUD_PANEL_HEIGHT};
    DrawHealthPanel(health, player);

    if (player->weapon.type != UNARMED)
    {
        const Rectangle weapon = {
            health.x + health.width + PANEL_GAP, SCREEN_MARGIN, HUD_PANEL_HEIGHT, HUD_PANEL_HEIGHT
        };
        DrawWeaponPanel(weapon, &player->weapon);
    }
}

void DrawPauseScreen(void)
{
    const char* title = "Paused";
    const char* hint = "Press P to resume";

    DrawDimOverlay();

    const float width = fmaxf((float)MeasureText(title, FONT_LARGE), (float)MeasureText(hint, FONT_SMALL))
        + 2 * WINDOW_PADDING;
    const Rectangle window = RectCentered(Vector2Scale(UiScreenSize(), .5f), width, WindowHeight(true, FONT_SMALL));
    const Rectangle content = DrawWindow(window, title);
    DrawLabel(hint, content.x + content.width / 2, content.y, FONT_SMALL, ALIGN_CENTER, UI_WOOD_DARK);
}

void DrawGameMenu(const Menu* menu, const int selected)
{
    DrawDimOverlay();
    DrawMenu(menu, selected);
}

static bool GetMouseTile(const TileMap* tile_map, const Camera2D camera, int* tile_x, int* tile_y)
{
    const Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
    *tile_x = (int)floorf(mouse.x / TILE_SIZE);
    *tile_y = (int)floorf(mouse.y / TILE_SIZE);
    return *tile_x >= 0 && *tile_x < tile_map->width && *tile_y >= 0 && *tile_y < tile_map->height;
}

void DrawEditorUi(const TileMap* tile_map, const Camera2D camera)
{
    const int row_count = EDITOR_BINDING_COUNT + 1;
    const Rectangle window = {
        SCREEN_MARGIN, SCREEN_MARGIN, 160, WindowHeight(true, (float)(row_count * ROW_HEIGHT))
    };
    const Rectangle content = DrawWindow(window, "Level Editor");

    for (int i = 0; i < EDITOR_BINDING_COUNT; i++)
    {
        DrawPropertyRow(content, i, EDITOR_BINDINGS[i].key, UI_ACCENT, EDITOR_BINDINGS[i].action);
    }

    int tile_x, tile_y;
    const char* tile = GetMouseTile(tile_map, camera, &tile_x, &tile_y) ? TextFormat("%d, %d", tile_x, tile_y) : "-";
    DrawPropertyRow(content, EDITOR_BINDING_COUNT, "Tile", UI_WOOD_DARK, tile);
}

/*
 * World space, one world pixel per UI pixel so it matches the sprites
 */

void DrawEnemyHealthBars(const World* world)
{
    for (int i = 0; i < world->enemy_count; i++)
    {
        const Enemy* enemy = &world->enemies[i];
        if (!enemy->alive || enemy->max_hp <= 0) continue;
        if (!enemy->angry && enemy->hp >= enemy->max_hp) continue;

        //Under the sprite with a 1px gap, the bottom rows of the tile are empty
        const float x = enemy->position.x;
        const float y = enemy->position.y + TILE_SIZE - 1;
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
    if (!GetMouseTile(tile_map, camera, &tile_x, &tile_y)) return;

    const Rectangle tile = {(float)(tile_x * TILE_SIZE), (float)(tile_y * TILE_SIZE), TILE_SIZE, TILE_SIZE};
    DrawRectangleLinesEx(tile, 1, UI_GOLD);
}
