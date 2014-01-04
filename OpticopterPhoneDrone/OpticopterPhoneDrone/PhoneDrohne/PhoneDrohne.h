/*
 * PhoneDrohne.h
 *
 *  Created on: Jul 24, 2013
 *      Author: helios
 */

#ifndef PHONEDROHNE_H_
#define PHONEDROHNE_H_

#include <AndroidAccessory.h>
#include <Arduino.h>

class PhoneDrohne {

private:
	uint8_t buf[16];
	AndroidAccessory* adk;

	Stream* serialArduino;
	Stream* serialUSB;

	union {
		uint8_t byte[4];
		float floating;
	} conv4;

	unsigned long t_1000ms;
public:
	PhoneDrohne();
	virtual ~PhoneDrohne();
	int main();
	void setup();
	void loop();
};
#endif /* PHONEDROHNE_H_ */
