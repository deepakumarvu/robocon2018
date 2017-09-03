#!/bin/bash
# My first script

echo "Hello World!"
#echo "Enter File name"
#read name
#echo "Enter Microcontroller no : "
#read mc
avr-gcc -g -Os -mmcu=atmega$2 -c $1.c
avr-gcc -g -mmcu=atmega$2 -o $1.elf $1.o
avr-objcopy -j .text -j .data -O ihex $1.elf $1.hex
sudo avrdude -c avrispmkII -p atmega$2 -U flash:w:$1.hex
