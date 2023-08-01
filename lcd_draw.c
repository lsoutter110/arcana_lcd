#include "lcd_hl.h"
#include "lcd_util.h"

// Drawing functions

void lcd_clear() {
	lcd_mem_write_begin(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);

	for(uint8_t x=0; x<LCD_WIDTH; x++)
		for(uint16_t y=0; y<LCD_HEIGHT; y++)
			lcd_mem_write_16(LCD_BG_COL);
	
	lcd_mem_write_end();
}

void lcd_draw_hline(const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t col) {
	lcd_mem_write_begin(x1, y1, x2, y1);

	for(uint8_t x=x1; x<=x2; x++)
		lcd_mem_write_16(col);

	lcd_mem_write_end();
}

void lcd_draw_vline(const uint8_t x1, const uint16_t y1, const uint16_t y2, const uint16_t col) {
	lcd_mem_write_begin(x1, y1, x1, y2);

	for(uint16_t y=y1; y<=y2; y++)
		lcd_mem_write_16(col);

	lcd_mem_write_end();
}

void lcd_draw_rect_fill(const uint8_t x1, const uint16_t y1, const uint8_t x2, const uint16_t y2, const uint16_t col) {
	lcd_mem_write_begin(x1, y1, x2, y2);

	for(uint8_t x=x1; x<=x2; x++)
		for(uint16_t y=y1; y<=y2; y++)
			lcd_mem_write_16(col);

	lcd_mem_write_end();
}

void lcd_draw_line(const int16_t x1, const int16_t y1, const int16_t x2, const int16_t y2, const uint16_t col) {
    const int16_t dx = abs(x1-x2);
    const int16_t dy = absn(y1-y2);
    const int16_t xi = x1<x2 ? 1 : -1;
    const int16_t yi = y1<y2 ? 1 : -1;
    int16_t err = dx + dy;
    int16_t x = x1, y = y1;

    lcd_mem_write_begin(x, y, x, y);
    while(1) {
        lcd_mem_write_16(col);

        if(x == x2 && y == y2) break;
        
        if(2*err >= dy) {
            if(x == x2) break;
            err += dy;
            x += xi;
            lcd_set_col_adr(x, x);
            lcd_cmd_w(LCD_MEMORY_WRITE);
        }

        if(2*err <= dx) {
            if(y == y2) break;
            err += dx;
            y += yi;
            lcd_set_page_adr(y, y);
            lcd_cmd_w(LCD_MEMORY_WRITE);
        }
    }
    lcd_mem_write_end();
}