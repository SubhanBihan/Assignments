#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define LEFT_THRESHOLD  1500
#define RIGHT_THRESHOLD 1700
#define DOWN_THRESHOLD  1500
#define UP_THRESHOLD    1700

// This function initializes all the peripherals except graphics
// void initialize();
// void ModifyLEDColor(bool leftButtonWasPushed, bool rightButtonWasPushed);

void initADC();
void startADC();
void initJoyStick();
void getSampleJoyStick(unsigned *X, unsigned *Y);

// int main(void)
// {

//     Graphics_Context g_sContext;

//     initialize();
//     InitGraphics(&g_sContext);
//     draw_Base(&g_sContext);

//     unsigned vx, vy;

//     while (1)
//     {

//         getSampleJoyStick(&vx, &vy);
//         bool joyStickPushedtoRight = false;
//         bool joyStickPushedtoLeft = false;
//         drawXY(&g_sContext, vx, vy);

//         if (vx < LEFT_THRESHOLD)
//         {
//             joyStickPushedtoLeft = true;
//         }

//         MoveCircle(&g_sContext, joyStickPushedtoLeft,joyStickPushedtoRight);
//      }
// }




// void initialize()
// {
//     // stop the watchdog timer
//     WDT_A_hold(WDT_A_BASE);

//     initADC();
//     initJoyStick();
//     startADC();
// }

#endif
