/*
 * PhoneDrohne.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: helios
 */

#include "PhoneDrohne.h"

void _delay(unsigned long ms) {
	unsigned long start = millis();
	while (start + ms > millis())
		;
}

PhoneDrohne::PhoneDrohne() {
	serialUSB = &Serial;
	serialArduino = &Serial1;
	t_1000ms = 0;
	adk = new AndroidAccessory("Sharpsoft", "PhoneDrone", "Phone Drone ADK by 3DRobotics", "1.0", "http://www.android.com", "0000000012345678");
}

PhoneDrohne::~PhoneDrohne() {
}

int PhoneDrohne::main() {
	this->setup();
	for (;;) {
		this->loop();
		if (serialEventRun)
			serialEventRun();
	}
	return 0;
}
void PhoneDrohne::setup() {
	Serial.begin(115200);
	Serial1.begin(115200);
	adk->powerOn();
}

void PhoneDrohne::loop() {
	if (adk->isConnected()) {
		int recievedFromAndroid = adk->read(buf, 16, 1);
		if (recievedFromAndroid > 0) {
			serialArduino->write((uint8_t*) buf, recievedFromAndroid);
		}
	}

	if (serialArduino->available() > 0) {
		for (int i = 0; i < 16; i++) {
			buf[i] = serialArduino->read();
		}
		if (adk->isConnected()) {
			adk->write(buf, 16);
		}
	}
}
