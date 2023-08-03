# Arcana LCD
LCD library for the Micro-Arcana series, written in C.

- `lcd_hl.h` and `lcd_hl.a` provide a high-level interface for drawing and writing text to the display
- `lcd_util.h` provides the low-level macros and functions for driving the display
- `img_to_h.py` converts a set of input images to a header containing `PGMSprite`s

For most projects, the high-level interfaces will have sufficient functionality. If lower level functions are required, `lcd_util.h` can be included alongside the high-level library. The low level interface *will* need (at least a basic) understanding of how the ili934x display driver is communicated with to be used effectively.

To use the library in a project, clone the repository and run `make` to generate `lcd_hl.a`. `lcd_hl.h` (and optionally `lcd_util.h`) should be included in the project, and `lcd_hl.a` should be linked when compiling (it is reccomended to copy the required files to the project directory).

## High-Level interface
### Drawing Functions

Basic drawing functions.

`void lcd_init()` - Initialises and clears the display.

`void lcd_clear()` - Clears the display.

`void lcd_draw_hline(x1, y1, x2, col)` - Draws a horizontal line from `(x1, y1)` to `(x2, y1)` of colour `col`.

`void lcd_draw_vline(x1, y1, y2, col)` - Draws a vertical line from `(x1, y1)` to `(x1, y2)` of colour `col`.

`void lcd_draw_rect_fill(x1, y1, x2, y2, col)` - Draws a filled rectangle from `(x1, y1)` to `(x2, y2)` of colour `col`.

`void lcd_draw_line(x1, y1, x2, y2, col)` - Draws a line from `(x1, y1)` to `(x2, y2)` using Bresenham's algorithm (slightly optimised for the ili934x display driver).

`void lcd_draw_rgb_triangle(x, y, w)` - Draws a 'colour test triange' of width/height `w` at `(x, y)`. 

### Sprites

Sprites are a nice way of wrapping up a rectangle of memory to be directly copied to the display.

`struct Sprite {pointer, width, height}` - Stores a width, height, and a pointer to the display data in RAM (stored in 16 bit colour). The display data should be `2*width*height` bytes long.

`struct PGMSprite {pointer, width, height}` - Stores a width, height, and a pointer to the display data in **program memory** (stored in 16 bit colour). The display data should be `2*width*height` bytes long. A python script `img_to_h.py` is provided to convert images into `PGMSprite`s.

`void lcd_draw_sprite(sprite, x, y)` - Draws a `Sprite` to the display at `(x, y)`.

`void lcd_draw_pgmsprite(sprite, x, y)` - Draws a `PGMSprite` to the display at `(x, y)`.

`void lcd_draw_sprite_scale(sprite, x, y, x_scale, y_scale)` - Draws a `Sprite` to the display at `(x, y)`, scaled by the provided scale factors.

`void lcd_draw_pgmsprite_scale(sprite, x, y, x_scale, y_scale)` - Draws a `PGMSprite` to the display at `(x, y)`, scaled by the provided scale factors.

### Masks

Masks are essentially 2 colour sprites that are much more memory efficient (1 bit per pixel).

`struct Mask {pointer, width, height}` - Stores a width, height, and a pointer to the display data in RAM (stored in 1 bit colour). The display data should be `width*height` **bits** long.

`struct PGMMask {pointer, width, height}` - Stores a width, height, and a pointer to the display data in **program memory** (stored in 1 bit colour). The display data should be `width*height` **bits** long. A python script `mask_to_h.py` is provided to convert images into `PGMMask`s.

`void lcd_draw_mask(mask, x, y, col1, col0)` - Draws a `Mask` to the display at `(x, y)`.

`void lcd_draw_pgmmask(mask, x, y, col1, col0)`- Draws a `PGMMask` to the display at `(x, y)`.

`void lcd_draw_mask_scale(mask, x, y, col1, col0, x_scale, y_scale)` - Draws a `Mask` to the display at `(x, y)`, scaled by the provided scale factors.

`void lcd_draw_pgmmask_scale(mask, x, y, col1, col0, x_scale, y_scale)` - Draws a `PGMMask` to the display at `(x, y)`, scaled by the provided scale factors.

In the above functions, `col1` and `col0` are the colours to draw in place of a `1` and `0` in the mask data.

### Text

The text portion of the library provides functionality for writing characters and strings to the display. A default font `default5x7` is provided for use with the text functions, and the header must be included into files that use it.

`struct PGMFont {data, char_offset, w, h, x_space, y_space}` - Stores a font and the associated parameters, to be passed into text drawing functions. Stores representations of the ASCII characters 32 to 127, where 127 is the error character (and will be written in place of any character outside of the 32 to 126 range). `x_space` and `y_space` refer to the blank space left between characters. A python script `font_to_h.py` is provided to convert images into `PGMFont`s.

`void lcd_write_char(font, c, x, y, col, bg)` - Writes a character to the display at `(x, y)`.

`uint8_t lcd_write_str(font, str, x1, y1, x2, y2, col, bg)` - Writes a string to the display beginning at `(x1, y1)`. Begins a new line if text reaches `x2` or `\n` enountered, and exits if `y2` encountered. Returns `0` if successful, `1` if unsuccessful in writing the entire string.

## Helper Scripts

### img_to_h

Converts a list of `n` by `m` images into a header file containing sprites. The sprites will be named `sprite_name` for an image of filename `sprite_name.xyz`. The python script expects the arguments `output` and `images`, where `output` is the name of the output header file, and `images` are a list of one or more images. For example:

`python img_to_h.py sprites.h sprite1.png sprite2.png` would produce a header file `sprites.h` containing definitions for `PGMSprite sprite1` and `PGMSprite sprite2`.

### mask_to_h

Exactly the same as `img_to_h.py`, except it produces masks. Converts black (#000000) pixels to `0`s and anything else to `1`s. Example use:

`python img_to_h.py masks.h mask1.png mask2.png` would produce a header file `masks.h` containing definitions for `PGMMask mask1` and `PGMMask mask2`.

### font_to_h

Converts a **single** `12*w` by `8*h` image file into a header file containing a font (where `w` and `h` are the dimensions of a character, in pixels). The sprites will be named `font_name` for an image of filename `font_name.xyz`. The script expects arguments `output` `image` `x_space` `y_space`, where `output` is the name of the output header file, `image` is the input image file, and `x_space` and `y_space` are fields of the produced `PGMFont`. Also provided in the `helper_scripts` folder is an example font `default5x7`. For example, to generate `default5x7.h`:

`python font_to_h.py default5x7.h default5x7.png 1 1`