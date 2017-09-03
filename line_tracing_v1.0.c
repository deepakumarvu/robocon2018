/*
 * uart.c
 *
 * Created: 09-04-2017 5.47.30 PM
 * Author : Deepak
 */ 
 
 /*
LCD con
Line mode
Threshold
J width 
UART ADD
Baud rate 0 9600
UART MODE 2(0-70 = valid 255 no line detected)

LSA 08
PORT A
tx-0
rx-2
~uen-3
Jpluse-5
vin-9
gnd-10
*/
#define F_CPU 14745600UL
#define motord PORTL
#define max_pwm 800
#define ackw_1		motord|=0x01
#define ckw_1		motord|=0x02
#define ackw_2		motord|=0x04
#define ckw_2		motord|=0x08
#define ackw_3		motord|=0x10
#define ckw_3		motord|=0x20
#define ackw_4		motord|=0x40
#define ckw_4		motord|=0x80
#define seb(r,p) r|=(1<<p)
#define clb(r,p) r&=~(1<<p)
#include <avr/io.h>
#include "util/delay.h"
void initpwm()
{
	//FAST PWM && INVERTING MODE && NO PRESCALER
	OCR1A=0;
	OCR1B=0;
	OCR1C=00;
	OCR4A=00;
	TCCR1A=0xff;
	TCCR1C=0x00;
	TCCR1B=0x09;
	TCCR4A=0xff;
	TCCR4C=0x00;
	TCCR4B=0x09;
}
uint8_t uart_read()
{
	uint8_t b;
	while(!(UCSR0A &(1<<RXC0)));
	b=UDR0;
	return b;
}
void uart_send(uint8_t data)
{
	UDR0=data;
	while(!(UCSR0A &(1<<TXC0)));
}
int receive()
{
		PORTE&=~(1<<2);
		uart_send(0x01);
		uart_send(0x4F);
		uart_send(0x02);
		uart_send(0x52);
		int b=uart_read();
		PORTE|=(1<<2);
		return b;
}
void calib()
{
		PORTE&=~(1<<2);
		uart_send(0x01);
		uart_send(0x43);
		uart_send(0x00);
		uart_send(0x44);
		PORTE|=(1<<2);
}
void bot_forward()
{
	motord=0x00;
	ckw_1;
	ackw_2;
	ackw_3;
	ckw_4;
	OCR1A=max_pwm;
	OCR1B=max_pwm;
	OCR1C=max_pwm;
	OCR4A=max_pwm;
}
void bot_reverse()
{
	motord=0x00;
	ackw_1;
	ckw_2;
	ckw_3;
	ackw_4;
	OCR1A=max_pwm;
	OCR1B=max_pwm;
	OCR1C=max_pwm;
	OCR4A=max_pwm;
}
void bot_right()
{
	motord=0x00;
	ckw_1;
	ckw_2;
	ckw_3;
	ckw_4;
	OCR1A=max_pwm;
	OCR1B=max_pwm;
	OCR1C=max_pwm;
	OCR4A=max_pwm;
}
void bot_left()
{
	motord=0x00;
	ackw_1;
	ackw_2;
	ackw_3;
	ackw_4;
	OCR1A=max_pwm;
	OCR1B=max_pwm;
	OCR1C=max_pwm;
	OCR4A=max_pwm;
}
void bot_stop()
{
	motord=0x00;
}

int main(void)
{
	DDRL=0xff;			//Locomotion Direction Bits		1|2|3|4
	DDRB=0xf0;			//PWM(4-Motor1	5,6,7-LOCO 1,2,3)
	DDRH=0xff;			//2-Chip Select	PWM(3-LOCO 4)
	DDRE|=(1<<1)|(1<<2);//TXD0 and ~UEN
	DDRE&=~(1<<0);	//RXD0
	DDRA=0XFF;		//output testing LED
	uint8_t b=0x00;
    UCSR0B|=(1<<RXEN0)|(1<<TXEN0);//TRANSFER AND RECIEVE ENABLE
	UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);//8 BIT MODE
	UBRR0L=95;//9600 bps DIVISOR VALUE FOR 14.74MHz
	UBRR0H=0;
	PORTL=0x00;
	initpwm();
	bot_stop();
	_delay_ms(2000);
	/*	OCR1A=0;
		OCR1B=0;
		OCR1C=00;
		OCR4A=00;
	while(1)
	{
		_delay_ms(1000);
		OCR1A++;
		OCR1B++;
		OCR1C++;
		OCR4A++;
		if(OCR1A==254)
		{
			OCR1A=0;
			OCR1B=0;
			OCR1C=00;
			OCR4A=00;
		}
	}*/
   while (1) 
    {
		b=receive();
		PORTA=b;
		if(b==255 || b>70 || b<0)
		{
			bot_stop();
		}
		else if (b<25)
		{
			bot_left();
		}
		else if(b>45)
		{
			bot_right();
		}
		else
		{
			bot_forward();
		}
		_delay_ms(10);
    }
}

