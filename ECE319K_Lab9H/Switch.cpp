/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA18INDEX] = 0x00040081; // input, no pull, brake
  IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00040081; // input, no pull, reset IMU
  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00040081; // input, no pull, acceleration
  IOMUX->SECCFG.PINCM[PA15INDEX] = 0x00040081; // input, no pull, use power up
}
// return current state of switches
uint32_t Switch_In(void){
    // write this
  uint32_t data = GPIOA->DIN31_0;
  data = ((data>>15)&0x0F); // Read 15-18
  return data; // returns the four switches status as some value 1,2,4,8
}
