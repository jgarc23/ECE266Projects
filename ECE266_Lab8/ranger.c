/*
 * range.c: Starter code for ECE 266 Lab 8, main.c, spring 2024
 *
 * Lab 8: Distance Measurement
 *
 * YOU NEED TO REVISE THE CODE IN PART 2 AND PART 3.
 *
 * Created by Zhao Zhang
 * Last update: 3/14/2024
 */

#include "ranger.h"

/*
 * Hardware configure: The ultrasonic ranger (and its signal pin) shall be connected to:
 *      Grove baseboard: Port J6, Pin 24
 *      Tiva C: GPIO PD1
 *      Timer pin: WT2CCP1
 */

/*
 * Global/static variables and declarations
 */
typedef struct {
    Event *callback_event;
    enum {
        IDLE,                   // ranger is idle
        EXPECT_START_EVENT_1,   // expecting the rising edge of the start pulse
        EXPECT_START_EVENT_2,   // expecting the falling edge of the start pulse
        EXPECT_DATA_EVENT_1,    // expecting the rising edge of the data pulse
        EXPECT_DATA_EVENT_2     // expecting the falling edge of the data pulse
    } state;
    bool new_data_ready;
    uint32_t start_time;  // Timestamp for start pulse
    uint32_t end_time;    // Timestamp for end pulse
} RangerState;

volatile RangerState ranger;

void RangerISR();

/*
 * Initialize the ranger. It should be connected to Grove jumper J6 (SIG connected to
 * pin 24). The GPIO pin is PD1, and the time pin is WT2CCP1. The timer is WTIMER2 and the
 * sub-Time is B.
 */
void RangerInit()
{
    // Initial ranger state in memory
    ranger.callback_event = NULL;
    ranger.new_data_ready = false;
    ranger.state = IDLE;

    // Enable GPIO Port D and Wide Timer 2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);

    // Configure PD2 as timer pin and connect it to WT2CCP1
    // Do NOT enable the time as this time
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PD1_WT2CCP1);

    // Count both edges as event
    TimerControlLevel(WTIMER2_BASE, TIMER_B, true);
    TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

    // Register an ISR to deal with the timeout event
    TimerIntRegister(WTIMER2_BASE, TIMER_B, RangerISR);
    TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);
}

/*
 * Send the start pulse to the ultrasonic ranger. The pin being used will be switched
 * to timer input pin at the end.
 */
void RangerTriggerReading()
{
    // Clean data ready flag and set the ranger state
    ranger.callback_event = NULL;
    ranger.new_data_ready = false;
    ranger.state = EXPECT_START_EVENT_1;

    // It is safe to disable a peripheral during configuration
    TimerDisable(WTIMER2_BASE, TIMER_B);

    // Configure WT2CCP1 for PWM to generate a pulse of 5 microseconds, with
    // 2 microseconds of leading low time
    TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER2_BASE, TIMER_B, 1000);
    TimerMatchSet(WTIMER2_BASE, TIMER_B, 500);
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);

    // Enable the timer, and start PWM waveform
    TimerEnable(WTIMER2_BASE, TIMER_B);
}

/*
 * If any new data is ready (for polling)
 */
bool RangerDataReady()
{
    return ranger.new_data_ready;
}

// Gets the raw data for the pulse width and returns it.
uint32_t RangerGetData()
{
    // Calculate pulse width of data pulse.
    uint32_t pulse_width = ranger.end_time - ranger.start_time;
    return pulse_width;
}

// Registers the ISR event.
void RangerEventRegister(Event *event)
{
    assert(EventInitialized(event));
    ranger.callback_event = event;
}

/*
 * RangerISR: Process the events in the start pulse (from Tiva C to ranger)
 * and the data pulse (from ranger to Tiva C). It implements a finite 
 * state machine in software.
 *
 * There are four events to process:
 * 1. Expecting the rising edge of the start pulse
 * 2. Expecting the falling edge of the start pulse
 * 3. Expecting the rising edge of the data pulse
 * 4. Expecting the falling edge of the data pulse
 *
 * You ISR needs to process each event approximately.
 * Event 3 and 4 mark the start and end of the the data pulse.
 */
void RangerISR()
{
    switch (ranger.state)
    {
    case EXPECT_START_EVENT_1:
        ranger.state = EXPECT_START_EVENT_2;
        break;

    case EXPECT_START_EVENT_2:
        ranger.state = EXPECT_DATA_EVENT_1;

        // It is safe to disable a peripheral during configuration
        TimerDisable(WTIMER2_BASE, TIMER_B);

        // Reconfigure WT2CCP1 sub-timer B for Time Capture
        TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP);
        TimerLoadSet(WTIMER2_BASE, TIMER_B, 1000000000000000); //Load must be set to a largr number
        TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);

        // Enable the timer, and start time capture
        TimerEnable(WTIMER2_BASE, TIMER_B);

        break;

    case EXPECT_DATA_EVENT_1:
        ranger.state = EXPECT_DATA_EVENT_2;
        ranger.start_time = TimerValueGet(WTIMER2_BASE, TIMER_B);

        break;

    case EXPECT_DATA_EVENT_2:
        ranger.end_time = TimerValueGet(WTIMER2_BASE, TIMER_B);
        ranger.new_data_ready = true;
        ranger.state = IDLE;
        RangerGetData();

        break;
    }

    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
}
