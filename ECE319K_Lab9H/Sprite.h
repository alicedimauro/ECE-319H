#ifndef SPRITE_H
#define SPRITE_H
#include <stdint.h>
#include "../inc/ST7735.h"

class Sprite {
private:
  int32_t x;      // x coordinate
  int32_t y;      // y coordinate
  int32_t vx,vy;  // pixels/30Hz 1/256
  const unsigned short *image; // ptr->image
  const unsigned short *black; // black background for erasing; same size as sprite
  int32_t w; // width
  int32_t h; // height
  uint32_t needDraw; // true if need to draw

public:
  // Constructor
  Sprite(int32_t x0, int32_t y0, int32_t vx0, int32_t vy0,
         const unsigned short *image0, const unsigned short *black0, int32_t w0, int32_t h0) {
    x = x0;
    y = y0;
    vx = vx0;
    vy = vy0;
    image = image0;
    black = black0;
    w = w0;
    h = h0;
    needDraw = 1; // draw initially
  }


  // Move
  void Move(){
    x += vx;
    y += vy;
    needDraw = 1;

    // Boundary checking
    if (x < 0) {
        x = 0;
        vx = -vx;
    } else if (x > 127-w) {
        x = 127-w;
        vx = -vx;
    }
    if (y < 0) {
        y = 0;
        vy = -vy;
    } else if (y > 159-h) {
        y = 159-h;
        vy = -vy;
    }
  }

  // Draw
  void Draw() {
    if (needDraw) {
      ST7735_DrawBitmap(x, y, black, w, h);
      ST7735_DrawBitmap(x, y, image, w, h);
      needDraw = 0;
    }
  }

  // Getters returns values of private members
  int32_t GetX() const { return x; }
  int32_t GetY() const { return y; }
  int32_t GetW() const { return w; }
  int32_t GetH() const { return h; }

  // Setters to set values of private values
  void SetX(int32_t newX) { x = newX; needDraw = 1; }
  void SetY(int32_t newY) { y = newY; needDraw = 1; }
  void SetVX(int32_t newVX) { vx = newVX; }
  void SetVY(int32_t newVY) { vy = newVY; }
};
#endif
