/*
 * Serializer.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: helios
 */

#include "Serializer.h"

Serializer::Serializer(Stream *serial) :
		serial(serial) {
	index = 0;
	id = 0;
	checkAnd = 0;
	checkXor = 0;
}
void Serializer::beginn(uint8_t iid) {
	id = iid;
	index = 0;
	checkAnd = 0;
	checkXor = 0;
	serial->write(PREEAMBLE_1);
	serial->write(PREEAMBLE_2);
	serial->write(id);
}
void Serializer::write(uint8_t bytes[], uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		write(bytes[i]);
	}
}

void Serializer::write(uint8_t byte) {
	buffer[index++] = byte;
	checkAnd += byte;
	checkXor ^= byte;
}
void Serializer::end() {
	serial->write(index);
	serial->write(buffer, index);
	serial->write(checkAnd);
	serial->write(checkXor);
	serial->flush();
}
Serializer::~Serializer() {
}

