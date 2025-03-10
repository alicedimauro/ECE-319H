// Dump.c
// Your solution to ECE319K Lab 3 Spring 2025
// Author: Alice Di Mauro
// Last Modified: 02/05/2025

  
#include <ti/devices/msp/msp.h>
#include "../inc/Timer.h"
#define MAXBUF 50
uint32_t DataBuffer[MAXBUF];
uint32_t TimeBuffer[MAXBUF];
uint32_t DebugCnt; // 0 to MAXBUF (0 is empty, MAXBUF is full) ; indexes into the arrays

// *****Debug_Init******
// Initializes your index or pointer.
// Input: none
// Output:none
void Debug_Init(void){ // First function
  DebugCnt = 0;
  TimerG12_Init();

// students write this for Lab 3
// This function should also initialize Timer G12, call TimerG12_Init.
// Initializes the buffers by clearing the index
 
}

// *****Debug_Dump******
// Records one data and one time into the two arrays.
// Input: data is value to store in DataBuffer
// Output: 1 for success, 0 for failure (buffers full)
uint32_t Debug_Dump(uint32_t data){ // 2nd function
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
// Records one data and one time into the two arrays
// Data is passed by value, which could be input, output, or any combo of it
// Time will be obtained by reading the 32-bit hardware counter value of Timer G12
// This 32-bit counter decrements each 12.5ns bus cycle. When the counter reaches 0, 
//it automatically rolls over and continues to count down from 0xFFFFFFFF. It takes about 53 seconds 
//to cycle once through all the 32-bit possible counter values. The measurements in Debug_Period will
// be correct as long as the period is less than 53 seconds. The function returns 1 if recording was 
//successful and 0 if the arrays were full and the data/time were not recorded. 

  if (DebugCnt >= MAXBUF) { // Records snapshot of data, measures excecution speed
        return 0; // Buffer is full
    }

    DataBuffer[DebugCnt] = data;
    TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR; // Read timer value
    DebugCnt++;

  return 1; // success
}
// *****Debug_Dump2******
// Always record data and time on the first call to Debug_Dump2
// However, after the first call
//    Records one data and one time into the two arrays, only if the data is different from the previous call.
//    Do not record data or time if the data is the same as the data from the previous call
// Input: data is value to store in DataBuffer
// Output: 1 for success (saved or skipped), 0 for failure (buffers full)
uint32_t Debug_Dump2(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
  
  return 1; // success
}

// *****Debug_Period******
// Calculate period of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in bus cycles
// Period is defined as rising edge (low to high) to the next rising edge.
// Return 0 if there is not enough collected data to calculate period .
uint32_t Debug_Period(uint32_t mask){ // 3rd function
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
 
 uint32_t firstTime = 0xFFFF; 
 // Place holder? Literal garbage I need a number
 uint32_t secondTime = 0; 
 // Initialize to zero so it doesn't have garbage later?
 uint8_t index = 0;
 uint32_t myArray[MAXBUF];
 uint32_t period[MAXBUF];
 uint32_t total = 0;

// Measures PWM signal frequency?

 for (int i=0; i < (MAXBUF) ; i++) {
   myArray[i] = mask & DataBuffer[i]; 
   // Mask specifies which bit to observe
   // This just makes my life easier for the next for loop
 }


  for (int i=0 ; i < (MAXBUF - 1) ; i++) {
    if ((myArray[i] == 0) && (myArray[i+1] != 0)) { //
      secondTime = TimeBuffer[i+1];
      period[index] = firstTime - secondTime;
      firstTime = secondTime;
      index++;
    }
  }


  if (index <= 1){ 
  // If there's less than 2 indexes that means you can;t
  // possibly calculate a period
    return 0;
  }


  for (int i = 1; i < index; i++) { // the 0th element is garbage!!
    total += period[i];
  }
 
 
  return (total / (index - 1)); // Outputs period in bus cycles
 
}



// *****Debug_Duty******
// Calculate duty cycle of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in percent (0 to 100)
// Period is defined as rising edge (low to high) to the next rising edge.
// High is defined as rising edge (low to high) to the next falling edge.
// Duty cycle is (100*High)/Period
// Return 0 if there is not enough collected data to calculate duty cycle.
uint32_t Debug_Duty(uint32_t mask) { // Extra credit
// takes a parameter called mask
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
// This function will calculate the average duty cycle (in bus cycles) for the signal. 
// Period is defined as rising edge (low to high) to the next rising edge. Pulse width is defined
// as the time from rising to fall edge. Duty cycle in percent is (100*Pulse width)/Period.  
// Be careful to look at the index or pointer and only calculate period on actual recorded data. 
// This function should not alter the recorded data. Return 0 if there is not enough collected data to 
// calculate period (i.e., no rising edge, falling edge, rising edge).
 uint32_t firstTime = 0xFFFF; 
 // Place holder? Literal garbage I need a number
 uint32_t secondTime = 0; 
 // Initialize to zero so it doesn't have garbage later?
 uint8_t index = 0;
 uint32_t myArray[MAXBUF];
 uint32_t cycle[MAXBUF];
 uint32_t total = 0;

 for (int i=0; i < (MAXBUF) ; i++) {
   myArray[i] = mask & DataBuffer[i]; 
   // Mask specifies which bit to observe
   // This just makes my life easier for the next for loop
 }

 for (int i=0 ; i < (MAXBUF - 1) ; i++) {
    if ((myArray[i] == 0) && (myArray[i+1] != 0) && (myArray[i+2] == 0)) { //
      secondTime = TimeBuffer[i+2];
 //     period[index] = firstTime - secondTime;
      firstTime = secondTime;
      index++;
    }
  }


  if (index <= 1){ 
  // If there's less than 2 indexes that means you can;t
  // possibly calculate a period
    return 0;
  }


  for (int i = 1; i < index; i++) { // the 0th element is garbage!!
    total += cycle[i];
  }
 

//    if (cycle == 0) {
        return 0; // Avoid division by zero
 //   }

//  uint32_t Period = totalPeriod / (index - 1);
//    uint32_t avgPulse = totalPulse / index;
//    return (100 * avgPulse) / avgPeriod; // Duty cycle in percentage
//uint32_t dutyCycle = 100 * pulseWidth / period;
//    return (dutyCycle); // Duty cycle percentage
}

// Lab2 specific debugging code
uint32_t Theperiod;
void Dump(void){
  uint32_t out = GPIOB->DOUT31_0&0x0070000; // PB18-PB16 outputs
  uint32_t in = GPIOB->DIN31_0&0x0F;        // PB3-PB0 inputs
  uint32_t data = out|in;                   // PB18-PB16, PB3-PB0
  uint32_t result = Debug_Dump(data);       // calls your Lab3 function
// Everytime Dump is called data goes in my result.
// When my buffer is full theperiod is calculated
  if(result == 0){ // not zero means full
    Theperiod = Debug_Period(1<<18);        // calls your Lab3 function
   __asm volatile("bkpt; \n"); // breakpoint here
// observe Theperiod
  }
}




