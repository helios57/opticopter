/*
 * ArducopterNg.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: helios
 */

#include "OptiCopter.h"
#include "../Arduino/Arduino.h"
#include "../Libraries/Hal/HalApm.h"

namespace arducopterNg {

	ArducopterNg::ArducopterNg() {
		emptyCycles = 0;
		hal = 0;
		serializer = 0;
		debug = 0;
		dm = 0;
		t_1000ms = 0;
		t_5ms = 0;
		t_10ms = 0;
		t_20ms = 0;
		sendData = true;
		newData = false;
	}

	ArducopterNg::~ArducopterNg() {
		delete hal;
		delete serializer;
		delete debug;
		delete dm;
	}

	int ArducopterNg::main() {
		this->setup();
		for (;;) {
			this->loop();
			if (serialEventRun)
				serialEventRun();
		}
		return 0;
	}

	void ArducopterNg::setup() {
		Serial.begin(115200);
		Serial1.begin(38400);
		Serial.flush();
		Serial1.flush();
		serializer = new Serializer(&Serial);
		debug = new DebugStream(serializer);
		hal = new HalApm(&Serial, debug);
		dm = new DataModel(hal);
		t_1000ms = millis();
		t_5ms = millis();
		t_10ms = millis();
		t_20ms = millis();
	}

	void ArducopterNg::sendAccel() {
		hal->getAccel(buffer);
		serializer->beginn(Serializer::ID_ACCEL);
		conv4.sint = buffer[0];
		serializer->write(conv4.byte, 4);
		conv4.sint = buffer[1];
		serializer->write(conv4.byte, 4);
		conv4.sint = buffer[2];
		serializer->write(conv4.byte, 4);
		serializer->end();
	}

	void ArducopterNg::sendGyro() {
		hal->getGyro(buffer);
		serializer->beginn(Serializer::ID_GYRO);
		conv4.sint = buffer[0];
		serializer->write(conv4.byte, 4);
		conv4.sint = buffer[1];
		serializer->write(conv4.byte, 4);
		conv4.sint = buffer[2];
		serializer->write(conv4.byte, 4);
		serializer->end();
	}

	void ArducopterNg::sendQuat() {
		hal->getQuat(buffer);
		serializer->beginn(Serializer::ID_QUAT);
		conv4.sint = buffer[0];
		serializer->write(conv4.byte, 4);
		conv4.sint = buffer[1];
		serializer->write(conv4.byte, 4);
		conv4.sint = buffer[2];
		serializer->write(conv4.byte, 4);
		conv4.sint = buffer[3];
		serializer->write(conv4.byte, 4);
		serializer->end();
	}

	void ArducopterNg::sendBaro() {
		conv4.floating = hal->getPressure();
		serializer->beginn(Serializer::ID_BARO);
		serializer->write(conv4.byte, 4);
		serializer->end();
	}

	void ArducopterNg::sendMag() {
		int16_t buffer[3];
		hal->getHeading(buffer);
		serializer->beginn(Serializer::ID_MAG);
		conv2.sint = buffer[0];
		serializer->write(conv2.byte, 2);
		conv2.sint = buffer[1];
		serializer->write(conv2.byte, 2);
		conv2.sint = buffer[2];
		serializer->write(conv2.byte, 2);
		serializer->end();
	}

	void ArducopterNg::sendGPS() {
		serializer->beginn(Serializer::ID_GPS);
		serializer->write(hal->getGPSDataBuffer(), 32);
		serializer->end();
	}

	void ArducopterNg::resetEmptyCycles() {
		serializer->beginn(Serializer::ID_CYCLES);
		conv8.ulong = emptyCycles;
		serializer->write(conv8.byte, 8);
		serializer->end();
		emptyCycles = 0;
	}

	void ArducopterNg::sendInput() {
		serializer->beginn(serializer->ID_INPUT);
		serializer->write(hal->IN7 - hal->IN0);
		for (uint8_t i = hal->IN0; i < hal->IN7; i++) {
			conv2.uint = hal->getPmw(i);
			serializer->write(i);
			serializer->write(conv2.byte, 2);
		}
		serializer->end();
	}

	void ArducopterNg::loop() {
		emptyCycles++;
		//uint16_t thrust = hal->getPmw(hal->IN0);
		//hal->setPmw(hal->OUT0, thrust);
		//hal->setPmw(hal->OUT1, thrust);
		//hal->setPmw(hal->OUT2, thrust);
		//hal->setPmw(hal->OUT3, thrust);

		if ((millis() - t_5ms) >= 5) {
			t_5ms = millis();
			resetEmptyCycles();
			if (hal->pollAccel()) {
				newData = true;
				if (sendData) {
					sendAccel();
					sendGyro();
				}
				//sendQuat();
			}
		}

		if ((millis() - t_10ms) >= 10) {
			t_10ms = millis();
			if (hal->pollBaro()) {
				newData = true;
				if (sendData) {
					sendBaro();
				}
			}
			if (hal->pollMag()) {
				newData = true;
				if (sendData) {
					sendMag();
				}
			}
		}
		if ((millis() - t_20ms) >= 20) {
			t_20ms = millis();
			if (hal->pollGPS()) {
				newData = true;
				if (sendData) {
					sendGPS();
				}
			}
		}
		if (newData && !sendData) {
			dm->calculate();

		}
		//1hz
		if ((millis() - t_1000ms) >= 100) {
			t_1000ms = millis();
			sendInput();
		}
	}
} /* namespace arducopterNg */
