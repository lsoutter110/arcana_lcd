#include "lcd_hl.h"
#include "lcd_util.h"

// Drawing functions

void lcd_clear() {
	lcd_mem_write_begin(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);

	for(uint8_t x=0; x<LCD_WIDTH; x++)
		for(uint16_t y=0; y<LCD_HEIGHT; y++)
			lcd_mem_write_16(0x0000);
	
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