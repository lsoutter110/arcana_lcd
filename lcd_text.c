#include "lcd_hl.h"
#include "lcd_util.h"

void lcd_write_char(const PGMFont font, char c, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg) {
    lcd_mem_write_begin(x, y, x+font.w-1, y+font.h-1);
    if(c < 32) c = 127;
    PGM_P data_ptr = font.data + ((uint16_t)(c-32))*font.char_offset;
    uint8_t mask = 0x80;
    uint8_t buf = pgm_read_byte(data_ptr++);
    for(uint16_t yr=y; yr<y+font.h; yr++)
        for(uint8_t xr=x; xr<x+font.w; xr++) {
            if(mask & buf) {
                lcd_mem_write_16(col);
            } else {
                lcd_mem_write_16(bg);
            }
            
            mask >>= 1;
            if(mask == 0) {
                mask = 0x80;
                buf = pgm_read_byte(data_ptr++);
            }
        }
    lcd_mem_write_end();
}

void lcd_write_str(const PGMFont font, char *str, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg) {
    const uint8_t xi = font.w + font.x_space;
    const uint16_t yi = font.h + font.y_space;
    uint8_t xr = x;
    uint16_t yr = y;
    for(char c; c = *str; str++) {
        if(c == '\n') {
            xr = x;
            yr += yi;
            continue;
        }
        if(xr > LCD_WIDTH - xi) {
            xr = x;
            yr += yi;
        }
        lcd_write_char(font, c, xr, yr, col, bg);
        xr += xi;
    }
}