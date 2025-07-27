/*
 * pwm_buzzer.c
 *
 *  Created on: Mar 22, 2024
 *      Author: josha
 */
#include <pwm_buzzer.h>

/*
 * Initialize the timer PWM functions connected to the buzzer.
 * Buzzer:       J17 ,PC5, WT0CCP1, WTimer 0, sub-timer B
 */
void PwmBuzzerInit()
{
    // Enable WTimer 0 and GPIO Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Connect pins to those timers
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);


    // Select PWM for Timer 2 sub-Timer B,
    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // Invert the PWM waveform, so that the Match register value is the pulse width.
    // Otherwise, the pulse width will be (Load value) - (Match value).
    // The inversion is done by enabling output inversion on the PWM pins.
    TimerControlLevel(WTIMER0_BASE, TIMER_B, true /* output inversion */);

    // Enable the WTimer 0's timer B
    TimerEnable(WTIMER0_BASE, TIMER_B);

}

/*
 * Set sound for the buzzer peripheral by setting the intensity of the pulse
 */
void PwmBuzzerSet(int pulse_period, int pulse_width)
{
    // Set the PWM parameters for Buzzer
    TimerLoadSet(WTIMER0_BASE, TIMER_B, pulse_period - 1);
    TimerMatchSet(WTIMER0_BASE, TIMER_B, pulse_width - 1);
}
