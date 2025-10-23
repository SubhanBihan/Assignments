#include <HAL/HAL.h>
#include <Application.h>

#define NUM_SCORES 5
#define MIN_LINE_SPACING 10

// // TODO: add states if needed
// typedef enum {S0, S1, S2, S3, S4} state_t;

// TODO: add shapes if needed
typedef enum {CIRCLE, LINE, SQUARE} shape_t;

// TODO: add colors if needed
typedef enum {RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {MAIN, INSTRUCTIONS, HS, GAME, GAMEOVER} screen_t;

typedef struct {
    unsigned PE;
    unsigned TR;
    unsigned year;

} game_state_t;

typedef struct {
    bool redraw;
    color_t pen_color;
    // shape_t shape;
    screen_t screen;
    bool screen_changed;
} display_t;

void InitGraphics(Graphics_Context *);
void InitFonts();
void Initialize(Graphics_Context *);

void MainScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);
void InstructionsScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);
void HSScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, unsigned current_score);

void GameScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t *game_state);