#include "Display.h"
// #include <Display.h>

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
}

uint32_t CircularDecrement(uint32_t value, uint32_t maximum) {
    return value = (value + maximum - 1) % maximum;
}

unsigned GetScore(int player_HP, unsigned game_time) {
    return sqrt(game_time) * player_HP * player_HP * (player_HP > 0);     // Rewards HP retention much more than game time
}

void TitleScreen(HAL* hal_p, display_t* disp_p) {
    static SWTimer TitleTimer;  // Timer for Title screen

    if (disp_p->screen_changed) {   // Only do these when first entering title screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);
        
        // Print Game name
        GFX_print(&disp_p->gfx, "Cookie Clicker", 1, 1);

        // Print Dev's name
        GFX_print(&disp_p->gfx, "Samin Rahman", 3, 1);

        // Construxt and start the timer
        TitleTimer = SWTimer_construct(TITLE_DURATION_MS);
        SWTimer_start(&TitleTimer);

        // Turn off the screen_changed flag
        disp_p->screen_changed = false;
    }

    // When the timer expries, change settings to move to Main Menu screen
    if (SWTimer_expired(&TitleTimer)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void MainScreen_Text(display_t* disp_p, uint8_t* j) {
    // static const char* lines[] = {"  New Game", "  Resume Game", "  Instructions", "  High Scores", "BB1: Cursor Up", "BB2: Cursor Down", "JSB: Select"};
    static const char* lines[] = {"  Play Game", "  Instructions"};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    Graphics_clearDisplay(&disp_p->gfx.context);
    GFX_setForeground(&disp_p->gfx, TEXT_COLOR);   // Text color
    
    uint8_t i = 0;

    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"MENU", -1, 5, MIN_LINE_SPACING * ++i, true);

    *j = i;

    for ( ; i - (*j) < num_lines; i++) {
        Graphics_drawString(&disp_p->gfx.context, lines[i - (*j)], -1, 5, MIN_LINE_SPACING * (i + 1), true);
    }
}

void MainScreen(HAL* hal_p, display_t* disp_p) {
    static option_t option_num;
    static uint8_t j;

    // Upon entry into function
    if (disp_p->screen_changed) {
        MainScreen_Text(&disp_p->gfx.context, &j);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
    }

    // Move cursor up
    if (Button_isTapped(&hal_p->boosterpackS1)) {
        // prev_option = option_num;
        option_num = CircularDecrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }

    // Move cursor down
    if (Button_isTapped(&hal_p->boosterpackS2)) {
        // prev_option = option_num;
        option_num = CircularIncrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }


    // Change the option arrow
    if (disp_p->redraw) {
        GFX_setForeground(&disp_p->gfx, ARROW_COLOR);

        uint8_t k;

        for (k = 0; k < NUM_OPTIONS; k++) {
            if (k == option_num)
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
            else
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
        }

        disp_p->redraw = false;
    }


    // Select the indicated option
    if (Button_isTapped(&hal_p->boosterpackJS)) {
        switch (option_num)
        {
            case OPTION_ONE:
                disp_p->screen = GAME;
                break;
            // case OPTION_TWO:
            //     disp_p->screen = RESUME;
            //     break;
            case OPTION_TWO:
                disp_p->screen = INSTRUCTIONS;
                break;
            // case OPTION_FOUR:
            //     disp_p->screen = HS;
            //     break;
        }
        disp_p->screen_changed = true;
    }

}


void InstructionsScreen(HAL* hal_p, display_t* disp_p) {
    // Instructions text
    static const char* lines[] = {"Move the character", "to avoid being hit by", "the bullets.", "Press BB1 to fire at", "the enemy.", "First one to run out", "of health loses."};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // MainScreen_Text(&disp_p->gfx.context, &j);

        // Clear the display
        GFX_clear(&disp_p->gfx);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 1;  // Tracking row of current line

        // Print Screen name
        GFX_print(&disp_p->gfx, "INSTRUCTIONS", i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print instructions
        i = GFX_printTextRows(&disp_p->gfx, lines, num_lines, i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print navigations instructions
        GFX_print(&disp_p->gfx, "Press BB2 to return", i, 1);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

// void HSScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
//     if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
//         // Clear the display
//         GFX_clear(&disp_p->gfx);

//         // Set foreground color
//         GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

//         uint8_t i = 1;  // Tracking row of current line

//         // Print Screen name
//         GFX_print(&disp_p->gfx, "HIGH SCORES", i, 1);
//         // i += MIN_SECTION_GAP_ROWS;

//         // Display the scores
//         char score_string[50];
//         for (i = 0; i < NUM_SCORES; i++) {
//             snprintf(score_string, sizeof(score_string), "%d. %d", i + 1, game_state_p->high_scores[i]);
//             GFX_print(&disp_p->gfx, score_string, i + 2, 2);
//         }

//         GFX_print(&disp_p->gfx, "BB2: Go Back", i + 5, 1);

//         // Set flags to stay on this screen
//         disp_p->screen_changed = false;
//     }

//     // Exit
//     if (Button_isTapped(&hal_p->boosterpackS2)) {
//         disp_p->screen = MAIN;
//         disp_p->screen_changed = true;
//     }
// }

void GameScreenGraphics(display_t* disp_p, game_state_t* game_state_p, Graphics_Rectangle* player_HPB, Graphics_Rectangle* enemy_HPB) {
    char line[20];

    disp_p->redraw = false;
}

void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    char line[20];

    static uint8_t j = 0;   // For tracking enemy bullet number

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);

        if (game_state_p->new_game) {
            // NewGameInit(disp_p, game_state_p, &num_enemy_bullets, &game_timer, &pattern_timer, &attack_timer, &player_power, &enemy_power, &player_HPB, &enemy_HPB, &player_power_timer);
        }

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
        disp_p->redraw = true;
    }


    // // Redraw Graphics
    // if (disp_p->redraw) {
    //     GameScreenGraphics(disp_p, game_state_p, &player_HPB, &enemy_HPB);
    // }

    // // Condition for Game Over
    // if (game_state_p->player.HP <= 0 || game_state_p->enemy.HP <= 0) {
    //     disp_p->screen = GAMEOVER;
    //     disp_p->screen_changed = true;
    //     game_state_p->new_game = true;
    // }

    // // Exit the game
    // if (Button_isTapped(&hal_p->boosterpackS2)) {
    //     disp_p->screen = MAIN;
    //     disp_p->screen_changed = true;
    // }
    
}


// Game Over Screen
void GameOverScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    char line[50];
    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 0, j = 0;

        // Print Winner message
        if (game_state_p->player.HP <= 0)
            GFX_print(&disp_p->gfx, " You LOSE :X", 6, 4);
        else
            GFX_print(&disp_p->gfx, " You WIN =)", 6, 4);
        // i += MIN_SECTION_GAP_ROWS;

        game_state_p->score = GetScore(game_state_p->player.HP, game_state_p->time);
        
        snprintf(line, sizeof(line), "Score: %d", game_state_p->score);
        GFX_print(&disp_p->gfx, line, 12, 0);
        GFX_print(&disp_p->gfx, "Press JSB to continue", 13, 0);

        // Update the high scores
        if (game_state_p->score > game_state_p->high_scores[NUM_SCORES - 1])         // No need to update if this isn't met
            for (i = 0; i < NUM_SCORES; i++) {
                if (game_state_p->score >= game_state_p->high_scores[i]) {
                    for (j = NUM_SCORES - 1; j > i; j--) {
                        game_state_p->high_scores[j] = game_state_p->high_scores[j-1];
                    }
                    game_state_p->high_scores[i] = game_state_p->score;

                    break;
                }
            }

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackJS)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}