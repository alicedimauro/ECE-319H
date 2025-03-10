/* ToggleLED
 * Jonathan Valvano
 * Sept 3, 2022
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 */

/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#define ONESEC 32000000
// 2. Global Declarations section
uint32_t Time; // elapsed time in seconds
// 3. Functions Section
int main(void){ // Program 3.3.1
  LaunchPad_Init();
  Time = 0;
  while(1){
    Clock_Delay(ONESEC);
    GPIOA->DOUTTGL31_0 =  RED1;
    Time = Time+1;
  }
}
 // PB22 is BLUE LED
 // PB26 is RED LED
 // PB27 is GREEN LED
// runs at 32 MHz
int main1(void){
  LaunchPad_Init();
  GPIOB->DOUT31_0 = BLUE | GREEN; // set pins 1 3 (clear bit 2)
  while(1){    /* toggle LEDs 1,2,3. */
    Clock_Delay(ONESEC);
    GPIOB->DOUT31_0 ^= (BLUE | RED | GREEN);
  }
}


int main3(void){ // Program 4.2.1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  SysTick_Init();
  while(1){
    SysTick_Wait10ms(100);
    GPIOB->DOUTTGL31_0 =  RED ;
  }
}




