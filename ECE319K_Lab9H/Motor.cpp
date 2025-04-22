#include "../inc/LaunchPad.h"
#include <ti/devices/msp/msp.h>

// LaunchPad.h defines all the indices into the PINCM table

#define MOTOR PB4INDEX

// initialize motor
void Motor_Init(void) {
  // write this
  // PINCM
  //   bit 25 is HiZ
  //   bit 20 is drive strength
  //   bit 18 is input enable control
  //   bit 17 is pull up control
  //   bit 16 is pull down control
  //   bit 7 is PC peripheral connected, enable transparent data flow
  //   bit 0 selects GPIO function
  IOMUX->SECCFG.PINCM[MOTOR] = (uint32_t)0x00000081;

  // DOE31_0 Data output enable
  GPIOB->DOE31_0 |= (1 << 4);
  GPIOB->DOUTCLR31_0 = (1 << 4); // Motor off
}

// turn on motor
void Motor_On(void) {
    GPIOB->DOUTSET31_0 = (1 << 4);
}

// turn off motor
void Motor_Off(void) {
  GPIOB->DOUTCLR31_0 = (1 << 4);
}