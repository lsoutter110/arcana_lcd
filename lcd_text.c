#include "lcd_hl.h"
#include "lcd_util.h"

void lcd_write_char(const PGMFont font, char c, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg) {
    if(c < 32) c = 127;
    PGM_P data_ptr = font.data + ((uint16_t)(c-32))*font.char_offset;
    uint8_t mask = 0x80;
    uint8_t buf = pgm_read_byte(data_ptr++);
    lcd_mem_write_begin(x, y, x+font.w-1, y+font.h-1);
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

uint8_t lcd_write_str(const PGMFont font, const char *str, const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t y2, const uint16_t col, const uint16_t bg) {
    const uint8_t xi = font.w + font.x_space;
    const uint16_t yi = font.h + font.y_space;
    uint8_t x = x1;
    uint16_t y = y1;
    for(char c; c = *str; str++) {
        if(y > y2 - yi)
            return 1;
        if(c == '\n') {
            x = x1;
            y += yi;
            continue;
        }
        if(x > x2 - xi) {
            x = x1;
            y += yi;
        }
        lcd_write_char(font, c, x, y, col, bg);
        x += xi;
    }
    return 0;
}