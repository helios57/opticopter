/*
 * PhoneDrohne.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: helios
 */

#include "PhoneDrohne.h"
#include <AndroidAccessory.h>
#include <Max3421e.h>
#include <Usb.h>

namespace phonedrohne {

	PhoneDrohne::PhoneDrohne() {
		timer = millis();
		len = 0;
		bytes_sent = 0;
		adk = new AndroidAccessory("Sharpsoft", "PhoneDrone", "Phone Drone ADK by 3DRobotics", "1.0", "http://www.android.com", "0000000012345678");
	}

	PhoneDrohne::~PhoneDrohne() {
		delete adk;
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
		adk->powerOn();
		Serial.begin(115200);
	}

	void PhoneDrohne::loop() {
		if (adk->isConnected()) {
			int recieved = adk->read(buf, sizeof(buf), 1);
			Serial.write((uint8_t*) buf, recieved);
			if (Serial.available() > 0) {
				int count = Serial.readBytes(buf, 64);
				adk->write(buf, count);
			}
		}
	}
} /* namespace phonedrohne */
