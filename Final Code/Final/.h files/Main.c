/*

  
   
*/ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232_H_file.h"		/* include USART library */
#include "i2c.h"
#include "i2c(ii).h"
#include "servo.h"



int main(void)
{
//---HC05---
	char Data_in;
	DDRD = 0xff;
    DDRB = 0xff;							/* make PORT as output port */
	DDRC = 0xff;
	USART_Init(9600);						/* initialize USART with 9600 baud rate */

//---LCD---
	i2c_init();
	i2c_start();
	i2c_write(0x70);
	lcd_init();

	lcd_cmd(0x80);
	lcd_msg("   Hey there!");
	_delay_ms(1000);
	lcd_cmd(0x01); //clear
	
//---Servo---
	servo_init();	//Initializing
	
	
	
	while(1)
	{
		
//---HC05---
		Data_in = USART_RxChar();						/* receive data from Bluetooth device*/
		if(Data_in =='1')
		{	
			PORTB = 0b00000001;						/* Turn ON LED */
			USART_SendString("LAMP_ON;  ");					/* send status of LED i.e. LED ON */
			lcd_cmd(0x80);
			lcd_msg("LAMP_ON");
			_delay_ms(1000);
			lcd_cmd(0x01); //clear
			
		}
		else if(Data_in =='2')
		{	
			PORTB = 0b00000000;						/* Turn OFF LED */
			USART_SendString("LAMP_OFF;  "); 				/* send status of LED i.e. LED OFF */
			lcd_cmd(0x80);
			lcd_msg("LAMP_OFF");
			_delay_ms(1000);
			lcd_cmd(0x01); //clear
		}
		else if(Data_in =='3')
		{
			PORTD = 0b10000000;//buzzer on
			PORTB = 0b00000010;
			USART_SendString("Heater_ON;  ");
			lcd_cmd(0x80);
			lcd_msg("Heater_ON");
			_delay_ms(1000);
			lcd_cmd(0x01); //clear
			PORTD = 0b00000000;
			_delay_ms(1000);
		}
		else if(Data_in =='4')
		{
			PORTB = 0b00000000;
			USART_SendString("Heater_OFF;  ");
			lcd_cmd(0x80);
			lcd_msg("Heater_OFF");
			_delay_ms(1000);
			lcd_cmd(0x01); //clear
		}
		else if(Data_in =='5')
		{
			PORTB = 0b00000100;
			USART_SendString("MOTOR_ON;  ");
			lcd_cmd(0x80);
			lcd_msg("MOTOR_ON");
			_delay_ms(1000);
			lcd_cmd(0x01); //clear
		}
		else if(Data_in =='6')
		{
			PORTB = 0b00000000;
			USART_SendString("MOTOR_OFF;  ");
			lcd_cmd(0x80);
			lcd_msg("MOTOR_OFF");
			_delay_ms(1000);
			lcd_cmd(0x01); //clear
		}
		else if(Data_in =='w')			//Forward		{			USART_SendString("Forward;  ");			PORTC = 0b01010000;			_delay_ms(3000);			PORTC = 0b00000000;					}		else if(Data_in =='s')			//Backward		{			USART_SendString("Backward;  ");			PORTC = 0b00101000;			_delay_ms(3000);			PORTC = 0b00000000;		}		else if(Data_in =='a')			//Left		{			USART_SendString("Left;  ");			PORTC = 0b01000000;			_delay_ms(1500);			PORTC = 0b00000000;		}		else if(Data_in =='d')			//Right		{			USART_SendString("Right;  ");			PORTC = 0b00010000;			_delay_ms(1500);			PORTC = 0b00000000;		}
		else if(Data_in =='7')			//Stop
		{
			USART_SendString("Stop;  ");
			PORTC = 0b00000000;
		}
		else
			USART_SendString("Select proper option!");	/* send message for selecting proper option */
	}		
	return 0;
}
