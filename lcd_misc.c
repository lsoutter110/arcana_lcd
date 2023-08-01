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