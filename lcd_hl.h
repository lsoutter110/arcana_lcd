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

typedef struct {
    uint8_t *data;
    uint8_t w;
    uint16_t h;
} Mask;

typedef struct {
    PGM_P data;
    const uint8_t w;
    const uint16_t h;
} PGMMask;

typedef struct {
    PGM_P data;
    const uint8_t char_offset;
    const uint8_t w;
    const uint8_t h;
    uint8_t x_space;
    uint8_t y_space;
} PGMFont;

// initialisation
void lcd_init();

// drawing functions
void lcd_clear();
void lcd_draw_hline(const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t col);
void lcd_draw_vline(const uint8_t x1, const uint16_t y1, const uint16_t y2, const uint16_t col);
void lcd_draw_rect_fill(const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t y2, const uint16_t col);
void lcd_draw_line(const int16_t x1, const int16_t y1, const int16_t x2, const int16_t y2, const uint16_t col);

// Sprite functions
void lcd_draw_sprite(const Sprite sprite, const uint8_t x, const uint16_t y);
void lcd_draw_pgmsprite(const PGMSprite sprite, const uint8_t x, const uint16_t y);
void lcd_draw_sprite_scale(const Sprite sprite, const uint8_t x, const uint16_t y, const uint8_t x_scale, const uint8_t y_scale);
void lcd_draw_pgmsprite_scale(const PGMSprite sprite, const uint8_t x, const uint16_t y, const uint8_t x_scale, const uint8_t y_scale);

// Mask functions
void lcd_draw_mask(const Mask mask, const uint8_t x, const uint16_t y, const uint16_t col1, const uint16_t col0);
void lcd_draw_pgmmask(const PGMMask mask, const uint8_t x, const uint16_t y, const uint16_t col1, const uint16_t col0);
void lcd_draw_mask_scale(const Mask mask, const uint8_t x, const uint16_t y, const uint16_t col1, const uint16_t col0, const uint8_t x_scale, const uint16_t y_scale);
void lcd_draw_pgmmask_scale(const PGMMask mask, const uint8_t x, const uint16_t y, const uint16_t col1, const uint16_t col0, const uint8_t x_scale, const uint16_t y_scale);

// Text functions
void lcd_write_char(const PGMFont font, const char c, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg);
uint8_t lcd_write_str(const PGMFont font, const char *str, const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t y2, const uint16_t col, const uint16_t bg);

// Misc
void lcd_draw_rgb_triangle(const uint16_t x, const uint16_t y, const uint16_t w);
static inline uint16_t rgb_to_u16(const uint8_t r, const uint8_t g, const uint8_t b) { return (r<<8)&0xF800 | (g<<3)&0x07E0 | (b>>3); }