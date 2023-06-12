/*
 * GccApplication1.c
 *
 * Created: 4/15/2022 10:44:28 PM
 * Author : Asus
 */ 

#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
unsigned int data;


int main(void)
{
sei(); //enable global interrupts 
UCSRB |=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE); //enable transmitting of USART
//set the data size
UCSRC |=(1<<UCSZ0)|(1<<UCSZ1);
//setting the baud rate
UCSRA |=(1<<U2X);
UBRRL=207;
UBRRH=0;
DDRB|=(1<<DDB0); //configure PB0 pin as an output pin
   
    while (1) 
    {
		if (data==("a"))
		{
			PORTB|=(1<<PB0); //HIGH
    }
	if (data==("b")){
		PORTB &=~(1<<PB0); //LOW
	}
}
ISR(USART_RXC_vect)
	{
		data=UDR; //interrupt handler
	}	


