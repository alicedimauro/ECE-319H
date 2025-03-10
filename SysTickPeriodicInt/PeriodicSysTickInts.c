/* PeriodicSysTickInts.c
 * Jonathan Valvano
 * May 19, 2023
 * Derived from timx_timer_mode_periodic_sleep_LP_MSPM0G3507_nortos_ticlang
 *              gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *
 */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
// DOUTTGL31_0 is used so the code is thread-safe.

uint32_t Count;
void SysTick_IntArm(uint32_t period, uint32_t priority){
  SysTick->CTRL  = 0x00;      // disable during initialization
  SysTick->LOAD  = period-1;  // set reload register
  //The ARM Cortex-M0+ only implements the most significant 2 bits of each 8-bit priority field (giving the 4 priority levels).
  SCB->SHP[1]    = (SCB->SHP[1]&(~0xC0000000))|priority<<30;    // set priority (bits 31,30)
  SysTick->VAL   = 0;         // clear count, cause reload
  SysTick->CTRL  = 0x07;      // Enable SysTick IRQ and SysTick Timer
}
#define PA12INDEX 33

int main(void){
  __disable_irq();
  LaunchPad_Init();
//  Clock_Init(OSCFREQ32MHZ);
  Clock_Init40MHz();
//  Clock_Init80MHz(0);
  Count =  0;
//  SysTick_IntArm(16000000,2); //32M/16M = 2Hz
  SysTick_IntArm(400000,2); //40M/400k = 100Hz
//  SysTick_IntArm(800000,2); //80M/800k = 100Hz
 __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void SysTick_Handler(void){
  GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
  GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
  Count++;
  GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
}


/*
void DAC_Init(void){
  GPIOB->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  GPIOB->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(2); // time for gpio to power up
  IOMUX->SECCFG.PINCM[PB3INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB2INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB1INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB0INDEX] = (uint32_t) 0x00100081;
  GPIOB->DOE31_0 |= 0x0F;  // enable outputs PB3-0
}
int main(void){
  __disable_irq();
  DAC_Init();
//  Clock_Init(OSCFREQ32MHZ); //32M/6400 = 5kHz
  Clock_Init48MHz(); //48M/9600 = 5kHz
//  Clock_Init80MHz(0); //80M/16000 = 5kHz
//  while(1){
//    for(uint32_t i=0; i<16; i++){
//      GPIOB->DOUT31_0 = i;
//    }
//  }
  SysTick_IntArm(9600,2);
  __enable_irq();
  while(1){
  }
}
const uint8_t wave[32] = {8,9,11,12,13,14,14,15,15,15,14,
  14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7};
const uint32_t wave2[32]={
 0x01000000,0x01000001,0x01000101,0x01010000,0x01010001,0x01010100,0x01010100,0x01010101,
 0x01010101,0x01010101,0x01010100,0x01010100,0x01010001,0x01010000,0x01000101,0x01000001,
 0x01000000,0x00010101,0x00010001,0x00010000,0x00000101,0x00000100,0x00000100,0x00000001,
 0x00000001,0x00000001,0x00000100,0x00000100,0x00000101,0x00010000,0x00010001,0x00010101
};
void SysTick_Handler(void){ static uint32_t i=0;  // from 0 to 31
//  GPIOB->DOUT3_0 = wave2[i];   // output one value
  GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x0F))|wave[i];   // output one value
  i = (i+1)&0x1F;
}
*/

