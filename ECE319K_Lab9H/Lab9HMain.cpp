// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Alice Di Mauro and Ravi Shah
// Last Modified: 12/26/2024

#include "../inc/Clock.h"
#include "../inc/DAC5.h"
#include "../inc/LaunchPad.h"
#include "../inc/ST7735.h"
#include "../inc/SlidePot.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "I2C.h"
#include "LED.h"
#include "Motor.h"
#include "SmallFont.h"
#include "Sound.h"
#include "Sprite.h"
#include "Switch.h"
#include "UART.h"
#include "images/images.h"
#include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp/msp.h>

#define IMU_BASE_ADDRESS 0x68
#define ANGLE_MULTIPLIER 10 // need this to convert IMU vals to a real angle

#define LED0 (1 << 17)
#define LED1 (1 << 16)
#define LED2 (1 << 13)
#define LED3 (1 << 19)
#define LED4 (1 << 12)

#define POWERUP 1
#define ACCEL 2
#define RESET_IMU 4
#define BRAKE 8

#define MAX_VELOCITY 3
#define MIN_VELOCITY 1

int16_t angular_velocity = 0; // gyroscope data received from IMU (z-axis)
float computed_angle = 0;     // angle computed from gyro data
float dt = 0.001;             // default val

uint32_t now;
int32_t currentAngle = 0;
uint32_t scorecounter = 0;
uint8_t lives = 3;
uint8_t powerup_used = 0;
uint8_t powerup_counter = 0;
uint8_t hasPowerup = 0;

sprite_t usercar, othercar1, othercar2, bolt;

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void) { // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0); // run this line for 80MHz
}

SlidePot Sensor(1904, 3); // copy calibration from Lab 7

// turn on motor, turn on all 5 LEDs, wait, then turn off all 5
void Startup_Sequence(void) {
  Motor_On();
  LED_On(LED0);
  Clock_Delay1ms(500);
  LED_On(LED1);
  Clock_Delay1ms(500);
  LED_On(LED2);
  Clock_Delay1ms(500);
  LED_On(LED3);
  Clock_Delay1ms(500);
  LED_On(LED4);
  Clock_Delay1ms(2000);
  LED_Off(LED0);
  LED_Off(LED1);
  LED_Off(LED2);
  LED_Off(LED3);
  LED_Off(LED4);
  Motor_Off();
}

