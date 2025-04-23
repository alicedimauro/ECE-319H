#include <stdint.h>
#include "../inc/ST7735.h"
#include "Sprite.h"
#include "Motor.h"


// Update the sprite's x and y coordinates
void sprite_update(sprite_t* sprite, int32_t new_x, int32_t new_y) {
  if (new_x < 30) {
    new_x = 30;
    Motor_On();
  } else if (new_x > 130) {
    new_x = 130;
    Motor_On();
  } else {
    Motor_Off();
  }
  sprite->x = new_x;
  sprite->y = new_y;
  sprite->needDraw = 1; // Set semaphore

}

// Draw the sprite on the screen
void sprite_draw(const sprite_t* sprite) {
  ST7735_DrawBitmap(sprite->x, sprite->y, sprite->image, sprite->w, sprite->h);
}

  
  // void Move(){
  //   x += vx;
  //   y += vy;
  //   needDraw = 1;

  //   // Boundary checking
  //   if (x < 0) {
  //       x = 0;
  //       vx = -vx;
  //   } else if (x > 127-w) {
  //       x = 127-w;
  //       vx = -vx;
  //   }
  //   if (y < 0) {
  //       y = 0;
  //       vy = -vy;
  //   } else if (y > 159-h) {
  //       y = 159-h;
  //       vy = -vy;
  //   }
  // }

  // Draw
  // void Draw() {
  //   if (needDraw) {
  //     ST7735_DrawBitmap(x, y, black, w, h);
  //     ST7735_DrawBitmap(x, y, image, w, h);
  //     needDraw = 0;
  //   }
  // }
