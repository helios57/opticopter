/*
 * DebugStream.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: helios
 */

#include "DebugStream.h"
#include <stdlib.h>

void DebugStream::println(const String &str) {
	serializer->beginn(Serializer::ID_DEBUG);
	for (uint16_t i = 0; i < str.length(); i++) {
		serializer->write(str[i]);
	}
	serializer->end();
}
void DebugStream::println(const double &fl) {
	char ascii[10];
	dtostrf(fl, 7, 2, ascii);
	println(ascii);
}

DebugStream::DebugStream(Serializer *serializer) :
		serializer(serializer) {

}

DebugStream::~DebugStream() {
}

