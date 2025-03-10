/* I2Ctestmain.c
 * Jonathan Valvano
 * November 23, 2022
 * Derived from i2c_controller_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
light and temperature sensors (I2C)
J1.8 ambient light (OPT3001) interrupt (digital)  PA27
J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  PB2_I2C1-SCL
J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) PB3_I2C1-SDA
J2.11 temperature sensor (TMP006) interrupt (digital) PB16 
The TMP006 was discontinued in 2017. Therefore, it is no longer populated on the board.
Documentation of the TMP006 is unchanged in this user's guide to accommodate users who still
have the device populated on the board
*/


//#define CONFIG_MSPM0G350X
//#define SYSCONFIG_WEAK __attribute__((weak))
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/I2C.h"
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
uint32_t light,count;
int main(void){
  count = 0;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  I2C_LightSensor_Init();
  light = I2C_LightSensor_Input();
  I2C_LightSensor_Start();
  while(1){
    Clock_Delay(48000000); // 1 sec
    if(I2C_LightSensor_End(&light)){
      I2C_LightSensor_Start();
      count++;
    }
  }
}
int32_t voltage,temperature;
// ***careful***, some MKII do not have this temperature chip
int main2(void){
  count = 0;
  Clock_Init40MHz();
  LaunchPad_Init();
  I2C_TempSensor_Init();
  I2C_TempSensor_Input(&voltage,&temperature);
  I2C_TempSensor_Start();
  while(1){
    Clock_Delay(48000000); // 1 sec
    if(I2C_TempSensor_End(&voltage,&temperature)){
        I2C_TempSensor_Start();
      count++;
    }
  }
}


