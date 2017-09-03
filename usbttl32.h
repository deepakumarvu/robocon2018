/*
 * usbttl32.h
 *
 * Created: 09-04-2017 5.47.30 PM
 * Author : Deepak
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
uint8_t uart_read()
{
	uint8_t b;
	while(!(UCSRA &(1<<RXC)));
	b=UDR;
	return b;
}
void uarts(uint8_t i)
{
	UDR=i;
	while(!((i=UCSRA) &(1<<TXC)));
}
void uart_sendstring(char *str)					/* Send string of USART data function */ 
{
	int i=0;																	
	while (str[i]!='\0')
	{
		uarts(str[i]);						/* Send each char of string till the NULL */
		i++;
		_delay_ms(10);
	}
}
void uart_send(float data,char x)
{
	char b[30],f[20];
	dtostrf( data, 3, 5, f);					/* Take values in buffer to send all parameters over USART */
	sprintf(b," A%c = %s \t",x,f);
	uart_sendstring(b);
}
void uart_init()
{
	DDRD|=(1<<1);//send
	DDRD&=~(1<<0);//recive
    UCSRB|=(1<<RXEN)|(1<<TXEN);//TRANSFER AND RECIEVE ENABLE
	UCSRC|=(1<<UCSZ0)|(1<<UCSZ1);//8 BIT MODE
	UBRRL=6;//9600 bps DIVISOR VALUE FOR 1MHz
	UBRRH=0;
}
