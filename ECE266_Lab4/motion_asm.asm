;  ******************************************************************************
; motion_asm.asm
;
;  Created on: Feb 15, 2024
;      Author: Joshua Garcia
;
;******************************************************************************

; reads the motion sensor, returns 0 or 1

        ; To include names declared in C
        .cdecls "motion.h"

        .text

;******************************************************************************
; Motion sensor signal pin usage: Grove base port J16, Tiva C PC4 (Port C, Pin 4)
;******************************************************************************

; Declare a field to store GPIO_PORTC_BASE (32-bit value) in program memory
_GPIO_PORTC_BASE     .field  GPIO_PORTC_BASE

;******************************************************************************
; C prototype: bool motionRead()
; Read the motion sensor. It calls GPIOPinRead() to read input from motion sensor.
;******************************************************************************
        .global MotionRead
        .asmfunc
MotionRead
        PUSH    {LR}            ; save the return address

		; Read the GPIO pin that the motion sensor uses
        LDR     r0, _GPIO_PORTC_BASE
        MOV     r1, #GPIO_PIN_4
        BL      GPIOPinRead

        POP     {PC}            ; return
        .endasmfunc
