/* LCDtestmain
 * Jonathan Valvano
 * November 21, 2022
 * Derived from spi_controller_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 *              spi_controller_command_data_control_LP_MSPM0G3507_nortos_ticlang
 *              spi_peripheral_register_format_LP_MSPM0G3507_nortos_ticlang
 */


//#define CONFIG_MSPM0G350X
//#define SYSCONFIG_WEAK __attribute__((weak))
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/SPI.h"
#include "../inc/LCD.h"
#include "../inc/ADC.h"
// Educational BoosterPack MKII (BOOSTXL-EDUMKII)
//SPI MKII LCD
//J1.7  SPI1 SCLK PB9_SPI1-SCK
//J2.13 SPI1 CS0  PB6_SPI1-CS0
//J2.15 SPI1 PICO PB8_SPI1-PICO
//J2.17 LCD !RST  PB15
//J4.31 LCD RS    PA13
//MKII  Microphone J1.5 PB24_ADC0.5
int main1(void){
 // Clock_Init80MHz(0);
    Clock_Init40MHz();
 // Clock_Init(OSCFREQ4MHZ);
  LaunchPad_Init();
  SPI_Init();
  GPIOB->DOUT31_0 |= BLUE ;
  while(1){
    SPI_Reset();
    GPIOB->DOUTTGL31_0 = BLUE;
  }
}
int main2(void){
uint32_t command=0;
  Clock_Init40MHz();
  LaunchPad_Init();
  SPI_Init();
  GPIOB->DOUT31_0 |= BLUE ;
  while(1){
    SPI_OutCommand(command);
    for(int i=0; i<8; i++){
      SPI_OutData(i);
    }
    command = (command+1)&0x0F;
    GPIOB->DOUT31_0 ^= BLUE;
  }
}

int main3(void){ uint32_t data=0;
 //Clock_Init80MHz(0);
  Clock_Init40MHz();
 // Clock_Init(OSCFREQ4MHZ);
  LaunchPad_Init();
  GPIOB->DOUT31_0 |= BLUE ;

  LCD_Init(); //SPI MKII LCD
  LCD_FillScreen(LCD_BLACK);
  LCD_SetCursor(0, 0);
  LCD_SetTextColor(LCD_CYAN);
  LCD_OutString("LCD test main3\n");
  while(1){
    LCD_SetCursor(0, 1);
    LCD_OutUDec4(data, LCD_YELLOW);
    data = (data+1)&0xFF;
    GPIOB->DOUTTGL31_0 = BLUE;
    Clock_Delay1ms(100); // 100ms
  }
}

const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";

int main4(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  LCD_Init(); //SPI MKII LCD
  LCD_FillScreen(LCD_BLACK);
  LCD_SetTextColor(LCD_CYAN);
  LCD_OutString("LCD test main4\n");
  LCD_DrawString(0, 1, (char *)Hello_English, LCD_RED);
  LCD_DrawString(0, 2, (char *)Hello_Spanish, LCD_WHITE);
  LCD_DrawString(0, 3, (char *)Hello_Portuguese, LCD_YELLOW);
  LCD_DrawString(0, 4, (char *)Hello_French, LCD_CYAN);
  GPIOB->DOUTTGL31_0 = BLUE;
  while(1){
    GPIOB->DOUTTGL31_0 = BLUE;
    Clock_Delay1ms(100); // 100ms
  }
}

int main(void){uint32_t sound; uint32_t time=0;
 //Clock_Init80MHz(0);
  Clock_Init40MHz();
// Clock_Init(OSCFREQ4MHZ);
  LaunchPad_Init();
  ADC0_Init(5,ADCVREF_VDDA); //MKII  Microphone J1.5 PB24_ADC0.5
  LCD_Init(); //SPI MKII LCD
  LCD_FillScreen(LCD_BLACK);
  LCD_Drawaxes(LCD_YELLOW, LCD_BLACK, "time","sound", LCD_WHITE, "", LCD_BLACK, 3075, 1024);
  while(1){
    time++;
    GPIOB->DOUTTGL31_0 = BLUE;
    sound = ADC0_In();
    LCD_PlotPoint(sound,LCD_WHITE);
    Clock_Delay1ms(1); // 1ms
    if(time>10){
      time = 0;
      LCD_PlotIncrement();
    }
  }
}


