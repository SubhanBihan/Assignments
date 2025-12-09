#include "Display.h"

void normalize_v(BigNum *x) {
    if (x->base >= BN_BASE_MAX) {
        x->base /= BN_BASE_FACTOR;
        x->mag++;
    }
    else if (x->base < BN_BASE_MIN && x->base > 0) {
        x->base *= BN_BASE_FACTOR;
        x->mag--;
    }
    else x->mag = 0;
}

BigNum normalize_r(BigNum x) {
    normalize_v(&x);
    return x;
}

void addsub_v(BigNum* a, const BigNum* b, bool add_or_sub) {
    BigNum temp = *b;
    if (temp.mag < a->mag) {
        // scale b upward
        while (temp.mag < a->mag) {
            temp.base /= BN_BASE_FACTOR;
            temp.mag++;
            if (temp.base < BN_BASE_MIN) break;
        }
    } else if (!add_or_sub && temp.mag > a->mag) {
        // scale a upward
        while (a->mag < temp.mag) {
            a->base /= BN_BASE_FACTOR;
            a->mag++;
            if (a->base < BN_BASE_MIN) break;
        }
    }

    a->base += (add_or_sub ? -temp.base : temp.base);
    normalize_v(a);
}

BigNum addsub_r(BigNum a, BigNum b, bool add_or_sub) {
    addsub_v(&a, &b, add_or_sub);
    return a;
}

void mul_v(BigNum* a, const BigNum* b) {
    a->base *= b->base;
    a->mag += b->mag;
    normalize_v(a);
}

BigNum mul_r(BigNum a, BigNum b) {
    mul_v(&a, &b);
    return a;
}

void muldiv_v(BigNum* a, const double b, bool mul_or_div) {
    a->base *= (mul_or_div ? 1.0/b : b);
    normalize_v(a);
}

BigNum muldiv_r(BigNum a, const double b, bool mul_or_div) {
    muldiv_v(&a, b, mul_or_div);
    return a;
}

bool GE_BigNum(const BigNum* a, const BigNum* b)
{
    // if (a->mag != b->mag) return a->mag > b->mag;
    // return a->base >= b->base;
    return (a->mag > b->mag) || (a->mag == b->mag && a->base >= b->base);
}

void Format_BigNum(const BigNum* x, char* out, size_t maxlen) {
    static const char* SUFFIXES[] = { "", "k", "M", "B", "T", "Q", "q" };
    if (x->mag == 0) {
        snprintf(out, maxlen, "%.0f", x->base);
    } else if (x->base >= 100.0) {
        snprintf(out, maxlen, "%.0f%s", x->base, SUFFIXES[x->mag]);
    } else if (x->base >= 10.0) {
        snprintf(out, maxlen, "%.1f%s", x->base, SUFFIXES[x->mag]);
    } else {
        snprintf(out, maxlen, "%.2f%s", x->base, SUFFIXES[x->mag]);
    }
}

BigNum GetCPS(const game_state_t* gs_p) {
    BigNum total_cursor_cps = gs_p->cursor_cpi;
    BigNum total_grandma_cps = gs_p->grandma_cpi;

    total_cursor_cps.base *= gs_p->cursor_num;
    total_grandma_cps.base *= gs_p->grandma_num;
    total_grandma_cps.base /= (GRANDMA_INTERVAL_MS / CURSOR_INTERVAL_MS);

    return addsub_r(total_cursor_cps, total_cursor_cps, false);
}

void PrintCookies(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    Format_BigNum(&gs_p->cookies, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, COOKIES_TEXT_ROW, COOKIES_COL);
}

void PrintCPS(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    gs_p->cps = GetCPS(gs_p);
    Format_BigNum(&gs_p->cps, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, COOKIES_TEXT_ROW, CPS_STATS_COL);
}

void PrintCursor(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    snprintf(BNbuffer, TEXT_BUFFER_LENGTH, "%d", gs_p->cursor_num);
    GFX_print(&hal_p->gfx, BNbuffer, CURSOR_TEXT_ROW, STATS_COL);
    Format_BigNum(&gs_p->cursor_cost, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, CURSOR_TEXT_ROW + 1, STATS_COL);
}

