//****************** TrafficLightFSM.s ***************
// Programs 3.2.1, 3.2.2, 3.2.3, 3.2.4, and 3.2.5 from the book
// Author: Jonathan Valvano
// Last Modified: April 17, 2023
// Brief description of the program:
// This is a simple assembly language project that reads the two switches
// on the LaunchPad and sets the red/green/blue LED.
// The red LED1 is on.
// If SW2 is pressed, the LED2 blue color toggles.
// If SW3 is pressed, the LED2 red color toggles.

       .data
       .align 2
// Declare global variables here if needed
// with the .space assembly directive

// I/O port addresses
    .include "../inc/msp.s"
       .global main
       .global SysTick_Init
       .global SysTick_Wait
       .global SysTick_10ms
       .global LaunchPad_Init
// Instructions go in FLASH ROM
       .text
       .thumb
       .align 2
/*
// Linked data structure, pointer version Program 5.2.1
 .equ OUT,0     //offset for output
 .equ WAIT,4    //offset for time
 .equ NEXT,8    //offset for next
goN:   .word 0x21 //North green, East red
       .word 300  //30 sec
       .word goN,waitN,goN,waitN
waitN: .word 0x22 //North yellow, East red
       .word 50   //5 sec
       .word goE,goE,goE,goE
goE:   .word 0x0C //North red, East green
       .word 300  //30 sec
       .word goE,goE,waitE,waitE
waitE: .word 0x14 //North red, East yellow
       .word 50   //5 sec
       .word goN,goN,goN,goN

main: MOVS R0,#0
      BL   Clock_Init80MHz
      BL   SysTick_Init
      BL   Traffic_Init

      LDR  R4,=goN
      LDR  R5,=GPIOB_DOUT31_0
      LDR  R6,=GPIOB_DIN31_0
loop:
      LDR  R0,[R4,#OUT]
      LDR  R1,[R5] // all of Port B
      MOVS R2,#0x3F
      BICS R1,R1,R2
      ORRS R0,R0,R1
      STR  R0,[R5] // output
      LDR  R0,[R4,#WAIT]
      BL   SysTick_Wait10ms
      LDR  R0,[R6] // all inputs
      MOVS R1,#0xC0
      ANDS R0,R0,R1 // just PB7,6
      LSRS R0,R0,#4 // Input*4=0,4,8,12
      ADDS R0,R0,#NEXT // 8,12,16,20
      LDR  R4,[R4,R0]  // next state
      B    loop
*/
/*
// Linked data structure, index version Program 5.2.2
// each state is 24 bytes
 .equ OUT,0     //offset for output
 .equ WAIT,4    //offset for time
 .equ NEXT,8    //offset for next
 .equ goN,0
 .equ waitN,1
 .equ goE,2
 .equ waitE,3
FSM:
SgoN:  .word 0x21 //North green, East red
       .word 300  //30 sec
       .word goN,waitN,goN,waitN
SwaitN:.word 0x22 //North yellow, East red
       .word 50   //5 sec
       .word goE,goE,goE,goE
SgoE:  .word 0x0C //North red, East green
       .word 300  //30 sec
       .word goE,goE,waitE,waitE
SwaitE:.word 0x14 //North red, East yellow
       .word 50   //5 sec
       .word goN,goN,goN,goN

main: MOVS R0,#0
      BL   Clock_Init80MHz
      BL   SysTick_Init
      BL   Traffic_Init

      MOVS R4,#goN  // S
      LDR  R5,=GPIOB_DOUT31_0
      LDR  R6,=GPIOB_DIN31_0
loop: //FSM+S*8+OUT
      LDR  R7,=FSM // base address
      MOVS R3,#24
      MULS R3,R3,R4 // S*24
      ADDS R7,R7,R3 // FSM+S*24
      LDR  R0,[R7,#OUT] // FSM+S*24+OUT
      LDR  R1,[R5] // all of Port B
      MOVS R2,#0x3F
      BICS R1,R1,R2
      ORRS R0,R0,R1
      STR  R0,[R5] // output
      LDR  R0,[R7,#WAIT] // FSM+S*24+WAIT
      BL   SysTick_Wait10ms
      LDR  R0,[R6] // all inputs
      MOVS R1,#0xC0
      ANDS R0,R0,R1 // just PB7,6
      LSRS R0,R0,#4 // Input*4=0,4,8,12
      MOVS R0,#12
      ADDS R0,R0,#NEXT // 8,12,16,20
      LDR  R4,[R7,R0]  // next state
      B    loop
*/
// Linked data structure, index version Program 5.2.2
// each state is 16 bytes
 .equ OUT,0     //offset for output
 .equ WAIT,4    //offset for time
 .equ NEXT,8    //offset for next
S5:  .word  5,1666667, S5, S6
S6:  .word  6,1666667, S6,S10
S10: .word 10,1666667,S10, S9
S9:  .word  9,1666667, S9, S5
main: MOVS R0,#0
      BL   Clock_Init80MHz
      BL   SysTick_Init
      BL   Stepper_Init

      LDR  R4,=S5
      LDR  R5,=GPIOB_DOUT31_0
      LDR  R6,=GPIOB_DIN31_0
loop:
      LDR  R0,[R4,#OUT]
      LDR  R1,[R5] // all of Port B
      MOVS R2,#0x0F
      BICS R1,R1,R2
      ORRS R0,R0,R1
      STR  R0,[R5] // output
      LDR  R0,[R4,#WAIT]
      BL   SysTick_Wait
      LDR  R0,[R6] // all inputs
      MOVS R1,#0x10
      ANDS R0,R0,R1 // just PB4
      LSRS R0,R0,#2 // Input*4=0,4
      ADDS R0,R0,#NEXT // 8,12
      LDR  R4,[R4,R0]  // next state
      B    loop



// turn on PortB
Traffic_Init:
   PUSH {LR}
   BL   LaunchPad_Init
   LDR  R1,=0x00050081
   LDR  R0,=IOMUXPB7
   STR  R1,[R0] // input, pull down
   LDR  R0,=IOMUXPB6
   STR  R1,[R0] // input, pull down
   LDR  R1,=0x00100081
   LDR  R0,=IOMUXPB5
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB4
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB3
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB2
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB1
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB0
   STR  R1,[R0] // output
   LDR  R0,=GPIOB_DOE31_0
   LDR  R2,[R0]
   MOVS R3,#0x3F
   ORRS R2,R2,R3
   STR  R2,[R0]  // PB5-0 output enable

   POP  {PC}

// turn on PortB
Stepper_Init:
   PUSH {LR}
   BL   LaunchPad_Init

   LDR  R1,=0x00050081
   LDR  R0,=IOMUXPB4
   STR  R1,[R0] // input
   LDR  R0,=IOMUXPB3
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB2
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB1
   STR  R1,[R0] // output
   LDR  R0,=IOMUXPB0
   STR  R1,[R0] // output
   LDR  R0,=GPIOB_DOE31_0
   LDR  R2,[R0]
   MOVS R3,#0x0F
   ORRS R2,R2,R3
   STR  R2,[R0]  // PB3-0 output enable

   POP  {PC}


    .end          // end of file
