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
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

// Initialize the motion sensor
void MotionInit();

//Read motion sensor input
bool MotionRead();


#endif /* MOTION_H_ */
