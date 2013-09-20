/*
 * DebugStream.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: helios
 */

#include "DebugStream.h"

void DebugStream::println(const String &str) {
	serializer->beginn(Serializer::ID_DEBUG);
	for (uint16_t i = 0; i < str.length(); i++) {
		serializer->write(str[i]);
	}
	serializer->end();
}

DebugStream::DebugStream(Serializer *serializer) :
		serializer(serializer) {

}

DebugStream::~DebugStream() {
}

