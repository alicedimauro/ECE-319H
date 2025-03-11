// BusyWait.s
// Student names: Alice Di Mauro and Ravi Shah
// Last modification date: change this to the last modification date or look very silly

// Note: these functions do not actually output to SPI or Port A. 
// They are called by the grader to see if the functions would work

// As part of Lab 6, students need to implement these two functions

      .global   SPIOutCommand
      .global   SPIOutData
      .text
      .align 2

// ***********SPIOutCommand*****************
// This is a helper function that sends an 8-bit command to the LCD.
// Inputs: R0 = 32-bit command (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI tx data register address
//         R3 = 32-bit GPIOA->DOUTCLR31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the clear register to clear will make it friendly
SPIOutCommand:
// --UUU-- Code to write a command to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
//3) Clear D/C (GPIO PA13) to zero, be friendly (R3 has address of GPIOA->DOUTCLR31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTCLR31_0
//4) Write the command to the SPI data register (R2 has address of SPI1->TXDATA)
//5) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)
    PUSH {R4-R5, LR}

step1command:
    LDR R4, [R1] // R1 has the address of SPI1 -> STAT
    LDR R5, =(1<<4) // Load bit mask
    ANDS R4, R4, R5 // != 0 if bit 4 is high
    BNE step1command

    LDR R4, =0x2000
    STR R4, [R3] // Write 0x2000 to GPIOA->DOUTCLR31_0
    
    STR R0, [R2] // Write command to SPI data register
step5command:
    LDR R4, [R1] // Read SPI data register and check bit 4
    LDR R5, =(1<<4) // Load bit mask
    ANDS R4, R4, R5 // != 0 if bit 4 is high
    BNE step5command

    POP {R4-R5, PC}    //   return



// ***********SPIOutData*****************
// This is a helper function that sends an 8-bit data to the LCD.
// Inputs: R0 = 32-bit data (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI data register address
//         R3 = 32-bit GPIOA->DOUTSET31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the set register to clear will make it friendly
SPIOutData:
// --UUU-- Code to write data to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 1,
//2) If bit 1 is low, loop back to step 1 (wait for TNF bit to be high)
//3) Set D/C (GPIO PA13) to one, be friendly (R3 has address of GPIOA->DOUTSET31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTSET31_0
//4) Write the data to the SPI data register (R2 has address of SPI1->TXDATA)
    PUSH {R4-R5, LR}
step1data: 
    LDR R4, [R1] // Read SPI status register
    LDR R5, =(1<<1) // Mask for bit 1
    ANDS R4, R4, R5 // Check bit 1
    BEQ step1data

    LDR R4, =0x2000
    STR R4, [R3]

    STR R0, [R2] // Write data to SPI -> TXDATA

    POP {R4-R5, PC}   // return
//****************************************************

// StringConversion.s
// Student names: Alice Di Mauro and Ravi Shah
// Last modification date: change this to the last modification date or look very silly
// Runs on any Cortex M0
// ECE319K lab 6 number to string conversion
//
// You write udivby10 and Dec2String
     .data
     .align 2
// no globals allowed for Lab 6
    .global OutChar    // virtual output device
    .global OutDec     // your Lab 6 function
    .global Test_udivby10

    .text
    .align 2
// Binding for OutDec2
    .equ myRemainder, 0
    .align 2

// **test of udivby10**
// since udivby10 is not AAPCS compliant, we must test it in assembly
Test_udivby10:
    PUSH {LR}

    MOVS R0,#123
    BL   udivby10
// put a breakpoint here
// R0 should equal 12 (0x0C)
// R1 should equal 3

    LDR R0,=12345
    BL   udivby10
// put a breakpoint here
// R0 should equal 1234 (0x4D2)
// R1 should equal 5

    MOVS R0,#0
    BL   udivby10
// put a breakpoint here
// R0 should equal 0
// R1 should equal 0
    POP {PC}

//****************************************************
// divisor=10
// Inputs: R0 is 16-bit dividend
// quotient*10 + remainder = dividend
// Output: R0 is 16-bit quotient=dividend/10
//         R1 is 16-bit remainder=dividend%10 (modulus)
// not AAPCS compliant because it returns two values
udivby10:
// write this
PUSH {R4, LR}        // Save registers

MOVS R1, #10         // Load divisor (fixed to 10)
MOVS R3, #0          // Quotient (initialize to 0)
MOVS R2, #16         // Loop counter (16-bit dividend)
LSLS R1, R1, #15      // Align divisor to highest bit position
LDR R4, =0x8000     // Bit mask (bit 15 set for quotient tracking)

loop:
CMP R0, R1       // Compare dividend with shifted divisor
BLO next         // If dividend < divisor, skip subtraction
SUBS R0, R0, R1  // Subtract divisor from dividend
ORRS R3, R3, R4  // Set corresponding quotient bit             

next:
LSRS R1, R1, #1  // Shift divisor right
LSRS R4, R4, #1  // Shift bit mask right
SUBS R2, R2, #1  // Decrement loop counter
BNE loop         // Repeat until all bits checked

MOVS R1, R0           // Store the correct remainder in R1
MOVS R0, R3           // Store the quotient in R0

POP {R4, PC}         // Restore registers and return

  
//-----------------------OutDec-----------------------
// Convert a 16-bit number into unsigned decimal format
// Call the function OutChar to output each character
// You will call OutChar 1 to 5 times
// OutChar does not do actual output, OutChar does virtual output used by the grader
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11
OutDec2:
   PUSH {LR}
// write this

   POP  {PC}
//* * * * * * * * End of OutDec * * * * * * * *

// ECE319H recursive version
// Call the function OutChar to output each character
// You will call OutChar 1 to 5 times
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11

OutDec:
   PUSH {LR}
// write this
    SUB SP, #4 // Alloate space for remainder
    CMP R0, #10 // Base case, it's a single digit
    BLO base

    BL udivby10
    STR R1, [SP, #myRemainder]

    BL OutDec

    LDR R0, [SP, #myRemainder]

base:
    ADDS R0, 0x30 // Convert to ASCII
    BL OutChar
    ADD SP, #4 // Deallocate memory
    POP  {PC}



     .end
