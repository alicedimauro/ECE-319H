/* TrafficLightmain.c
 * This is a C language project that implements this Moore FSM.
 * Jonathan Valvano
 * May 17, 2023
 */


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"

/* Output: PB5 is red LED for North road
    Output: PB4 is yellow LED for North road
    Output: PB3 is green for North road
    Output: PB2 is red LED for East road
    Output: PB1 is yellow LED for East road
    Output: PB0 is green for East road
    Input: PB7 is positive logic switch, North car sensor
    Input: PB6 is positive logic switch, East car sensor
    */
void Traffic_Init(void);
// Linked data structure
/*
struct State {
  uint32_t Out;
  uint32_t Time;
  const struct State *Next[4];};
typedef const struct State State_t;
#define goN   &FSM[0]
#define waitN &FSM[1]
#define goE   &FSM[2]
#define waitE &FSM[3]
State_t FSM[4]={
 {0x21,300,{goN,waitN,goN,waitN}},
 {0x22, 50,{goE,goE,goE,goE}},
 {0x0C,300,{goE,goE,waitE,waitE}},
 {0x14, 50,{goN,goN,goN,goN}}};
State_t *Pt;  // state pointer
int main(void){ uint32_t Input;
  Clock_Init80MHz(0);
  SysTick_Init();
  Traffic_Init();
  Pt = goN;
  while(1){
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x3F))|Pt->Out;
    SysTick_Wait10ms(Pt->Time);
    Input = (GPIOB->DIN31_0&0xC0)>>6;   // read sensors
    Pt = Pt->Next[Input];
  }
}
*/
/*
// Linked data structure using indexing
struct State{
  uint32_t Out;
  uint32_t Time;
  uint32_t Next[4];};
typedef const struct State State_t;
#define goN   0
#define waitN 1
#define goE   2
#define waitE 3
State_t FSM[4]={
 {0x21,3000,{goN,waitN,goN,waitN}},
 {0x22, 500,{goE,goE,goE,goE}},
 {0x0C,3000,{goE,goE,waitE,waitE}},
 {0x14, 500,{goN,goN,goN,goN}}};
uint32_t S;  // index to the current state
int main(void){ uint32_t Input;
  Clock_Init80MHz(0);
  SysTick_Init();
  Traffic_Init();
  S = goN;
  while(1){
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x3F)) | FSM[S].Out;
    SysTick_Wait10ms(FSM[S].Time/10);
    Input = (GPIOB->DIN31_0&0xC0)>>6;
    S = FSM[S].Next[Input];
  }
}
*/
/*
struct State{
  uint32_t Out;
  uint32_t Wait; // 10ms
  const struct State *Next[2];
};
typedef const struct State St_t;
#define H10  &FSM[0]
#define L10  &FSM[1]
#define C10  &FSM[2]
#define H30  &FSM[3]
#define L30  &FSM[4]
#define C30  &FSM[5]
#define H50  &FSM[6]
#define L50  &FSM[7]
#define C50  &FSM[8]
#define H70  &FSM[9]
#define L70  &FSM[10]
#define C70  &FSM[11]
#define H90  &FSM[12]
#define L90  &FSM[13]
#define C90  &FSM[14]
St_t FSM[15]={
    { 2, 5,L10,C10}, // H10
    { 0,45,H10,C10}, // L10
    { 0, 5,L30,C10}, // C10
    { 2,15,L30,C30}, // H30
    { 0,35,H30,C30}, // L30
    { 0, 5,L50,C30}, // C30
    { 2,25,L50,C50}, // H50
    { 0,25,H50,C50}, // L50
    { 0, 5,L70,C50}, // C50
    { 2,35,L70,C70}, // H70
    { 0,15,H70,C70}, // L70
    { 0, 5,L90,C70}, // C70
    { 2,45,L90,C90}, // H90
    { 0, 5,H90,C90}, // L90
    { 0, 5,L10,C90}  // C90
};
St_t *Pt;  // Current State
void Init(void);
int main(void){uint32_t in;
  Clock_Init80MHz(0);
  SysTick_Init();
  Init(); // PB1 out,PB0 in
  Pt = H30;     // initial state
  while(1){
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x02))|Pt->Out;
    SysTick_Wait10ms(Pt->Wait);
    in = GPIOB->DIN31_0&0x01;
    Pt = Pt->Next[in]; // next
  }
}
*/
// Stepper motor FSM

struct State{
  uint32_t Out;
  uint32_t Wait; // 12.5ns
  const struct State *Next[2];
};
typedef const struct State St_t;
#define S5  &FSM[0]
#define S6  &FSM[1]
#define S10 &FSM[2]
#define S9  &FSM[3]
St_t FSM[4]={
  { 5,1666667,S5,S6},
  { 6,1666667,S6,S10},
  {10,1666667,S10,S9},
  { 9,1666667,S9,S5}
};
St_t *Pt;  // Current State
void Stepper_Init();
int main(void){uint32_t in;
  Clock_Init80MHz(0);
  SysTick_Init();
  Stepper_Init(); // PB4 in, PB3-0 output
  Pt = S5;        // initial state
  while(1){
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x0F))|Pt->Out;
    SysTick_Wait(Pt->Wait);
    in = (GPIOB->DIN31_0&0x10)>>4;
    Pt = Pt->Next[in]; // next
  }
}

// Initialize LED and switch
void Init(void){
// Reset GPIO peripheral
// RSTCLR
//   bits 31-24 unlock key 0xB1
//   bit 1 is Clear reset sticky bit
//   bit 0 is reset gpio port
  GPIOB->GPRCM.RSTCTL = (uint32_t)0xB1000003;

// Enable power to GPIO peripheral
// PWREN
//   bits 31-24 unlock key 0x26
//   bit 0 is Enable Power
  GPIOB->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(4);  // short delay

// PINCM
//   bit 25 is HiZ
//   bit 20 is drive strength
//   bit 18 is input enable control
//   bit 17 is pull up control
//   bit 16 is pull down control
//   bit 7 is PC peripheral connected, enable transparent data flow
//   bit 0 selects GPIO function

  IOMUX->SECCFG.PINCM[PB0INDEX] = (uint32_t) 0x00040081; // GPIO input
  IOMUX->SECCFG.PINCM[PB1INDEX] = (uint32_t) 0x00000081; // GPIO output
  GPIOB->DOE31_0 = 0x02; // enable output PB1
}
void Traffic_Init(void){
  GPIOB->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  GPIOB->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(2); // time for gpio to power up
  IOMUX->SECCFG.PINCM[PB7INDEX] = (uint32_t) 0x00050081; // input, pull down
  IOMUX->SECCFG.PINCM[PB6INDEX] = (uint32_t) 0x00050081; // input, pull down
  IOMUX->SECCFG.PINCM[PB5INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB4INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB3INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB2INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB1INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB0INDEX] = (uint32_t) 0x00100081;
  GPIOB->DOE31_0 |= 0x3F;  // enable outputs PB5-0
}

void Stepper_Init(void){
  GPIOB->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  GPIOB->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(2); // time for gpio to power up
  IOMUX->SECCFG.PINCM[PB4INDEX] = (uint32_t) 0x00050081; // input, pull down
  IOMUX->SECCFG.PINCM[PB3INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB2INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB1INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB0INDEX] = (uint32_t) 0x00100081;
  GPIOB->DOE31_0 |= 0x0F;  // enable outputs PB3-0
}


