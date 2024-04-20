/*
 * motion.h
 *
 *  Created on: Feb 15, 2024
 *      Author: Joshua Garcia
 */

#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_gpio.h>
#include <inc/hw_adc.h>
#include <inc/hw_timer.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/adc.h>
#include <driverlib/timer.h>
#include <assert.h>

#include "event.h"

// Initialize the motion sensor
void MotionSensorInit();

//Read motion sensor input
bool MotionSensorRead();

//Set the callback event for the motion sensor ISR
void MotionSensorEventRegister(Event* callback_event);

void MotionSensorSetDebouncingDelay(int debouncing_delay);

#endif /* MOTION_H_ */
