/* TimerPeriodicIntmain.c
 * Jonathan Valvano
 * August 8, 2023
 * Derived from timx_timer_mode_periodic_sleep_LP_MSPM0G3507_nortos_ticlang
 *              gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *
 */


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/Timer.h"
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
// DOUTTGL31_0 is used so the code is thread-safe.

// power Domain PD0
// initialize G0/G8 for periodic interrupt
// for 32MHz bus clock, Timer clock is 32MHz
// for 40MHz bus clock, Timer clock is ULPCLK 20MHz
// for 80MHz bus clock, Timer clock is ULPCLK 40MHz
// frequency = TimerClock/prescale/period

uint32_t count;
int main(void){
  __disable_irq();
//  Clock_Init80MHz(0);
  Clock_Init40MHz();
//  Clock_Init(OSCFREQ32MHZ);
  count=0;
  LaunchPad_Init();
  TimerG0_IntArm(10000,40,2); // 40MHz/40/10000 = 100Hz
  __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void TIMG0_IRQHandler(void){
  if((TIMG0->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}


int main1(void){
  __disable_irq();
//  Clock_Init80MHz(0);
  Clock_Init40MHz();
//  Clock_Init(OSCFREQ32MHZ);
  count=0;
  LaunchPad_Init();
  TimerG8_IntArm(10000,20,2); // 20MHz/20/10000 = 100Hz
  __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void TIMG8_IRQHandler(void){
  if((TIMG8->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}


// Power Domain PD1
// initialize A0/A1/G6/G7/G12 for periodic interrupt
// for 32MHz bus clock, Timer clock is 32MHz
// for 40MHz bus clock, Timer clock is MCLK 40MHz
// for 80MHz bus clock, Timer clock is MCLK 80MHz
// frequency = TimerClock/prescale/period

int main2(void){
  __disable_irq();
  Clock_Init80MHz(0);
  count=0;
  LaunchPad_Init();
  TimerA0_IntArm(10000,80,2); // 80MHz/10000/80 = 100Hz
  __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void TIMA0_IRQHandler(void){
  if((TIMA0->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}



int main3(void){
  __disable_irq();
  Clock_Init80MHz(0);
  count=0;
  LaunchPad_Init();
  TimerA1_IntArm(10000,80,2); // 80MHz/10000/80 = 100Hz
  __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void TIMA1_IRQHandler(void){
  if((TIMA1->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}



int main4(void){
  __disable_irq();
  Clock_Init40MHz();
  count=0;
  LaunchPad_Init();
  TimerG7_IntArm(10000,40,2); // 40MHz/10000/40 = 100Hz
  __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void TIMG7_IRQHandler(void){
  if((TIMG7->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}


int main5(void){
  __disable_irq();
  Clock_Init40MHz();
  count=0;
  LaunchPad_Init();
  TimerG6_IntArm(10000,40,2); // 40MHz/10000/40 = 100Hz
  __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void TIMG6_IRQHandler(void){
  if((TIMG6->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}


int main6(void){
  __disable_irq();
  Clock_Init40MHz();
  count=0;
  LaunchPad_Init();
  TimerG12_IntArm(400000,2); // 40MHz/400000 = 100Hz
  __enable_irq();
  while(1){
    GPIOA->DOUTTGL31_0 = RED1;
  }
}

void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}


void myfunction(void){
  SysTick_Wait10ms(5);
}
int main7(void){
uint16_t elapsed,start,end;
  Clock_Init80MHz(0);
  SysTick_Init();
  TimerG8_Init(4,100); // 10us
  start = TIMG8->COUNTERREGS.CTR;
  myfunction();
  end = TIMG8->COUNTERREGS.CTR;
  elapsed = start-end; // should be 5000, 5000*10us=50ms
  while(1){ }
}


int main8(void){
uint16_t elapsed,start,end;
  Clock_Init80MHz(0);
  SysTick_Init();
  TimerG8_Init(1,1); // 25ns
  start = TIMG8->COUNTERREGS.CTR;
  SysTick_Wait(10000); // 12.5ns*10000 = 125us
  end = TIMG8->COUNTERREGS.CTR;
  elapsed = start-end; // 5000*25ns = 125us
  while(1){ }
}
int main9(void){
uint16_t elapsed,start,end;
  Clock_Init80MHz(0);
  TimerG8_Init(1,1); // 25ns
  start = TIMG8->COUNTERREGS.CTR;
  Clock_Delay(10000); // 12.5ns*10000 = 125us
  end = TIMG8->COUNTERREGS.CTR;
  elapsed = start-end; // 5000*25ns = 125us
  while(1){ }
}

uint32_t udiv(uint32_t x, uint32_t y){
  return x/y;
}
int main10(void){ uint32_t z;
uint16_t elapsed,start,end;
  Clock_Init80MHz(0);
  TimerG8_Init(1,1); // 25ns
  start = TIMG8->COUNTERREGS.CTR;
  z = udiv(0xFFFF,1);
  end = TIMG8->COUNTERREGS.CTR;
  elapsed = start-end-4; // 159*25ns
  while(1){ }
}
int main11(void){ uint32_t z;
uint32_t elapsed,start,end;
  Clock_Init80MHz(0);
  SysTick->LOAD = 0xFFFFFF;
  SysTick->VAL = 0;
  SysTick->CTRL = 5;
  start = SysTick->VAL;
  z = udiv(0xFFFF,1);
  end = SysTick->VAL;
  elapsed = (start-end)&0xFFFFFF; // 328*12.5ns
  while(1){ }
}
uint32_t Time[100], Place[100], N;
void Profile(uint32_t p){
  if(N<100){
    Time[N] = SysTick->VAL; // record time
    Place[N] = p;           // record place
    N++;
  }
}
uint32_t sqrt2(uint32_t s){ int n; // loop counter
uint32_t t;     // t*t will become s
Profile(0);
  t = s/16+1;        // initial guess
  for(n = 16; n; --n){ // will finish
Profile(1);
    t = ((t*t+s)/t)/2;
  }
Profile(2);
  return t;
}

int main12(void){ uint32_t z;
uint32_t elapsed,start,end;
  Clock_Init80MHz(0);
  SysTick->LOAD = 0xFFFFFF;
  SysTick->VAL = 0;
  SysTick->CTRL = 5;
  start = SysTick->VAL;
  sqrt2(10000);
  end = SysTick->VAL;
  elapsed = (start-end)&0xFFFFFF;
  while(1){ }
}
uint32_t sqrt3(uint32_t s){ int n; // loop counter
uint32_t t;     // t*t will become s
  GPIOB->DOUTSET31_0 = 1<<22;
  t = s/16+1;        // initial guess
  for(n = 16; n; --n){ // will finish
    GPIOB->DOUTTGL31_0 = 1<<26;
    t = ((t*t+s)/t)/2;
  }
  GPIOB->DOUTCLR31_0 = 1<<22;
  return t;
}
int main13(void){ uint32_t z;
uint32_t elapsed,start,end;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  SysTick->LOAD = 0xFFFFFF;
  SysTick->VAL = 0;
  SysTick->CTRL = 5;
  start = SysTick->VAL;
  sqrt3(10000);
  end = SysTick->VAL;
  elapsed = (start-end)&0xFFFFFF;
  while(1){ }
}
/*
volatile uint32_t Counts,Flag,Data;
#include "../inc/ADC.h"
void TIMG0_IRQHandler(void){           // interrupts at 1 kHz
  if((TIMG0->CPU_INT.IIDX) == 1){ // this will acknowledge
GPIOB->DOUTSET31_0 = 1<<27;        // PB27=1
    Data = ADC0_In();       // real time sampling
    Flag = 1;               // means Data has information
GPIOB->DOUTCLR31_0 = 1<<27;        // PB27=0
  }
}
void SysTick_IntArm(uint32_t period, uint32_t priority){
  SysTick->CTRL  = 0x00;      // disable during initialization
  SysTick->LOAD  = period-1;  // set reload register
  //The ARM Cortex-M0+ only implements the most significant 2 bits of each 8-bit priority field (giving the 4 priority levels).
  SCB->SHP[1]    = (SCB->SHP[1]&(~0xC0000000))|priority<<30;    // set priority (bits 31,30)
  SysTick->VAL   = 0;         // clear count, cause reload
  SysTick->CTRL  = 0x07;      // Enable SysTick IRQ and SysTick Timer
}
void SysTick_Handler(void){
GPIOB->DOUTSET31_0 = 1<<26;        // PB26=1
  Counts = Counts + 1;
GPIOB->DOUTCLR31_0 = 1<<26;        // PB26=0
}
int main14(void){
  __disable_irq();           // I=1, disable interrupts
  Clock_Init40MHz();
  LaunchPad_Init();
  ADC0_Init(7,ADCVREF_VDDA); // PA22
  SysTick_IntArm(900,2);    // 40M/900 = 44.4kHz
  TimerG0_IntArm(1000,20,3); // 1ms, priority 3
  Flag = 0;                  // means Data is empty
  __enable_irq();            // I=0, enable interrupts
  while(1){
    if(Flag){ // process Data
GPIOB->DOUTTGL31_0 = 1<<22;  // toggle PB22
      Flag = 0;              // means Data is empty
    }
  }
}
*/




