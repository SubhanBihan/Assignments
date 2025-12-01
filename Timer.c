/*
 * Timer.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#include "HAL/LED.h"
#include "HAL/Timer.h"

/**
 * Initializes the global system timing. This function should be called
 * immediately after the Watchdog timer is reset, so that the system clock is
 * set appropriately.
 *
 * To change the system clock to different frequencies, use the #define on the
 * SYSTEM_CLOCK in Timer.h. DO NOT MODIFY THIS FUNCTION UNLESS YOU KNOW WHAT YOU
 * ARE DOING. You can potentially brick your board, which requires a factory
 * reset to fix.
 */
void InitSystemTiming() {
    // Before initializing anything else, disable all interrupts
    Interrupt_disableMaster();

    // Before changing the clock frequency, we need to change the flash control to
    // use 2 wait states (2 delayed cycles per flash read). IF YOU DO NOT CHANGE
    // YOUR FLASH CONTROL BEFORE CALLING CS_setDCOFrequency(), YOU WILL BRICK YOUR
    // BOARD AND WILL NEED TO PERFORM A FACTORY RESET.
    //
    // The reason why we need to change the Flash Control settings is because
    // although calling CS_setDCOFrequency() may change the system clock frequency
    // itself, the flash memory has NOT been configured for instruction fetches at
    // a higher frequency. Omitting these two lines WILL cause your board to
    // attempt to fetch instructions before flash memory has properly fetched
    // them, meaning your board will begin to read garbage instructions and will
    // no longer be flashable without a factory reset.
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    // Set the system clock frequency to user-specified frequency
    CS_setDCOFrequency(SYSTEM_CLOCK);

    // After DCO is set, configure all other clock signals to use a source from
    // DCO. DO NOT CHANGE THIS
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2); // TODO: Calculate new frequency for SMCLK
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);


    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
                       TIMER32_PERIODIC_MODE);
    //TODO: intialize the other Timer32 instance


    //TODO: clear the relevant interrupts and talk to the nvic controller
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
    Timer32_enableInterrupt(TIMER32_0_BASE);

    // Enable interrupts again, after all system timing has been set up properly
    Interrupt_enableMaster();
}

//TODO
void StartHWTimer1(uint32_t waitTime_ms) {
}

//TODO
void StartHWTimer2(uint32_t waitTime_ms) {
}

//TODO
bool HWTimer1Expired() {
    return false;
}

//TODO
bool HWTimer2Expired() {
    return false;
}
