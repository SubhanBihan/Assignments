#include "Display.h"
// #include <Display.h>

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
}

uint32_t CircularDecrement(uint32_t value, uint32_t maximum) {
    return value = (value + maximum - 1) % maximum;
}

void TitleScreen(HAL* hal_p, display_t* disp_p) {
    static SWTimer TitleTimer, buzzer_timer;  // Timer for Title screen
    

    if (disp_p->screen_changed) {   // Only do these when first entering title screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);
        
        // Print Game name
        GFX_print(&disp_p->gfx, "Cookie Clicker", 1, 1);

        // Print Dev's name
        GFX_print(&disp_p->gfx, "Samin Rahman", 3, 1);

        // C4 Note
        note_t note = Note_Construct(NOTE_C4, 1000, 0.5);

        // Construxt and start the timer
        TitleTimer = SWTimer_construct(TITLE_DURATION_MS);
        buzzer_timer = SWTimer_construct(note.duration_ms);

        SWTimer_start(&TitleTimer);
        Buzzer_Play(&note, &buzzer_timer);

        // Turn off the screen_changed flag
        disp_p->screen_changed = false;
    }

    if (SWTimer_expired(&buzzer_timer)) Buzzer_Stop();

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
    if (hal_p->JS.TappedY == UP) {
        // prev_option = option_num;
        option_num = CircularDecrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }

    // Move cursor down
    if (hal_p->JS.TappedY == DOWN) {
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
            
            case OPTION_TWO:
                disp_p->screen = INSTRUCTIONS;
                break;
            
        }
        disp_p->screen_changed = true;
    }

}


void InstructionsScreen(HAL* hal_p, display_t* disp_p) {
    // Instructions text - CHANGE THE LINES
    static const char* lines[] = {"JSB for Cookies", "JS L/R to See Upgrade", "JS U/D to See Building", "BB1 to B/S Building", "BB2 to B/S Upgrade", "LB1/LB2 to Switch", "between Buy/Sell Mode"};
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
        GFX_print(&disp_p->gfx, "JSB: Go Back", i, 1);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackJS)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}


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

        GFX_print(&disp_p->gfx, "GAME", 1, 1);

        GFX_print(&disp_p->gfx, "JSB: Go Back", 12, 1);

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

    // Exit the game
    if (Button_isTapped(&hal_p->boosterpackJS)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
    
}


// // Game Over Screen
// void GameOverScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
//     char line[50];
//     if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
//         // Set foreground color
//         GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

//         uint8_t i = 0, j = 0;

//         // Print Winner message
//         if (game_state_p->player.HP <= 0)
//             GFX_print(&disp_p->gfx, " You LOSE :X", 6, 4);
//         else
//             GFX_print(&disp_p->gfx, " You WIN =)", 6, 4);
//         // i += MIN_SECTION_GAP_ROWS;

//         game_state_p->score = GetScore(game_state_p->player.HP, game_state_p->time);
        
//         snprintf(line, sizeof(line), "Score: %d", game_state_p->score);
//         GFX_print(&disp_p->gfx, line, 12, 0);
//         GFX_print(&disp_p->gfx, "Press JSB to continue", 13, 0);

//         // Update the high scores
//         if (game_state_p->score > game_state_p->high_scores[NUM_SCORES - 1])         // No need to update if this isn't met
//             for (i = 0; i < NUM_SCORES; i++) {
//                 if (game_state_p->score >= game_state_p->high_scores[i]) {
//                     for (j = NUM_SCORES - 1; j > i; j--) {
//                         game_state_p->high_scores[j] = game_state_p->high_scores[j-1];
//                     }
//                     game_state_p->high_scores[i] = game_state_p->score;

//                     break;
//                 }
//             }

//         // Set flags to stay on this screen
//         disp_p->screen_changed = false;
//     }

//     if (Button_isTapped(&hal_p->boosterpackJS)) {
//         disp_p->screen = MAIN;
//         disp_p->screen_changed = true;
//     }
// }