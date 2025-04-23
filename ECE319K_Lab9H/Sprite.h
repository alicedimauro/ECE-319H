#ifndef SPRITE_H
#define SPRITE_H
#include <stdint.h>
#include "../inc/ST7735.h"


typedef struct {
    int32_t x, y;             // Position (pixels)
    int32_t vx, vy;           // Velocity (pixels/frame)
    uint16_t const *image;    // Pointer to sprite image
    uint32_t w, h;            // Width and height of the sprite
    uint8_t needDraw = 0;
} sprite_t;


void sprite_update(sprite_t* sprite, int32_t new_x, int32_t new_y);
// Function to update sprite values

void sprite_draw(const sprite_t* sprite);
// Function to update screen

void create_op(sprite_t* sprite);
// Function to create opponent sprite

void move_opponent(sprite_t* sprite);
// Function to move opponent sprite based on velocity

uint8_t checkCollision(const sprite_t* sprite1, const sprite_t* sprite2);
// Returns 1 if collision, 0 if not

#endif
