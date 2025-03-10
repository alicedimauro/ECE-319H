//****************** Toggle.s ***************
// Program initially written by: Yerraballi and Valvano
// Author: Jonathan Valvano
// Last Modified: 1/16/2023
// Brief description of the program: Toggle LED
// Hardware connections:
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
       .data
       .align 2
// Declare global variables here if needed
// with the .space assembly directive

// I/O port addresses
    .include "../inc/msp.s"

// Instructions go in FLASH ROM
       .text
       .thumb
       .align 2
       .global main
       .global LaunchPad_Init
main:
   BL   Init // make PA0 output
   LDR  R5,=GPIOA_DOUT31_0
   MOVS R4,#1  // PA0
loop:
   LDR  R2,[R5]  // contents of GPIOA
   EORS R2,R2,R4 // flip bit 0
   STR  R2,[R5]  // GPIOA->DOUT31_0 ^= 1
   LDR  R0,=16000000 // 500ms
   BL   Delay
   B    loop

// PA0 is output
Init:
   PUSH {LR}
   BL   LaunchPad_Init
   LDR  R0,=IOMUXPA0
   MOVS R1, #0x81
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PA0INDEX]  = (uint32_t) 0x00000081;
   LDR  R0,=GPIOA_DOE31_0
   MOVS R4,#1  // PA0
   LDR  R2,[R0]
   ORRS R2,R2,R4
   STR  R2,[R0] //GPIOA->DOE31_0 |= 1;
   POP  {PC}


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
