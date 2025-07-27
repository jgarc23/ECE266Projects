/*
 * Music code for, Music box (PWM), spring 2024
 *
 * PWM LED/BUZZER functions
 *
 * Created by Joshua Garcia
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include "launchpad.h"

// Define the number of supported music pitches
#define NA 0.00
#define C2 CPU_CLOCK_RATE / (130.8128/2)
#define C2D CPU_CLOCK_RATE/ (138.5913/2)
#define D2 CPU_CLOCK_RATE / (146.8324/2)
#define D2E CPU_CLOCK_RATE/ (155.5635/2)
#define E2 CPU_CLOCK_RATE / (163.8138/2)
#define F2 CPU_CLOCK_RATE / (174.6141/2)
#define F2G CPU_CLOCK_RATE/ (184.9972/2)
#define G2 CPU_CLOCK_RATE / (195.997/2)
#define G2A CPU_CLOCK_RATE/ (207.6523/2)
#define A2 CPU_CLOCK_RATE / (220.00/2)
#define A2B CPU_CLOCK_RATE/ (233.0819/2)
#define B2 CPU_CLOCK_RATE / (246.9417/2)

#define C3 CPU_CLOCK_RATE / 130.8128
#define C3D CPU_CLOCK_RATE/ 138.5913
#define D3 CPU_CLOCK_RATE / 146.8324
#define D3E CPU_CLOCK_RATE/ 155.5635
#define E3 CPU_CLOCK_RATE / 163.8138
#define F3 CPU_CLOCK_RATE / 174.6141
#define F3G CPU_CLOCK_RATE/ 184.9972
#define G3 CPU_CLOCK_RATE / 195.997
#define G3A CPU_CLOCK_RATE/ 207.6523
#define A3 CPU_CLOCK_RATE / 220.00
#define A3B CPU_CLOCK_RATE/ 233.0819
#define B3 CPU_CLOCK_RATE / 246.9417

#define C4 CPU_CLOCK_RATE / 261.63
#define C4D CPU_CLOCK_RATE/ 277.1826
#define D4 CPU_CLOCK_RATE / 293.66
#define D4E CPU_CLOCK_RATE/ 311.1270
#define E4 CPU_CLOCK_RATE / 329.63
#define F4 CPU_CLOCK_RATE / 349.23
#define F4G CPU_CLOCK_RATE/ 369.9944
#define G4 CPU_CLOCK_RATE / 392.00
#define G4A CPU_CLOCK_RATE/415.3047
#define A4 CPU_CLOCK_RATE / 440.00
#define A4B CPU_CLOCK_RATE/ 466.1638
#define B4 CPU_CLOCK_RATE / 493.88

#define C5 CPU_CLOCK_RATE / (2*261.63)
#define C5D CPU_CLOCK_RATE/ (2*277.1826)
#define D5 CPU_CLOCK_RATE / (2*293.66)
#define D5E CPU_CLOCK_RATE/ (2*311.1270)
#define E5 CPU_CLOCK_RATE / (2*329.63)
#define F5 CPU_CLOCK_RATE / (2*349.23)
#define F5G CPU_CLOCK_RATE/ (2*369.9944)
#define G5 CPU_CLOCK_RATE / (2*392.00)
#define G5A CPU_CLOCK_RATE/ (2*415.3047)
#define A5 CPU_CLOCK_RATE / (2*440.00)
#define A5B CPU_CLOCK_RATE/ (2*466.1638)
#define B5 CPU_CLOCK_RATE / (2*493.88)

#define C6 CPU_CLOCK_RATE / (4*261.63)
#define C6D CPU_CLOCK_RATE/ (4*277.1826)
#define D6 CPU_CLOCK_RATE / (4*293.66)
#define D6E CPU_CLOCK_RATE/ (4*311.1270)
#define E6 CPU_CLOCK_RATE / (4*329.63)
#define F6 CPU_CLOCK_RATE / (4*349.23)
#define F6G CPU_CLOCK_RATE/ (4*369.9944)
#define G6 CPU_CLOCK_RATE / (4*392.00)
#define G6A CPU_CLOCK_RATE/ (4*415.3047)
#define A6 CPU_CLOCK_RATE / (4*440.00)
#define A6B CPU_CLOCK_RATE/ (4*466.1638)
#define B6 CPU_CLOCK_RATE / (4*493.88)

// Define number of supported songs and song length (in notes)
#define NUM_NOTE            300
#define NUM_VOLUME_LEVEL    4
#define NUM_SONGS           4

// Initialize the LEDs for music note playing
void MusicInitLed();

// Set the LED for a given pitch and volume level
void MusicSetLed(int pitch, int volume);

// Turn off LED
void MusicTurnOffLed();

// Initialize the buzzer for music note playing
void MusicInitBuzzer();

// Set the buzzer for music note and volume level
void MusicSetBuzzer(int note, float volume, int song);

// Turn off buzzer
void MusicTurnOffBuzzer();

#endif /* MUSIC_H_ */
