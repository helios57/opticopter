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

uint8_t Serializer::read(uint8_t *data) {
	if (serial->available()) {
		uint8_t read = serial->read();
		if (PREEAMBLE_1 == read) {
			read = serial->read();
			if (PREEAMBLE_2 == read) {
				uint8_t id = serial->read();
				if (id == 0) {
					return 0;
				}
				uint8_t length = serial->read();
				int i = 0;
				for (; i < (length + 2); i++) {
					data[i] = serial->read();
				}
				if (i < (length + 2)) {
					return 0;
				}
				uint8_t and_ = 0;
				uint8_t xor_ = 0;
				for (i = 0; i < length; i++) {
					and_ += data[i];
					xor_ ^= data[i];
				}
				if (and_ == data[length] && xor_ == data[length + 1]) {
					return id;
				}
			}
		}
	}
	return 0;
}

Serializer::~Serializer() {

}
