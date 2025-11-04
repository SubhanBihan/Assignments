#include <Display.h>

void TitleScreen(HAL* hal_p, display_t* disp_p) {
    if (disp_p->screen_changed) {
        // Game name
        GFX_print(&disp_p->gfx, "Touhou", 1, 1);

        // Dev's name
        GFX_print(&disp_p->gfx, "Samin Rahman", 1, 1);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        
    }
}