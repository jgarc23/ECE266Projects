/*
 * light_sensor.h:
 *
 * Light sensor module
 *
 * Created by Joshua Garcia
 */

#ifndef ANGLE_SENSOR_H_
#define ANGLE_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "launchpad.h"

// Initialize ADC to use rotary angle sensor
void AsInit();

// Trigger rotary angle reading
void AsTriggerSampling();

// Check if any new reading is ready (for I/O polling)
bool AsDataReady();

// Read the most recent rotary angle reading
uint32_t AsReadData();

// Set callback event for rotary angle sensor ISR
void AsEventRegister(Event *event);

#endif /* ANGLE_SENSOR_H_ */
