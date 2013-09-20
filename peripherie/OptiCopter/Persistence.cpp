/*
 * Persistence.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: helios
 */

#include "Persistence.h"
#include "Storage/EEPROMStorage.h"

void Persistence::saveMagMax(uint16_t* xyz) {
	for (uint16_t i = LOC_MAG_MAX; i < LOC_MAG_MAX + 2 * 3; i++) {

	}
}

void Persistence::saveMagMin(uint16_t* xyz) {
}

void Persistence::readMagMax(uint16_t* xyz) {
}

void Persistence::readMagMin(uint16_t* xyz) {
}

void Persistence::saveRcMax(uint8_t ch, uint16_t max) {
}

void Persistence::saveRcMin(uint8_t ch, uint16_t min) {
}

uint16_t Persistence::readRcMax(uint8_t ch) {
	return 0;
}

uint16_t Persistence::readRcMin(uint8_t ch) {
	return 0;
}
