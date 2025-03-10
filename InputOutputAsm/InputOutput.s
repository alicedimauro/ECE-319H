//****************** InputOutput.s ***************
// Author: Jonathan Valvano
// programs 2.2.4, 2.2.5, 2.2.6, 2.2.8
// Last Modified: December 19, 2024
// Brief description of the program:
// This is a simple assembly language project that reads the two switches
// on the LaunchPad and sets the red/green/blue LED.
// The red LED1 is on.
// If SW2 is pressed, the LED2 blue color toggles.
// If SW3 is pressed, the LED2 red color toggles.
    .include "../inc/msp.s"

       .data
       .align 2
// Declare global variables here if needed
// with the .space assembly directive

   .global LaunchPad_Init

// Instructions go in FLASH ROM
       .text
       .thumb
       .align 2
       .global main

LED1_On:
  LDR  R0,=GPIOA_DOUT31_0
  LDR  R1,[R0]  // previous
  MOVS R2,#1
  BICS R1,R1,R2 // PA0=0
  STR  R1,[R0]  // LED on
  BX   LR
LED1_Off:
  LDR  R0,=GPIOA_DOUT31_0
  LDR  R1,[R0]  // previous
  MOVS R2,#1
  ORRS R1,R1,R2 // PA0=1
  STR  R1,[R0]  // LED off
  BX   LR
  // main0 toggles the red LED
main0: BL  LaunchPad_Init
loop0: BL  LED1_On
      LDR R0,=16000000 // 1sec
      BL  Delay
      BL  LED1_Off
      LDR R0,=16000000
      BL  Delay
      B   loop0

  // main1 toggles the red LED
main1: BL  LaunchPad_Init
     MOVS R5,#1
     LDR R4,=GPIOA_DOUTTGL31_0
loop8:
      STR R5,[R4]
      LDR R0,=16000000 // 1sec
      BL  Delay
      B   loop8

// main2 reads the switches and changes LEDs on LaunchPad
main: BL LaunchPad_Init

      MOVS R0,#1
      BL   LaunchPad_LED1
      LDR  R4,=1<<22  // blue
      LDR  R5,=1<<26  // red
      MOVS R6,R4  // data
loop: LDR  R0,=16000000 // 31.25ns*16000000 = 500ms
      BL   Delay
      BL   LaunchPad_InS2
      CMP  R0,#0
      BEQ  skip
      EORS R6,R6,R4
skip: BL   LaunchPad_InS3
      CMP  R0,#0
      BEQ  out
      EORS R6,R6,R5
out:  MOVS R0,R6
      BL   LaunchPad_LED
      B    loop





   // return R0 bit 18
LaunchPad_InS2:  // positive logic
  LDR  R1,=GPIOA_DIN31_0
  LDR  R0,[R1]     // value of port A
  LDR  R3,=(1<<18) // PA18
  ANDS R0,R0,R3
  BX   LR


   // return R0 bit 21
LaunchPad_InS3:  // negative logic
  LDR  R1,=GPIOB_DIN31_0
  LDR  R0,[R1]     // value of port B
  LDR  R3,=(1<<21) // PB21
  EORS R0,R0,R3    // convert to positive logic
  ANDS R0,R0,R3
  BX   LR


// R0 bit 0 is RED1
LaunchPad_LED1:
  LDR  R1,=GPIOA_DOUT31_0
  LDR  R2,[R1]     // value of port A
  MOVS R3,#1
  EORS R0,R0,R3 // convert to positive logic
  BICS R2,R2,R3
  ORRS R2,R2,R0
  STR  R2,[R1]
  BX   LR

TogglePA0:
  LDR  R1, =GPIOA_DOUTTGL31_0
  MOVS R0, #1
  STR  R0, [R1]
  BX   LR

// Set LED2
// input R0 bit 22 is blue, bit 26 is red, bit 27 is green
LaunchPad_LED:
  LDR  R1,=GPIOB_DOUT31_0
  LDR  R2,[R1]     // value of port B
  LDR  R3,=((1<<22)|(1<<26)|(1<<27))
  BICS R2,R2,R3
  ORRS R2,R2,R0
  STR  R2,[R1]
  BX   LR
// Set LED2 to white
LEDwhite:
  LDR  R1,=GPIOB_DOUTSET31_0
  LDR  R0,=0x0C400000
  STR  R0,[R1]
  BX   LR
// Turn LED2  off
LEDoff:
  LDR  R1,=GPIOB_DOUTCLR31_0
  LDR  R0,=0x0C400000
  STR  R0,[R1]
  BX   LR


// delay
// input: R0 bus cycles
// output: none
Delay:
   SUBS R0,R0,#2
dloop:
   SUBS R0,R0,#4 // C=1 if no overflow
   NOP           // C=0 on pass through 0
   BHS  dloop
   BX   LR

    .end          // end of file
