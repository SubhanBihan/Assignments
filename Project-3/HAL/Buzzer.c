/*
 * Buzzer.c
 *
 *  Created on: Nov 12, 2025
 *      Author: Richard Harrison
 */

#include "HAL/Buzzer.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// TODO: Set the tick per ms for the clock source
#define CLKSOURCE_VAL          1
#define CLKSOURCE_DIV_VAL      1

// TODO: Set up timer for duration
#define DT_DIVIDER            1
#define DT_DIVIDER_VAL        1
#define DT_TICKS_PER_MS       1

note_t Note_Construct(float frequency_hz, uint32_t duration_ms, float duty_cycle)
{
    const static float frequency_base = CLKSOURCE_VAL / CLKSOURCE_DIV_VAL;

    note_t note;

    // TODO: Set the basic values of notes

    // TODO: Compute the period and compare counts

    return note;
}

void Buzzer_Init()
{
    // TODO: Initialize buttons to correct port & pin in header

    // TODO: Set up TimerPWM config

    // TODO: Enable countdown timer

}

void Buzzer_Play(note_t* note)
{
    // TODO: Fill TimerPWM config from note

    // TODO: Generate PWM

    // TODO: Configure duration timer upgrade mode

    // Start timer
}

void Buzzer_Stop()
{
}
