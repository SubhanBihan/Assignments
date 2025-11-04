#include <Application.h>

void ScreenSelect(HAL* hal_p, Application* app_p) {
    switch (app_p->disp.screen)
    {
        case TITLE:
            TitleScreen(hal_p, &app_p->disp);
            break;
        
        case MAIN:
            MainScreen(hal_p, &app_p->disp);
            break;

        case INSTRUCTIONS:
            InstructionsScreen(hal_p, &app_p->disp);
            break;
        
        case HS:
            HSScreen(hal_p, &app_p->disp);
            break;
        
        // case GAME:
        //     GameScreen(hal_p, &app_p->disp);
        //     break;
        
        // case GAMEOVER:
        //     GameOverScreen(hal_p, &app_p->disp);
        //     break;
        
        default:
            break;
    }
}


void Application_loop(HAL* hal_p, Application* app_p) {
    // // Restart/Update communications if first run or S2 is pressed
    // if (Button_isTapped(&hal_p->launchpadS2) || app_p->firstCall) {
    //     Application_updateCommunications(app_p, hal_p);
    // }

    ScreenSelect(hal_p, app_p);

    // if (app_p->disp.screen == GRIDSELECT && app_p->game_state.defstats != STATS_DONE)   // Take UART input while in game or gridselect screen
    //     StatsInput(app_p, hal_p);
    // else if (app_p->disp.screen == GAME)
    //     GameInput(app_p, hal_p);


    // app_p->counter++;     // Update software counter every loop  
}