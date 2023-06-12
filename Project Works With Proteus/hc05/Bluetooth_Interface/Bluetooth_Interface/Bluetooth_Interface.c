/*

   D.H.Chaami
   
*/ 

#include <avr/io.h>
#include "USART_RS232_H_file.h"		/* include USART library */


int main(void)
{
	char Data_in;
    DDRB = 0xff;							/* make PORT as output port */
	USART_Init(9600);						/* initialize USART with 9600 baud rate */
	
	while(1)
	{
		Data_in = USART_RxChar();						/* receive data from Bluetooth device*/
		if(Data_in =='1')
		{	
			PORTB = 0b00000001;						/* Turn ON LED */
			USART_SendString("LED_ON");					/* send status of LED i.e. LED ON */
			
		}
		else if(Data_in =='2')
		{	
			PORTB = 0b00000000;						/* Turn OFF LED */
			USART_SendString("LED_OFF"); 				/* send status of LED i.e. LED OFF */
		}
		else if(Data_in =='3')
		{
			PORTB = 0b00000010;
			USART_SendString("H_ON");
		}
		else if(Data_in =='4')
		{
			PORTB = 0b00000000;
			USART_SendString("H_OFF");
		}
		else if(Data_in =='5')
		{
			PORTB = 0b00000100;
			USART_SendString("M_ON");
		}
		else if(Data_in =='6')
		{
			PORTB = 0b00000000;
			USART_SendString("M_OFF");
		}
		else
			USART_SendString("Select proper option");	/* send message for selecting proper option */
	}		
	return 0;
}
