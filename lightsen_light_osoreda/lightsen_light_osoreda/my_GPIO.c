/*
 * my_GPIO.c
 *
 * Created: 2018/09/19 17:04:00
 *  Author: yama
 */ 

#define F_CPU 16000000UL //É}ÉCÉRÉìÇÃìÆçÏé¸îgêî(16MHz)

#include <avr/io.h>

void PortDirection(uint8_t port, uint8_t dir)
{
	//P0 = PD0
	if( (port & 0x01) != 0 ) dir ? (DDRD |= 0x01) : (DDRD &= ~0x01);
	//P1 = PD1
	if( (port & 0x02) != 0 ) dir ? (DDRD |= 0x02) : (DDRD &= ~0x02);
	//P2 = PB0
	if( (port & 0x04) != 0 ) dir ? (DDRB |= 0x01) : (DDRB &= ~0x01);
	//P3 = PB1
	if( (port & 0x08) != 0 ) dir ? (DDRB |= 0x02) : (DDRB &= ~0x02);
	//P4 = PD4
	if( (port & 0x10) != 0 ) dir ? (DDRD |= 0x10) : (DDRD &= ~0x10);
	//P5 = PB2
	if( (port & 0x20) != 0 ) dir ? (DDRB |= 0x04) : (DDRB &= ~0x04);
	//P6 = PD6
	if( (port & 0x40) != 0 ) dir ? (DDRD |= 0x40) : (DDRD &= ~0x40);
	//P7 = PD7
	if( (port & 0x80) != 0 ) dir ? (DDRD |= 0x80) : (DDRD &= ~0x80);
}

void PortWriteBit(uint8_t port, uint8_t val)
{
	//P0 = PD0
	if( (port & 0x01) != 0 ) val ? (PORTD |= 0x01) : (PORTD &= ~0x01);
	//P1 = PD1
	if( (port & 0x02) != 0 ) val ? (PORTD |= 0x02) : (PORTD &= ~0x02);
	//P2 = PB0
	if( (port & 0x04) != 0 ) val ? (PORTB |= 0x01) : (PORTB &= ~0x01);
	//P3 = PB1
	if( (port & 0x08) != 0 ) val ? (PORTB |= 0x02) : (PORTB &= ~0x02);
	//P4 = PD4
	if( (port & 0x10) != 0 ) val ? (PORTD |= 0x10) : (PORTD &= ~0x10);
	//P5 = PB2
	if( (port & 0x20) != 0 ) val ? (PORTB |= 0x04) : (PORTB &= ~0x04);
	//P6 = PD6
	if( (port & 0x40) != 0 ) val ? (PORTD |= 0x40) : (PORTD &= ~0x40);
	//P7 = PD7
	if( (port & 0x80) != 0 ) val ? (PORTD |= 0x80) : (PORTD &= ~0x80);	
}


