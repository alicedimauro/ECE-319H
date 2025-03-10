//****************** Toggle.s ***************
// Program initially written by: Yerraballi and Valvano
// Author: Jonathan Valvano
// Last Modified: 11/25/2022
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
FuncPt: .space 4

       .text
       .thumb
       .align 2
    .macro NegR0
    RSBS R0,R0,#0
    .endm
    .macro Neg,reg
    RSBS \reg,\reg,#0
    .endm
       .global main
// test of hardfault handler
  LDR R2,=0x400A1381
 // STRH R3,[R2] // halfword unaligned
  LDRH R3,[R2] // halfword unaligned
  LDR R0,=0x20200000
  PUSH {R0,R1}
faultloop:
 // PUSH {R0} // stack overflow after many
  POP {R0}  // stack underflow on third execution
  B faultloop
  LDR R1,[R0] // word unaligned crash on second time
  ADDS R0,#1
  B faultloop
main:


  MOVS R2,#2 //I=2, will call Fun2
  LDR  R1, =FList      //R1 points to list of functions
  LDR  R3, FListaddr      //R1 points to list of functions
  B ddd
  // hardfault happens on unaligned access
  LDR  R6,PiAddr
  MOVS R4,#4
  LDR  R7, [R6,R4] //aligned access Load 32-bit from R6+R4 address to R7
  MOVS R4,#1
  LDR  R7, [R6,R4] //unaligned access Load 32-bit will hardfault
  BKPT #0
// SVC
  B ddd
  SVC  #1
  WFI
// SUB
  SUBS  R7, R5, #2    //R7=R1-2, sets the flags on the result
  SUBS  R2, R1, R3    //R2=R1-R3, sets the flags on the result
  SUBS  R6, #240      //R6=R6-240, sets the flags on the result
  SUB   SP,SP,#8      // allocated two local variables
  SUB   SP,#8      // allocated two local variables

