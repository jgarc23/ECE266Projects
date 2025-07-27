/*
 * rotary_angle_sensor.c
 *
 *  Created on: Mar 14, 2024
 *      Author: jdcam
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/adc.h>
#include <rotary_angle_sensor.h>
#include "launchpad.h"

/*
 * Global declarations
 */

// Light sensor reading states
typedef struct {
    Event *callback_event;      // the callback event for data processing
    uint32_t raw_data;          // the raw reading from the ADC connected to the sensor
    bool new_data_ready;        // if any new data is ready (for I/O polling)
} AngleSensorState;

AngleSensorState angle_sensor;

// Pre-declare the ISR function
static void AngleSensorISR();

/*
 * Initialize ADC to use rotary angle sensor
 *
 * Resources: J5, ADC0, sequencer #1,
 * Configurations: processor trigger, interrupt enabled, use step 0 only
 */
void AsInit()
{
    // Initialize sensor state in memory
    angle_sensor.callback_event = NULL;
    angle_sensor.new_data_ready = false;

    // Enable the ADC0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Configure ADC0's sequencer #1
    ADCSequenceConfigure(ADC0_BASE, 1 /* sequencer */, ADC_TRIGGER_PROCESSOR, 0 /* priority */);

    // Configure step 0 of sequencer 1 to use the rotary angle sensor, with
    // interrupt enable, as the end of the sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1 /* sequencer */, 0 /* step */,
                             ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);

    // Enable ADC0, sequencer 1
    ADCSequenceEnable(ADC0_BASE, 1 /* sequencer */);

    // Configure the interrupt system for rotary angle sensor
    ADCIntRegister(ADC0_BASE, 1 /* sequencer */, AngleSensorISR);
    ADCIntEnable(ADC0_BASE, 1 /* sequencer */);
}

/*
 * Trigger rotary angle reading
 */
void AsTriggerSampling()
{
    // Trigger the sampling
    ADCProcessorTrigger(ADC0_BASE, 1 /* sequencer */);
}

/*
 * Check if any new data is ready (for I/O polling)
 */
bool AsDataReady()
{
    return angle_sensor.new_data_ready;
}

/*
 * Read the current input from the rotary angle sensor
 */
uint32_t AsReadData()
{
    // Reset new data ready flag and return the reading
    angle_sensor.new_data_ready = false;
    return angle_sensor.raw_data;
}

/*
 * Set callback event for rotary angle sensor ISR
 */
void AsEventRegister(Event *event)
{
    angle_sensor.callback_event = event;
}

/*
 * ISR for reading rotary angle data. It is called when ADC conversion is done.
 */

static void AngleSensorISR()
{
    // Read the ADC data
    ADCSequenceDataGet(ADC0_BASE, 1 /* sequencer */,
                       &angle_sensor.raw_data /* pointer to data buffer */);

    // Set the flag for new data ready
    angle_sensor.new_data_ready = true;

    // Schedule the callback event
    if (angle_sensor.callback_event != NULL)
        EventSchedule(angle_sensor.callback_event, EventGetCurrentTime());

    // IMPORTANT: Clear the interrupt flag
    ADCIntClear(ADC0_BASE, 1 /* sequencer */);
}


