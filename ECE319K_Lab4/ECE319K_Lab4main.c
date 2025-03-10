/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Alice Di Mauro and Ravi Shah
 */

#include "../inc/Clock.h"
#include "../inc/Dump.h" // student's Lab 3
#include "../inc/LaunchPad.h"
#include "../inc/Timer.h"
#include "../inc/UART.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
// put both EIDs in the next two lines
const char EID1[] = "rds3747"; //  ;replace abc123 with your EID
const char EID2[] = "ad55559"; //  ;replace abc123 with your EID
// Hint implement Traffic_Out before creating the struct, make struct match your
// Traffic_Out

struct State {
  uint32_t Out;
  uint32_t Time;
  const struct State *Next[8];
};
typedef const struct State State_t;
#define westG &FSM[0]
#define westY &FSM[1]
#define allred &FSM[2]
#define walks &FSM[3]
#define redOn1 &FSM[4]
#define redOff1 &FSM[5]
#define redOn2 &FSM[6]
#define redOff2 &FSM[7]
#define southG &FSM[8]
#define southY &FSM[9]
#define allred3 &FSM[10]
#define allred2 &FSM[11]
#define allred4 &FSM[12]

State_t FSM[13] = {
    {0x00020401, 100, {westG, westG, westY, westY, westY, westY, westY, westY}},
    // put something here
    {0x01020402,
     100,
     {allred, allred, allred, allred, allred, allred, allred, allred}},
    {0x02020404,
     200,
     {westG, westG, southG, southG, walks, walks, southG, southG}},
    {0x03070404,
     100,
     {redOn1, redOn1, redOn1, redOn1, redOn1, redOn1, redOn1, redOn1}},
    {0x04020404,
     100,
     {redOff1, redOff1, redOff1, redOff1, redOff1, redOff1, redOff1, redOff1}},
    {0x05000404,
     100,
     {redOn2, redOn2, redOn2, redOn2, redOn2, redOn2, redOn2, redOn2}},
    {0x06020404,
     100,
     {redOff2, redOff2, redOff2, redOff2, redOff2, redOff2, redOff2, redOff2}},
    {0x07000404,
     100,
     {allred, allred4, allred, allred4, allred, allred4, allred, allred4}},
    {0x08020104,
     100,
     {southG, southY, southG, southY, southY, southY, southY, southY}},
    {0x09020204,
     100,
     {allred, allred3, allred, allred3, allred2, allred2, allred2, allred2}},
    {0x0A020404, 200, {westG, westG, westG, westG, westG, westG, westG, westG}},
    {0x0B020404, 200, {walks, walks, walks, walks, walks, walks, walks, walks}},
    {0x0C020404,
     200,
     {westG, westG, westG, westG, westG, westG, westG, westG}}};

State_t *Pt; // state pointer

// initialize all 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void) { // assumes LaunchPad_Init resets and powers A and B
  IOMUX->SECCFG.PINCM[PB15INDEX] = 0x00040081; // GPIO input
  IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081; // GPIO input
  IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00040081; // GPIO output

  IOMUX->SECCFG.PINCM[PB2INDEX] = 0x81; // GPIO input
  IOMUX->SECCFG.PINCM[PB1INDEX] = 0x81; // GPIO input
  IOMUX->SECCFG.PINCM[PB0INDEX] = 0x81; // GPIO output
  IOMUX->SECCFG.PINCM[PB8INDEX] = 0x81; // GPIO output
  IOMUX->SECCFG.PINCM[PB7INDEX] = 0x81; // GPIO input
  IOMUX->SECCFG.PINCM[PB6INDEX] = 0x81; // GPIO input

  GPIOB->DOE31_0 |=
      ((1 << 2) + (1 << 1) + (1) + (1 << 8) + (1 << 7) + (1 << 6));
}
/* Activate LEDs
 * Inputs: west is 3-bit value to three east/west LEDs
 *         south is 3-bit value to three north/south LEDs
 *         walk is 3-bit value to 3-color positive logic LED on PB22,PB26,PB27
 * Output: none
 * - west =1 sets west green
 * - west =2 sets west yellow
 * - west =4 sets west red
 * - south =1 sets south green
 * - south =2 sets south yellow
 * - south =4 sets south red
 * - walk =0 to turn off LED
 * - walk bit 22 sets blue color
 * - walk bit 26 sets red color
 * - walk bit 27 sets green color
 * Feel free to change this. But, if you change the way it works, change the
 * test programs too Be friendly*/
