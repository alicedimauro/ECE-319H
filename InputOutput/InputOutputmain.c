/* InputOutputmain.c
 * Input from Switches, output to LED
 * Programs 3.2.1, 3.2.2, 3.2.3, 3.2.4, and 3.2.5 from the book
 * Jonathan Valvano
 * April 17, 2023
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 */
// Brief description of the program:
// This is a simple C language project that reads the two switches
// on the LaunchPad and sets the red/green/blue LED.
// The red LED1 is on.
// If SW2 is pressed, the LED2 blue color toggles.
// If SW3 is pressed, the LED2 red color toggles.


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
//  PA0 is red LED1,   index 0 in IOMUX PINCM table, negative logic
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
// Insert J5: Connects PB22 to blue LED2
// Insert J6: Connects PB26 to red LED2
// Insert J7: Connects PB27 to green LED2
int main(void){
  uint32_t sw2,sw3,data;
  LaunchPad_Init();
  LaunchPad_LED1(RED1);  // LED1 on
  data = BLUE;
  while(1){
    Clock_Delay(16000000); // 31.25ns*16000000= 500ms
    sw2 = LaunchPad_InS2();
     if(sw2){
      data = data ^ BLUE;
    }
    sw3 = LaunchPad_InS3();
    if(sw3){
      data = data ^ RED;
    }
    LaunchPad_LED(data);
  }
}

int main2(void){
  Clock_Init(OSCFREQ32MHZ);
  LaunchPad_Init();
  while(1){
    Clock_Delay1ms(500);
    LaunchPad_LEDwhite();
    LaunchPad_LED1off();
    Clock_Delay1ms(500);
    LaunchPad_LEDoff();
    LaunchPad_LED1on();
  }
}


