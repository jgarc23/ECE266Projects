/*
 * temp_sensor.h: Starter code for ECE 266 Lab 6, spring 2024
 *
 * Temperature sensor module
 *
 * Created by Zhao Zhang, fall 2022
 * Last update: Feb. 18, 2024
 */

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>
#include "launchpad.h"

// Initialize ADC to use Tiva C's internal temperature sensor
void TsInit();

// Trigger temperature reading
void TsTriggerSampling();

// Check if any new reading is ready (for I/O polling)
bool TsDataReady();

// Read the most recent temperature reading
uint32_t TsReadData();

// Set callback event for temperature sensor ISR
void TsEventRegister(Event *event);

#endif /* TEMP_SENSOR_H_ */