void Traffic_Out(uint32_t west, uint32_t south, uint32_t walk) {
  // Switch statement for west
  switch (west) {

  case 1:
    // PB6 Set West Green
    GPIOB->DOUTSET31_0 = 1 << 6;
    GPIOB->DOUTCLR31_0 = 1 << 7; // Turn off yellow
    GPIOB->DOUTCLR31_0 = 1 << 8; // Turn off red
    break;

  case 2:
    // PB7 Set West Yellow
    GPIOB->DOUTSET31_0 = 1 << 7;
    GPIOB->DOUTCLR31_0 = 1 << 6; // Turn off green
    GPIOB->DOUTCLR31_0 = 1 << 8; // Turn off red
    break;

  case 4:
    // PB8 Set West Red
    GPIOB->DOUTSET31_0 = 1 << 8;
    GPIOB->DOUTCLR31_0 = 1 << 6; // Turn off green
    GPIOB->DOUTCLR31_0 = 1 << 7; // Turn off yellow
    break;

  default:
    // Fail if input not valid
    assert(0);
  }

  // switch statement for south
  switch (south) {

  case 1:
    // PB0 Set South Green
    GPIOB->DOUTSET31_0 = 1;
    GPIOB->DOUTCLR31_0 = 1 << 1; // Turn off yellow
    GPIOB->DOUTCLR31_0 = 1 << 2; // Turn off red
    break;

  case 2:
    // PB1 Set South Yellow
    GPIOB->DOUTSET31_0 = 1 << 1;
    GPIOB->DOUTCLR31_0 = 1;      // Turn off green
    GPIOB->DOUTCLR31_0 = 1 << 2; // Turn off red
    break;

  case 4:
    // PB2 Set South Red
    GPIOB->DOUTSET31_0 = 1 << 2;
    GPIOB->DOUTCLR31_0 = 1;      // Turn off green
    GPIOB->DOUTCLR31_0 = 1 << 1; // Turn off yellow
    break;

  default:
    // Fail if invalid input
    assert(0);
  }

  // switch statement for walk
  switch (walk) {

  case 0:
    // Led flicker is off
    GPIOB->DOUTCLR31_0 = ((1 << 22) + (1 << 26) + (1 << 27));
    break;

  case 2:
    // Red light warning flicker
    GPIOB->DOUTSET31_0 = 1 << 26;
    GPIOB->DOUTCLR31_0 = ((1 << 22) + (1 << 27));

    break;

  case 7:
    // White light pedestrians can walk
    GPIOB->DOUTSET31_0 = (1 << 22) + (1 << 26) + (1 << 27);
    break;

  default:
    // Fail if invalid input
    assert(0);
  }
}

/* Read sensors
 * Input: none
 * Output: sensor values
 * - bit 0 is west car sensor
 * - bit 1 is south car sensor
 * - bit 2 is walk people sensor
 * Feel free to change this. But, if you change the way it works, change the
 * test programs too
 */
uint32_t Traffic_In(void) {
  uint32_t input = GPIOB->DIN31_0;
  uint32_t output = 0;
  output = (input >> 15) & 7;
  // shift bits 17-15 to bits 0-2, clear all bits except 0-2 
  return output;
}

// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main1(void) { // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab4Grader(0); // print assignment, no grading
  while (1) {
  }
}
// use main2 to debug LED outputs
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_Out  function
int main2(void) { // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();
  if ((GPIOB->DOE31_0 & 0x20) == 0) {
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  UART_Init();
  UART_OutString("Lab 4, Spring 2025, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= ");
  UART_OutString((char *)EID1);
  UART_OutString("\n\r");
  UART_OutString("EID2= ");
  UART_OutString((char *)EID2);
  UART_OutString("\n\r");
  uint32_t out;
  while (1) {
    // write code to test your Traffic_Out
    Traffic_Out(2, 4, 7); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(1, 4, 7); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(4, 1, 7); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(2, 2, 0); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(2, 1, 7); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(2, 4, 0); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(2, 4, 2); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(1, 1, 0); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(1, 1, 2); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(4, 2, 2); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    Traffic_Out(4, 2, 0); // Test your own
    out = GPIOB->DOUT31_0;
    Debug_Dump(out);
    if ((GPIOB->DOUT31_0 & 0x20) == 0) {
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main3 to debug the three input switches
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_In  function
int main3(void) { // main3
  uint32_t last = 0, now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2025, Step 2. Debug switches\n\r");
  UART_OutString("EID1= ");
  UART_OutString((char *)EID1);
  UART_OutString("\n\r");
  UART_OutString("EID2= ");
  UART_OutString((char *)EID2);
  UART_OutString("\n\r");
  while (1) {
    now = Traffic_In(); // Your Lab4 input
    if (now != last) {  // change
      UART_OutString("Switch= 0x");
      UART_OutUHex(now);
      UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void) { // main4
  uint32_t input;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
                  // set initial state
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2025, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= ");
  UART_OutString((char *)EID1);
  UART_OutString("\n\r");
  UART_OutString("EID2= ");
  UART_OutString((char *)EID2);
  UART_OutString("\n\r");
  // initialize your FSM
  SysTick_Init(); // Initialize SysTick for software waits
  Pt = westG;
  uint32_t inputs;

  while (1) {
    Traffic_Out((Pt->Out & 0xFF), ((Pt->Out & 0xFF00) >> 8),
                ((Pt->Out & 0xFF0000) >> 16));
    // Isolate west, south, walk bits. 
    SysTick_Wait10ms(Pt->Time);
    inputs = Traffic_In();
    Pt = Pt->Next[inputs];
    // 1) output depending on state using Traffic_Out
    // call your Debug_Dump logging your state number and output
    // 2) wait depending on state
    // 3) hard code this so input always shows all switches pressed
    // 4) next depends on state and input
  }
}
// use main5 to grade
int main(void) { // main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
                  // initialize your FSM
  Debug_Init();
  SysTick_Init(); // Initialize SysTick for software waits
  // initialize your FSM
  Lab4Grader(1); // activate UART, grader and interrupts

  Pt = westG;
  uint32_t inputs;

  while (1) {
    Traffic_Out((Pt->Out & 0xFF), ((Pt->Out & 0xFF00) >> 8),
                ((Pt->Out & 0xFF0000) >> 16));
    Debug_Dump(Pt->Out);
    SysTick_Wait10ms(Pt->Time);
    inputs = Traffic_In();
    Pt = Pt->Next[inputs];
    // 1) output depending on state using Traffic_Out
    // call your Debug_Dump logging your state number and output
    // 2) wait depending on state
    // 3) input from switches
    // 4) next depends on state and input
  }
}
