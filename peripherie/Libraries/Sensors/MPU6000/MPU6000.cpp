/*
 * MPU6000.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */

#include "MPU6000.h"
#include <avr/pgmspace.h>
#include "Util/WirthMedianSInt16.h"

void MPU6000::delay(unsigned long ms) {
	unsigned long start = millis();
	while (start + ms > millis())
		;
}

void MPU6000::init() {
	if (initialised) {
		return;
	}
	initialised = initialize();
}

bool MPU6000::poll() {
	uint16_t fIfoCount = getFIFOCount();
	if (fIfoCount > 1020) {
		console->println("Fifo count too high");
		resetFIFO();
		return false;
	}
	if (fIfoCount >= fifoBufferLength) {
		while (fIfoCount >= fifoBufferLength) {
			readFIFOBytes(fifoBufferLength, fifoBuffer);
			for (uint8_t i = 0; i < 6; i++) {
				motionSum[i] += (((int16_t) fifoBuffer[i * 2]) << 8) | fifoBuffer[i * 2 + 1];
			}
			motionCount++;
			fIfoCount = getFIFOCount();
		}
		return true;
	} else {
		return false;
	}
}

void MPU6000::getMotion6(int16_t *axyzgxyz) {
	for (uint8_t i = 0; i < 6; i++) {
		axyzgxyz[i] = motionSum[i] / motionCount;
		motionSum[i] = 0;
	}
	motionCount = 0;
}

bool MPU6000::initialize() {
	// reset device
	//console->println("Resetting MPU6050...");
	//reset
	register_write(MPUREG_PWR_MGMT_1, _BV(MPU6050_PWR1_DEVICE_RESET_BIT));
	delay(50); // wait after reset

	// disable sleep mode
	//console->println("Disabling sleep mode and set clock...");
	register_write(MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_CLK_XGYRO);
	delay(5);

	//console->println("Setting sample rate to 200Hz...");
	register_write(MPU6050_RA_SMPLRT_DIV, 2); // 1khz / (1 + 4) = 200 Hz
	//register_write(MPU6050_RA_SMPLRT_DIV, 0); // 1khz

	//console->println("Setting DLPF bandwidth to 42Hz...");
	uint8_t oldConfig = register_read(MPU6050_RA_CONFIG);
	oldConfig = (oldConfig & ~0x07) | MPU6050_DLPF_BW_256;
	register_write(MPU6050_RA_CONFIG, oldConfig);

	//Gyro to (+/- 2000 degrees/sec)
	register_write(MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_2000);

	//console->println("Setting accel sensitivity to +/- 16g ...");
	register_write(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_16);

	// Disable I2C bus (recommended on datasheet)
	register_write(MPU6050_RA_USER_CTRL, BIT_USER_CTRL_I2C_IF_DIS);
	resetFIFO();
	register_write(MPU6050_RA_FIFO_EN, MPU6050_RA_FIFO_EN_VALUE);
	enableFIFO();
	resetFIFO();
	return true; // success
}

uint8_t MPU6000::register_read(uint8_t reg) {
	spi->begin();
	spi->transfer(reg | 0x80);  // Set most significant bit
	uint8_t result = spi->transfer(0);
	spi->end();
	return result;
}

void MPU6000::register_write(uint8_t reg, uint8_t val) {
	spi->begin();
	spi->transfer(reg);
	spi->transfer(val);
	spi->end();
}

uint16_t MPU6000::getFIFOCount() {
	uint8_t fifoCountH = register_read(MPU6050_RA_FIFO_COUNTH);
	uint8_t fifoCountL = register_read(MPU6050_RA_FIFO_COUNTL);
	uint16_t fifoCount = (((uint16_t) (fifoCountH)) << 8) | fifoCountL;
	return fifoCount;
}

void MPU6000::resetFIFO() {
	register_write(MPU6050_RA_USER_CTRL, register_read(MPU6050_RA_USER_CTRL) | _BV(MPU6050_USERCTRL_FIFO_RESET_BIT));
}

void MPU6000::readFIFOBytes(uint16_t fifoCount, uint8_t *fifoBuffer) {
	spi->begin();
	spi->transfer(MPU6050_RA_FIFO_R_W | 0x80);
	for (unsigned short i = 0; i < fifoCount; i++) {
		fifoBuffer[i] = spi->transfer(0);
	}
	spi->end();
}

void MPU6000::enableFIFO() {
	register_write(MPU6050_RA_USER_CTRL, register_read(MPU6050_RA_USER_CTRL) | _BV(MPU6050_USERCTRL_FIFO_EN_BIT));
}

uint8_t MPU6000::getIntStatus() {
	return register_read(MPU6050_RA_INT_STATUS);
}

