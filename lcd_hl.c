#include "lcd_hl.h"
#include "lcd_util.h"

void lcd_init() {
	//disable jtag
	MCUCR |= _BV(JTD);
	MCUCR |= _BV(JTD);

	CTRL_DDR = 0x7F;
	DATA_DDR = 0xFF;

	//perform reset
	lcd_ctrl_lo(RESET);
	_delay_ms(50);
	lcd_ctrl_hi(RESET);
	_delay_ms(50);

	//set default pin values
	lcd_ctrl_hi(WR);
	lcd_ctrl_hi(RD);
	lcd_ctrl_hi(RS);
	lcd_ctrl_hi(VSYNC);
	lcd_ctrl_hi(FMARK);
	lcd_ctrl_lo(CS);

	lcd_cmd_w(LCD_DISPLAY_OFF);
	//exit sleep
	lcd_cmd_w(LCD_SLEEP_OUT);
	_delay_ms(5); //wait for sleep out to finish

	// ----- setup preferences -----

	//set orientation
	lcd_cmd_w(LCD_MEMORY_ACCESS_CONTROL);
	lcd_data_w(0x48);

	//set color depth to 16 bit {REG 3A : DBI[2:0] = 0b101}
	lcd_cmd_w(LCD_PIXEL_FORMAT_SET);
	lcd_data_w(0x55);

	//clear display
	lcd_clear();

	//turn on display after reset
	lcd_cmd_w(LCD_DISPLAY_ON);
	_delay_ms(20);
	
	//enable backlight
	lcd_ctrl_hi(BLC);
}

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

void lcd_draw_rgb_triangle(const uint16_t x, const uint16_t y, const uint16_t w) {
    //test triangle
    const uint16_t rb_max = 0x1F, r_off = 11;
    const uint16_t g_max = 0x3F, g_off = 5;
    const uint16_t               b_off = 0;

    lcd_mem_write_begin(x, y, x+w-1, y+w-1);
    for(uint16_t yr=0; yr<w; yr++) {
		int16_t g = g_max - g_max*yr/w;
		int16_t w_yr_2 = (w-yr)/2;
        for(uint16_t xr=0; xr<w; xr++) {
            if(2*xr + yr < w || yr + w < 2*xr) {
                lcd_mem_write_16(0);
                continue;
            }
            const int16_t r = rb_max - rb_max*(xr + w_yr_2)/w;
            const int16_t b = rb_max*(xr - w_yr_2)/w;
			const uint16_t col = r<<r_off | g<<g_off | b<<b_off;
            lcd_mem_write_16(col);
        }
	}
    lcd_mem_write_end();
}