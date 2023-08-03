#include "lcd_hl.h"
#include "lcd_util.h"

void lcd_draw_mask(const Mask mask, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg) {
    uint8_t *data_ptr = mask.data;
    uint8_t bitmask = 0x80;
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
    PGM_P data_ptr = mask.data;
    uint8_t bitmask = 0x80;
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

void lcd_draw_mask_scale(const Mask mask, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg, const uint8_t x_scale, const uint16_t y_scale) {
    uint8_t *data_ptr = mask.data;
    uint8_t bitmask = 0x80;
    uint8_t buf = *(data_ptr++);
    lcd_mem_write_begin(x, y, x+(mask.w*x_scale)-1, y+(mask.h*y_scale)-1);
    for(uint16_t yr=y; yr<y+mask.h; yr++) {
        uint8_t *row_ptr = data_ptr;
        uint8_t row_bitmask = bitmask;
        uint8_t row_buf = buf;
        for(uint16_t ys=0; ys<y_scale; ys++) {
            //rewind to start of row state
            data_ptr = row_ptr;
            bitmask = row_bitmask;
            buf = row_buf;
            for(uint8_t xr=x; xr<x+mask.w; xr++) {
                if(bitmask & buf) {
                    for(uint8_t xs=0; xs<x_scale; xs++)
                        lcd_mem_write_16(col);
                } else {
                    for(uint8_t xs=0; xs<x_scale; xs++)
                        lcd_mem_write_16(bg);
                }
                bitmask >>= 1;
                if(!bitmask) {
                    bitmask = 0x80;
                    buf = *(data_ptr++);
                }
            }
        }
    }
    lcd_mem_write_end();
}

void lcd_draw_pgmmask_scale(const PGMMask mask, const uint8_t x, const uint16_t y, const uint16_t col, const uint16_t bg, const uint8_t x_scale, const uint16_t y_scale) {
    PGM_P data_ptr = mask.data;
    uint8_t bitmask = 0x80;
    uint8_t buf = pgm_read_byte(data_ptr++);
    lcd_mem_write_begin(x, y, x+(mask.w*x_scale)-1, y+(mask.h*y_scale)-1);
    for(uint16_t yr=y; yr<y+mask.h; yr++) {
        PGM_P row_ptr = data_ptr;
        uint8_t row_bitmask = bitmask;
        uint8_t row_buf = buf;
        for(uint16_t ys=0; ys<y_scale; ys++) {
            //rewind to start of row state
            data_ptr = row_ptr;
            bitmask = row_bitmask;
            buf = row_buf;
            for(uint8_t xr=x; xr<x+mask.w; xr++) {
                if(bitmask & buf) {
                    for(uint8_t xs=0; xs<x_scale; xs++)
                        lcd_mem_write_16(col);
                } else {
                    for(uint8_t xs=0; xs<x_scale; xs++)
                        lcd_mem_write_16(bg);
                }
                bitmask >>= 1;
                if(!bitmask) {
                    bitmask = 0x80;
                    buf = pgm_read_byte(data_ptr++);
                }
            }
        }
    }
    lcd_mem_write_end();
}
