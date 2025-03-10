/* InputCaptureIntmain.c
 * Period measurement from PA12
 * Jonathan Valvano
 * December 18, 2022
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *              gpio_input_capture_LP_MSPM0G3507_nortos_ticlang
 */
// Brief description of the program:
// An interrupt is invoked on rising edge of PA12
// Main program creates a 10ms wave on PB27
// Connect PB27 output to PA12 input
// Input capture used to measure period
// resolution = 1us
// precision = 16 bits
// range = 65535us (max)


#include <ti/devices/msp/msp.h>
#include "../inc/InputCapture.h"
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"

uint32_t Data[20];
uint32_t Index=0;
uint32_t Time,Last,Period;
// connect PB27 to PA12
// Period = Data*1us
// Data = 10ms*1MHz = 10000
int main(void){
  __disable_irq();
  Clock_Init40MHz(); // BUSCLK is 20MHz
  LaunchPad_Init();

  InputCapture_Init(INPUTCAPTUREUSEBUSCLK,19,1); // 1us resolution
  Index = 0; Last = 0xFFFF;
  __enable_irq();
  while(1){
 //  delays were chosen to create 10ms wave on PA0
    Clock_Delay(300000);
    GPIOB->DOUTSET31_0 = GREEN;  // PB27 on
    Clock_Delay(100000);
    GPIOB->DOUTCLR31_0 = GREEN; // PB27 off
  }
}
uint32_t iidx,count=0;
void TIMG0_IRQHandler(void){
  count++;
  iidx = TIMG0->CPU_INT.IIDX;// this will acknowledge
  if((iidx) == 5){ // 5 means capture CCD0
    Time = TIMG0->COUNTERREGS.CC_01[0]; // time now
    Period  = (Last-Time)&0xFFFF; // elapsed time since last
    Last = Time;
    if(Index < 20){
      Data[Index] = Period;
      Index++;
    }
  }
}

