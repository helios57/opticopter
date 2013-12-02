/*
 * DigitalSource.h
 *
 *  Created on: Jun 16, 2013
 *      Author: helios
 */

#ifndef DIGITALSOURCE_H_
#define DIGITALSOURCE_H_

class DigitalSource {
public:
	const unsigned char _bit;
	const unsigned char port;
	DigitalSource(const unsigned char bit, const unsigned char port) :
			_bit(bit), port(port) {
	}
	virtual ~DigitalSource();
	void mode(unsigned char output);
	unsigned char read();
	void write(unsigned char value);
};

#endif /* DIGITALSOURCE_H_ */