// sample the IMU and compute a relative angle from reset point (global)
void SampleIMU(void) {
  // startTime = SysTick->VAL;
  angular_velocity =
      IMU_Z_Axis(IMU_BASE_ADDRESS); // avg of 3 samples from IMU gyroscope
  // stopTime = SysTick->VAL;
  // dt = (((startTime - stopTime) & 0x0FFFFFF) - offset) *
  //      (1.25e-8); // in seconds
  computed_angle +=
      angular_velocity * dt * ANGLE_MULTIPLIER; // compute angle by integration
}
// reset current position to have an angle of 0
void ResetIMU(void) { computed_angle = 0; }

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void) {
  uint32_t pos, msg;

  scorecounter++; // increment score

  if ((TIMG12->CPU_INT.IIDX) == 1) { // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
                                // game engine goes here

    // 1) sample IMU
    SampleIMU(); // reads computed angle
    UART_OutString("computed angle:");
    UART_OutSDec((int32_t)computed_angle);
    UART_OutString("\n");

    // 2) read input switches
    now = Switch_In(); // read button state
    if (now == RESET_IMU) {
      ResetIMU(); // reset current angle as 0
    }
    // only one of these can be processed, with powerup > accel > brake
    if (now == POWERUP && hasPowerup == 1) {
      if (powerup_used == 0) {
        LED_On(LED0);
        LED_On(LED1);
        LED_On(LED2);
        LED_On(LED3);
        LED_On(LED4);

        othercar1.vy = MAX_VELOCITY * 2;
        othercar2.vy = MAX_VELOCITY * 2;
        bolt.vy = MAX_VELOCITY * 2;

        powerup_used = 1;
        powerup_counter++;
        hasPowerup = 0;

        // show some kind of boost - double current velocity
      }
    } else if (now == ACCEL) {
      // increment velocity - if velocity is max, stay at max
      if (othercar1.vy >= MAX_VELOCITY) {
        othercar1.vy = MAX_VELOCITY;
      } else {
        othercar1.vy++;
      }

      if (othercar2.vy >= MAX_VELOCITY) {
        othercar2.vy = MAX_VELOCITY;
      } else {
        othercar2.vy++;
      }

      if (bolt.vy >= MAX_VELOCITY) {
        bolt.vy = MAX_VELOCITY;
      } else {
        bolt.vy++;
      }
    } else if (now == BRAKE) {
      // decrement velocity - if velocity is min, keep at min
      if (othercar1.vy <= MIN_VELOCITY) {
        othercar1.vy = MIN_VELOCITY;
      } else {
        othercar1.vy -= 2;
      }

      if (othercar2.vy <= MIN_VELOCITY) {
        othercar2.vy = MIN_VELOCITY;
      } else {
        othercar2.vy -= 2;
      }

      if (bolt.vy <= MIN_VELOCITY) {
        bolt.vy = MIN_VELOCITY;
      } else {
        bolt.vy -= 2;
      }
    } else {

      if (scorecounter % 60 == 0) {
        // decrement velocity (less than brake) - if velocity is min, stay at
        // min
        if (othercar1.vy <= MIN_VELOCITY) {
          othercar1.vy = MIN_VELOCITY;
        } else {
          othercar1.vy--;
        }
        if (othercar2.vy <= MIN_VELOCITY) {
          othercar2.vy = MIN_VELOCITY;
        } else {
          othercar2.vy--;
        }

        if (bolt.vy <= MIN_VELOCITY) {
          bolt.vy = MIN_VELOCITY;
        } else {
          bolt.vy--;
        }
      }
    }
    // 3) move sprites
    currentAngle = (int32_t)(computed_angle);
    if (currentAngle != 0) {
      sprite_update(&usercar, usercar.x + computed_angle,
                    usercar.y); // update x coordinate, should increase y later
    }

    // check if powerup time limit
    if (powerup_used == 1) {
      if (powerup_counter % 33 == 0) {
        powerup_used = 0;
        powerup_counter = 0;
        othercar1.vy /= 2;
        othercar2.vy /= 2;
        LED_Off(LED0);
        LED_Off(LED1);
        LED_Off(LED2);
        LED_Off(LED3);
        LED_Off(LED4);
      } else {
        powerup_counter++;
      }
    }

    // Checking opponent car placements
    if (scorecounter % 60 == 0) {
      if (othercar1.y < 0 || othercar1.y > 127) {
        create_op(&othercar1);
      }
    } else if (scorecounter % 60 == 30) {
      if (othercar2.y < 0 || othercar2.y > 127) {
        create_op(&othercar2);
      }
    } else if (scorecounter % 200 == 0) {
      if (bolt.y < 0 || bolt.y > 127) {
        create_op(&bolt);
      }
    }

    // move opponent sprites position based on velocity
    move_opponent(&othercar1);
    move_opponent(&othercar2);
    move_opponent(&bolt);

    // 4) start sounds
    // 5) set semaphore

    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }

  if (checkCollision(&usercar, &othercar1)) {
    lives--;
  }

  if (checkCollision(&usercar, &othercar2)) {
    lives--;
  }

  if (checkCollision(&usercar, &bolt)) {
    hasPowerup = 1;
  }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void) {
  return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));
}

typedef enum { English, Spanish } Language_t;
Language_t myLanguage = English;
typedef enum { HELLO, GOODBYE, LANGUAGE } phrase_t;
const char Hello_English[] = "Hello";
const char Hello_Spanish[] = "\xADHola!";
// const char Hello_Portuguese[] = "Ol\xA0";
// const char Hello_French[] ="All\x83";
const char Goodbye_English[] = "Goodbye";
const char Goodbye_Spanish[] = "Adi\xA2s";
// const char Goodbye_Portuguese[] = "Tchau";
// const char Goodbye_French[] = "Au revoir";
const char Language_English[] = "English";
const char Language_Spanish[] = "Espa\xA4ol";
// const char Language_Portuguese[]="Portugu\x88s";
// const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][2] = {{Hello_English, Hello_Spanish},
                             {Goodbye_English, Goodbye_Spanish},
                             {Language_English, Language_Spanish}};
