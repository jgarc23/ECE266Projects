/*
 * Lab Project Music Box, main.c, spring 2024
 *
 * Lab Project: Music Box
 *
 * Created by Joshua Garcia
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "rotary_angle_sensor.h"
#include "music.h"
#include "pwm_buzzer.h"
#include "pwm_led.h"

// The state of the 7-segment display. Initially it shows "   0" with the colon turned off.
Seg7Display seg7 = { { 0, 10, 5, 10 }, true /* colon off */};

// System state
typedef struct
{
    bool music_on;        // Music system on or off
    int song;             // Current song
    int note;             // Current note
    float volume;         // Current volume level
    int angle_data;       // Current reading from rotary angle sensor
    bool buzzer_on;       // If buzzer is turned on or off
} SysState;

SysState sys = {true, 0, 0, 0, 0, false };

int note_break[NUM_SONGS][NUM_NOTE] =
{

 //Fancy by Twice
 {  230,80,80,80,80,80,80,230,80,80,
    80,80,80,80,230,80,80,80,80,80,
    80,500,80,80,80,80,80,80,230,80,
    80,230,80,80,80,80,230,80,80,230,
    80,80,230,80,80,80,80,230,80,80,
    80,80,80,80,80,80,80,80,80,80,
    230,80,80,80,80,80,80,230,80,80,
    230,80,80,230,80,80,80,80,230,80,
    80,230,80,80,230,80,80,80,80,80,
    80,80,230,80,80,80,80,230,80,80,

    80,80,80,80,80,80,80,80,80,80,
    80,80,80,80,80,80,80,80,80,80,
    80,80,80,80,80,80,80,80,80,80,
    80,80,80,80,80,80,80,80,80,80,
    80,80,80,80,80,80,80,80,80,80,
    80,80,80,80,80,80,80,80,80,1000,

 },
 // Star Wars Darth Vader Theme Song 65
 {  100,100,100,100,100,100,100,100,100,100,
    100,100,100,100,100,100,100,100,100,100,
    100,100,100,100,40,50,100,100,100,100,
    100,100,100,70,40,100,100,100,100,100,
    100,100,100,100,100,100,100,100,40,50,
    100,100,100,100,100,70,40,100,100,100,
    100,100,100,100,100,3000

 },
 // When I was Your Man by Bruno Mars 230
 {  100,100,100,40,100,100,100,100,100,100,
    400,100,100,100,100,100,100,100,100,100,
    100,100,100,3000,100,100,100,100,100,100,
    100,100,100,100,100,100,2800,100,100,100,
    100,100,100,100,100,100,100,100,100,100,
    100,100,100,100,1800,100,100,100,100,100,
    100,100,100,100,100,100,100,100,100,100,
    100,100,100,100,100,100,100,1800,100,100,
    100,100,100,100,100,100,100,500,100,100,
    100,100,100,100,100,1800,100,100,100,500,

    100,100,100,100,100,100,100,500,100,100,
    100,100,250,100,100,100,100,100,100,100,
    100,100,100,100,100,100,100,100,100,100,
    100,100,100,100,100,100,100,100,100,100,
    50,100,100,100,100,100,100,100,100,100,
    100,100,100,100,300,100,100,100,100,100,
    100,100,100,100,100,100,100,2800,100,100,
    100,100,100,100,100,100,100,100,100,100,
    2800,100,100,100,100,100,100,100,100,100,
    100,100,100,100,100,100,100,100,1800,100,

    100,100,100,100,100,100,100,100,100,100,
    100,100,100,100,100,100,100,100,100,100,
    100,1800,100,100,100,100,100,100,100,100,
    100,300,100,100,100,100,100,100,100,3000,
 },
 // Halo Theme Song
 {  50,100,100,100,100,100,100,400,50,100,
    100,100,100,100,100,100,50,100,100,100,
    100,100,100,100,100,100,100,100,100,100,
    100,100,50,50,50,100,50,50,50,100,
    50,50,50,100,50,50,50,100,60,60,
    100,50,50,50,100,50,50,50,100,50,
    50,50,100,50,50,50,100,60,60,100,
    50,100,100,100,100,100,100,400,50,100,
    100,100,100,100,100,100,50,100,100,100,
    100,100,100,100,100,100,100,100,100,100,

    100,3000
 }
};

