/*
 * SPIDeviceDriver.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */

#include "SPIDeviceDriver.h"
#include <Arduino.h>

SPIDeviceDriver::~SPIDeviceDriver() {
}
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV32 0x06

void SPIDeviceDriver::init() {
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (SPI_CLOCK_DIV16 & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((SPI_CLOCK_DIV16 >> 2) & SPI_2XCLOCK_MASK);

	gpio->pinMode(miso, GPIO_INPUT);
	gpio->pinMode(mosi, GPIO_OUTPUT);
	gpio->pinMode(sck, GPIO_OUTPUT);
	gpio->pinMode(cs, GPIO_OUTPUT);

	gpio->write(cs, 1);
	active = false;

	/* Enable the SPI0 peripheral as a master */
	SPCR = _BV(SPE) | _BV(MSTR);
}

void SPIDeviceDriver::begin() {
	if (active) {
		console->println("SPI begin called but already active!");
		return;
	}
	active = true;
	gpio->write(cs, 0);
}
void SPIDeviceDriver::end() {
	gpio->write(cs, 1);
	active = false;
}

unsigned char SPIDeviceDriver::transfer(unsigned char data) {
	if (!active) {
		console->println("SPI transfer called but not active!");
		return 0;
	}
	SPDR = data;
	while (!(SPSR & _BV(SPIF)))
		;
	return SPDR;
}

