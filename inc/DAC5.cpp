/* DAC5.cpp
 * Students put your names here
 * Modified: December 26, 2024
 * 5-bit binary-weighted DAC connected to PB4-PB0
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/DAC5.h"
// **************DAC5_Init*********************
// Initialize 5-bit DAC, called once
// Input: none
// Output: none
void DAC5_Init(void){
// Assumes LaunchPad_Init has been called
// I.e., PortB has already been reset and activated (do not reset PortB here again)
     // write this
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x81; // GPIO input
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x81; // GPIO input
  IOMUX->SECCFG.PINCM[PA26INDEX] = 0x81; // GPIO output
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x81; // GPIO output
  IOMUX->SECCFG.PINCM[PA28INDEX] = 0x81; // GPIO output
  GPIOA->DOE31_0 |=
      ((1 << 24) + (1 << 25) + (1 << 26) + (1 << 27) + (1 << 28));
}
// **************DAC5_Out*********************
// output to DAC5
// Input: 5-bit data, 0 to 31
// Input=n is converted to n*3.3V/31
// Output: none
// Note: this solution must be friendly
void DAC5_Out(uint32_t data){
     // write this
     GPIOA->DOUT31_0 = (GPIOA->DOUT31_0 & (~0x1F000000)) | (data << 24);
}