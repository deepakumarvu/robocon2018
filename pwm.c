/*
 * pwm.cpp
 *
 * Created: 08-Nov-16 6:17:33 PM
 * Author : Deepak
 */ 
#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
double duty=0,x=20;

int main(void)
{
    /* Replace with your application code */
	DDRB=(1<<5);
	TCCR1A=(1<<COM1A1)|(1<<WGM11)|(1<<WGM10);
	TCCR1B=(1<<WGM12);
	OCR1A=((duty/100)*255);
	sei();
	TCCR1B|=(1<<CS10);
    while (1) 
    {
		duty+=x;
		if(duty>99)
		{
			duty=0;
		}
		_delay_ms(1000);
		OCR1A=(duty/100)*255;
    }
}
