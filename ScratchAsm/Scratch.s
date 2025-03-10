//****************** Scratch.s ***************
//
// Author: your name
// Last Modified: date
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
main:

loop:

   B    loop


    .end          // end of file
