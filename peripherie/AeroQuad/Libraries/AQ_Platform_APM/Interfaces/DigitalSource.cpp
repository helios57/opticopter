/*
 * AVRDigitalSource.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: helios
 */

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "GPIO.h"
#include "DigitalSource.h"
#include <Arduino.h>

DigitalSource::~DigitalSource() {
}

void DigitalSource::mode(unsigned char output) {
	volatile unsigned char* reg = portModeRegister(port);
	if (output == GPIO_INPUT) {
		unsigned char oldSREG = SREG;
		cli();
		*reg &= ~_bit;
		SREG = oldSREG;
	} else {
		unsigned char oldSREG = SREG;
		cli();
		*reg |= _bit;
		SREG = oldSREG;
	}
}

unsigned char DigitalSource::read() {
	if (*portInputRegister(port) & _bit)
		return 1;
	return 0;
}

void DigitalSource::write(unsigned char value) {

	volatile unsigned char* out = portOutputRegister(port);

	unsigned char oldSREG = SREG;
	cli();

	if (value == 0) {
		*out &= ~_bit;
	} else {
		*out |= _bit;
	}

	SREG = oldSREG;
}
