# Arcana LCD
LCD library for the Micro-Arcana series, written in C.

- `lcd_hl.h` and `lcd_hl.a` provide a high-level interface for drawing and writing text to the display
- `lcd_util.h` provides the low-level macros and functions for driving the display
- `img_to_h.py` converts a set of input images to a header containing `PGMSprite`s

For most projects, the high-level interfaces will have sufficient functionality. If lower level functions are needed, `lcd_util.h` can be included alongside the high-level library.

## High-Level interface
### Drawing Functions

`void lcd_init()`

Initialises and clears the display.

`void lcd_clear()`

Clears the display.

`void lcd_draw_hline(x1, y1, x2, col)`

Draws a horizontal line from `(x1, y1)` to `(x2, y1)` of colour `col`.

`void lcd_draw_vline(x1, y1, y2, col)`

Draws a vertical line from `(x1, y1)` to `(x1, y2)` of colour `col`.

`void lcd_draw_rect_fill(x1, y1, x2, y2, col)`

Draws a filled rectangle from `(x1, y1)` to `(x2, y2)` of colour `col`.

`void lcd_draw_line(x1, y1, x2, y2, col)`

Draws a line from `(x1, y1)` to `(x2, y2)` using Bresenham's algorithm (slightly optimised for the ili934x display driver).

### Sprites

`struct Sprite {pointer, width, height}`

A sprite stores a width, height, and a pointer to the display data in RAM (stored in 16 bit colour). The display data should be `2*width*height` bytes long.

`struct PGMSprite {pointer, width, height}`

A sprite stores a width, height, and a pointer to the display data in **program memory** (stored in 16 bit colour). The display data should be `2*width*height` bytes long. A python script is provided to convert images into `PGMSprite`s.

`void lcd_draw_sprite(sprite, x, y)`

Draws a `Sprite` to the display at `(x, y)`.

`void lcd_draw_pgmsprite(sprite, x, y)`

Draws a `PGMSprite` to the display at `(x, y)`.

`void lcd_draw_sprite_scale(sprite, x, y, x_scale, y_scale)`

Draws a `Sprite` to the display at `(x, y)`, scaled by the provided scale factors.

`void lcd_draw_pgmsprite_scale(sprite, x, y, x_scale, y_scale)`

Draws a `PGMSprite` to the display at `(x, y)`, scaled by the provided scale factors.

`void lcd_draw_rgb_triangle(x, y, w)`

Draws a 'colour test triange' of width/height `w` at `(x, y)`. 
