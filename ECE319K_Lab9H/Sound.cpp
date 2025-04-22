// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"



void SysTick_IntArm(uint32_t period, uint32_t priority){
  // write this
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
// write this
 
}
extern "C" void SysTick_Handler(void);
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    // output one value to DAC if a sound is active

}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
// write this
  
}

// Sound emitted when user car runs into another vehicle/object 
void Sound_Crash(void){
// write this
Sound_Start(crashsound, 6438);

}

// Sound emitted when user car picks up a powerup
void Sound_Powerup(void){
// write this
Sound_Start(powerup, 38493);

}

// Sound emitted when user uses a powerup
void Sound_usePowerup(void){

}

// Sound emmitted when user reaches edges of the track
void Sound_OffSides(void){
// Write this
Sound_Start(offsides, 33697);

}

