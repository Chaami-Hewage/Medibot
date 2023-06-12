/*
 * GccApplication1.c
 *
 * LCD
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "i2c(ii).h"


int main(void)
{
	i2c_init();
	i2c_start();
	i2c_write(0x70);
	lcd_init();
	
	while (1)
	{
		lcd_cmd(0x80);
		lcd_msg("   Hey there!");
		_delay_ms(1000);
		lcd_cmd(0xC0);
		_delay_ms(1000);
		lcd_msg("Heater is ");
		lcd_msg("ON");
		lcd_msg("!");
		_delay_ms(1000);
		lcd_cmd(0x01);
		_delay_ms(1000);
	}
}