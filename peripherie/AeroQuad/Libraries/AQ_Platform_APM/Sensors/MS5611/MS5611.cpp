/*
 * MS5611.cpp
 *
 *  Created on: Aug 17, 2013
 *      Author: helios
 */

#include "MS5611.h"
#include "AQ_Platform_APM/Util/Writh.h"

void MS5611::delay(unsigned long ms) {
	unsigned long start = millis();
	while (start + ms > millis())
		;
}
void MS5611::init() {
	if (initialised) {
		return;
	}
	register_write(CMD_MS5611_RESET);
	delay(20);

	pressSens_C1 = ((uint32_t) register_read(CMD_MS5611_PROM_C1)) << 15;
	pressOffset_C2 = ((uint32_t) register_read(CMD_MS5611_PROM_C2)) << 16;
	tempCoeffPressSens_C3 = register_read(CMD_MS5611_PROM_C3);
	tempCoeffPressOffset_C4 = register_read(CMD_MS5611_PROM_C4);
	refTemp_C5 = register_read(CMD_MS5611_PROM_C5);
	tempCoeffTemp_C6 = register_read(CMD_MS5611_PROM_C6);
	initialised = true;
}

void MS5611::sendTempD2Command() {
	spi->begin();
	spi->transfer(CMD_CONVERT_D2_OSR4096);
	spi->end();
}

void MS5611::sendPressD1Command() {
	spi->begin();
	spi->transfer(CMD_CONVERT_D1_OSR4096);
	spi->end();
}
/**
 * Poll, because a delay between request and recieve is needed,
 * switch through states of a State - Machine,
 * because the temp changes less fast only read temp in 1:7 cases
 */
bool MS5611::poll() {
	switch (readState) {
	case 1:
		sendTempD2Command();
		break;
	case 2:
		readTemperature();
		sendPressD1Command();
		break;
	case 3:
		readPressure();
		sendPressD1Command();
		break;
	case 4:
		readPressure();
		sendPressD1Command();
		break;
	case 5:
		readPressure();
		sendPressD1Command();
		break;
	case 6:
		readPressure();
		sendPressD1Command();
		break;
	case 7:
		readPressure();
		sendPressD1Command();
		break;
	case 8:
		readPressure();
		break;
	default:
		readState = 1;
		sendTempD2Command();
		break;
	}
	readState++;
	return true;
}

void MS5611::readPressure() {
	read4BytesIntoBuffer();
	digitalPress_D1 = (((uint32_t) (buff[1])) << 16) | (((uint32_t) (buff[2])) << 8) | ((uint32_t) (buff[3]));
	digitalPress_D1_ring[digitalPress_D1_ringIndex++] = digitalPress_D1;
	if (digitalPress_D1_ringIndex > 9) {
		digitalPress_D1_ringIndex = 0;
	}
	calculated = false;
}

void MS5611::readTemperature() {
	read4BytesIntoBuffer();
	digitalTemp_D2 = (((uint32_t) (buff[1])) << 16) | (((uint32_t) (buff[2])) << 8) | ((uint32_t) (buff[3]));
	digitalTemp_D2_ring[digitalTemp_D2_ringIndex++] = digitalTemp_D2;
	if (digitalTemp_D2_ringIndex > 9) {
		digitalTemp_D2_ringIndex = 0;
	}
	calculated = false;
}

void MS5611::calculate() {
	if (calculated) {
		return;
	}
	digitalPress_D1_avg = median(digitalPress_D1_ring,10);
	digitalTemp_D2_avg = median(digitalTemp_D2_ring,10);

	dT = digitalTemp_D2_avg - (((uint32_t) refTemp_C5) << 8);
	temp = (2000.0 + ((float) dT * (float) tempCoeffTemp_C6) / 8388608);
	off = pressOffset_C2 + ((float) tempCoeffPressOffset_C4 * (float) dT) / 128;
	sens = pressSens_C1 + ((float) tempCoeffPressSens_C3 * (float) dT) / 256;
	if (temp < 2000) {
		//< 20°
		dT2 = ((float) (dT) * (float) (dT)) / 2147483648.0;
		off2 = (5 * (temp - 2000) * (temp - 2000)) / 2;
		sens2 = off2 / 2;
		if (temp < -1500) {
			off2 += 7 * (temp + 1500) * (temp + 1500);
			sens2 += (11 * (temp + 1500) * (temp + 1500)) / 2;
		}
		temp -= dT2;
		off -= off2;
		sens -= sens2;
	}
	press = (((digitalPress_D1_avg * sens) / (1L << 21)) - off) / (100L << 15);
	temp /= 100;
	calculated = true;
}

uint32_t MS5611::getDigitalPressure() {
	return digitalPress_D1;
}

uint32_t MS5611::getDigitalTemperature() {
	return digitalTemp_D2;
}
float MS5611::getPressure() {
	calculate();
	return press;
}

float MS5611::getTemperature() {
	calculate();
	return temp;
}
float MS5611::getBarometerAltitude() {
	calculate();
	return 44330 * (1 - pow((press / sea_press), 1 / 5.257));
}

void MS5611::read4BytesIntoBuffer() {
	spi->begin();
	buff[0] = spi->transfer(0);
	buff[1] = spi->transfer(0);
	buff[2] = spi->transfer(0);
	buff[3] = spi->transfer(0);
	spi->end();
}

uint16_t MS5611::register_read(uint8_t reg) {
	spi->begin();
	spi->transfer(reg | 0x80);  // Set most significant bit
	uint8_t result1 = spi->transfer(0);
	uint8_t result2 = spi->transfer(0);
	spi->end();
	return (((uint16_t) result1) << 8) + (uint16_t) result2;
}

void MS5611::register_write(uint8_t reg) {
	spi->begin();
	spi->transfer(reg);
	spi->end();
}
