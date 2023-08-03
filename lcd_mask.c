#include "lcd_hl.h"
#include "lcd_util.h"

void lcd_draw_mask(const Mask mask, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg) {
    uint8_t bitmask = 0x80;
    uint8_t *data_ptr = mask.data;
    uint8_t buf = *(data_ptr++);
    lcd_mem_write_begin(x, y, x+mask.w-1, y+mask.h-1);
    for(uint16_t yr=y; yr<y+mask.h; yr++) {
        for(uint8_t xr=x; xr<x+mask.w; xr++) {
            if(bitmask & buf) {
                lcd_mem_write_16(col);
            } else {
                lcd_mem_write_16(bg);
            }
            bitmask >>= 1;
            if(!bitmask) {
                bitmask = 0x80;
                buf = *(data_ptr++);
            }
        }
    }
    lcd_mem_write_end();
}

void lcd_draw_pgmmask(const PGMMask mask, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg) {
    uint8_t bitmask = 0x80;
    PGM_P data_ptr = mask.data;
    uint8_t buf = pgm_read_byte(data_ptr++);
    lcd_mem_write_begin(x, y, x+mask.w-1, y+mask.h-1);
    for(uint16_t yr=y; yr<y+mask.h; yr++) {
        for(uint8_t xr=x; xr<x+mask.w; xr++) {
            if(bitmask & buf) {
                lcd_mem_write_16(col);
            } else {
                lcd_mem_write_16(bg);
            }
            bitmask >>= 1;
            if(!bitmask) {
                bitmask = 0x80;
                buf = pgm_read_byte(data_ptr++);
            }
        }
    }
    lcd_mem_write_end();
}
