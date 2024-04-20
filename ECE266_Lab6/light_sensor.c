/*
 * light_sensor.c
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
#include "light_sensor.h"
#include "launchpad.h"

/*
 * Global declarations
 */

// Light sensor reading states
typedef struct {
    Event *callback_event;      // the callback event for data processing
    uint32_t raw_data;          // the raw reading from the ADC connected to the sensor
    bool new_data_ready;        // if any new data is ready (for I/O polling)
} LightSensorState;

LightSensorState light_sensor;

// Pre-declare the ISR function
//static void LightSensorISR();

/*
 * Initialize ADC to use Tiva C's internal temperature sensor
 *
 * Resources: ADC0, sequence #0, special channel LS (LIGHT sensor)
 * Configurations: processor trigger, interrupt enabled, use step 0 only
 */
void LsInit()
{
    // Initialize sensor state in memory
    light_sensor.callback_event = NULL;
    light_sensor.new_data_ready = false;

    // Enable the ADC0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Configure ADC0's sequencer #0
    ADCSequenceConfigure(ADC0_BASE, 1 /* sequencer */, ADC_TRIGGER_PROCESSOR, 0 /* priority */);

    // Configure step 0 of sequencer 0 to use the temperature sensor, with
    // interrupt enable, as the end of the sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1 /* sequencer */, 0 /* step */,
                             ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);

    // Enable ADC0, sequencer 1
    ADCSequenceEnable(ADC0_BASE, 1 /* sequencer */);

    // Configure the interrupt system for temperature sensor
    ADCIntRegister(ADC0_BASE, 1 /* sequencer */, LightSensorISR);
    ADCIntEnable(ADC0_BASE, 1 /* sequencer */);
}

/*
 * Trigger temperature reading
 */
void LsTriggerSampling()
{
    // Trigger the sampling
    ADCProcessorTrigger(ADC0_BASE, 1 /* sequencer */);
}

/*
 * Check if any new data is ready (for I/O polling)
 */
bool LsDataReady()
{
    return light_sensor.new_data_ready;
}

/*
 * Read the current input from the temperature sensor
 */
uint32_t LsReadData()
{
    // Reset new data ready flag and return the reading
    light_sensor.new_data_ready = false;
    return light_sensor.raw_data;
}

/*
 * Set callback event for temperature sensor ISR
 */
void LsEventRegister(Event *event)
{
    light_sensor.callback_event = event;
}

/*
 * ISR for reading temperature data. It is called when ADC conversion is done.
 */

///*static void LightSensorISR()
//{
//    // Read the ADC data
//    ADCSequenceDataGet(ADC0_BASE, 1 /* sequencer */,
//                       &light_sensor.raw_data /* pointer to data buffer */);
//
//    // Set the flag for new data ready
//    light_sensor.new_data_ready = true;
//
//    // Schedule the callback event
//    if (light_sensor.callback_event != NULL)
//        EventSchedule(light_sensor.callback_event, EventGetCurrentTime());
//
//    // IMPORTANT: Clear the interrupt flag
//    ADCIntClear(ADC0_BASE, 1 /* sequencer */);
//}
//*/

