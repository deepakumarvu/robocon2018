/*
 * usbttl.c
 *
 * Created: 09-04-2017 5.47.30 PM
 * Author : Deepak
 */ 
#define F_CPU 14745600UL
//#define F_CPU 7372800UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
uint8_t uart_read()
{
	uint8_t b;
	while(!(UCSR0A &(1<<RXC0)));
	b=UDR0;
	return b;
}
void uarts(uint8_t i)
{
	UDR0=i;
	while(!((i=UCSR0A) &(1<<TXC0)));
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
	char b[20],f[10];
	dtostrf( data, 3, 5, f);					/* Take values in buffer to send all parameters over USART */
	sprintf(b," A%c = %s g\t",x,f);
	uart_sendstring(b);
}
void uart_init()
{
	DDRE|=(1<<1);//send
	DDRE&=~(1<<0);//recive
    UCSR0B|=(1<<RXEN0)|(1<<TXEN0);//TRANSFER AND RECIEVE ENABLE
	UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);//8 BIT MODE
	UBRR0L=95;//9600 bps DIVISOR VALUE FOR 14.7MHz
	UBRR0H=0;
}
/*void main()
{
	uart_init();
	while(1)
	{
		uart_send(23445);
		//_delay_ms(100);
		uart_send(3);
		//_delay_ms(100);
		uart_send(4);
		//_delay_ms(100);
		uart_send(5);
		//_delay_ms(100);
	}
}*/
