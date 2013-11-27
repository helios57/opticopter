/*
 * EEPROMStorage.h
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#ifndef EEPROMSTORAGE_H_
#define EEPROMSTORAGE_H_

#include <avr/io.h>
#include <avr/eeprom.h>

class EEPROMStorage {
public:
	EEPROMStorage();
	virtual ~EEPROMStorage();
	uint8_t read_byte(uint16_t loc);
	uint16_t read_word(uint16_t loc);
	uint32_t read_dword(uint16_t loc);
	void read_block(void *dst, uint16_t src, size_t n);

	void write_byte(uint16_t loc, uint8_t value);
	void write_word(uint16_t loc, uint16_t value);
	void write_dword(uint16_t loc, uint32_t value);
	void write_block(uint16_t dst, void* src, size_t n);
};

#endif /* EEPROMSTORAGE_H_ */