int note_length[NUM_SONGS][NUM_NOTE] =
{

 // Fancy by Twice
 {  1500,500,250,500,250,250,500,1500,500,250,
    500,250,250,500,1500,500,250,500,250,250,
    500,1000,250,250,1500,250,250,400,100,250,
    400,100,250,400,400,250,500,250,400,100,
    250,400,100,250,400,400,250,500,250,400,
    400,250,400,400,250,800,500,250,250,250,
    1000,250,250,250,500,250,250,500,250,400,
    100,250,400,100,250,400,400,250,500,250,
    400,100,250,400,100,250,400,400,250,800,
    250,400,100,250,400,400,250,500,500,250,

    250,250,400,900,250,250,250,400,400,500,
    800,250,250,1200,250,250,250,250,250,250,
    250,250,250,250,250,800,500,500,250,250,
    250,250,250,500,250,250,1000,250,250,250,
    250,250,250,250,250,250,250,400,400,400,
    400,400,400,250,250,250,500,250,250,400,


 },
 // Darth Vader 65
 {  500,500,500,400,100,500,400,100,1200,500,
    500,500,400,100,600,400,100,1200,500,250,
    250,500,250,250,200,100,500,250,600,250,
    200,100,250,600,250,600,250,250,600,250,
    250,1200,500,250,250,500,250,250,200,100,
    500,250,600,250,200,100,250,600,250,600,
    400,100,500,400,100,1200

 },
 // When I Was Your Man by Bruno Mars 240
 {  3000,3000,4000,500,1200,3000,3000,1200,1200,1200,
    1200,500,500,250,250,500,250,250,250,250,
    500,250,500,1700,500,500,250,250,250,250,
    500,250,250,500,250,500,1700,250,500,500,
    250,250,250,250,250,250,250,250,500,250,
    250,250,500,500,1700,250,500,500,250,250,
    250,1200,250,250,1200,250,1700,250,500,500,
    500,500,1200,1200,500,1200,500,3000,1200,500,
    500,500,500,500,250,250,250,1200,500,500,
    250,250,500,500,500,1200,250,250,500,1200,

    500,250,250,250,500,500,500,1200,250,250,
    1000,500,1700,500,250,250,500,500,500,250,
    250,800,250,500,250,250,800,250,500,250,
    250,500,500,800,500,500,500,500,800,300,
    500,300,300,300,300,300,1200,1200,1200,1200,
    1200,1200,1200,1200,1200,500,500,250,250,500,
    250,250,250,250,500,250,500,1200,500,500,
    250,250,250,250,500,250,250,500,250,500,
    1200,250,500,500,250,250,250,250,250,250,
    250,250,500,250,250,250,500,500,1200,250,

    500,500,250,250,250,750,250,250,750,250,
    1200,250,500,500,500,500,1000,1000,250,1000,
    250,2100,1000,500,500,500,500,500,250,250,
    250,1200,500,500,250,250,500,500,500,700
 },
 // Halo Theme Song 102
 {  1500,500,500,500,500,500,500,2000,2000,500,
    500,800,500,500,500,1300,1000,500,500,500,
    500,500,1000,500,500,500,500,500,500,500,
    1000,1000,150,150,150,1300,150,150,150,1500,
    150,150,150,1500,150,150,150,1000,150,150,
    150,150,150,150,1500,150,150,150,1500,150,
    150,150,1500,150,150,150,1000,150,150,150,
    1500,500,500,500,500,500,1000,1500,2000,500,
    500,1000,500,500,500,1500,1000,500,500,500,
    500,500,1000,500,500,500,500,500,500,500,

    1000,3000
 }

};
// The events
Event asensor_trigger_reading_event;
Event asensor_check_reading_event;
Event check_push_button_event;
Event play_sound_event;

/*
 * Trigger the rotary angle sensor reading periodically
 */
void AsTriggerReading(Event *event)
{
    // Trigger ADC sampling for the rotary angle sensor
    AsTriggerSampling();

    // Schedule the next callback in 50 milliseconds
    EventSchedule(event, event->time + 50);
}

/*
 * Check rotary angle sensor reading, print if ready
 */
