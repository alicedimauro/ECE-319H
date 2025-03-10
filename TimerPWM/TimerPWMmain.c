/* TimerPWMmain.c
 * Jonathan Valvano
 * December 19, 2022
 * Derived from timx_timer_mode_pwm_edge_sleep_LP_MSPM0G3507_nortos_ticlang
 *              gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *
 */


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/PWM.h"
// PA12 is CCP0 PWM output
// PA13 is CCP1 PWM output
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
int main(void){
  uint32_t sw3,lasts3;
  uint32_t Duty,Period,Change;
  Clock_Init40MHz();
  LaunchPad_Init();
//  PWM_Init(PWMUSELFCLK,0,1000,250,750);    // 32.768 Hz
//  Duty = 100;
//  Period = 1000;
//  Change = 100;
  PWM_Init(PWMUSEBUSCLK,19,10000,2500,7500); // 100Hz
  Duty = 1000;
  Period = 10000;
  Change = 1000;
  lasts3 = (~(GPIOB->DIN31_0)) & S3;
  while(1){
    Clock_Delay(1000000); // debounce switch
    sw3 = (~(GPIOB->DIN31_0)) & S3;
    if(sw3 && (lasts3==0)){ // touch s3
      Duty = Duty+Change;
      if(Duty >= Period){
        Duty = Change;
      }
      PWM_SetDuty(Duty,Period-Duty);
    }
    lasts3 = sw3;
  }
}



