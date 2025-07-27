/*
 * pwm_buzzer.h
 *
 *  Created on: Mar 22, 2024
 *      Author: josha
 */

#ifndef PWM_BUZZER_H_
#define PWM_BUZZER_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void PwmBuzzerInit();

/*
 * Set the LED parameters for the three sub-LEDs
 */
void PwmBuzzerSet(int pulsePeriod, int pulseWidth);

#endif /* PWM_BUZZER_H_ */
