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
	Serial.flush();
	Serial1.begin(115200);
	Serial1.flush();
	adk->powerOn();
}

void PhoneDrohne::loop() {
	if (adk->isConnected()) {
		int recievedFromAndroid = adk->read(buf, 16, 1);
		if (recievedFromAndroid > 0) {
			serialUSB->print("Android to Arduino ");

			for (int i = 0; i < 16; i++) {
				serialUSB->print(buf[i], HEX);
				serialUSB->print(" ");
			}
			serialUSB->println();

			serialArduino->write((uint8_t*) buf, recievedFromAndroid);
		}
	}

	if (serialArduino->available() >= 16) {
		serialUSB->print("Arduino to Android ");
		bool valid = false;
		for (int i = 0; i < 16; i++) {
			uint8_t pre0 = (uint8_t) serialArduino->read();
			if (pre0 != preamble0) {
				continue;
			}
			uint8_t pre1 = (uint8_t) serialArduino->read();
			if (pre0 == preamble0 && pre1 == preamble1) {
				valid = true;
				buf[0] = pre0;
				buf[1] = pre1;
				break;
			}
		}
		if (valid) {
			for (int i = 2; i < 16; i++) {
				buf[i] = (uint8_t) serialArduino->read();
			}

			for (int i = 0; i < 16; i++) {
				serialUSB->print(buf[i], HEX);
				serialUSB->print(" ");
			}
			serialUSB->println();

			if (adk->isConnected()) {
				adk->write(buf, 16);
			}
		}
	}
}
