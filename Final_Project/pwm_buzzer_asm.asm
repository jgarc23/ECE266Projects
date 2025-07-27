;  ******************************************************************************
; pwm_buzzer_asm.asm
;
;  Created on: Mar 25, 2024
;      Author: Joshua Garcia
;
;******************************************************************************

; PWM Buzzer

        ; To include names declared in C
        .cdecls "pwm_buzzer.h"

        .text

;******************************************************************************
; PWM Buzzer pin usage: GPIO Port C Pin 5, WTimer 0 Sub-Timer B
;******************************************************************************
_SYSCTL_PERIPH_WTIMER0     .field  SYSCTL_PERIPH_WTIMER0
_SYSCTL_PERIPH_GPIOC       .field  SYSCTL_PERIPH_GPIOC
_GPIO_PORTC_BASE           .field  GPIO_PORTC_BASE
_GPIO_PC5_WT0CCP1          .field  GPIO_PC5_WT0CCP1
_WTIMER0_BASE              .field  WTIMER0_BASE

;******************************************************************************
; Initialize the timer PWM functions connected to the buzzer.
; C prototype: void PwmBuzzerInit();
;******************************************************************************
        .global PwmBuzzerInit
        .asmfunc
PwmBuzzerInit
        PUSH    {LR}        ; save the return address
        ; Enable WTimer 0 and GPIO Port C
        LDR    r0, _SYSCTL_PERIPH_WTIMER0
        BL     SysCtlPeripheralEnable
        LDR    r0, _SYSCTL_PERIPH_GPIOC
        BL     SysCtlPeripheralEnable

        ; Connect pins to those timers
        LDR    r0, _GPIO_PORTC_BASE
        MOV    r1, #GPIO_PIN_5
        BL     GPIOPinTypeTimer          ; Conncet Port C Pin 5
        LDR    r0, _GPIO_PC5_WT0CCP1
        BL     GPIOPinConfigure          ; Conncet pin 5 to WTIMER0

        ; Select PWM for Timer 2 sub-Timer B
        LDR    r0, _WTIMER0_BASE
        MOV    r2, #TIMER_CFG_SPLIT_PAIR
        MOV    r3, #TIMER_CFG_B_PWM
        ORR    r1, r2, r3             ; bit-wise or of TIMER_CFG_SPLIT_PAIR and TIMER_CFG_B_PWM
        BL     TimerConfigure

        ; Invert the PWM waveform, so that the Match register value is the pulse width.
        ; Otherwise, the pulse width will be (Load value) - (Match value).
        ; The inversion is done by enabling output inversion on the PWM pins.
        LDR    r0, _WTIMER0_BASE
        MOV    r1, #TIMER_B
        MOV    r2, #1      ; output inversion
        BL     TimerControlLevel

        ; Enable the WTimer 0's timer B
        LDR    r0, _WTIMER0_BASE
        MOV    r1, #TIMER_B
        BL     TimerEnable

        POP   {PC}
        .endasmfunc
;******************************************************************************
; Set sound for the buzzer peripheral by setting the intensity of the pulse
; C prototype: void PwmBuzzerSet(int pulse_period, int pulse_width);
;******************************************************************************
        .global PwmBuzzerSet
        .asmfunc
PwmBuzzerSet
        PUSH    {LR}

        ; Move passed arguments pulse_period and pulse_width in registers r2 and r3
        MOV     r2, r0          ; r2 = pulse_period
        MOV     r3, r1          ; r3 = pulse_width

        ; Set the PWM parameters for Buzzer
        LDR     r0, _WTIMER0_BASE
        MOV     r1, #TIMER_B
        SUB     r2, r2, #1          ; r2 = pulse_period - 1
        BL      TimerLoadSet

        LDR     r0, _WTIMER0_BASE
        MOV     r1, #TIMER_B
        SUB     r2, r3, #1          ; r2 = pulse_width - 1
        BL      TimerMatchSet

        POP     {PC}
        .endasmfunc


