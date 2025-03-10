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
OutDec:
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

OutDec2:
   PUSH {LR}
// write this

   POP  {PC}



     .end
