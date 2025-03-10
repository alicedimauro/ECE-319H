//****************** ECE319K_Lab2H.s ***************
// Your solution to Lab 2 in assembly code
// Author: Alice Di Mauro
// Last Modified: 02/04/2025
// ECE319H Spring 2025 (ECE319K students do Lab2)

    .include "../inc/msp.s"

        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "AD55559" // replace ZZZ123 with your EID here
        .align 2
  .equ dot,100
  .equ dash,(3*dot)
  .equ shortgap,(2*dot)  // because it will send an interelement too
  .equ interelement,dot
  Morse:
  .long  dot,  dash,    0,    0, 0 // A
  .long  dash,  dot,  dot,  dot, 0 // B
  .long  dash,  dot, dash,  dot, 0 // C
  .long  dash,  dot,  dot,    0, 0 // D
  .long  dot,     0,    0,    0, 0 // E
  .long  dot,   dot, dash,  dot, 0 // F
  .long  dash, dash,  dot,    0, 0 // G
  .long  dot,   dot,  dot,  dot, 0 // H
  .long  dot,   dot,    0,    0, 0 // I
  .long  dot,  dash, dash, dash, 0 // J
  .long  dash,  dot, dash,    0, 0 // K
  .long  dot,  dash,  dot,  dot, 0 // L
  .long  dash, dash,    0,    0, 0 // M
  .long  dash,  dot,    0,    0, 0 // N
  .long  dash, dash, dash,    0, 0 // O
  .long  dot,  dash, dash,  dot, 0 // P
  .long  dash, dash,  dot, dash, 0 // Q
  .long  dot,  dash,  dot,    0, 0 // R
  .long  dot,   dot,  dot,    0, 0 // S
  .long  dash,    0,    0,    0, 0 // T
  .long  dot,   dot, dash,    0, 0 // U
  .long  dot,   dot,  dot, dash, 0 // V
  .long  dot,  dash, dash,    0, 0 // W
  .long  dash,  dot,  dot, dash, 0 // X
  .long  dash,  dot, dash, dash, 0 // Y
  .long  dash, dash,  dot,  dot, 0 // Z

  .align 2
  .global Lab2Grader
  .global Lab2
  .global Debug_Init // Lab3 programs
  .global Dump       // Lab3 programs

// Switch input: PB2 PB1 or PB0, depending on EID
// LED output:   PB18 PB17 or PB16, depending on EID
// logic analyzer pins PB18 PB17 PB16 PB2 PB1 PB0
// analog scope pin PB20
Lab2:
// Initially the main program will
//   set bus clock at 80 MHz,
//   reset and power enable both Port A and Port B
// Lab2Grader will
//   configure interrupts  on TIMERG0 for grader or TIMERG7 for TExaS
//   initialize ADC0 PB20 for scope,
//   initialize UART0 for grader or TExaS
    BL   Lab2Init   // you initialize input pin and output pin
    BL Debug_Init
    MOVS R0,#3  // 0 for info, 1 debug with logic analyzer, 2 debug with scope, 10 for grade
    BL Lab2Grader // Initially this was above the Init, put back there?
hloop:  
    BL dashs // H
    BL dashs
    BL dashs 

    B hloop


loop:
    BL WaitForRelease
    BL WaitForPress
    BL SendLetter
    B   loop // so that letter is sent everytime you press switch



SendLetter: 
    PUSH {LR}
    SUBS R0, #65 // A = 65 in ASCII, we need A = 0
    MOVS R1, #20 // Each row = 5 x 4 bytes = 20 bytes
    MULS R1, R1, R0 // R0 x 20 = correct offset
    LDR R5, =Morse // Jumps to Morse table
    ADDS R5, R1 // if B, then 0 + 20 = correct offset
CheckMorseLoop:
    LDR R4, [R5] // Load instruction
    CMP R4, #0
    BEQ done
    CMP R4, dot // instruction - dot
    BNE defdash
    BL dots
    ADDS R5, #4 // Next instruction
    B CheckMorseLoop // Only way it wouldn't be 0 is if already took dash or dot functions  
defdash:
    BL dashs
    ADDS R5, #4 // Next instruction
    B CheckMorseLoop
done:
    POP {PC} // Returns to main loop



dashs:  
    PUSH {LR}
    BL LED_ON
    BL Dump
    LDR R0, =24000000 // delay count = 80MHz/delay count = 3.33Hz = 1/3.33 = .3s = 300ms
    BL Delay
    BL LED_OFF
    BL Dump
    LDR R0, =8000000 // Inter-element timing = 100ms
    BL Delay
    POP {PC}


WaitForRelease:
    PUSH {LR}
DupeRelease:
    LDR R1, =GPIOB_DIN31_0
    LDR R0, [R1]
    LDR R2, =0x00000002 // Mask for PB1
    ANDS R0, R0, R2
    BNE DupeRelease // Keeps looping if R0 is 0; R0 = x1 if pressed so exits
    POP {PC}

WaitForPress:
    PUSH {LR}
DupePress:
    LDR R1, =GPIOB_DIN31_0
    LDR R0, [R1]
    LDR R2, =0x00000002 // Mask for PB1
    ANDS R0, R0, R2
    BEQ DupePress // Keeps looping if R0 is 0; R0 = x1 if pressed so exits
    POP {PC}

Delay:
    SUBS R0, R0, #2
dloop:
    SUBS R0, R0, #4
    NOP
    BHS dloop
    BX LR

LED_OFF: 
    PUSH {LR}
    LDR R0, =GPIOB_DOUT31_0
    LDR R1, [R0] // Reads previous, masks bit 18
    LDR R2, =0x00040000 // mask x40000
    BICS R1, R1, R2 // PB18 = 0
    STR R1, [R0] // LED off
    POP {PC}

LED_ON: 
    PUSH {LR}
    LDR R0, =GPIOB_DOUT31_0
    LDR R1, [R0] // Reads previous
    LDR R2, =0x00040000 // mask
    ORRS R1, R1, R2 // PB18 = 1
    STR R1, [R0] // LED ON
    POP {PC}

dots:
    PUSH {LR}
    BL LED_ON
    BL Dump
    LDR R0, =8000000 // delay count = 80MHz/delay count = 10Hz = 1/10 = .1s = 100ms
    BL Delay
    BL LED_OFF
    BL Dump
    LDR R0, =8000000 // delay count = 80MHz/delay count = 10Hz = 1/10 = .1s = 100ms
    BL Delay
    POP {PC}




// make switch an input, LED an output
// PortB is already reset and powered
// Set IOMUX for your input and output
// Set GPIOB_DOE31_0 for your output (be friendly)
Lab2Init:
// ***do not reset/power Port A or Port B, already done****
// Set up PB1 as input  
   PUSH {LR}
   LDR R1, =IOMUXPB1 // PINCM
   LDR R0, =0x00040081 // Sets enable input pin and GPIO
   STR R0, [R1] // GPIO input

// Set up PB18 as output

   MOVS R1, #0x81
   LDR R0, =IOMUXPB18 // Loads PINCM  register
   STR R1, [R0] // PB1 is GPIO
   LDR R0, =GPIOB_DOE31_0
   LDR R1, [R0] // Previous
   LDR R2, =0x00040000 // Mask
   ORRS R1, R1, R2 // Friendly
   STR R1, [R0] // Enable out

   POP {PC}

   .end
