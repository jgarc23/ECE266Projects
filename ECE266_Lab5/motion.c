/******************************************************************************
 * Lab 5 motion.c code
 * ECE 266, spring 2024
 *
 * motion.c: This file contains the initialization function for the motion sensor.
 *
 * Created by Joshua Garcia
 *****************************************************************************/

#include "motion.h"
#include <driverlib/rom_map.h>
#include <driverlib/rom.h>
#include <driverlib/gpio.h>

/*
 * Global declarations
 */

// Motion sensor input state
// DECLARE TO HAVE VARIABLES
typedef struct {
    bool pin_value;
    Event *callback_event;
    uint32_t debouncing_delay;

} MotionSensorState;

MotionSensorState motion_sensor;

// pre-declare the ISR function
static void MotionSensorISR();

/******************************************************************************
 * Initialize the motion sensor with interrupt enabled
 * Pin usage: Grove base port J16, Tiva C PC4 (Port C, Pin 4)
 *****************************************************************************/

void MotionSensorInit() // SAME AS BEFORE, ONLY ADDED MOTION SENSOR,
{
    //Initialize motion sensor state
    motion_sensor.callback_event = NULL;
    motion_sensor.debouncing_delay = 100;
    bool pin_value = false;

    // Enable Port C and configure PC4
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    // Set interrupt on Port C, pin 4 (motion sensor)
    GPIOIntRegister(GPIO_PORTC_BASE, MotionSensorISR); // register the interrupt handler
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, // interrupt on falling edge, change to rising edge?
                  GPIO_BOTH_EDGES);
    IntPrioritySet(INT_GPIOC, 0); // set interrupt level to 0 (0 is the highest for programmable interrupts)
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4); // enable interrupts on motion sensor input
}

void MotionSensorEventRegister(Event* callback_event) // REGISTER THE WHOLET THING
{
    assert(EventInitialized(callback_event));
    motion_sensor.callback_event = callback_event;
}


void MotionSensorSetDebouncingDelay(int debouncing_delay)
{
    motion_sensor.debouncing_delay = debouncing_delay;
}

/*
 * Motion Sensor ISR
 */
static void MotionSensorISR()
{
    static uint32_t last_event_time = 0;      // remember last pushing time

    // Read Port C. Motion sensor is active high so read normally
    bool pin_value = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);

    uint32_t current_time = EventGetCurrentTime();
    if (current_time > last_event_time + motion_sensor.debouncing_delay)
    {
        motion_sensor.pin_value = pin_value;
        last_event_time = current_time;
    }
    // Schedule callback event
    if (motion_sensor.callback_event != NULL)
    {
        EventSchedule(motion_sensor.callback_event, EventGetCurrentTime());
    }

    // IMPORTANT: Clear interrupt flag
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);

}


