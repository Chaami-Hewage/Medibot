/*

   D.H.Chaami
   
*/ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART_RS232_H_file.h"		/* include USART library */
#include "i2c.h"
#include "i2c(ii).h"
#include "servo.h"

#define  Trigger_pin	PA3	/* Trigger pin  */


static volatile unsigned int tCount = 0;//to hold the counter value from TCNT1.
static volatile int i = 0; //to indicate the state of ultrasonic sensor.
double distance;//to store the measured distance in cm

void trig(); //to send pulse

void goAhead();
void goBack();
void goLeft();
void stopRobot();
void isBlocked();

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;		/* Increment Timer Overflow count */
}


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
	
//--Servo+Motor pump+ultrasonic--	
	
	char string[10];
	long count;
	double distance;
	
	DDRA = 0b00001000;
	/* Make trigger pin as output */
	DDRD = 0b00000011;
	PORTD = 0x00;
	
	DDRC = 0x01; //Makes RC0 output pin
	PORTC = 0x00;
	
	sei();					/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;				/* Set all bit to zero Normal operation */
	
	//--DC motors
	DDRC = DDRC |(1 << DDC3); // Set pin as an output
	DDRC = DDRC |(1 << DDC4);
	DDRC = DDRC |(1 << DDC5);
	DDRC = DDRC |(1 << DDC6);
	DDRA = DDRA |(1 << DDA1); // to set Trigger pin as an output
	DDRD = DDRD & ~(1 << DDD2) ; // to set Echo pin(INT0 - PD2 on atmega32) as an input.
	PORTD = PORTD & ~(1 << PD2) ;
	_delay_ms(50);
	
	GICR |= (1<<INT0); //to enable interrupt for INT0
	MCUCR |= (1<<ISC00);//to make any logical change on INT0 generate an interrupt
	TIMSK |= (1 << TOIE1);//to enable Timer/Counter 1, Overflow interrupt
	
	sei();//to enable global interrupt bit
	_delay_ms(500);

	
	
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
			PORTD = 0b10000000; //buzzer on
			PORTB = 0b00000010;
			USART_SendString("Heater_ON;  ");
			lcd_cmd(0x80);
			lcd_msg("Heater_ON");
			_delay_ms(1000);
			lcd_cmd(0x01); //clear
			PORTB = 0b00000000; //buzzer off
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
		
			else if(Data_in =='m')			//Stop
			{
				isBlocked(); //to call isBlocked function
				_delay_ms(200);
			}
		else
			USART_SendString("Select proper option");	/* send message for selecting proper option */
	}	
	
	//--Servo+Motor pump+ultrasonic--	
	
	PORTA |= (1 << Trigger_pin);/* Give 10us trigger pulse on trig. pin to HC-SR04 */
	_delay_us(10);
	PORTA &= (~(1 << Trigger_pin));
	
	TCNT1 = 0;			/* Clear Timer counter */
	TCCR1B = 0x41;		/* Setting for capture rising edge, No pre-scaler*/
	TIFR = 1<<ICF1;		/* Clear ICP flag (Input Capture flag) */
	TIFR = 1<<TOV1;		/* Clear Timer Overflow flag */

	/*Calculate width of Echo by Input Capture (ICP) on PortD PD6 */
	
	while ((TIFR & (1 << ICF1)) == 0);	/* Wait for rising edge */
	TCNT1 = 0;			/* Clear Timer counter */
	TCCR1B = 0x01;		/* Setting for capture falling edge, No pre-scaler */
	TIFR = 1<<ICF1;		/* Clear ICP flag (Input Capture flag) */
	TIFR = 1<<TOV1;		/* Clear Timer Overflow flag */
	TimerOverflow = 0;	/* Clear Timer overflow count */

	while ((TIFR & (1 << ICF1)) == 0); /* Wait for falling edge */
	count = ICR1 + (65535 * TimerOverflow);	/* Take value of capture register */
	/* 8MHz Timer freq, sound speed =343 m/s, calculation mentioned in doc. */
	distance = (double)count / 466.47;

	dtostrf(distance, 2, 2, string);/* Convert distance into string */
	strcat(string, " cm   ");
	_delay_ms(200);
	
	
	
	if (distance > 35 && distance < 40)
	{
		PORTD = 0b00000001;
	}
	else if (distance < 35 )
	{
		PORTD = 0b00000000;
		//Rotate Motor to 0 degree
		PORTC = 0x01;
		_delay_us(1000);
		PORTC = 0x00;

		//Rotate Motor to 90 degree
		PORTC = 0x01;
		_delay_us(1500);
		PORTC = 0x00;

	}
	else
	{
		PORTD = 0b00000000;
		//Rotate Motor to 0 degree
		PORTC = 0x01;
		_delay_us(1000);
		PORTC = 0x00;
	}

//DC motor

	
	return 0;
}
