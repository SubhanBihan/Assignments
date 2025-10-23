#include <HAL/HAL.h>
#include <Application.h>

#define NUM_SCORES 5
#define MIN_LINE_SPACING 10
#define MAX_GRID 5*5
#define GRID_V_START 24
#define GRID_V_END 124
#define GRID_H_START 14
#define GRID_H_END 114

#define SQR(x) ((x) * (x))

// // TODO: add states if needed
// typedef enum {S0, S1, S2, S3, S4} state_t;

// TODO: add shapes if needed
typedef enum {CIRCLE, LINE, SQUARE} shape_t;

// TODO: add colors if needed
typedef enum {BLACK, RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {MAIN, INSTRUCTIONS, HS, GAME, GAMEOVER} screen_t;

typedef struct {
    // unsigned char row, col;
    Graphics_Rectangle R;
    color_t color;
} square_t;


typedef struct {
    bool redraw;
    color_t pen_color;
    // shape_t shape;
    screen_t screen;
    bool screen_changed;
} display_t;


typedef struct {
    unsigned PE;
    unsigned TR;
    unsigned year;
    char grid_length;       // 3 or 5
    square_t squares[MAX_GRID];
} game_state_t;


void InitGraphics(Graphics_Context *);
void InitFonts();
void Initialize(Graphics_Context *);

void MainScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);
void InstructionsScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);
void HSScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, unsigned current_score);

void GameScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t *game_state);

void draw_territory_square(Graphics_Context* g_sContext_p, display_t* disp_p, game_state_t *game_state, unsigned char square_index);