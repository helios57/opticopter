/*
 * SPIDeviceDriver0.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */
#include "SPIDeviceDriver0.h"
#include <Arduino.h>

SPIDeviceDriver0::~SPIDeviceDriver0() {
}
#define SPI0_SPCR_8MHz   0
#define SPI0_SPSR_8MHz   _BV(SPI2X)
#define SPI0_SPCR_500kHz _BV(SPR1)
#define SPI0_SPSR_500kHz _BV(SPI2X)

void SPIDeviceDriver0::init() {
	gpio->pinMode(miso, GPIO_INPUT);
	gpio->pinMode(mosi, GPIO_OUTPUT);
	gpio->pinMode(sck, GPIO_OUTPUT);
	gpio->pinMode(cs, GPIO_OUTPUT);
	gpio->write(cs, 1);
	active = false;
	setHighRate(false);

	/* Enable the SPI0 peripheral as a master */
	SPCR = _BV(SPE) | _BV(MSTR);
}

void SPIDeviceDriver0::begin() {
	if (active) {
		console->println("SPI begin called but already active!");
		return;
	}
	active = true;
	gpio->write(cs, 0);
}
void SPIDeviceDriver0::end() {
	gpio->write(cs, 1);
	active = false;
}

unsigned char SPIDeviceDriver0::transfer(unsigned char data) {
	if (!active) {
		console->println("SPI transfer called but not active!");
		return 0;
	}
	SPDR = data;
	while (!(SPSR & _BV(SPIF)))
		;
	return SPDR;
}

void SPIDeviceDriver0::setHighRate(bool high) {
	const uint8_t valid_spcr_mask = (_BV(CPOL) | _BV(CPHA) | _BV(SPR1) | _BV(SPR0));
	if (high) {
		uint8_t new_spcr = (SPCR & ~valid_spcr_mask) | (SPI0_SPCR_8MHz & valid_spcr_mask);
		SPCR = new_spcr;
		const uint8_t valid_spsr_mask = _BV(SPI2X);
		uint8_t new_spsr = (SPSR & ~valid_spsr_mask) | (SPI0_SPSR_8MHz & valid_spsr_mask);
		SPSR = new_spsr;
	} else {
		uint8_t new_spcr = (SPCR & ~valid_spcr_mask) | (SPI0_SPCR_500kHz & valid_spcr_mask);
		SPCR = new_spcr;
		const uint8_t valid_spsr_mask = _BV(SPI2X);
		uint8_t new_spsr = (SPSR & ~valid_spsr_mask) | (SPI0_SPSR_500kHz & valid_spsr_mask);
		SPSR = new_spsr;
	}
}
