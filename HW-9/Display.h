#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <stdlib.h>

typedef enum {BLACK, WHITE, RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {TITLE, MAIN, INSTRUCTIONS, HS, GAME, GAMEOVER} screen_t;

typedef enum {OPTION_ONE, OPTION_TWO, OPTION_THREE, NUM_OPTIONS} option_t;

typedef struct {
    GFX gfx;
    bool screen_changed;
    bool redraw;
    screen_t screen;
    color_t pen_color;
} display_t;

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