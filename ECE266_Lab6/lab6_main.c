/*
 * Starter code for ECE 266 Lab 6, main.c, spring 2024
 *
 * Lab 6: Ambient Sensing (ADC)
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "temp_sensor.h"
#include "light_sensor.h"

// The state of the 7-segment display. Initially it shows "00:00" with the colon turned on.
Seg7Display seg7 = { { 10, 10, 10, 10 }, true /* colon on */};



// The events
Event tsensor_trigger_reading_event;
Event tsensor_check_reading_event;
Event lsensor_trigger_reading_event;
Event lsensor_check_reading_event;

/*
 * Trigger the light sensor reading periodically
 */
void LsTriggerReading(Event *event)
{
    // Trigger ADC sampling for the light sensor
    LsTriggerSampling();

    // Schedule the next callback in 4000 milliseconds
    EventSchedule(event, event->time + 4000);
}

/*
 * Check light sensor reading, print if ready
 */
void LsProcessReading(Event *event)
{
    // Get the current reading for the light sensor
    uint32_t light_data = LsReadData();

    // Print on terminal. Note: TivaWare printf() has issue with "%f"
    uprintf("Light Sensor Reading: %u \r\n", (uint32_t)light_data);

    // Extract digits to display to 7-seg.
    int digit[4];
    int temp_data = light_data;
    int i = 0;

    if (temp_data != 0) {
        // Fill digit array with digits of the light sensor data.
        while (temp_data > 0)
        {
            digit[i] = temp_data % 10;
            temp_data = temp_data / 10;
            i++;
        }

        // Assign digits to corresponding 7-seg digit.
        seg7.digit[0] = digit[0];
        seg7.digit[1] = digit[1];
        seg7.digit[2] = digit[2];
        seg7.digit[3] = digit[3];
    }
    else {
        // If light sensor data = 0 manually assign 7-seg values.
        seg7.digit[0] = 0;
        seg7.digit[1] = 10;
        seg7.digit[2] = 10;
        seg7.digit[3] = 10;
    }

/* IGNORE
//    int j;
//
//    for (j = 3; j>=0; j--) {
//        if (seg7.digit[j] == 0 && j!=0) {
//            seg7.digit[j] = 10;
//        }
//        else {
//            break;
//        }
//    }
*/
    seg7.colon_on = false;

    // Update 7-seg
    Seg7Update(&seg7);
}

/*
 * Trigger the temperature sensor reading periodically
 */
void TsTriggerReading(Event *event)
{
    // Trigger ADC sampling for the temperature sensor
    TsTriggerSampling();

    // Schedule the next callback in 4000 milliseconds
    EventSchedule(event, event->time + 4000);
}

/*
 * Check temperature sensor reading, print if ready
 */
void TsProcessReading(Event *event)
{
    // Get the current reading and convert it to temperature in Celsius
    // See Tiva C datasheet (ver. DS-TM4C123GH6PM-15842.2741), page 813
    uint32_t raw_data = TsReadData();
    float temp_C = 147.5 - (75 * 3.3 * (float) raw_data / 4096);

    // Print on terminal. Note: TivaWare printf() has issue with "%f"
    uprintf("Temperature Sensor Reading: %u.%u C\r\n", (uint32_t)temp_C, (uint32_t)(temp_C*10)%10);

    // Convert to Fahrenheit
    float temp_F = (temp_C * 1.8) + 32;
    int tempdecimal = 10*temp_F - 10*(int)temp_F;
    seg7.digit[0] = 10;
    seg7.digit[1] = tempdecimal;
    seg7.digit[2] = (int)temp_F % 10;
    seg7.digit[3] = (int)temp_F / 10;
    seg7.colon_on = true;

    Seg7Update(&seg7);
}


/*
 * The main function: Initialize and the execute the callback scheduler loop
 */
int main(void)
{
    LaunchPadInit();
    TsInit();
    Seg7Init();
    LsInit();

    // Initialize the event objects
    EventInit(&tsensor_trigger_reading_event, TsTriggerReading);
    EventInit(&tsensor_check_reading_event, TsProcessReading);
    EventInit(&tsensor_trigger_reading_event, TsTriggerReading);

    EventInit(&lsensor_trigger_reading_event, LsTriggerReading);
    EventInit(&lsensor_check_reading_event, LsProcessReading);
    EventInit(&lsensor_trigger_reading_event, LsTriggerReading);
    // Register ISR event
    TsEventRegister(&tsensor_check_reading_event);
    LsEventRegister(&lsensor_check_reading_event);

    // Schedule time event
    EventSchedule(&tsensor_trigger_reading_event, 100);
    EventSchedule(&lsensor_trigger_reading_event, 2100);


    uprintf("%s\n\r", "Lab 6: Ambient Sensing (ADC)");

    // Loop forever
    while (true)
    {
        // Wait for interrupt to happen
        __asm("    wfi");

        // Call the callback scheduler
        EventExecute();
    }
}
