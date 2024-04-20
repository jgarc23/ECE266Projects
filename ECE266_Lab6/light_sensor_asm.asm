;  ******************************************************************************
; motion_asm.asm
;
;  Created on: Feb 15, 2024
;      Author: Joshua Garcia
;
;******************************************************************************

; Light sensor ISR

        ; To include names declared in C
        .cdecls "light_sensor.h"

        .text
; Field/pointer to light sensor state variable in .c file

				.ref light_sensor
_light_sensor  .field light_sensor
;******************************************************************************
; Light sensor signal pin usage: ADC0 Sequence #1 Channel #7
;******************************************************************************
; Declare a field to store ADC0_BASE (32-bit value) in program memory
_ADC0_BASE     .field  ADC0_BASE
;******************************************************************************
; C prototype: static void LightSensorISR();
;******************************************************************************
        .global LightSensorISR
        .asmfunc
LightSensorISR
        PUSH    {LR}            ; save the return address

        ; Call ADCSequenceDataGet
        LDR     r0, _ADC0_BASE      ; Load register r0 with ADC0_BASE
        MOV     r1, #1              ; Assign to sequencer #1
        LDR     r2, _light_sensor   ; Load register r2 with light_sensor struct variable
        ADD     r2, r2, #4          ; Increment address of r2 by 4, assigning r2 poiner to raw_data
        BL      ADCSequenceDataGet  ; ADCSequenceDataGet(r0, r1, r2) = ADCSequenceDataGet(ADC0_BASE, 1  sequencer ,
                                    ; &light_sensor.raw_data  pointer to data buffer)
        ; Set new_data_ready flag to true
        LDR     r0, _light_sensor   ; Load register r0 with light_sensor
        MOV     r1, #1              ; Assign r1 to #1
        STR     r1, [r0, #8]        ; Store 1 into bool new_data_ready in light_sensor variable

        ; Schedule callback event
        BL      EventGetCurrentTime ; Call EventGetCurrentTime returns time in r0
        MOV     r1, r0              ; Assign the current time to r1
        LDR     r0, _light_sensor   ; Load register r0 with light_sensor
        LDR     r0, [r0]            ; Load register r0 with *callback_event
        BL      EventSchedule       ; Call the function EventSchedule(r0, r1) =
	                                ; EventSchedule(light_sensor.callback_event, EventGetCurrentTime());

        ; Clear interrupt flag
        LDR     r0, _ADC0_BASE      ; Load register r0 with ADC0_BASE
        MOV     r1, #1              ; Set r1 as #1 to use a sequencer value
        BL	    ADCIntClear         ; Call ADCIntClear(r0, r1) = ADCIntClear(ADC0_BASE, 1 /* sequencer */)


        POP     {PC}            ; return
        .endasmfunc
