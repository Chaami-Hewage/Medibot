/*
 * 
 *Servo rotation in pill case
 * 
 * 
 */ 

#include<avr/io.h>
#include<util/delay.h>
#include "servo.h"
#define F_CPU 8000000UL


int main(void)
{
	servo_init();	//Initializing
    
    while (1)
	 
    {
		set_degree(90);		//For 90 degree rotation
		_delay_ms(1000);	//Delay for 1s
    }
}

