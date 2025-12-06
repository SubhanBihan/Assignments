#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "HAL/HAL.h"
#include <stdlib.h>
#include <math.h>

// General
#define BACKGROUND_COLOR GRAPHICS_COLOR_BLACK
#define TEXT_COLOR GRAPHICS_COLOR_WHITE
#define ARROW_COLOR GRAPHICS_COLOR_GREEN
#define TITLE_DURATION_MS 2000
#define MIN_LINE_SPACING 12
#define MAX_LINE_SPACING 25
#define MIN_SECTION_GAP_ROWS 2

// Game aspects
#define CURSOR_STARTING_PRICE 2
#define GRANDMA_STARTING_PRICE 2
#define CURSOR_PRICE_MULT 2
#define GRANDMA_PRICE_MULT 5
#define CURSOR_STARTING_CPI 2
#define GRANDMA_STARTING_CPI 10
#define CURSOR_UPG_STARTING_PRICE 1
#define GRANDMA_UPG_STARTING_PRICE 2
#define CURSOR_UPG_PRICE_MULT 5
#define GRANDMA_UPG_PRICE_MULT 10
#define CURSOR_UPG_CPI_MULT 2
#define GRANDMA_UPG_CPI_MULT 5
#define CURSOR_INTERVAL_MS 1000
#define GRANDMA_INTERVAL_MS 5000

// For BigNum
#define BN_BASE_MIN 1.0
#define BN_BASE_MAX 1000.0
#define BN_BASE_FACTOR 1000.0

// Small functional macros for convenience
#define SQR(x) ((x) * (x))
#define AVG(a, b) (((a) + (b)) / 2)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Screen type
typedef enum {TITLE, MAIN, INSTRUCTIONS, GAME} screen_t;

// Options for menus
typedef enum {OPTION_ONE, OPTION_TWO, NUM_OPTIONS} option_t;

// Game mode
typedef enum {BUY, SELL} mode_t;

// Struct for big numbers
typedef struct {
    double base;       // always in [1, 1000)
    uint8_t mag;       // 0 = none, 1 = k, 2 = M, 3 = B, 4 = T, 5 = Q, 6 = q
} BigNum;

// Struct for screen info
typedef struct {
    bool screen_changed;
    bool redraw;
    screen_t screen;
} display_t;

// Struct for game state
typedef struct {
    BigNum cookies;
    mode_t mode;
    uint16_t cursors_num, grandmas_num;
    BigNum cursors_cpi, grandmas_cpi;
} game_state_t;


// Normalization functions
void normalize_v(BigNum *x);
BigNum normalize_r(BigNum x);

// Arithmetic functions
void addsub_v(BigNum* a, const BigNum* b, bool add_or_sub);     // 0 = add, 1 = sub
BigNum addsub_r(BigNum a, BigNum b, bool add_or_sub);
void mul_v(BigNum* a, const BigNum* b);
BigNum mul_r(BigNum a, BigNum b);
void muld_v(BigNum* a, const double b);
BigNum muld_r(BigNum a, const double b);

// Comparison functions
bool GE_BigNum(const BigNum* a, const BigNum* b);

// String format function
void Format_BigNum(const BigNum* x, char* out, size_t maxlen);

// Increments a value and wraps around to 0 if it exceeds maximum
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

// Decrements a value and wraps around to maximum if it goes below 0
uint32_t CircularDecrement(uint32_t value, uint32_t maximum);



// Draw the Title Screen
void TitleScreen(HAL* hal_p, display_t* disp_p);

// // Draw the text elements for the main screen
// void MainScreen_Text(HAL* hal_p, uint8_t* j);

// Handle the main menu screen logic and display
void MainScreen(HAL* hal_p, display_t* disp_p);

// Instructions screen
void InstructionsScreen(HAL* hal_p, display_t* disp_p);

// Game Screen
void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

#endif