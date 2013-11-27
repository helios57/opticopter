/*
 * EEPROMStorage.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#include "EEPROMStorage.h"

EEPROMStorage::EEPROMStorage() {
}

EEPROMStorage::~EEPROMStorage() {
}

uint8_t EEPROMStorage::read_byte(uint16_t loc) {
	return eeprom_read_byte((uint8_t*) loc);
}

uint16_t EEPROMStorage::read_word(uint16_t loc) {
	return eeprom_read_word((uint16_t*) loc);
}

uint32_t EEPROMStorage::read_dword(uint16_t loc) {
	return eeprom_read_dword((uint32_t*) loc);
}

void EEPROMStorage::read_block(void *dst, uint16_t src, size_t n) {
	eeprom_read_block(dst, (const void*) src, n);
}

void EEPROMStorage::write_byte(uint16_t loc, uint8_t value) {
	eeprom_write_byte((uint8_t*) loc, value);
}

void EEPROMStorage::write_word(uint16_t loc, uint16_t value) {
	eeprom_write_word((uint16_t*) loc, value);
}

void EEPROMStorage::write_dword(uint16_t loc, uint32_t value) {
	eeprom_write_dword((uint32_t*) loc, value);
}

void EEPROMStorage::write_block(uint16_t dst, void *src, size_t n) {
	eeprom_write_block(src, (void*) dst, n);
}
