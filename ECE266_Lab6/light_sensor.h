/*
 * light_sensor.h:
 *
 * Light sensor module
 *
 * Created by Joshua Garcia
 */

#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "launchpad.h"

// Initialize ADC to use Tiva C's internal temperature sensor
void LsInit();

// Trigger temperature reading
void LsTriggerSampling();

// Check if any new reading is ready (for I/O polling)
bool LsDataReady();

// Read the most recent temperature reading
uint32_t LsReadData();

// Set callback event for temperature sensor ISR
void LsEventRegister(Event *event);

void LightSensorISR();

#endif /* LIGHT_SENSOR_H_ */
