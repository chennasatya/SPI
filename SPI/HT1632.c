/*
 * HT1632.c
 *
 * Created: 4/25/2015 12:39:39 PM
 *  Author: Lovelyhearts
 
 */ 
#include "HT1632.h"
#define LOW 0
#define HIGH 1
 
#define PIN_OUT_CS (1 << CS)
#define PIN_OUT_DATA (1 << DATA)
#define PIN_OUT_WRITE (1 << WRITE)

#define CS PINB7
#define DATA PINB4
#define WRITE PINB5

#define PINMODE(X)  (DDRB |=  PIN_OUT_##X)



char matrix[16 * 24];

void setPixel(uint16_t i) {
	matrix[i/8] |= _BV(i%8);
}

void clrPixel(uint16_t i) {
	matrix[i/8] &= ~_BV(i%8);
}
void drawPixel(uint8_t x, uint8_t y, uint8_t color) {
	if (y >= HEIGHT) return;
	if (x >= WIDTH) return;

	uint8_t m;
	// figure out which matrix controller it is
	m = x / 24;
	x %= 24;

	uint16_t i;

	if (x < 8) {
		i = 7;
		} else if (x < 16) {
		i = 128 + 7;
		} else {
		i = 256 + 7;
	}
	i -= (x % 8);

	if (y < 8) {
		y *= 2;
		} else {
		y = (y-8) * 2 + 1;
	}

	i += y * 8;

	if (color)
	setPixel(i);
	else
	clrPixel(i);
}

void sendcommand(uint8_t cmd) {
	uint16_t data = 0;
	data = HT1632_COMMAND;
	data <<= 8;
	data |= cmd;
	data <<= 1;
	
	digitalWrite(CS, LOW);
	writedata(DATA, 12);
	digitalWrite(CS, HIGH);
}

void writedata(uint16_t d, uint8_t bits) {
	PINMODE(DATA);
	for (uint8_t i=bits; i > 0; i--) {
		digitalWrite(WRITE, LOW);
		if (d & _BV(i-1)) {
			digitalWrite(DATA, HIGH);
			} else {
			digitalWrite(DATA, LOW);
		}
		digitalWrite(WRITE, HIGH);
	}
	//pinMode(_data, INPUT);
}


void begin(uint8_t type) {
	
	PINMODE(CS);
	digitalWrite(CS, HIGH);
	PINMODE(WRITE);
	digitalWrite(WRITE, HIGH);
	PINMODE(DATA);
	
	//if (_rd >= 0) {
		//pinMode(_rd, OUTPUT);
		//digitalWrite(_rd, HIGH);
	//}
	
	//DDRB = (1 << PINB7) | (1 << PINB5) | (1 << PINB4);//MISO -- PB6, MOSI -- PB5, SCK - PB7, SS -- PB4
	
	sendcommand(HT1632_SYS_EN);
	sendcommand(HT1632_LED_ON);
	sendcommand(HT1632_BLINK_OFF);
	sendcommand(HT1632_MASTER_MODE);
	sendcommand(HT1632_INT_RC);
	sendcommand(type);
	sendcommand(HT1632_PWM_CONTROL | 0xF);
	
	WIDTH = 24;
	HEIGHT = 16;
}

void writeScreen() {

	digitalWrite(CS, LOW);

	writedata(HT1632_WRITE, 3);
	// send with address 0
	writedata(0, 7);

	for (uint16_t i=0; i<(WIDTH*HEIGHT/8); i+=2) {
		uint16_t d = matrix[i];
		d <<= 8;
		d |= matrix[i+1];

		writedata(d, 16);
	}
	digitalWrite(CS, HIGH);
}

void digitalWrite(int pinNum, int value)
{
	if(value == 0)
	{
		PORTB = (1 << pinNum);	
	}
	
	else 
	{
		PORTB = ~(1 << pinNum);
	}
	
}