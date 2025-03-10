//****************** ECE319K_Lab1.s ***************
// Your solution to Lab 1 in assembly code
// Author: Alice Di Mauro
// Last Modified: 01/28/2025
// Spring 2025
        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "AD55559" // replace ZZZ123 with your EID here

        .global Phase
        .align 2
Phase:  .long 10
// Phase= 0 will display your objective and some of the test cases, 
// Phase= 1 to 5 will run one test case (the ones you have been given)
// Phase= 6 to 7 will run one test case (the inputs you have not been given)
// Phase=10 will run the grader (all cases 1 to 7)
        .global Lab1
// Input: R0 points to the list
// Return: R0 as specified in Lab 1 assignment and terminal window
// According to AAPCS, you must save/restore R4-R7
// If your function calls another function, you must save/restore LR
Lab1: PUSH {R4-R7,LR}
       LDR R1, =EID // R1 = pointer to EID
       LDRB R2, [R1] // First letter of my EID
       LDR R3, [R0] // address of list
       LDRB R4, [R3] // loads first letter of comparison
       MOVS R7, 0 // Null check
LOOP:
       CMP R4, R7 // at the end of the list
       BEQ FAIL // no match end of program

       CMP R2, R4 // compare letters
       BEQ MATCH // letters matched
       ADDS R0, #8
       LDR R3, [R0] // address of list
       LDRB R4, [R3] // loads first letter of comparison
       LDR R1, =EID // R1 = pointer to EID
       LDRB R2, [R1] // First letter of my EID
       B LOOP 
       
MATCH:       
       ADDS R3, #1
       LDRB R4, [R3]
       ADDS R1, #1
       LDRB R2, [R1]  
       CMP R2, R7 // Are we done?
       BEQ DEFMATCH // A complete match was found!!
       B LOOP // unconditional loop

DEFMATCH:
       ADDS R0, #4 // Access score
       LDR R0, [R0]
       B OVER

FAIL:    
      MOVS R0, #0 // Can I put -1 this way?  
      SUBS R0, #1

OVER: 
      POP  {R4-R7,PC} // return

        .align 2
        .global myClass
myClass: .long pAB123  // pointer to EID
         .long 95      // Score
         .long pXYZ1   // pointer to EID
         .long 96      // Score
         .long pAB5549 // pointer to EID
         .long 94      // Score
         .long 0       // null pointer means end of list
         .long 0
pAB123:  .string "AB123"
pXYZ1:   .string "XYZ1"
pAB5549: .string "AB5549"
        .end