// use main1 to observe special characters
int main1(void) { // main1
  char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_SetRotation(1);
  ST7735_FillScreen(0x0000); // set screen to black
  for (int myPhrase = 0; myPhrase <= 2; myPhrase++) {
    for (int myL = 0; myL < 2; myL++) {
      ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
      ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000); // set screen to black
  l = 128;
  while (1) {
    Clock_Delay1ms(2000);
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 16; i++) {
        ST7735_SetCursor(7 * j + 0, i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7 * j + 4, i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void) { // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  // note: if you colors are weird, see different options for
  //  ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);

  // Space Invaders Sprites:
  // ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  // ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  // ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  // ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  // ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  // ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  // ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  // ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  // ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  // ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  ST7735_DrawBitmap(64, 80, redcar, 30,
                    70); // Place user car towards middle bottom

  for (uint32_t t = 500; t > 0; t = t - 5) {
    SmallFont_OutVertical(t, 104, 6); // top left
    Clock_Delay1ms(50);               // delay 50 msec
  }
  ST7735_FillScreen(0x0000); // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString((char *)"GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString((char *)"Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)"Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while (1) {
  }
}

// use main3 to test switches and LEDs
int main3(void) { // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  while (1) {
    // write code to test switches and LEDs
  }
}
// use main4 to test sound outputs
int main4(void) {
  uint32_t last = 0, now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init();          // initialize switches
  LED_Init();             // initialize LED
  Sound_Init();           // initialize sound
  TExaS_Init(ADC0, 6, 0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while (1) {
    now = Switch_In(); // one of your buttons
    if ((last == 0) && (now == 1)) {
      // Sound_Shoot(); // call one of your sounds
      Sound_Crash();
    }
    if ((last == 0) && (now == 2)) {
      // Sound_Killed(); // call one of your sounds
      Sound_Powerup();
    }
    if ((last == 0) && (now == 4)) {
      // Sound_Explosion(); // call one of your sounds
      Sound_usePowerup();
    }
    if ((last == 0) && (now == 8)) {
      // Sound_Fastinvader1(); // call one of your sounds
      Sound_OffSides();
    }
    last = now;
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void) { // final main

  uint32_t choice = 0; // initialize user language choice

  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  // note: if you colors are weird, see different options for
  // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0, 0, &TExaS_LaunchPadLogicPB27PB26); // PB27 and PB2
  Motor_Init();
  I2C_Init();
  IMU_Init(IMU_BASE_ADDRESS);

  Clock_Delay1ms(100);

  // Language selection
  ST7735_DrawBitmap(0, 159, mainmenu, 127, 160);
  ST7735_SetRotation(1); // Rotate horizontal
  ST7735_DrawBitmap(50, 50, redcar, 11, 25);

  // Initial formatting
  ST7735_SetTextColor(ST7735_BLUE);
  ST7735_SetCursor(2, 11);
  ST7735_OutString((char *)Phrases[2][0]); // prints English

  ST7735_SetTextColor(ST7735_BLUE);
  ST7735_SetCursor(12, 11);
  ST7735_OutString((char *)Phrases[2][1]); // prints Spanish

  // Sampling to check if user selected Engish or Spanish
  for (uint32_t i = 0; i < 300; i++) {
    uint32_t pos = Sensor.In(); // 0 to 4095

    if (pos < 2045) {
      choice = 0; // English selected
      // Highlight the word English

      ST7735_SetTextColor(ST7735_BLUE);
      ST7735_SetCursor(2, 11);
      ST7735_OutString((char *)Phrases[2][0]); // prints English

      ST7735_SetTextColor(
          ST7735_BLACK); // Don't highlight Spanish bc English is selected
      ST7735_SetCursor(12, 11);
      ST7735_OutString((char *)Phrases[2][1]); // prints Spanish

    } else {
      choice = 1; // Spanish selected
      // Highlight the word Spanish

      ST7735_SetTextColor(ST7735_BLUE);
      ST7735_SetCursor(12, 11);
      ST7735_OutString((char *)Phrases[2][1]); // prints Spanish

      ST7735_SetTextColor(ST7735_BLACK); // Don't highlight English because
                                         // Spanish is selected
      ST7735_SetCursor(2, 11);
      ST7735_OutString((char *)Phrases[2][0]); // prints English
    }
    // Delay to sample
    Clock_Delay1ms(20);
  }

  // Setup track + initialize user car
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetRotation(0); // Rotate vertical
  ST7735_DrawBitmap(0, 159, track, 128, 160);
  ST7735_SetRotation(1); // Rotate horizontal
  ST7735_DrawBitmap(70, 110, redcar, 11, 25);

  ST7735_SetRotation(0); // Rotate vertical

  Startup_Sequence(); // Race is about to begin

  // initialize interrupts on TimerG12 at 30 Hz

  TimerG12_IntArm(2666666, 2); // 30hz/12.5ns

  // initialize all data structures
  usercar.w = 11;
  usercar.h = 25;
  usercar.image = redcar;
  usercar.x = 70;
  usercar.y = 110;

  othercar1.w = 11;
  othercar1.h = 25;
  othercar1.image = orangecar;
  othercar1.y = -1; // Hardcode off screen

  othercar2.w = 11;
  othercar2.h = 25;
  othercar2.image = bluecar;
  othercar2.y = -1; // Hardcode off the screen

  bolt.w = 12;
  bolt.h = 20;
  bolt.image = powerup;
  bolt.y = -1; // Hardcode off the screen

  __enable_irq();

  while (1) {
    // wait for semaphore

    ST7735_SetRotation(0); // Rotate vertical
    ST7735_DrawBitmap(0, 159, track, 128, 160);
    ST7735_SetRotation(1);   // Rotate horizontal
    sprite_draw(&usercar);   // Drawcar based on updated position
    sprite_draw(&othercar1); // Drawcar based on updated position
    sprite_draw(&othercar2); // Drawcar based on updated position
    sprite_draw(&bolt);      // Drawcar based on updated position

    if (usercar.needDraw) {
      usercar.needDraw = 0;
    }
    // wait for semaphore
    // Opponent #1
    if (othercar1.needDraw) {
      othercar1.needDraw = 0;
    }

    // wait for semaphore
    // Opponent #2
    if (othercar2.needDraw) {
      othercar2.needDraw = 0;
    }

    // wait for semaphore
    // Opponent #2
    if (bolt.needDraw) {
      bolt.needDraw = 0;
    }

    ST7735_SetTextColor(
        ST7735_BLUE); // Don't highlight Spanish bc English is selected
    ST7735_SetCursor(0, 0);
    ST7735_OutString("Lives: ");
    ST7735_OutUDec(lives);

    // clear semaphore
    // update ST7735R
    // check for end game or level switch
  }
}

// main6 - i2c tester
int main6(void) {
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  I2C_Init(); // initialize I2C1
  LED_Init();
  UART_Init();

  // enable systick to track time differential
  uint32_t startTime, stopTime;
  SysTick->LOAD = 0xFFFFFF;   // max
  SysTick->VAL = 0;           // any write to current clears it
  SysTick->CTRL = 0x00000005; // enable SysTick with core clock
  startTime = SysTick->VAL;
  stopTime = SysTick->VAL;
  uint32_t offset = (startTime - stopTime) & 0x0FFFFFF; // in bus cycles

  uint8_t whoami_data = 0;
  whoami_data = IMU_WhoAmI(IMU_BASE_ADDRESS); // make sure this is 0x67

  IMU_Init(IMU_BASE_ADDRESS); // initialize IMU registers
  LED_Toggle(LED2);

  whoami_data = 0;
  whoami_data = IMU_WhoAmI(IMU_BASE_ADDRESS); // KEEP THIS - MUST DO IT TWICE!
  whoami_data = IMU_WhoAmI(IMU_BASE_ADDRESS); // make sure this is 0x67

  int16_t angular_velocity = 0; // gyroscope data received from IMU (z-axis)
  float computed_angle = 0;     // angle computed from gyro data
  float dt = 0.0017;            // default val

  while (1) {
    startTime = SysTick->VAL;
    angular_velocity =
        IMU_Z_Axis(IMU_BASE_ADDRESS); // avg of 3 samples from IMU gyroscope
    stopTime = SysTick->VAL;
    dt = (((startTime - stopTime) & 0x0FFFFFF) - offset) *
         (1.25e-8); // in seconds

    computed_angle += angular_velocity * dt *
                      ANGLE_MULTIPLIER; // compute angle by integration

    UART_OutString("computed angle: ");
    UART_OutSDec((int32_t)computed_angle);
    UART_OutString("\n");
    LED_Toggle(LED2);
    Clock_Delay1ms(33); // 30 Hz
  }
}

// main7 - LED tester
int main7(void) {
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LED_Init();

  while (1) {
    LED_On(LED0);
    LED_On(LED1);
    LED_On(LED2);
    LED_On(LED3);
    LED_On(LED4);

    Clock_Delay1ms(500); // wait 500ms

    LED_Off(LED0);
    LED_Off(LED1);
    LED_Off(LED2);
    LED_Off(LED3);
    LED_Off(LED4);

    Clock_Delay1ms(500); // wait 500ms
  }
}
