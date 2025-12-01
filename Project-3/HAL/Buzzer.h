/*
 * Buzzer.h
 *
 *  Created on: Nov 12, 2025
 *      Author: Richard Harrison
 */

#ifndef HAL_BUZZER_H_
#define HAL_BUZZER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "HAL/pitches.h"

// TODO: Set the following values to the correct values for the buzzer
#define BUZZER_PORT                 1
#define BUZZER_PIN                  1

#define BUZZER_TIMER                1
#define BUZZER_CHANNEL              1

// TODO: Set the following values to the correct values for the pwm
#define PWM_CLKSOURCE               1
#define PWM_CLKSOURCE_DIVIDER       1
#define PWM_COMPARE_OUTPUT_MODE     1
#define PWM_COMPARE_REGISTER        1

typedef struct {
    float frequency_hz;
    uint32_t duration_ms;
    float duty_cycle;

    uint16_t period_counts;
    uint16_t compare_counts;
} note_t;

note_t Note_Construct(float frequency_hz, uint32_t duration_ms, float duty_cycle);

void Buzzer_Init();

void Buzzer_Play(note_t* note);

void Buzzer_Stop();

#endif /* HAL_BUZZER_H_ */
