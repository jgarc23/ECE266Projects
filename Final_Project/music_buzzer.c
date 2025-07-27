/*
 * music_buzzer.c
 *
 *  Created on: Mar 22, 2024
 *      Author: josha
 */

#include "music.h"
#include "pwm_buzzer.h"


/*
 * Note and volume settings for the buzzer for a giveb
 * pitch level and volume level.
 */

/*
 * MUSIC NOTES
 */



// Note settings for a given pitch. CPU_CLOCK_RATE defined as 50000000L (50MHz)
uint32_t buzzer_period[NUM_SONGS][NUM_NOTE] =
{

   // Fancy by Twice
   { G5,A5B,A5B,C6,A5B,G5A,G5,G5,A5B,A5B,
     C6,A5B,G5A,G5,G5,A6B,A5B,C6,A5B,G5A,
     G5,G5,G5,G5A,A4B,C5,C5,G5,F5,F5,
     G5A,G5,G5,A5B,G5A,G5,F5,C5,G5,F5,
     F5,G5A,G5,G5,A5B,C6,G5,F5,D5E,D5E,
     G5,D5,D5,G5,D5,D5E,F5,C5,C5,G5,
     F5,C5,C5,G5,F5,C5,G5,F5,C5,G5,
     F5,F5,G5A,G5,G5,A5B,G5A,G5,F5,C5,
     G5,F5,F5,G5A,G5,G5,A5B,C6,G5,F5,
     D5E,D5E,G5,D5,D5,G5,D5,D5E,F5,G5,

     F5,D5E,G5,F5,A5B,G5A,G5,G5,F5,C6,
     A5B,A5B,G5A,G5,A5B,A5B,C6,G5,G5A,A5B,
     G5,F5,D5E,C5,G5,G5,G5A,A5B,A5B,A5B,
     D6E,D6,C6,A5B,A5B,G5A,G5,G5,A5B,A5B,
     C6,G5,G5A,A5B,G5,F5,D5E,G6,F5,G5A,
     G5,C6,A5B,C6,G5,G5A,A5B,D5E,D5E,D6E,

   },
   // Star Wars Darth Vader Theme Song
   {  G4,G4,G4,D4E,A4B,G4,D4E,A4B,G4,D5,
      D5,D5,D5E,A4B,F4G,A4B,A4B,G4,G5,G4,
      G4,G5,F5G,F5,E5,D5E,E5,G4A,C5D,C5,
      B4,A4B,A4,A4B,D4E,F4G,D4E,F4G,A4B,G4,
      A4B,D5,G5,G4,G4,G5,F5G,F5,E5,D5E,
      E5,G4A,C5D,C5,B4,A4B,A4,A4B,D4E,F4G,
      D4E,A4B,G4,D4E,A4B,G4

   },
   // When I was Your Man by Bruno Mars
   {  C4,C4,E4,D4,E4,C4,C4,E4,E4,E4,
      E4,E4,E4,E4,D4,E4,E4,D4,E4,D4,
      C4,E4,F4,F4,D4,D4,D4,C4,D4,C4,
      D4,D4,C4,A3,D4,E4,E4,A3,E4,E4,
      E4,D4,E4,D4,E4,D4,E4,D4,C4,E4,
      F4,G4,F4,E4,D4,G3,D4,D4,D4,C4,
      D4,C4,D4,C4,D4,D4,C4,C4,C4,D4,
      A3,B3,C5,B4,A4,G4,A4,B4,F4,F4,
      F4,F4,F4,F4,F4,E4,F4,E4,C4,A4,
      G4,E4,G4,A4,E4,D4,G3,A3,C4,E4,

      A4,A4,G4,E4,G4,A4,E4,D4,A3,C4,
      D4,E4,C4,A4,G4,G4,G4,C4,A4,G4,
      G4,G4,E4,A4,G4,G4,G4,F4,E4,D4,
      E4,G4,A4,A4,G4,G4,A4,E4,C4,A3,
      C4,D4,E4,D4,C4,D4,C4,C4,E4,E4,
      F4,E4,E4,E4,D4,E4,E4,E4,D4,E4,
      E4,D4,E4,D4,C4,E4,F4,F4,D4,D4,
      D4,C4,D4,C4,D4,D4,C4,A3,D4,E4,
      E4,A3,E4,E4,E4,D4,E4,D4,E4,D4,
      E4,D4,C4,E4,F4,G4,F4,E4,D4,G3,

      D4,D4,D4,C4,D4,C4,D4,C4,D4,D4,
      C4,C4,C4,D4,A4,B4,C5,B4,A4,G4,
      A4,B4,F4,F4,F4,F4,F4,F4,F4,E4,
      F4,E4,C4,A4,G4,E4,G4,A4,E4,D4,
   },
   // Halo Theme Song
   {  B2,F4G,G4,F4G,A4,G4,F4G,E4,B2,B4,
      C5D,D5,C5D,A4,C5D,B4,B2,B3,D4,E4,
      G4,A4,F4G,E4,G4,F4G,E4,F4G,D4,C4D,
      B3,E3,E3,E3,E3,E4,E3,E3,E3,F4G,
      E3,E3,E3,G4,E3,E3,E3,A4,A4,G4,
      F4G,B2,B2,B2,E4,C3D,C3D,C3D,F4G,D3,
      D3,D3,G4,E3,E3,E3,A4,A4,G4,F4G,
      E2,F4G,G4,F4G,A4,G4,F4G,E4,B2,B4,
      C5D,D5,C5D,A4,C5D,B4,B2,B3,D4,E4,
      G4,A4,F4G,E4,G4,F4G,E4,F4G,D4,C4D,

      B3,E3
   }

};

/*
 * Initialize the Buzzer for music note playing
 */
void MusicInitBuzzer() {

    PwmBuzzerInit();

}

/*
 * Set the Buzzer for a given pitch and volume level.
 *
 * The function calculates the PWM parameters for the buzzer,
 * and then call PwmBuzzerSet() to set those parameters.
 */
void MusicSetBuzzer(int note, float volume, int song)
{
    int pulse_width = volume * buzzer_period[song][note];

    PwmBuzzerSet(buzzer_period[song][note], pulse_width);
}

/*
 * Turn off the LED
 */
void MusicTurnOffBuzzer()
{
    PwmBuzzerSet(0, 0);
}
