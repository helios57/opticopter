/*
 * Persistence.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: helios
 */

#include "Persistence.h"
#include "Storage/EEPROMStorage.h"
#include <Arduino.h>

uint8_t Persistence::read(uint16_t loc) {
	return eeprom_read_byte((uint8_t*) loc);
}
void Persistence::write(uint16_t loc, uint8_t value) {
	if (read(loc) != value) {
		eeprom_write_byte((uint8_t*) loc, value);
	}
}
void Persistence::readSInt(int16_t* data, uint16_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		conv2.byte[0] = read(start + i * 2);
		conv2.byte[1] = read(start + i * 2 + 1);
		data[i] = conv2.sint;
	}
}
void Persistence::writeSInt(int16_t* data, uint16_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		conv2.sint = data[i];
		write(start + i * 2, conv2.byte[0]);
		write(start + i * 2 + 1, conv2.byte[1]);
	}
}
void Persistence::readUInt(uint16_t* data, uint16_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		conv2.byte[0] = read(start + i * 2);
		conv2.byte[1] = read(start + i * 2 + 1);
		data[i] = conv2.uint;
	}
}
void Persistence::writeUInt(uint16_t* data, uint16_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		conv2.uint = data[i];
		write(start + i * 2, conv2.byte[0]);
		write(start + i * 2 + 1, conv2.byte[1]);
	}
}
void Persistence::readUInt(uint8_t* data, uint16_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		data[i] = read(start + i);
	}
}
void Persistence::writeUInt(uint8_t* data, uint16_t start, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		write(start + i, data[i]);
	}
}

void Persistence::saveMagMax(int16_t* xyz) {
	writeSInt(xyz, LOC_MAG_MAX, 3);
}

void Persistence::readMagMax(int16_t* xyz) {
	readSInt(xyz, LOC_MAG_MAX, 3);
}

void Persistence::saveMagMin(int16_t* xyz) {
	writeSInt(xyz, LOC_MAG_MIN, 3);
}

void Persistence::readMagMin(int16_t* xyz) {
	readSInt(xyz, LOC_MAG_MIN, 3);
}

void Persistence::saveRcMax(uint16_t* max, uint8_t count) {
	writeUInt(max, LOC_RC_IN_MAX, min(count, 8));
}
void Persistence::readRcMax(uint16_t* max, uint8_t count) {
	readUInt(max, LOC_RC_IN_MAX, min(count, 8));
}

void Persistence::saveRcMin(uint16_t* mini, uint8_t count) {
	writeUInt(mini, LOC_RC_IN_MIN, min(count, 8));
}
void Persistence::readRcMin(uint16_t* mini, uint8_t count) {
	readUInt(mini, LOC_RC_IN_MIN, min(count, 8));
}

void Persistence::saveRcDefault(uint16_t* def, uint8_t count) {
	writeUInt(def, LOC_RC_IN_DEFAULT, min(count, 8));
}
void Persistence::readRcDefault(uint16_t* def, uint8_t count) {
	readUInt(def, LOC_RC_IN_DEFAULT, min(count, 8));
}

void Persistence::saveDeclinationAngle(float angle) {
	conv4.floating = angle;
	writeUInt(conv4.byte, LOC_DECLINATION_ANGLE, 4);
}

float Persistence::readDeclinationAngle() {
	readUInt(conv4.byte, LOC_DECLINATION_ANGLE, 4);
	return conv4.floating;
}

void Persistence::saveRcInputFunction(uint8_t* function, uint8_t count) {
	writeUInt(function, LOC_RC_IN_FUNCTION, min(count, 8));
}

void Persistence::readRcInputFunction(uint8_t* function, uint8_t count) {
	readUInt(function, LOC_RC_IN_FUNCTION, min(count, 8));
}

void Persistence::savePID(float* rollPitchYaw) {
	for (uint8_t i = 0; i < 3; i++) {
		conv4.floating = rollPitchYaw[i];
		writeUInt(conv4.byte, LOC_PID + i * 4, 4);
	}
}

void Persistence::readPID(float* rollPitchYaw) {
	for (uint8_t i = 0; i < 3; i++) {
		readUInt(conv4.byte, LOC_PID + i * 4, 4);
		rollPitchYaw[i] = conv4.floating;
	}
}
