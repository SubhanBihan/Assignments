/*
 * Timer.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#ifndef HAL_TIMER_H_
#define HAL_TIMER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define MS_DIVISION_FACTOR 1000     // Number of milliseconds in one second
#define US_DIVISION_FACTOR 1000000  // Number of microseconds in one second

// A globally-defined system clock variable. Changing this variable will change
// the system clock across the ENTIRE BOARD. Any API calls which use the system
// clock as part of its timing therefore should parameterize their variables to
// this #define and thus #include <API/Timer.h>.
#define SYSTEM_CLOCK 48000000
#define CLOCK_CYCLES_IN_MS \
  (SYSTEM_CLOCK / 1000)  // number of clock cycles in 1ms

#define LOADVALUE 0xFFFFFFFF
#define PRESCALER 1

// Initializes the global clock system for the MSP432, as well as hardware
// timers.
void InitSystemTiming();

//TODO set up hardware timers to use
void StartHWTimer1(uint32_t waitTime_ms);
void StartHWTimer2(uint32_t waitTime_ms);

bool HWTimer1Expired();
bool HWTimer2Expired();

#endif /* HAL_TIMER_H_ */
