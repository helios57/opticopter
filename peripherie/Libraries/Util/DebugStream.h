/*
 * DebugStream.h
 *
 *  Created on: Aug 31, 2013
 *      Author: helios
 */

#ifndef DEBUGSTREAM_H_
#define DEBUGSTREAM_H_
#include "Serializer/Serializer.h"
class DebugStream {
private:
	Serializer *serializer;
public:
	DebugStream(Serializer *serializer);
	void println(const String &str);
	void println(const double &fl);
	virtual ~DebugStream();
};

#endif /* DEBUGSTREAM_H_ */
