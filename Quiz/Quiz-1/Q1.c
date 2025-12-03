
typedef struct {int a, b, c;} Graphics_Context;

void InitGraphics (Graphics_Context *g_sContext_p);
void intilialize (Graphics_Context *g_sContext_p) {
    InitGraphics(g_sContext_p);
}

int main () {
    Graphics_Context g_sContext;

    InitGraphics(&g_sContext);
}
