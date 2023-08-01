/*  
 * All code in this file was written by Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 */
 
#include <avr/io.h>
#include <util/delay.h>

// #ifndef PORTA
// #define PORTA *(uint8_t*)0
// #define PORTC *(uint8_t*)0
// #define DDRA *(uint8_t*)0
// #define DDRC *(uint8_t*)0
// #endif

//Port definitions
#ifndef USE_PORT_BD

#define CTRL_PORT	PORTA
#define CTRL_DDR	DDRA
#define CTRL_PIN	PINA
#define DATA_PORT	PORTC
#define DATA_DDR	DDRC
#define DATA_PIN	PINC

#else //USE_PORT_BD

#define CTRL_PORT	PORTB
#define CTRL_DDR	DDRB
#define CTRL_PIN	PINB
#define DATA_PORT	PORTD
#define DATA_DDR	DDRD
#define DATA_PIN	PIND

#endif //USE_PORT_BD

//Control port pins
enum {
	CS		= 0,
	BLC		= 1,
	RESET	= 2,
	WR		= 3,
	RS		= 4,
	RD		= 5,
	VSYNC	= 6,
	FMARK	= 7,
};

//Display control instructions
#define lcd_ctrl_lo(pin)	CTRL_PORT &= ~_BV(pin)
#define lcd_ctrl_hi(pin)	CTRL_PORT |= _BV(pin)
// #define lcd_data_write(x)	DATA_PORT = x

// Basic Commands
enum {
	LCD_NO_OPERATION				= 0x00,
	LCD_SOFTWARE_RESET				= 0x01,
	LCD_READ_DISPLAY_ID				= 0x04,
	LCD_READ_DISPLAY_STATUS			= 0x09,
	LCD_READ_DISPLAY_POWER_MODE		= 0x0A,
	LCD_READ_DISPLAY_MADCTL			= 0x0B,
	LCD_READ_DISPLAY_PIXEL_FORMAT	= 0x0C,
	LCD_READ_DISPLAY_IMAGE_FORMAT	= 0x0D,
	LCD_READ_DISPLAY_SIGNAL_MODE	= 0x0E,
	LCD_RAED_DISPLAY_SELF_DIAG_RSLT	= 0x0F,
	LCD_ENTER_SLEEP_MODE			= 0x10,
	LCD_SLEEP_OUT					= 0x11,
	LCD_PARTIAL_MODE_ON				= 0x12,
	LCD_NORMAL_DISPLAY_MODE_ON		= 0x13,
	LCD_DISPLAY_INVERSION_OFF		= 0x20,
	LCD_DISPLAY_INVERSION_ON		= 0x21,
	LCD_GAMMA_SET					= 0x26,
	LCD_DISPLAY_OFF					= 0x28,
	LCD_DISPLAY_ON					= 0x29,
	LCD_COLUMN_ADDRESS_SET			= 0x2A,
	LCD_PAGE_ADDRESS_SET			= 0x2B,
	LCD_MEMORY_WRITE				= 0x2C,
	LCD_COLOR_SET					= 0x2D,
	LCD_MEMORY_READ					= 0x2E,
	LCD_PARTIAL_AREA				= 0x30,
	LCD_VERTICAL_SCROLLING_DEF		= 0x33,
	LCD_TEARING_EFFECT_LINE_OFF		= 0x34,
	LCD_TEARING_EFFECT_LINE_ON		= 0x35,
	LCD_MEMORY_ACCESS_CONTROL		= 0x36,
	LCD_VERTICAL_SCROLL_START_ADR	= 0x37,
	LCD_IDLE_MODE_OFF				= 0x38,
	LCD_IDLE_MODE_ON				= 0x39,
	LCD_PIXEL_FORMAT_SET			= 0x3A,
	LCD_WRITE_MEMORY_CONTINUE		= 0x3C,
	LCD_READ_MEMORY_CONTINUE		= 0x3E,
	LCD_SET_TEAR_SCANLINE			= 0x44,
	LCD_GET_SCANLINE				= 0x45,
	LCD_WRITE_DISPLAY_BRIGHTNESS	= 0x51,
	LCD_READ_DISPLAY_BRIGHTNESS		= 0x52,
	LCD_WRITE_CTRL_DISPLAY			= 0x53,
	LCD_READ_CTRL_DISPLAY			= 0x54,
	LCD_WRITE_CABC					= 0x55,
	LCD_READ_CABC					= 0x56,
	LCD_WRITE_CABC_MIN_BRIGHTNESS	= 0x5E,
	LCD_READ_CABC_MIN_BRIGHTNESS	= 0x5F,
	LCD_READ_ID1					= 0xDA,
	LCD_READ_ID2					= 0xDB,
	LCD_READ_ID3					= 0xDC,
};

#define LCD_HEIGHT 320
#define LCD_WIDTH 240

#define lcd_cmd_w(cmd) \
{ \
	lcd_ctrl_lo(RS); \
	lcd_ctrl_lo(WR); \
	DATA_PORT = cmd; \
	lcd_ctrl_hi(WR); \
	lcd_ctrl_hi(RS); \
}

#define lcd_data_w(data) \
{ \
	lcd_ctrl_lo(WR); \
   	DATA_PORT = data; \
	lcd_ctrl_hi(WR); \
}

#define set_pixel

//used to read internal status or data
//bro never reads?
// #define lcd_data_r(data) \
// { \
// 	DATA_DDR = 0x00; \
// 	lcd_ctrl_lo(RD); \
// 	lcd_ctrl_hi(RD); \
// 	data = DATA_PIN; \

// }

// #define lcd_data_w16(data) \
// { \
// 	write_data((data) >> 8); \
// 	write_data((data) & 0xFF); \
// }

// #define write_cmd_data(cmd, ndata, data) \
// { \
// 	uint8_t i; \
// 	char *d = data; \
// 	write_cmd(cmd); \
// 	for(i=0; i<ndata; i++) \
// 		write_data(*d++); \
// }

//drawing macros
#define lcd_set_col_adr(x1, x2) \
{\
	lcd_cmd_w(LCD_COLUMN_ADDRESS_SET);\
	lcd_data_w((uint8_t)((x1)>>8));\
	lcd_data_w((uint8_t)(x1));\
	lcd_data_w((uint8_t)((x2)>>8));\
	lcd_data_w((uint8_t)(x2));\
}

#define lcd_set_page_adr(y1, y2) \
{\
	lcd_cmd_w(LCD_PAGE_ADDRESS_SET);\
	lcd_data_w((uint8_t)((y1)>>8));\
	lcd_data_w((uint8_t)(y1));\
	lcd_data_w((uint8_t)((y2)>>8));\
	lcd_data_w((uint8_t)(y2));\
}

#define lcd_mem_write_begin(x1, y1, x2, y2) \
{\
	lcd_set_col_adr(x1, x2);\
	lcd_set_page_adr(y1, y2);\
	lcd_cmd_w(LCD_MEMORY_WRITE);\
}

#define lcd_mem_write_8(d) lcd_data_w((uint8_t)(d))
#define lcd_mem_write_16(d) { lcd_data_w((uint8_t)((d)>>8)); lcd_data_w((uint8_t)(d)); }
#define lcd_mem_write_end() lcd_cmd_w(LCD_NO_OPERATION);