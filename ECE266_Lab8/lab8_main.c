/*
 * lab8_main.c: Starter code for ECE 266 Lab 8, main.c, spring 2024
 * Note: In this semester, a small part of starter code is provided
 * (none was provided before).
 *
 * Lab 8: Distance Measurement
 *
 * Created by Zhao Zhang
 * Last update: 3/14/2024
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
#include "seg7.h"
#include "ranger.h"

/*
 * Global variables
 */

// The state of the 7-segment display. Initially it shows "00:00" with the colon turned on.
Seg7Display seg7 = { { 10, 10, 10, 10 }, false /* colon on */};

// System state
typedef struct {
    enum {Millimeter, Inch} display_mode;       // display inch or millimeter
    int pulse_width;
} SysState;

// The events
Event check_push_button_event;
Event trigger_ranger_reading_event;

SysState sys = {Millimeter} /* Millimeter Mode*/;
/*******************************************
 * Task 1: Trigger and read the ranger
 ******************************************/

// Trigger the ranger reading every 0.5 seconds
void TriggerRangerReading(Event *event)
{
    RangerTriggerReading();
    sys.pulse_width = RangerGetData();
    uprintf("Ranger Sensor Reading: %u.%u \r\n", (uint32_t)sys.pulse_width);

    int distance;         // Used for storing converted distance in either millimeters or inches/feet
    int digit[4];         // Used for temporary array
    int temp_data;
    int temp_data2;
    int i = 0;

    if (sys.display_mode == Millimeter)
    {
        // Convert pulse width to millimeters using the formula:
        // distance =  1000 * (pulse_width * sound_speed) / (2 * clock_frequency)
        distance = 1000 * (sys.pulse_width * SOUND_SPEED_M_PER_SEC) / (2 * CPU_CLOCK_RATE);

        //uprintf("Ranger Sensor Reading Millimeters: %u mm\r\n", (uint32_t)distance);

        // Set limit to 9999
        if (distance > 9999) {
            distance = 9999;
        }
        temp_data = distance;

        // Assign temp data array with distance values
        if (temp_data != 0)
        {
            // Clear digit array
            digit[0] = 10;
            digit[1] = 10;
            digit[2] = 10;
            digit[3] = 10;

            // Fill digit array with digits of the distance data (millimeters).
            while (temp_data > 0)
            {
                digit[i] = temp_data % 10;
                temp_data = temp_data / 10;
                i++;
            }
        }
        else {
                // If distance data = 0 manually assign seg-7 values.
                seg7.digit[0] = 0;
                seg7.digit[1] = 10;
                seg7.digit[2] = 10;
                seg7.digit[3] = 10;
        }
        seg7.colon_on = false;

    } /* end of Millimeter Mode if statement*/

    else if (sys.display_mode == Inch)
        {
            // Find distance in millimeters and then convert to inches
            distance = 1000 * (sys.pulse_width * SOUND_SPEED_M_PER_SEC) / (2 * CPU_CLOCK_RATE);
            distance = distance/25.4;

            //uprintf("Ranger Sensor Reading Inches: %u in\r\n", (uint32_t)distance);

            // Set limit to 9999
            if (distance > 9999) {
                distance = 9999;
            }

            // Assign distance value to temp variable.
            temp_data = distance;

            // Assign temp data array with distance values
            if (temp_data != 0)
            {
                // Clear digit array
                digit[0] = 10;
                digit[1] = 10;
                digit[2] = 10;
                digit[3] = 10;

                // Fill digit array with digits of distance data (feet:inches)
                temp_data2 = temp_data % 12;    // Extract inches
                digit[0] = temp_data2 % 10;     // Extract first digit of inches
                digit[1] = temp_data2 / 10;     // Extract second digit of inches
                temp_data2 = temp_data / 12;    // Extract feet
                digit[2] = temp_data2 % 10;     // Extract first digit of feet
                digit[3] = temp_data2 / 10;     // Extract second digit of feet.

            }
            else
            {
                // If distance data = 0 manually assign seg-7 values.
                seg7.digit[0] = 0;
                seg7.digit[1] = 10;
                seg7.digit[2] = 10;
                seg7.digit[3] = 10;
            }
            // Gets rid of leading zero's in the inches segment
            if (digit[2] != 0 && digit[1] == 0) {
                digit[1] = 10;
            }

            seg7.colon_on = true;
        } /* end of Inch Mode if statement*/

        // Get rid of leading zero's
        for (i = 3; i >= 0; i--)
        {
            if (digit[i] == 0 && i != 0)
            {
                digit[i] = 10;
            }
            else if (digit[i] != 0)
            {
                break;
            }
        }
        // Assign digits to corresponding 7-seg digit.
        seg7.digit[0] = digit[0];
        seg7.digit[1] = digit[1];
        seg7.digit[2] = digit[2];
        seg7.digit[3] = digit[3];

        // Updates seg-7 display
        Seg7Update(&seg7);

        EventSchedule(event, event->time + 500);
}


/**************************************************************
 * Pushbutton function for control of 7-seg.
 * Switches 7-seg display between millimeters and inches and feet.
 * SW1 for Millimeter Mode. SW2 for Inch Mode
 **************************************************************/
void CheckPushButton(Event *event)
{
    int code = PushButtonRead();        // read the pushbutton
    switch (code)
    {
    case 1:                     // SW1 is pushed brings display into Millimeter mode
        sys.display_mode = Millimeter;
        break;

    case 2:                     // SW2 is pushed brings display into Inch mode
        sys.display_mode = Inch;
        break;
    }
}

/*******************************************
 * The main function
 ******************************************/
void main(void)
{
    // Initialize the LaunchPad and peripherals
    LaunchPadInit();
    RangerInit();
    Seg7Init();

    uprintf("%s\n\r", "Lab 8: Distance Measurement (Time Capture)");

    // Initialize the events
    EventInit(&trigger_ranger_reading_event, TriggerRangerReading);
    EventInit(&check_push_button_event, CheckPushButton);

    // Register the ISRs
    RangerEventRegister(&trigger_ranger_reading_event);
    PushButtonEventRegister(&check_push_button_event);

    // Schedule time event
    EventSchedule(&trigger_ranger_reading_event, 100);

    // Loop forever
    while (true)
    {
        // Wait for interrupt
        asm("   wfi");

        // Execute scheduled callbacks
        EventExecute();
    }
}