void AsProcessReading(Event *event)
{
    // Get the current reading for the rotary angle sensor.
    sys.angle_data = AsReadData();
    // Set the volume to angle data (max volume is 50%).
    sys.volume = sys.angle_data / w.0;
}

void PlaySound(Event *event)
{
    // Set initial delay.
    int delay = 100;
    int brightness = 0;
    int color = 0;
    // Update song number on seg-7 display.
    seg7.digit[0] = sys.song + 1;

    // Play music box if system is not paused (controlled by PB2).
    if (sys.music_on)
    {
        // Play the notes by alternating between their length and break.
        if (!sys.buzzer_on)
        {
            // Updates note.
            sys.note = (sys.note + 1) % NUM_NOTE;
            // Turn on the buzzer with the current PWM setting.
            MusicSetBuzzer(sys.note, sys.volume, sys.song);
            // Sets the brightness of the LED based on volume (higher volume higher LED intensity).
            if (sys.volume < 0.1) {
                brightness = 0;
            }
            else if (sys.volume < 0.2) {
                brightness = 1;
            }
            else if (sys.volume < 0.3) {
                brightness = 2;
            }
            else {
                brightness = 3;
            }
            // Sets color of LED
            color = sys.note % 6;
            MusicSetLed(color, brightness);


            sys.buzzer_on = true;
            // Sets the play length of the buzzer/note.
            delay = note_length[sys.song][sys.note];
        }
        else
        {
            // Turn off the buzzer
            MusicTurnOffBuzzer();
            MusicTurnOffLed();
            sys.buzzer_on = false;
            // Sets the break time of the buzzer/note.
            delay = note_break[sys.song][sys.note];
        }
    }

    // Update seg-7 board with corresponding song number.
    Seg7Update(&seg7);
    // Increment the position and schedule the next callback
    EventSchedule(&play_sound_event, event->time + delay);
}

void CheckPushButton(Event *event)
{
    // Process the pushbutton input
    switch (PushButtonRead())
    {
    // PUSH BUTTON 1 skips to the next song.
    case 1:
        // Update song by 1 and resets notes to 0.
        sys.song = (sys.song + 1) % NUM_SONGS;
        sys.note = 0;
        // Prints corresponding song name.
        if (sys.song == 0)
        {
            uprintf("%s\n\r", "Now Playing: Fancy by Twice");

        }
        else if (sys.song == 1)
        {
            uprintf("%s\n\r", "Now Playing: Star Wars Darth Vader Theme Song");

        }
        else if (sys.song == 2)
        {
            uprintf("%s\n\r", "Now Playing: When I Was Your Man by Bruno Mars");
        }
        else if (sys.song == 3)
        {
            uprintf("%s\n\r", "Now Playing: Halo Theme Song");
        }
        break;

    // PUSH BUTTON 2 pauses and plays the song.
    case 2:
        // Stops PlaySound function but still saves current song.
        if (!sys.music_on) {
            sys.music_on = true;
        }
        else{
            sys.music_on = false;
        }
        // Turns off buzzer.
        MusicTurnOffBuzzer();
        break;
    }
}


/*
 * The main function: Initialize and the execute the callback scheduler loop
 */
int main(void)
 {
    LaunchPadInit();
    Seg7Init();
    AsInit();
    MusicInitLed();
    MusicInitBuzzer();

    // Initialize the event objects
    EventInit(&asensor_trigger_reading_event, AsTriggerReading);
    EventInit(&asensor_check_reading_event, AsProcessReading);
    EventInit(&play_sound_event, PlaySound);
    EventInit(&check_push_button_event, CheckPushButton);

    // Register ISR event
    AsEventRegister(&asensor_check_reading_event);
    PushButtonEventRegister(&check_push_button_event);

    // Schedule time event
    EventSchedule(&asensor_trigger_reading_event, 100);
    EventSchedule(&play_sound_event, 100);


    uprintf("%s\n\r", "Lab Project: Music Box");
    uprintf("%s\n\r", "Now Playing: Fancy by Twice");

    // Loop forever
    while (true)
    {
        // Wait for interrupt to happen
        __asm("    wfi");

        // Call the callback scheduler
        EventExecute();
    }
}