// STR
  STR  R2, [R7,#64] //32-bit store value of R2 into address R7+4
  STR  R3, [SP,#8] //32-bit store value of R3 into address SP+8
  STR  R0, [R1,R2] //32-bit store value of R0 into address R1+R2
  STRB R7, [R6]    //store low 8 bits of R7 to address specified by R6
  STRB R1, [R2,#5] //store low 8 bits of R1 to address specified by R2+5
  STRB R0, [R3,R4] //store low 8 bits of R0 to address specified by R3+R4

  STRH R7,[R6]    //store low 16 bits of R7 to address specified by R6
  STRH R1,[R2,#6] //store low 16 bits of R1 to address specified by R2+5
  STRH R0,[R3,R4] //store low 16 bits of R0 to address specified by R3+R4

// SBCS
  MOVS  R0,#1
  MOVS  R1,#1
  LSLS  R1,R1,#31
  MOVS  R3,R1
  MOVS  R2,R0
  ADDS  R1,#1
  SUBS  R1, R3
  SBCS  R0, R2   //64 bit subtract (R0:R1) = (R0:R1)-(R2:R3)
// ADC
  MOVS  R0,#1
  MOVS  R1,#1
  LSLS  R1,R1,#31
  MOVS  R3,R1
  MOVS  R2,R0
  ADDS  R3,#1
  ADDS  R1, R3
  ADCS  R0, R2   //64 bit add (R0:R1) = (R0:R1)+(R2:R3)

// RSB instructions
  MOVS R6,#2
  RSBS  R7, R6, #0     //R7=-R6, negate, sets the flags
  NegR0                //same as RSBS R3,R3,#0
  Neg R0
  RSBS  R0, R0,#0         //R0= 0-R0, negate

// ROR instruction
  MOVS R4,#0x0F
  MOVS R6,#33
  RORS R4, R4, R6  //rotate R4 8 bits to the right

//  BL fun43
  B ddd
//push pop instuctions
//example subroutine, with local variable
Func23: PUSH {R4-R7,LR}  //save registers
      .EQU  sum,0          //32-bit local variable, stored on the stack
      MOVS R0,#0
      PUSH {R0}        //allocate and initialize a local variable
//body of subroutine
      LDR  R1,[SP,#sum] //R1=sum
      ADD  R1,R0        //R1=R0+sum
      STR  R1,[SP,#sum] //sum=R0+sum
//end of subroutine
      ADD  SP,#4        //deallocate sum
      POP  {R4-R7,PC}   //restore registers and return

  LDM R0!,{R2-R7}
  STM R0!,{R2-R7}
fun43:
  PUSH  {R7,LR}     //push 2 words from stack and place into R2, LR
//  PUSH  {R0-R2,LR}  //push 4 words from stack and place into R0,R1,R2,LR
  PUSH  {R0,R4-R7}  //push 5 words from stack and place into R0,R4,R5,R6,R7
  PUSH  {R5}        //push 32 bits from stack and place it in R5

  POP  {R5}        //pop 32 bits from stack and place it in R5
  POP  {R0,R4-R7}  //pop 5 words from stack and place into R0,R4,R5,R6,R7
//  POP  {R2,PC}     //pop 2 words from stack and place into R2, PC
  POP  {R0-R2,PC}  //pop 4 words from stack and place into R0,R1,R2,PC

//example subroutine
Func22: PUSH {R4-R7,LR}  //save registers
//body of subroutine
      POP  {R4-R7,PC}  //restore registers and return

// ORRS instruction
  MOVS  R2,#0x05
  ORRS  R7, R2     //R7 = R7 | R2, sets bits 2,0
  MOVS  R3,#0x10
  ORRS  R5, R5, R3 //R5 = R5 | R3, sets bit 4

 // MVNS instruction
   MOVS R0,#2
   MVNS R0, R0           //R0 = -2

// MUL instructions
  MOVS R1,#0
  SUBS R1,#1
  LSRS R3,R1,#1
  MOVS R5,#2
  MULS  R1, R1, R5   //R1 = R1 * R5, sets N and Z flags
  MOVS R1,#0
  MOVS R0,#2
  MULS  R0, R1       //R0 = R0 * R1, sets N and Z flags
  B ddd
// MOV instructions
  MOVS  R1, #10    // R11=10, N and Z flags get updated
  MOVS  R0, R5     // R0=R5, N and Z flags get updated
  MOV   R1, SP     // R1=SP
  MOV   SP, R1     // SP=R1

//LSR instructions
  LSRS R7, R5, #32  //R7 = R5>>9 (similar to R7=R5/512)
  LSRS R4, R4, R6  //R4 = R4>>R6 (similar to R4=R4/2R6)

// LSL instructions
  LSLS R7, R6, #0  //R7 = R6<<9 (similar to R7=R6*512)
  LSLS R7, R6, #1  //R7 = R6<<9 (similar to R7=R6*512)
  LSLS R7, R6, #31  //R7 = R6<<9 (similar to R7=R6*512)
  LSLS R5, R6      //R5 = R5<<R6 (similar to R5=R5*2R6)
  LSLS R5, R5, R6  //R5 = R5<<R6 (similar to R5=R5*2R6)

// LDRB instructions
  LDR  R6,PiAddr
  LDR  R2,PiAddr
  LDRH  R7, [R6]    //Load 16-bit unsigned from R6 address to R7
  LDRH  R1, [R2,#6] //Load 16-bit unsigned from (R2+6) address to R1
  LDRSH R0, [R1,R2] //Load 16-bit signed from (R1+R2) address to R0

  LDRB  R7, [R6]    //Load 8-bit unsigned from R6 address to R7
  LDRB  R1, [R2,#5] //Load 8-bit unsigned from (R2+5) address to R1
  LDRSB R0, [R1,R2] //Load 8-bit signed from (R1+R2) address to R0

// LDR instructions
  LDR  R6,PiAddr
  LDR  R2,PiAddr
  MOVS R4,#4
  LDR   R7, [R6,R4]     //Load 32-bit from R6 address to R7
  LDR   R7, [R6]     //Load 32-bit from R6 address to R7
  LDR   R1, [R2,#8]  //Load 32-bit from (R2+8) address to R1
  LDR   R0, Pi       //R0=314159
  LDR   R0, =314159  //R0=314159
  B ddd
 .align 4
Pi: .long 314159
PiAddr: .long Pi
//eor instruction
mask .req R3
TogglePA2:
    LDR  R1, GPIOB_DOUT31_0   //R1 points to output register
    LDR  R2, [R1]             //read all data of Port A
    MOVS mask, #0x04
    EORS R2, R2, mask           //R2 = R2^~0x04 (toggle bit 2)
    STR  R2, [R1]             //update Port A
    BX   LR
//GPIOA_DOUT31_0: .long 0x400A1280

//cp instructions
  CPSIE I  //Clears the Priority Mask Register (PRIMASK)
  CPSIE F  //Clears the Fault Mask Register (FAULTMASK)
  CPSID I  //Sets the Priority Mask Register (PRIMASK)
  CPSID F  //Sets the Fault Mask Register (FAULTMASK)

//CMP instructions
  CMP R2, #64
  BGT gothere        //branch to gothere if R2>64 (signed)
  CMP R2, R3
  BLO gothere        //branch to gothere if R2<R3 (unsigned)

// CMN instructions
  MOVS R3,#25
  CMN  R2, R3   // compare R2 to -25
  BEQ  gothere  //branch to gothere if R2 equals -25
gothere:
// BX instructions
     MOVS R0,#2
     MOVS R1,#1
     BL Linear //R0=4*2+1 = 9
     BL SysTick_Handler2 // fake invocation
     B ddd

// Inputs:  x in R0
//          y in R1
// Outputs: z = 4*x+y in R0
Linear:
  LSLS R0,#2    //R0=4*x
  ADDS R0,R0,R1 //R0=4*x+y
  BX   LR       //return from subroutine

SysTick_Handler2:
  // body of ISR
  BX   LR       //return from interrupt

// BLX instructions
//Assume R2 contains an index I from 0 to 3
  MOVS R2,#2 //I=2, will call Fun2
  LDR  R1, =FList      //R1 points to list of functions
  LDR  R1, FListaddr      //R1 points to list of functions
  LSLS R3,R2,#2           //R3=4*I
  ADDS R4,R1,R3           //R4=FList+4*I
  LDR  R0,[R4]            //R0 points to subroutine to execute
  MOVS R5,#1
  ORRS R0,R0,R5           //set thumb bit
  BLX  R0                 //call subroutine, return address in LR
//end of example
   B ddd
Fun0: //body of function 0
   BX LR
Fun1: //body of function 1
   BX LR
Fun2: //body of function 2
   BX LR
Fun3: //body of function 3
   BX LR

   .align 2
FList: .long Fun0,Fun1,Fun2,Fun3 //pointers to four functions
FListaddr: .long FList

//BKPT instructions
   B ddd
   MOVS R0,#5
   BKPT #1
   MOVS R0,#6
   BKPT
   MOVS R0,#7
   B ddd
//BL instructions
      BL   Func   //Call to Func, return address in LR
ddd:  B  ddd
//example subroutine
Func: PUSH {LR}  //save LR
//body of Func subroutine
      BL   Help  //call Help function
      POP  {PC}  //return
Help:
// body of Help function
      BX   LR

// bic instructions
  MOVS  R1,#255
  MOVS  R7, #0x25    // mask selects bits 5,2,0
  BICS  R1, R7       //R1=R1&(~R7), clears bits 5,2,0
  MOVS  R0,#255
  MOVS  R2, #0x0F    // mask selects bits 3,2,1,0
  BICS  R0, R0, R2   //R0=R0&(~R2), clears bits 3,2,1,0

// asr instructions
  MOVS R5,#255
  ASRS  R7, R5, #9  //R7 = R5>>9, signed, (similar to R7 = R5/512)
  MOVS R2,#3
  MOVS R1,#100
  ASRS  R1, R1, R2  //R0 = R1>>R2, signed (similar to R0 = R1/2R2)

// and instructions
  SUBS  R1,#0 // sets carry bit
  ANDS  R1, R7       //R1=R1&R7
  ANDS  R0, R0, R5   //R0=R0&R5

// add instructions
  ADD   R11, R3       //R11=R11+R3
  ADDS  R4, R4, #200  //R4=R4+100, set flags

       BL Set
       BL Hello
loop:  B loop
Set:   ADR  R0, Hello  //R0 points to function Hello
       MOVS R2,#1
       ORRS R0,R0,R2   //set Thumb bit (this step IS necessary)
       LDR  R1,FuncPtAddr
       STR  R0,[R1]    //FuncPt points to Hello
       BX   LR
       .align
FuncPtAddr: .long  FuncPt
       .align 2       //ADR requires 4-byte alignment
Hello: PUSH {LR}
       ADR  R0, Name  //R0 is the address of the string at Name
       BL   OutString //Print welcome
       POP  {PC}
       .align 2       //ADR requires 4-byte alignment
Name:  .string  "Hello world"

OutString: BX LR


   .align 2
n10000000:  .long 10000000
// PA0 is output
Init:
   PUSH {LR}
   LDR  R0,GPIOA_RSTCTL
   LDR  R1,ResetValue
   STR  R1,[R0] // GPIOA->GPRCM.RSTCTL = (uint32_t)0xB1000003;
   LDR  R0,GPIOA_PWREN
   LDR  R1,PowerValue
   STR  R1,[R0] // GPIOA->GPRCM.PWREN = (uint32_t)0x26000001;
   MOVS R0,#24
   BL   Delay
   LDR  R0,IOMUXPA0
   MOVS R1, #0x81
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PA0INDEX]  = (uint32_t) 0x00000081;
   LDR  R0,GPIOA_DOE31_0
   MOVS R4,#1  // PA0
   LDR  R2,[R0]
   ORRS R2,R2,R4
   STR  R2,[R0] //GPIOA->DOE31_0 |= 1;
   POP  {PC}

   .align 2
GPIOA_RSTCTL:   .long 0x400A0804
GPIOA_PWREN:    .long 0x400A0800
GPIOA_DOE31_0:  .long 0x400A12C0
GPIOA_DOUT31_0: .long 0x400A1280
GPIOA_DIN31_0:  .long 0x400A1380
GPIOB_RSTCTL:   .long 0x400A2804
GPIOB_PWREN:    .long 0x400A2800
GPIOB_DOE31_0:  .long 0x400A32C0
GPIOB_DOUT31_0: .long 0x400A3280
GPIOB_DOUTSET31_0: .long 0x400A3290
GPIOB_DIN31_0:  .long 0x400A3380
PowerValue:     .long 0x26000001
ResetValue:     .long 0xB1000003
IOMUXPA0:       .long 0x40428004+4*0
IOMUXPA18:      .long 0x40428004+4*39
IOMUXPB21:      .long 0x40428004+4*48
IOMUXPB22:      .long 0x40428004+4*49
IOMUXPB26:      .long 0x40428004+4*56
IOMUXPB27:      .long 0x40428004+4*57
n0x00060081:    .long 0x00060081
n0x00050081:    .long 0x00050081
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

    .end          ; end of file
