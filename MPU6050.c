/*
 * ata_daddy_chidla.c

 *
 *  Created on: 05-Nov-2016
 *      Author: pinakwadikar
 *
 ** For MPU6050: write
 * START
 * 7 bit address of slave	+	(r/(w)')	(here 	+		0)
 * 8 bit register address
 * acknowledge
 * 8 bit data in that register
 * acknowledge
 * stop

 * For MPU6050: read
 * START
 * 7 bit address of slave	+	(r/(w)')	(here 	+		0)
 * 8 bit register address that is going to be  read
 * acknowledge
 * START (again)
 * 7 bit address of slave	+	(r/(w)')	(here 	+		1)
 * acknowledge
 * data(From Slave)
 * ends with not acknowledge(From Master) SDA high while SCK full cycle
PC1 SDA
 * PC0 SCL*/
#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "usbttl.h"
#define sbi(x,y) x|=(1<<y)
#define cbi(x,y) x&=~(1<<y)

#define status (TW_STATUS & 0xF8)
#define address_w 0b11010000
#define address_r 0b11010001

void start_TWI()
{
	TWCR=((1<<TWEN)|(1<<TWINT)|(1<<TWSTA));
	while((TWCR & (1 << TWINT) ) == 0);
	while(status!=TW_START);
}
void address_write()
{
	TWDR=address_w;
	TWCR=((1<<TWEN)|(1<<TWINT));
	while((TWCR & (1 << TWINT) ) == 0);
	_delay_ms(10);
	while(status!=TW_MT_SLA_ACK);//SLA+W transmitted, ACK received
}
void address_read()
{
	TWDR=address_r;
	TWCR=((1<<TWEN)|(1<<TWINT));
	while((TWCR & (1 << TWINT) ) == 0);
	_delay_ms(10);
	while(status!=TW_MR_SLA_ACK);//SLA+R transmitted, ACK received
}
void data_send(unsigned char data)
{
	TWDR=data;
	TWCR=((1<<TWEN)|(1<<TWINT));
	while((TWCR & (1 << TWINT) ) == 0);
	_delay_ms(10);
	while(status!=TW_MT_DATA_ACK);//data transmitted, ACK received
}
void start_inter()
{
	TWCR=((1<<TWEN)|(1<<TWINT)|(1<<TWSTA));
	while((TWCR & (1 << TWINT) ) == 0);
	while(status!=TW_REP_START);//repeated start condition transmitted
}
unsigned char get_data()
{
	TWCR=((1<<TWEN)|(1<<TWINT));
	while((TWCR & (1 << TWINT) ) == 0);
	return(TWDR);
}
void stop_TWI()
{
	TWCR=((1<<TWEN)|(1<<TWINT)|(1<<TWSTO));
}
void data_write(unsigned char address,unsigned char data)
{
	start_TWI();
	address_write();
	data_send(address);//0x6B
	data_send(data);//0x02
	stop_TWI();
	_delay_ms(10);
}
unsigned char read_IMU(unsigned char address)
	{
		unsigned char data;
		start_TWI();
		address_write();
		data_send(address);//0x3E
		start_inter();
		address_read();
		data=get_data();
		stop_TWI();
		_delay_ms(10);
		return(data);
	}

int main()
{
	unsigned int x=0,y=0,z=0;
	float d;
	DDRA=0xFF;
	DDRC=0xFF;
	DDRD=0xFF;
	TWCR=(1<<TWEN);
	TWSR=0x00;
	TWBR=0x02;
	uart_init();
	uart_send(0x12,'e');
	data_write(0x6B,0x02);
	data_write(0x6C,0x00);
	data_write(0x1C,0x00);

	while(1)
	{
		x=~((read_IMU(0x3B)<<8|read_IMU(0x3C))-1);
		y=~((read_IMU(0x3D)<<8|read_IMU(0x3E))-1);
		z=~((read_IMU(0x3F)<<8|read_IMU(0x40))-1);
		d=(float)x/16384.0;
		uart_send(d,'x');
		d=(float)y/16384.0;
		uart_send(d,'y');
		d=(float)z/16384.0;
		uart_send(d,'z');
		uarts(0x0a);
		_delay_ms(1000);
	}

	return 0;
}
