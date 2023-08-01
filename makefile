objects = lcd_draw.o lcd_text.o

CC = avr-gcc
CFLAGS = -mmcu=atmega644p -DF_CPU=12000000 -Os

lcd_hl.a : $(objects)
	avr-ar rcs lcd_hl.a $(objects)
	rm lcd_draw.o lcd_text.o

.PHONY : clean
clean :
	rm lcd_hl.a