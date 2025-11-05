#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <HAL/HAL.h>
#include <stdlib.h>

#define TITLE_DURATION_MS 3000
#define MIN_SECTION_GAP_ROWS 2
#define NUM_SCORES 5
#define MIN_LINE_SPACING 12
#define MAX_LINE_SPACING 25
#define MAX_GRID 5*5
#define GRID_Y_START 23
#define GRID_Y_END 128
#define GRID_X_START 13
#define GRID_X_END 118
#define GRID_Y_RANGE (GRID_Y_END - GRID_Y_START)
#define GRID_X_RANGE (GRID_X_END - GRID_X_START)
#define SQUARE_SPACING 5
#define MAX_INIT_STATS 1000

// typedef enum {BLACK, WHITE, RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {TITLE, MAIN, INSTRUCTIONS, HS, GAME, GAMEOVER} screen_t;

typedef enum {OPTION_ONE, OPTION_TWO, OPTION_THREE, NUM_OPTIONS} option_t;

typedef struct {
    GFX gfx;
    bool screen_changed;
    bool redraw;
    screen_t screen;
} display_t;

typedef struct
{
    bool Auto;              // Is motion autonomous?
    bool V, H;              // Type of movement allowed
    bool up_down;           // If V = 1, up_down 1/0 is up/down resp.
    bool left_right;        // If H = 1, left_right 1/0 is left/right resp.
    uint8_t V_px, H_px;     // Number of pixels for vertical and horizontal movement
    uint16_t refresh_ms;    // Time interval for redrawing in ms
} motion_t;

typedef struct {
    Graphics_Rectangle body;
    uint32_t color;
    uint16_t HP;
    // bool HP_change;
    motion_t motion;
} character_t;

typedef struct
{
    character_t player, enemy;
    uint32_t time;
    uint32_t high_scores[NUM_SCORES];
} game_state_t;


// Increments a value and wraps around to 0 if it exceeds maximum
void CircularIncrement(uint32_t* value, uint32_t maximum);

// Decrements a value and wraps around to maximum if it goes below 0
void CircularDecrement(uint32_t* value, uint32_t maximum);

// Draw the Title Screen
void TitleScreen(HAL* hal_p, display_t* disp_p);

// // Draw the text elements for the main screen
// void MainScreen_Text(uint8_t* j);

// Handle the main menu screen logic and display
void MainScreen(HAL* hal_p, display_t* disp_p);

// Instructions screen
void InstructionsScreen(HAL* hal_p, display_t* disp_p);

// High Scores Screen
void HSScreen(HAL* hal_p, display_t* disp_p);


#endif