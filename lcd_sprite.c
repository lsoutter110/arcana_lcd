#include "lcd_hl.h"
#include "lcd_util.h"

void lcd_draw_sprite(const Sprite sprite, const uint8_t x, const uint16_t y) {
	lcd_mem_write_begin(x, y, x + sprite.w - 1, y + sprite.h - 1);

	uint16_t *data_ptr = sprite.data;
	for(uint8_t x_r=0; x_r<sprite.w; x_r++)
		for(uint16_t y_r=0; y_r<sprite.h; y_r++)
			lcd_mem_write_16(*(data_ptr++));
	
	lcd_mem_write_end();
}

void lcd_draw_pgmsprite(const PGMSprite sprite, const uint8_t x, const uint16_t y) {
	lcd_mem_write_begin(x, y, x + sprite.w - 1, y + sprite.h - 1);

	PGM_P data_ptr = sprite.data;
	for(uint8_t x_r=0; x_r<sprite.w; x_r++)
		for(uint16_t y_r=0; y_r<sprite.h; y_r++) {
			lcd_mem_write_8(pgm_read_byte(data_ptr++));
			lcd_mem_write_8(pgm_read_byte(data_ptr++));
		}
	
	lcd_mem_write_end();
}

void lcd_draw_pgmsprite_scale(const PGMSprite sprite, const uint8_t x_pos, const uint16_t y_pos, const uint8_t x_scale, const uint8_t y_scale) {
	lcd_mem_write_begin(x_pos, y_pos, x_pos+(sprite.w*x_scale)-1, y_pos+(sprite.h*y_scale)-1);

	PGM_P row_ptr = sprite.data;
	for(uint16_t y=y_pos; y<y_pos+sprite.h; y++) {
		for(uint8_t y_r=0; y_r<y_scale; y_r++) {
			PGM_P data_ptr = row_ptr;
			for(uint8_t x=x_pos; x<x_pos+sprite.w; x++) {
				uint8_t col_h = pgm_read_byte(data_ptr++);
				uint8_t col_l = pgm_read_byte(data_ptr++);
				for(uint8_t x_r=0; x_r<x_scale; x_r++) {
					lcd_mem_write_8(col_h);
					lcd_mem_write_8(col_l);
				}
			}
		}
		row_ptr += 2*sprite.w;
	}
	
	lcd_mem_write_end();
}