#include <Display.h>

void CircularIncrement(uint32_t* value, uint32_t maximum) {
  *value = (*value + 1) % maximum;
}

void CircularDecrement(uint32_t* value, uint32_t maximum) {
  *value = *value ? *value - 1 : maximum - 1;
}


void TitleScreen(HAL* hal_p, display_t* disp_p) {
    static SWTimer TitleTimer;  // Timer for Title screen

    if (disp_p->screen_changed) {   // Only do these when first entering title screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);
        
        // Print Game name
        GFX_print(&disp_p->gfx, "Touhou", 1, 1);

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

void MainScreen(HAL* hal_p, display_t* disp_p) {
    static option_t option_num;     // Tracking the current option (cursor)
    static uint8_t j;               // Tracking row of first option

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // MainScreen_Text(&disp_p->gfx.context, &j);

        // Options text
        char* options[] = {"  Play", "  Instructions", "  High Scores"};
        uint8_t num_options = sizeof(options) / sizeof(char*);
        char* navg_lines[] = {"BB1: Move Cursor Up", "BB2: Move Cursor Down"};

        // Clear the display
        GFX_clear(&disp_p->gfx);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, GRAPHICS_COLOR_WHITE);

        uint8_t i = 0;  // Tracking row of current line

        // Print Screen name
        GFX_print(&disp_p->gfx, "MENU", i++, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print options
        j = i;
        GFX_printTextRows(&disp_p->gfx, options, num_options, i, 1);

        // Print navigations instructions
        i += num_options + MIN_SECTION_GAP_ROWS;
        GFX_printTextRows(&disp_p->gfx, navg_lines, sizeof(navg_lines) / sizeof(char*), i, 1);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
    }

    if (Button_isTapped(&hal_p->boosterpackS1)) {
        CircularDecrement(&option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }
    if (Button_isTapped(&hal_p->boosterpackS2)) {
        CircularIncrement(&option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }


    if (disp_p->redraw) {
        GFX_setForeground(&disp_p->gfx, GRAPHICS_COLOR_BLUE);

        // Erase any precious cursor
        char* cursor_eraser[] = {">", ">", ">"};
        GFX_printTextRows(&disp_p->gfx, cursor_eraser, sizeof(cursor_eraser) / sizeof(char*), j, 1);
        
        // Print new cursor
        GFX_print(&disp_p->gfx, ">", j + option_num, 1);

        disp_p->redraw = false;
    }


    if (Button_isTapped(&hal_p->boosterpackJS)) {
        switch (option_num)
        {
            case OPTION_ONE:
                disp_p->screen = GAME;
                break;
            case OPTION_TWO:
                disp_p->screen = INSTRUCTIONS;
                break;
            case OPTION_THREE:
                disp_p->screen = HS;
                break;
        }
        disp_p->screen_changed = true;
    }

}