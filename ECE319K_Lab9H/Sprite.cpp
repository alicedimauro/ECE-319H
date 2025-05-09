#include "Sprite.h"
#include "../inc/ST7735.h"
#include "Motor.h"
#include <stdint.h>
#include "Sound.h"

uint32_t M = 1;
static uint32_t Random32(void) {
  M = 1664525 * M + 1013904223;
  return M;
}
static uint32_t Random(uint32_t n) { return (Random32() >> 16) % n; }

// Update the sprite's x and y coordinates
void sprite_update(sprite_t *sprite, int32_t new_x, int32_t new_y) {
  if (new_x < 20) {
    new_x = 20;
    Motor_On();
    Sound_OffSides();
  } else if (new_x > 130) {
    new_x = 130;
    Motor_On();
    Sound_OffSides();
  } else {
    Motor_Off();
  }
  sprite->x = new_x;
  sprite->y = new_y;
  sprite->needDraw = 1; // Set semaphore
}

void create_op(sprite_t *sprite) {

  uint32_t pos_x = 30 + Random(95); // Randomize op x placement

  sprite->x = pos_x;
  sprite->y = 25;       // Fixed y at top of screen
  sprite->needDraw = 1; // Set semaphore
}

void move_opponent(sprite_t *sprite) {
  sprite->y += sprite->vy;
  sprite->needDraw = 1;
}

// Draw the sprite on the screen
void sprite_draw(const sprite_t *sprite) {
  ST7735_DrawBitmap(sprite->x, sprite->y, sprite->image, sprite->w, sprite->h);
}

uint8_t checkCollision(const sprite_t *sprite1, const sprite_t *sprite2) {
  return !(sprite1->x + sprite1->w <= sprite2->x || // Sprite 1 is left of Sprite 2
           sprite2->x + sprite2->w <= sprite1->x || // Sprite 2 is left of Sprite 1
           sprite1->y + sprite1->h <= sprite2->y || // Sprite 1 is below Sprite 2
           sprite2->y + sprite2->h <= sprite1->y);  // Sprite 2 is below Sprite 1
}
