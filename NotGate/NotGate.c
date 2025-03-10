/* NotGate.c
 * Input from Switch PB0, output to LED on PB1
 * Jonathan Valvano
 * September 9, 2023
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 */
// Brief description of the program:
// This is a simple C language project that creates a not gate
// If SW is pressed, the LED is off.
// If SW is not pressed, the LED is on.

#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#define PA0INDEX 0
#define PB0INDEX 11
#define PB1INDEX 12
#define PB2INDEX 14
#define PB3INDEX 15

uint32_t Input,Output;
void Init(void);

// Example 2.3.2, Program 2.3.5 NOT gate
int main(void){
  Init();
  while(1){
    Input = GPIOB->DIN31_0 & 0x01; // read input
    Output = (Input^0x01)<<1;  // not gate, shift into bit 1
    GPIOB->DOUT31_0 = Output;
  }
}

// Initialize LED and switch
void Init(void){
  LaunchPad_Init();

// PINCM
//   bit 25 is HiZ
//   bit 20 is drive strength
//   bit 18 is input enable control
//   bit 17 is pull up control
//   bit 16 is pull down control
//   bit 7 is PC peripheral connected, enable transparent data flow
//   bit 0 selects GPIO function

  IOMUX->SECCFG.PINCM[PB0INDEX] = 0x00040081; // GPIO input
  IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00000081; // GPIO output
  GPIOB->DOE31_0 = 0x02; // enable output PB1
}

// Initialize PA0
void InitA(void){
    LaunchPad_Init();

// PINCM
//   bit 25 is HiZ
//   bit 20 is drive strength
//   bit 18 is input enable control
//   bit 17 is pull up control
//   bit 16 is pull down control
//   bit 7 is PC peripheral connected, enable transparent data flow
//   bit 0 selects GPIO function

  IOMUX->SECCFG.PINCM[PA0INDEX] = (uint32_t) 0x00000081; // GPIO output
  GPIOA->DOE31_0 = 0x01; // enable output PA0
}
// Toggle PA0, Program 1.10.1
int maina(void){
  InitA();
  uint32_t data;
  while(1){
    data = GPIOA->DOUT31_0; // read previous
    data = data^0x01;  // toggle
    GPIOA->DOUT31_0 = data;
    Clock_Delay(16000000); // 500ms
  }
}

// test of program 3.2.8
// positive logic switch on PB3
// Initialize PB3 input, external pulldown resistor
void Switch_Init(void){
  GPIOB->GPRCM.RSTCTL = 0xB1000003;
  GPIOB->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24);  // short delay
  IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00040081; // GPIO input
}
uint32_t Switch_Input(void){
  return (GPIOB->DIN31_0 & 0x08);
}
uint32_t Switch;
int mainb(void){
  Switch_Init();
  while(1){
    Switch = Switch_Input();
  }
}

// test of Program 3.2.9
void LED_Init(void){
  GPIOB->GPRCM.RSTCTL = 0xB1000003;
  GPIOB->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24);  // short delay
  IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00100081; // DRV
  GPIOB->DOE31_0 |= 0x02; // enable output PB1

}
void LED_Off(void){
  GPIOB-> DOUTCLR31_0 = 0x02;
}
void LED_On(void){
  GPIOB-> DOUTSET31_0 = 0x02;
}
void LED_Toggle(void){
  GPIOB-> DOUTTGL31_0 = 0x02;
}
int mainc(void){
  LED_Init();
  LED_On();
  LED_Off();
  while(1){
    LED_Toggle();
    Clock_Delay(16000000); // 500 ms
  }
}

// test of Program 3.2.10
int maind(void){
  GPIOB->GPRCM.RSTCTL = 0xB1000003;
  GPIOB->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24);  // short delay
  IOMUX->SECCFG.PINCM[PB0INDEX] = 0x00100081; // DRV
  IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00100081; // DRV
  GPIOB->DOE31_0 |= 0x03; // enable output PB1
  GPIOB-> DOUTSET31_0 = 0x02; // PB1=1
  GPIOB-> DOUTCLR31_0 = 0x01; // PB0=0
  while(1){
    GPIOB-> DOUTTGL31_0 = 0x03;
  }
}

