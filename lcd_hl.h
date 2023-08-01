#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

typedef struct {
    uint16_t *data;
    uint8_t w;
    uint16_t h;
} Sprite;

typedef struct {
    PGM_P data;
    const uint8_t w;
    const uint16_t h;
} PGMSprite;

// initialisation
void lcd_init();

// drawing functions
void lcd_clear();
void lcd_draw_hline(const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t col);
void lcd_draw_vline(const uint8_t x1, const uint16_t y1, const uint16_t y2, const uint16_t col);
void lcd_draw_rect_fill(const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t y2, const uint16_t col);
void lcd_draw_sprite(const Sprite sprite, const uint8_t x, const uint16_t y);
void lcd_draw_pgmsprite(const PGMSprite sprite, const uint8_t x, const uint16_t y);
void lcd_draw_pgmsprite_scale(const PGMSprite sprite, const uint8_t x_pos, const uint16_t y_pos, const uint8_t x_scale, const uint8_t y_scale);
void lcd_draw_rgb_triangle(const uint16_t x, const uint16_t y, const uint16_t w);