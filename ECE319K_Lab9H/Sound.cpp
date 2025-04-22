// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data
#include "Sound.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"
#include "sounds/sounds.h"
#include <stdint.h>
#include <ti/devices/msp/msp.h>


static const uint8_t *soundPtr = NULL;
static uint32_t soundCount = 0;
static uint32_t soundIndex = 0;
static uint32_t period = 0;

void SysTick_IntArm(uint32_t period, uint32_t priority) {
  // write this
  SysTick->CTRL = 0;          // disable SysTick during setup
  SysTick->LOAD = period - 1; // reload value
  SysTick->VAL = 0;           // any write to current clears it
  SCB->SHP[priority] =
      SCB->SHP[priority] & (~0xC0000000) | 0x40000000; // set priority = 2
  SysTick->CTRL = 0x0007; // enable SysTick with core clock and interrupts
}

// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void) {
  // write this
  DAC5_Init();
  period = 7256; // 80M/7256 = 11.025kHz
}


extern "C" void SysTick_Handler(void);
void SysTick_Handler(void) { // called at 11 kHz
                             // output one value to DAC if a sound is active
  if (soundPtr != NULL && soundIndex < soundCount) {
    DAC5_Out(soundPtr[soundIndex]); // Output sample to DAC
    soundIndex++;                   // Increment index
  } else {
    // Sound finished, stop Systick
    SysTick->CTRL = 0; // Disable systick
    soundPtr = NULL;   // Indicate sound is done
  }
}

//******* Sound_Start ************
// This function does not output to the DAC.
// Rather, it sets a pointer and counter, and then enables the SysTick
// interrupt. It starts the sound, and the SysTick ISR does the output feel free
// to change the parameters Sound should play once and stop Input: pt is a
// pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count) {
  // write this
  __disable_irq();

  // Set sound parameters
  soundPtr = pt;
  soundCount = count;
  soundIndex = 0;

  // Arm SysTick for sound playback
  SysTick_IntArm(period, 0);

  // Enable Interrupts
  __enable_irq();
}

// Sound emitted when user car runs into another vehicle/object
void Sound_Crash(void) {
  // write this
  Sound_Start(crashsound, 6438);
}

// Sound emitted when user car picks up a powerup
void Sound_Powerup(void) {
  // write this
  Sound_Start(powerup, 38493);
}

// Sound emitted when user uses a powerup
void Sound_usePowerup(void) {
  // Write this
  Sound_Start(usepowerup, 13212);
}

// Sound emmitted when user reaches edges of the track
void Sound_OffSides(void) {
  // Write this
  Sound_Start(offsides, 33697);
}