// test of Program 3.2.11
void Stepper_Init(void){
  GPIOB->GPRCM.RSTCTL = 0xB1000003;
  GPIOB->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24);  // short delay
  IOMUX->SECCFG.PINCM[PB0INDEX] = 0x00100081; // DRV
  IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00100081; // DRV
  IOMUX->SECCFG.PINCM[PB2INDEX] = 0x00100081; // DRV
  IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00100081; // DRV
  GPIOB->DOE31_0 |= 0x0F; // enable output PB3-0
  GPIOB-> DOUTSET31_0 = 0x09; // output=1001
  GPIOB-> DOUTCLR31_0 = 0x06;
}
void Stepper_Toggle(uint32_t n){
  GPIOB-> DOUTTGL31_0 = n;
}
#define T20800us (20800000/32)  // 20.8ms
int maine(void){
  Stepper_Init();
  while(1){
    Stepper_Toggle(0x0C);
    Clock_Delay(T20800us);
    Stepper_Toggle(0x03);
    Clock_Delay(T20800us);
  }
}

#define T5ms (80000*5)
int mainf(void){
  Clock_Init80MHz(0);
  SysTick_Init();
  Stepper_Init();
  while(1){
    Stepper_Toggle(0x0C);
    SysTick_Wait(T5ms);
    Stepper_Toggle(0x03);
    SysTick_Wait(T5ms);
  }
}
uint32_t sqrt2(uint32_t s){ int n; // loop counter
uint32_t t;     // t*t will become s
  t = s/16+1;        // initial guess
  for(n = 16; n; --n){ // will finish
    t = ((t*t+s)/t)/2;
  }
  return t;
}

int maing(void){ uint32_t tt;
  Clock_Init80MHz(0);
  LED_Init();
  while(1){
    LED_On();
    tt = sqrt2(230400);
    LED_Off();
  }
}

#define SIZE 32
uint8_t DBuf[SIZE];
uint16_t TBuf[SIZE];
uint32_t Cnt;
void Save4(void){
  if(Cnt < SIZE){
    DBuf[Cnt]=GPIOB->DOUT31_0;
    TBuf[Cnt]=TIMG8->COUNTERREGS.CTR;
    Cnt++;
  }
}
void Stepper_Toggle2(uint32_t n){
  GPIOB-> DOUTTGL31_0 = n;
  Save4();
}
int mainh(void){
  Clock_Init80MHz(0); // SYSCLK = 40 MHz
  TimerG8_Init(5,80); // 10us

  SysTick_Init();
  Stepper_Init();
  while(1){
    Stepper_Toggle2(0x0C);
    SysTick_Wait(400000); // 5ms
    Stepper_Toggle2(0x03);
    SysTick_Wait(400000); // 5ms
  }
}
uint16_t Last=0 ;
void Save5(void){
  if(Cnt < SIZE){ uint16_t now ;
    DBuf[Cnt] = GPIOB->DOUT31_0;
    now = TIMG8->COUNTERREGS.CTR;
    TBuf[Cnt] = Last-now;
    Last = now;
    Cnt++;
  }
}
void Stepper_Toggle3(uint32_t n){
  GPIOB-> DOUTTGL31_0 = n;
  Save5();
}
int maini(void){
  Clock_Init80MHz(0); // SYSCLK = 40 MHz
  TimerG8_Init(5,80); // 10us

  SysTick_Init();
  Stepper_Init();
  while(1){
    Stepper_Toggle3(0x0C);
    SysTick_Wait(400000); // 5ms
    Stepper_Toggle3(0x03);
    SysTick_Wait(400000); // 5ms
  }
}



