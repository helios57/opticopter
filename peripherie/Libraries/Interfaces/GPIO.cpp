#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/common.h>

#include "GPIO.h"
#include <Arduino.h>

/* private-ish: only to be used from the appropriate interrupt */
Proc _interrupt_6;

SIGNAL(INT6_vect) {
	if (_interrupt_6) {
		_interrupt_6();
	}
}

GPIO::GPIO() {

}
GPIO::~GPIO() {

}

void GPIO::pinMode(const unsigned char pin, const unsigned char mode) {
	unsigned char bit = digitalPinToBitMask(pin);
	unsigned char port = digitalPinToPort(pin);
	pinMode(bit, port, mode);
}

void GPIO::pinMode(const DigitalSource* digitalSource, const unsigned char mode) {
	pinMode(digitalSource->_bit, digitalSource->port, mode);
}

void GPIO::pinMode(unsigned char bit, unsigned char port, const unsigned char mode) {
	if (port == 0)
		return;
	volatile unsigned char* reg;
	// JWS: can I let the optimizer do this?
	reg = portModeRegister(port);
	if (mode == GPIO_INPUT) {
		unsigned char oldSREG = SREG;
		cli();
		*reg &= ~bit;
		SREG = oldSREG;
	} else {
		unsigned char oldSREG = SREG;
		cli();
		*reg |= bit;
		SREG = oldSREG;
	}
}

unsigned char GPIO::read(unsigned char pin) {
	unsigned char bit = digitalPinToBitMask(pin);
	unsigned char port = digitalPinToPort(pin);
	return read(bit, port);
}

unsigned char GPIO::read(const DigitalSource* digitalSource) {
	return read(digitalSource->_bit, digitalSource->port);
}

unsigned char GPIO::read(const unsigned char bit, const unsigned char port) {
	if (port == 0)
		return 0;
	if (*portInputRegister(port) & bit)
		return 1;
	return 0;
}

void GPIO::write(unsigned char pin, unsigned char value) {
	unsigned char bit = digitalPinToBitMask(pin);
	unsigned char port = digitalPinToPort(pin);
	write(bit, port, value);
}

void GPIO::write(const DigitalSource* digitalSource, const unsigned char value) {
	write(digitalSource->_bit, digitalSource->port, value);
}

void GPIO::write(unsigned char bit, unsigned char port, unsigned char value) {
	if (port == 0)
		return;
	volatile unsigned char* out;
	out = portOutputRegister(port);
	unsigned char oldSREG = SREG;
	cli();
	if (value == 0) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}
	SREG = oldSREG;
}

/* Implement GPIO Interrupt 6, used for MPU6000 data ready on APM2. */
bool GPIO::attach_interrupt(unsigned char interrupt_num, Proc proc, unsigned char mode) {
	/* Mode is to set the ISCn0 and ISCn1 bits.
	 * These correspond to the GPIO_INTERRUPT_ defs in AP_HAL.h */
	if (!((mode == 0) || (mode == 1) || (mode == 2) || (mode == 3)))
		return false;
	if (interrupt_num == 6) {
		unsigned char oldSREG = SREG;
		cli();
		_interrupt_6 = proc;
		/* Set the ISC60 and ICS61 bits in EICRB according to the value
		 * of mode. */EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (mode << ISC60);
		EIMSK |= (1 << INT6);
		SREG = oldSREG;
		return true;
	} else {
		return false;
	}
}

DigitalSource* GPIO::pinToDigitalSource(unsigned short pin) {
	unsigned char bit = digitalPinToBitMask(pin);
	unsigned char port = digitalPinToPort(pin);
	return new DigitalSource(bit, port);
}
