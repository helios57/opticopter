/*
 * SPIDeviceDriver3.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */
#include "SPIDeviceDriver3.h"
#include <Arduino.h>

void SPIDeviceDriver3::init() {
	gpio->pinMode(miso, GPIO_INPUT);
	gpio->pinMode(mosi, GPIO_OUTPUT);
	gpio->pinMode(sck, GPIO_OUTPUT);
	gpio->pinMode(cs, GPIO_OUTPUT);
	gpio->write(cs, 1);
	active = false;

	/* UMSELn1 and UMSELn2: USART in SPI Master mode */
	UCSR3C = _BV(UMSEL31) | _BV(UMSEL30);
	/* Enable RX and TX. */
	UCSR3B = _BV(RXEN3) | _BV(TXEN3);

	/* set the device UCSRnC configuration bits.
	 * only sets data order, clock phase, and clock polarity bits (lowest
	 * three bits)  */
	/* dataflash uses mode 0 and a clock of 8mhz
	 * ucsr3c = 0
	 * ubrr3 = 0 */

	const uint8_t new_ucsr3c = (UCSR3C & ~0x07) | (0 & (0x07));
	UCSR3C = new_ucsr3c;
	/* set the device baud rate */
	UBRR3 = 0;
}

void SPIDeviceDriver3::begin() {
	if (active) {
		console->println("SPI begin called but already active!");
		return;
	}
	active = true;
	gpio->write(cs, 0);
}
void SPIDeviceDriver3::end() {
	gpio->write(cs, 1);
	active = false;
}

unsigned char SPIDeviceDriver3::transfer(unsigned char data) {
	if (!active) {
		console->println("SPI transfer called but not active!");
		return 0;
	}
	/* Wait for empty transmit buffer */
	while (!( UCSR3A & _BV(UDRE3)))
		;
	/* Put data into buffer, sends the data */
	UDR3 = data;
	/* Wait for data to be received */
	while (!(UCSR3A & _BV(RXC3)))
		;
	/* Get and return received data from buffer */
	return UDR3;
}

