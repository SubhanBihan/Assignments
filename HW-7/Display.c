#include <Display.h>

// This function initializes the graphics
void InitGraphics(Graphics_Context *g_sContext_p) {

    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt16);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);
}

// This function initializes the fonts we can use
void InitFonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

void Initialize(Graphics_Context* g_sContext_p) {
    // WDT_A_hold(WDT_A_BASE);

    InitGraphics(g_sContext_p);
}

// This function updates the foreground color of the graphic context based on the "color" argument
void change_pen_color(Graphics_Context* g_sContext_p, color_t color) {
    switch(color) {
    case RED:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
        break;
    case ORANGE:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
        break;
    case YELLOW:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_YELLOW);
        break;
    case BLUE:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
        break;
    case GREEN:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_GREEN);
        break;
        // TODO: Add more cases if needed.
    }
}

void MainScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p) {
    static unsigned char option_num = 0;

    if (disp_p->screen_changed) {
        // // wipe the screen by drawing over it with the black color as the the background
        // Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);

        // Might need to change this to only covering the '>'- DONE (?)
        Graphics_clearDisplay(g_sContext_p);

        // Game's name
        Graphics_drawString(g_sContext_p, (int8_t *)"Pixel Monarch", -1, 10, 10, true);

        // Developer's name
        Graphics_drawString(g_sContext_p, (int8_t *)"Samin Rahman", -1, 10, 20, true);

        // Options
        Graphics_drawString(g_sContext_p, (int8_t *)"  Play Game", -1, 10, 30, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"  Instructions", -1, 10, 40, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"  High Scores", -1, 10, 50, true);

        // Basic navigation
        Graphics_drawString(g_sContext_p, (int8_t *)"BB1: Select Item", -1, 10, 60, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Move Cursor Down", -1, 10, 70, true);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = 0;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        CircularIncrement(option_num, GAME);
        disp_p->redraw = true;
    }

    // char op1[50], op2[50], op3[50];

    if (disp_p->redraw == true)
        switch (option_num) {
            case 0:
            default:
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 10, 30, true);
                Graphics_drawString(g_sContext_p, (int8_t *)" ", -1, 10, 40, true);
                Graphics_drawString(g_sContext_p, (int8_t *)" ", -1, 10, 50, true);
                break;
            case 1:
                Graphics_drawString(g_sContext_p, (int8_t *)" ", -1, 10, 30, true);
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 10, 40, true);
                Graphics_drawString(g_sContext_p, (int8_t *)" ", -1, 10, 50, true);
                break;
            case 2:
                Graphics_drawString(g_sContext_p, (int8_t *)" ", -1, 10, 30, true);
                Graphics_drawString(g_sContext_p, (int8_t *)" ", -1, 10, 40, true);
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 10, 50, true);
                break;
        }

    disp_p->redraw = false;

    if (Button_isTapped(&hal_p->boosterpackS1)) {
        disp_p->screen = option_num == 0 ? GAME : option_num;
        disp_p->screen_changed = true;
    }

}

/*
Game Instructions

Balance People (PE),
Treasury (TR), and
territory (boxes).
When a crisis hits,
type a command:
SP: Spend (uses TR)
AP: Appeal (uses PE)
IG: Ignore

BB2: Go Back
*/
void InstructionsScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p) {
    if (disp_p->screen_changed) {
        Graphics_clearDisplay(g_sContext_p);

        // Constant fluff-text
        Graphics_drawString(g_sContext_p, (int8_t *)"Game Instructions", -1, 10, 10, true);

        Graphics_drawString(g_sContext_p, (int8_t *)"Balance People (PE),", -1, 5, 20, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"Treasury (TR), and", -1, 5, 30, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"territory (boxes).", -1, 5, 40, true);

        Graphics_drawString(g_sContext_p, (int8_t *)"When a crisis hits,", -1, 5, 50, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"type a command:", -1, 5, 60, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"SP: Spend (uses TR)", -1, 5, 70, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"AP: Appeal (uses PE)", -1, 5, 80, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"IG: Ignore", -1, 5, 90, true);

        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Go Back", -1, 5, 100, true);

        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void HSScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, unsigned current_score) {
    static unsigned int highscores[NUM_SCORES] = {0};

    if (disp_p->screen_changed) {

        // Update the scores
        char i, j;

        if (current_score > highscores[NUM_SCORES])         // No need to update if this isn't met
            for (i = 0; i < NUM_SCORES; i++) {
                if (current_score >= highscores[i]) {
                    for (j = NUM_SCORES; j > i; j--) {
                        highscores[j] = highscores[j-1];
                    }
                    highscores[i] = current_score;

                    break;
                }
            }

        // Display the scores
        Graphics_drawString(g_sContext_p, (int8_t *)"High Scores", -1, 30, 20, true);

        char score_string[18];
        for (i = 0; i < NUM_SCORES; i++) {
            snprintf(score_string, sizeof(score_string), "%d. %d year(s)", i + 1, highscores[i]);
            Graphics_drawString(g_sContext_p, score_string, -1, 30, 30 + MIN_LINE_SPACING * i, true);
        }

        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Go Back", -1, 10, 100, true);

        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void GameScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t *game_state) {

}