void PrintGrandma(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    snprintf(BNbuffer, TEXT_BUFFER_LENGTH, "%d", gs_p->grandma_num);
    GFX_print(&hal_p->gfx, BNbuffer, GRANDMA_TEXT_ROW, STATS_COL);
    Format_BigNum(&gs_p->grandma_cost, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, GRANDMA_TEXT_ROW + 1, STATS_COL);
}

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
}

uint32_t CircularDecrement(uint32_t value, uint32_t maximum) {
    return value = (value + maximum - 1) % maximum;
}

void TitleScreen(HAL* hal_p, display_t* disp_p) {
    static SWTimer TitleTimer;  // Timer for Title screen
    

    if (disp_p->screen_changed) {   // Only do these when first entering title screen (function)
        // Clear the display
        GFX_clear(&hal_p->gfx);
        
        // Print Game name
        GFX_print(&hal_p->gfx, "Cookie Clicker", 1, 4);

        // Print Dev's name
        GFX_print(&hal_p->gfx, "Samin Rahman", 3, 4);

        // Construct and start the timer
        TitleTimer = SWTimer_construct(TITLE_DURATION_MS);
        
        SWTimer_start(&TitleTimer);

        // Turn off the screen_changed flag
        disp_p->screen_changed = false;
    }

    // When the timer expries, change settings to move to Main Menu screen
    if (SWTimer_expired(&TitleTimer)) {
    // if (hal_p->buttons.AnyButtonTap) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}


void MainScreen(HAL* hal_p, display_t* disp_p) {
    static option_t option_num;
    static uint8_t j;
    static const char* lines[] = {"  Play Game", "  Instructions"};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    // Upon entry into function
    if (disp_p->screen_changed) {
        // MainScreen_Text(&hal_p->gfx.context, &j);
        Graphics_clearDisplay(&hal_p->gfx.context);
        GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);   // Text color

        uint8_t i = 0;

        Graphics_drawString(&hal_p->gfx.context, (int8_t *)"MENU", -1, 5, MIN_LINE_SPACING * ++i, true);
        j = i;

        for (i = 0; i - j < num_lines; i++) {
            Graphics_drawString(&hal_p->gfx.context, lines[i - j], -1, 5, MIN_LINE_SPACING * (i + 1), true);
        }

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
    }

    // Move cursor/arrow
    if (hal_p->JS.TappedY != CENTERY) {
        option_num = hal_p->JS.TappedY == UP ? CircularDecrement(option_num, NUM_OPTIONS) : CircularIncrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }

    // Change the option arrow
    if (disp_p->redraw) {
        uint8_t k; for (k = 0; k < NUM_OPTIONS; k++) {
            if (k == option_num)
                Graphics_drawString(&hal_p->gfx.context, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
            else
                Graphics_drawString(&hal_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
        }
        disp_p->redraw = false;
    }

    // Select the indicated option
    if (hal_p->buttons.JSBtapped) {
        disp_p->screen = option_num == OPTION_ONE ? GAME : INSTRUCTIONS;
        disp_p->screen_changed = true;
    }
}


void InstructionsScreen(HAL* hal_p, display_t* disp_p) {
    // Instructions text - CHANGE THE LINES
    static const char* lines[] = {"LB1/LB2: Switch Mode", "Buy(B)/Sell(S)", "JSB: +1 Cookie", "JS L/R: See Upgrade", "JS U/D: See Building", "BB1: B/S Building", "BB2: B/S Upgrade"};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // MainScreen_Text(&hal_p->gfx.context, &j);

        // Clear the display
        GFX_clear(&hal_p->gfx);

        // Set foreground color
        GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);

        uint8_t i = 1;  // Tracking row of current line

        // Print Screen name
        GFX_print(&hal_p->gfx, "INSTRUCTIONS", i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print instructions
        i = GFX_printTextRows(&hal_p->gfx, lines, num_lines, i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print navigations instructions
        GFX_print(&hal_p->gfx, "JSB: Go Back", i, 1);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (hal_p->buttons.JSBtapped) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}


void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* gs_p) {
    static const char* Mode[] = {"Buy ", "Sell"};

    static option_t build_opn = OPTION_ONE;
    static uint16_t *Current_num_p;
    static BigNum *Current_cost_p, *Current_expense_p, *Current_upg_cost_p, *Current_upg_expense_p, *Current_cpi_p;
    static SWTimer cursor_timer, grandma_timer, buzzer_timer;

    static bool Build_isviable = false, Build_wasviable = false;
    bool cookies_changed = false, cps_changed = false, mode_changed = false, varrow_changed = false, cursor_changed = false, grandma_changed = false;

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Clear the display
        GFX_clear(&hal_p->gfx);

        gs_p->cookies = gs_p->grandma_expense = gs_p->cursor_expense = gs_p->grandma_upg_expense = gs_p->cursor_upg_expense = gs_p->grandma_cpi = gs_p->cursor_cpi = gs_p->cps = (BigNum) {0.0, 0};
        gs_p->cursor_num = gs_p->grandma_num = gs_p->cursor_lvl = gs_p->grandma_lvl = 0;
        gs_p->cursor_cost = (BigNum) {CURSOR_STARTING_PRICE, 0}; gs_p->grandma_cost = (BigNum) {GRANDMA_STARTING_PRICE, 0};
        gs_p->cursor_upg_cost = (BigNum) {CURSOR_UPG_STARTING_PRICE, 0}; gs_p->grandma_upg_cost = (BigNum) {GRANDMA_UPG_STARTING_PRICE, 0};
        gs_p->mode = BUY;

        cookies_changed = cps_changed = mode_changed = varrow_changed = cursor_changed = grandma_changed = true;

        GFX_print(&hal_p->gfx, "Cookies",   COOKIES_TEXT_ROW, TEXT_MIN_COL);
        GFX_print(&hal_p->gfx, "CPS",       COOKIES_TEXT_ROW, CPS_TEXT_COL);
        GFX_print(&hal_p->gfx, "BUILDINGS", MODE_TEXT_ROW, TEXT_MIN_COL);
        // GFX_print(&hal_p->gfx, "|",         MODE_TEXT_ROW, MODE_SEP_COL);
        GFX_print(&hal_p->gfx, "  Cursor",  CURSOR_TEXT_ROW, TEXT_MIN_COL);
        GFX_print(&hal_p->gfx, "  Cost",    CURSOR_TEXT_ROW + 1, TEXT_MIN_COL);
        GFX_print(&hal_p->gfx, "  Grandma", GRANDMA_TEXT_ROW, TEXT_MIN_COL);
        GFX_print(&hal_p->gfx, "  Cost",    GRANDMA_TEXT_ROW + 1, TEXT_MIN_COL);

        cursor_timer = SWTimer_construct(CURSOR_INTERVAL_MS);
        grandma_timer = SWTimer_construct(GRANDMA_INTERVAL_MS);
        SWTimer_start(&cursor_timer);
        SWTimer_start(&grandma_timer);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
        disp_p->redraw = true;
    }

    // JSB tapped = +1 Cookie
    if (hal_p->buttons.JSBtapped) {
        gs_p->cookies.base++;
        normalize_v(&gs_p->cookies);
        cookies_changed = true;
    }

    // Add Cursor CPI every 1s
    if (SWTimer_expired(&cursor_timer)) {
        if (gs_p->cursor_num > 0) {
            addsub_v(&gs_p->cookies, &gs_p->cursor_cpi, false);
            cookies_changed = true;
        }
        SWTimer_start(&cursor_timer);
    }

    // Add Grandma CPI every 5s
    if (SWTimer_expired(&grandma_timer)) {
        if (gs_p->grandma_num > 0) {
            addsub_v(&gs_p->cookies, &gs_p->grandma_cpi, false);
            cookies_changed = true;
        }
        SWTimer_start(&grandma_timer);
    }

    // Change Mode
    if (hal_p->buttons.LB1tapped || hal_p->buttons.LB2tapped) {
        gs_p->mode = 1 - gs_p->mode;
        mode_changed = true;

        // If post = yes, cursor_changed and grandma_changed will be set to true here!
    }

    // Move cursor/arrow
    if (hal_p->JS.TappedY != CENTERY) {
        build_opn = hal_p->JS.TappedY == UP ? CircularDecrement(build_opn, NUM_OPTIONS) : CircularIncrement(build_opn, NUM_OPTIONS);
        varrow_changed = true;
    }

    // Change pointers to current building specs
    if (varrow_changed) {       // Building selection changed if this this true (at this point)
        if (build_opn == OPTION_ONE) {
            Current_num_p = &gs_p->cursor_num; Current_cost_p = &gs_p->cursor_cost; Current_expense_p = &gs_p->cursor_expense; Current_cpi_p = &gs_p->cursor_cpi;
        }
        else {
            Current_num_p = &gs_p->grandma_num; Current_cost_p = &gs_p->grandma_cost; Current_expense_p = &gs_p->grandma_expense; Current_cpi_p = &gs_p->grandma_cpi;
        }
    }

    
    // Buy/Sell Building
    if (hal_p->buttons.BB1tapped && Build_isviable) {
        if (gs_p->mode == BUY) {
            addsub_v(&gs_p->cookies, &Current_cost_p, true);        // Subtract the building cost from cookies
            (*Current_num_p)++;                                     // Add a building
            muldiv_v(Current_cost_p, build_opn == OPTION_ONE ? CURSOR_PRICE_MULT : GRANDMA_PRICE_MULT, false);      // Scale up both the (next) buy and sell prices
            muldiv_v(Current_expense_p, build_opn == OPTION_ONE ? CURSOR_PRICE_MULT : GRANDMA_PRICE_MULT, false);
        }
        else {
            addsub_v(&gs_p->cookies, &Current_expense_p, false);        // Add the building expense to cookies
            (*Current_num_p)--;                                         // Subtract a building
            muldiv_v(Current_cost_p, build_opn == OPTION_ONE ? CURSOR_PRICE_MULT : GRANDMA_PRICE_MULT, true);      // Scale down both the (next) buy and sell prices
            muldiv_v(Current_expense_p, build_opn == OPTION_ONE ? CURSOR_PRICE_MULT : GRANDMA_PRICE_MULT, true);
        }
        
        // NOTE: CPI is per building unit, so is unchanged by building number

        cookies_changed = true;
        cps_changed = true;
        cursor_changed = (build_opn == OPTION_ONE);
        grandma_changed = (build_opn == OPTION_TWO);
    }

    // Check if Building B/S viable
    if (cookies_changed || mode_changed) {
        Build_isviable = (gs_p->mode == BUY && GE_BigNum(&gs_p->cookies, Current_cost_p)) || (gs_p->mode == SELL && *Current_num_p > 0);
        if (Build_isviable != Build_wasviable) varrow_changed = true;
        Build_wasviable = Build_isviable;
    }
    
    // Print Vertical (Bulding selector) arrow
    if (varrow_changed) {
        if (!Build_isviable) GFX_setForeground(&hal_p->gfx, TEXT_DARK_COLOR);
        GFX_print(&hal_p->gfx, ">",  build_opn == OPTION_ONE ? CURSOR_TEXT_ROW : GRANDMA_TEXT_ROW, STATS_COL);
        GFX_print(&hal_p->gfx, "  ", build_opn == OPTION_TWO ? CURSOR_TEXT_ROW : GRANDMA_TEXT_ROW, STATS_COL);
        if (!Build_isviable) GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);
    }

    // Print the other stats
    if (cookies_changed) PrintCookies(hal_p, gs_p);
    
    if (mode_changed) GFX_print(&hal_p->gfx, Mode[gs_p->mode], MODE_TEXT_ROW, MODE_COL);
    
    if (cps_changed) PrintCPS(hal_p, gs_p);

    if (cursor_changed) PrintCursor(hal_p, gs_p);

    if (grandma_changed) PrintGrandma(hal_p, gs_p);

}