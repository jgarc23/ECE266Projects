;
; clockupdate.asm: ECE 266 Lab 3 Starter code, spring 2023
;
; Assembly code for updating the 7-seg to show a running clock.
;
; Created by Zhao Zhang
;

; include C header file
        .cdecls "clockupdate.h"
        .text

; Declaration fields (pointers) to seg7 and ClockUpdate
_seg7           .field  seg7
_ClockUpdate    .field  ClockUpdate

;************************************************************************************
; Task 1: Update the clock
;
; C prototype: void ClockUpdate(uint32_t time)
;
; This is the ClockUpdate callback function in assembly. It replaces the
; same function in your lab2_main.c.
;
; This is the STARTER CODE. At this time it only flashes the colon of the 7-seg.
; Complete the code so that it updates all the four clock digits.
;************************************************************************************
        .global ClockUpdate
        .asmfunc
ClockUpdate     
        PUSH    {LR, r0}        	; push return address and event

        ; check seg7.colon_on and turn it on/off
        LDR     r0, _seg7           ; load the address of seg7 to r0
        LDRB    r1, [r0, #4]        ; r1 = seg7.colon_on
        CMP     r1, #0              ; compare r1 and 0
        BEQ     turn_on_colon       ; if the colon is turned off, jump to the code turning it on
        MOVS    r1, #0              ; now the colon must have been turned on, so turn it off
        STRB    r1, [r0, #4]        ; store 0 to seg7.colon_on
        B       update_seg7_and_return   ; jump to return

turn_on_colon
        MOV     r1, #1              ; r1 = 1
        STRB    r1, [r0, #4]        ; store 1 to seg7.colon_on

        ;**************************************************************
        ; YOUR CODE STARTS HERE
        ;**************************************************************

        LDRB    r1, [r0, #0]       ; Load digit[0]
        ADD     r1, r1, #1         ; Increment digit[0]
        STRB    r1, [r0, #0]
        CMP     r1, #10
        BEQ    update_digit_1 ; If not, jump to update_digit0
        B       update_seg7_and_return   ; jump to return

update_digit_1
        ; first reset previous digit (seconds) to 0

        MOV     r1, #0              ; r1 = 0
        STRB    r1, [r0, #0]        ; stores 0 in seg7.digit[0]

        ; load in next digit, increment by 1, and compare to reset conditon
        LDRB    r1, [r0, #1]        ; r1 = seg7.digit[1]
        ADD     r1, r1, #1          ; add 1 to r1
        STRB    r1, [r0, #1]        ; store updated r1 to seg7.digit[1]
        CMP     r1, #6              ; compare r1 and 6
        BEQ     update_digit_2      ; jump to code to update next digit (minutes)
        B       update_seg7_and_return   ; jump to return

update_digit_2
        ; first reset previous digit (seconds) to 0
        MOV     r1, #0              ; r1 = 0
        STRB    r1, [r0, #1]        ; stores 0 in seg7.digit[1]

        ; load in next digit, increment by 1, and compare to reset conditon
        LDRB    r1, [r0, #2]        ; r1 = seg7.digit[2]
        ADD     r1, r1, #1          ; add 1 to r1
        STRB    r1, [r0, #2]        ; store updated r1 to seg7.digit[2]
        CMP     r1, #10             ; compare r1 and 10
        BEQ     update_digit_3      ; jump to code to update next digit
        B       update_seg7_and_return   ; jump to return

update_digit_3
        ; first reset previous digit (minutes) to 0
        MOV     r1, #0              ; r1 = 0
        STRB    r1, [r0, #2]        ; stores 0 in seg7.digit[2]

        ; load in next digit, increment by 1, and compare to reset conditon
        LDRB    r1, [r0, #3]        ; r1 = seg7.digit[3]
        ADD     r1, r1, #1          ; add 1 to r1
        STRB    r1, [r0, #3]        ; store updated r1 to seg7.digit[3]
        CMP     r1, #6              ; compare r1 and 6
        BEQ     reset_clock         ; jump to code to reset clock
        B       update_seg7_and_return   ; jump to return

reset_clock
        ; resets digit[3] the 4th digit to 0 and jumps to return
        MOV     r1, #0              ; r1 = 0
        STRB    r1, [r0, #3]        ; stores 0 in seg7.digit[2]
        B       update_seg7_and_return   ; jump to return


        ;**************************************************************
        ; YOUR CODE ENDS HERE
        ;**************************************************************

update_seg7_and_return
        ; Physically update the 7-seg
        ;    Call Seg7Update(&seg)
        BL      Seg7Update          ; note that r0 = &seg7 at this time

        ; Schedule a callback after 0.5 seconds:
        ;   Call ScheduleCallback(event, event->time + 500);
        POP     {r0}                ; restore event and time
        LDR     r1, [r0, #8]        ; r1 = event->time
        ADD     r1, #500            ; time += 500
        BL      EventSchedule

        POP     {PC}                ; return
        .end
