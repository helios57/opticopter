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
		persistence = 0;
		emptyCycles = 0;
		hal = 0;
		serializer = 0;
		debug = 0;
		dm = 0;
		t_2ms = 0;
		t_10ms = 0;
		t_20ms = 0;
		t_50ms = 0;
		t_sendData = 0;
		sendData = false;
	}

	ArducopterNg::~ArducopterNg() {
		delete hal;
		delete serializer;
		delete debug;
		delete dm;
		delete persistence;
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
		persistence = new Persistence();
		serializer = new Serializer(&Serial);
		debug = new DebugStream(serializer);
		hal = new HalApm(&Serial, debug);
		dm = new DataModel(hal, persistence);
		t_10ms = millis();
		t_20ms = millis();
		t_50ms = millis();
	}

	void ArducopterNg::sendMotion6() {
		serializer->beginn(Serializer::ID_MOTION6);
		for (uint8_t i = 0; i < 6; i++) {
			conv2.sint = axyzgxyz[i];
			serializer->write(conv2.byte, 2);
		}
		serializer->end();
	}

	void ArducopterNg::sendBaro() {
		conv4.floating = hal->getBarometerAltitude();
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
		for (uint8_t i = hal->IN0; i <= hal->IN7; i++) {
			conv2.uint = hal->getPmw(i);
			serializer->write(i);
			serializer->write(conv2.byte, 2);
		}
		serializer->end();
	}

	void ArducopterNg::handleInSendData() {
		sendData = true;
		for (uint8_t i = 0; i < 8; i++) {
			conv8.byte[i] = commandBuffer[i];
		}
		t_sendData = millis() + conv8.ulong;
	}

	void ArducopterNg::handleInReadParam() {
		serializer->beginn(Serializer::ID_PARAM);
		serializer->write(commandBuffer[0]);
		switch (commandBuffer[0]) {
		case Serializer::ID_IN_PARAM_MAG_MAX:
			persistence->readMagMax(buffer_16t);
			for (uint8_t i = 0; i < 3; i++) {
				conv2.sint = buffer_16t[i];
				serializer->write(conv2.byte[0]);
				serializer->write(conv2.byte[1]);
			}
			break;
		case Serializer::ID_IN_PARAM_MAG_MIN:
			persistence->readMagMin(buffer_16t);
			for (uint8_t i = 0; i < 3; i++) {
				conv2.sint = buffer_16t[i];
				serializer->write(conv2.byte[0]);
				serializer->write(conv2.byte[1]);
			}
			break;
		case Serializer::ID_IN_PARAM_RC_IN_MAX:
			persistence->readRcMax(buffer_u16t, 8);
			for (uint8_t i = 0; i < 8; i++) {
				conv2.uint = buffer_u16t[i];
				serializer->write(conv2.byte[0]);
				serializer->write(conv2.byte[1]);
			}
			break;
		case Serializer::ID_IN_PARAM_RC_IN_MIN:
			persistence->readRcMin(buffer_u16t, 8);
			for (uint8_t i = 0; i < 8; i++) {
				conv2.uint = buffer_u16t[i];
				serializer->write(conv2.byte[0]);
				serializer->write(conv2.byte[1]);
			}
			break;
		case Serializer::ID_IN_PARAM_RC_IN_DEFAULT:
			persistence->readRcDefault(buffer_u16t, 8);
			for (uint8_t i = 0; i < 8; i++) {
				conv2.uint = buffer_u16t[i];
				serializer->write(conv2.byte[0]);
				serializer->write(conv2.byte[1]);
			}
			break;
		case Serializer::ID_IN_PARAM_RC_IN_FUNCTION:
			persistence->readRcInputFunction(buffer_u8t, 8);
			serializer->write(buffer_u8t, 8);
			break;
		case Serializer::ID_IN_PARAM_DECLINATION_ANGLE:
			conv4.floating = persistence->readDeclinationAngle();
			serializer->write(conv4.byte, 4);
			break;
		case Serializer::ID_IN_PARAM_PID:
			persistence->readPID(buffer_float);
			for (uint8_t i = 0; i < 9; i++) {
				conv4.floating = buffer_float[i];
				serializer->write(conv4.byte, 4);
			}
			break;
		case Serializer::ID_IN_PARAM_KALMAN:
			break;
		default:
			break;
		}
		serializer->end();
	}

	void ArducopterNg::handleInWriteData() {
		switch (commandBuffer[0]) {
		case Serializer::ID_IN_PARAM_MAG_MAX:
			for (uint8_t i = 0; i < 3; i++) {
				conv2.byte[0] = commandBuffer[1 + i * 2];
				conv2.byte[1] = commandBuffer[1 + i * 2 + 1];
				buffer_16t[i] = conv2.sint;
			}
			persistence->saveMagMax(buffer_16t);
			break;
		case Serializer::ID_IN_PARAM_MAG_MIN:
			for (uint8_t i = 0; i < 3; i++) {
				conv2.byte[0] = commandBuffer[1 + i * 2];
				conv2.byte[1] = commandBuffer[1 + i * 2 + 1];
				buffer_16t[i] = conv2.sint;
			}
			persistence->saveMagMin(buffer_16t);
			break;
		case Serializer::ID_IN_PARAM_RC_IN_MAX:
			for (uint8_t i = 0; i < 8; i++) {
				conv2.byte[0] = commandBuffer[1 + i * 2];
				conv2.byte[1] = commandBuffer[1 + i * 2 + 1];
				buffer_u16t[i] = conv2.uint;
			}
			persistence->saveRcMax(buffer_u16t, 8);
			break;
		case Serializer::ID_IN_PARAM_RC_IN_MIN:
			for (uint8_t i = 0; i < 8; i++) {
				conv2.byte[0] = commandBuffer[1 + i * 2];
				conv2.byte[1] = commandBuffer[1 + i * 2 + 1];
				buffer_u16t[i] = conv2.uint;
			}
			persistence->saveRcMin(buffer_u16t, 8);
			break;
		case Serializer::ID_IN_PARAM_RC_IN_DEFAULT:
			for (uint8_t i = 0; i < 8; i++) {
				conv2.byte[0] = commandBuffer[1 + i * 2];
				conv2.byte[1] = commandBuffer[1 + i * 2 + 1];
				buffer_u16t[i] = conv2.uint;
			}
			persistence->saveRcDefault(buffer_u16t, 8);
			break;
		case Serializer::ID_IN_PARAM_RC_IN_FUNCTION:
			for (uint8_t i = 0; i < 8; i++) {
				buffer_u8t[i] = commandBuffer[1 + i];
			}
			persistence->saveRcInputFunction(buffer_u8t, 8);
			break;
		case Serializer::ID_IN_PARAM_DECLINATION_ANGLE:
			for (uint8_t i = 0; i < 4; i++) {
				conv4.byte[i] = commandBuffer[1 + i];
			}
			persistence->saveDeclinationAngle(conv4.floating);
			break;
		case Serializer::ID_IN_PARAM_PID:
			for (uint8_t i = 0; i < 9; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					conv4.byte[j] = commandBuffer[1 + i * 4 + j];
				}
				buffer_float[i] = conv4.floating;
			}
			persistence->savePID(buffer_float);
			dm->initPID(buffer_float);
			break;
		case Serializer::ID_IN_PARAM_KALMAN:
			break;
		default:
			break;
		}
		serializer->end();
	}

	void ArducopterNg::handleIn(uint8_t id) {
		switch (id) {
		case Serializer::ID_IN_SEND_DATA:
			handleInSendData();
			break;
		case Serializer::ID_IN_READ_PARAM:
			handleInReadParam();
			break;
		case Serializer::ID_IN_WRITE_PARAM:
			handleInWriteData();
			break;
		default:
			break;
		}
	}

	void ArducopterNg::loop() {
		emptyCycles++;
		uint8_t id = serializer->read(commandBuffer);

		if (id > 0) {
			handleIn(id);
		}

		if (sendData && millis() > t_sendData) {
			sendData = false;
		}

		if ((millis() - t_2ms) >= 2) {
			t_2ms = millis();
			hal->pollMotion();
		}

		if ((millis() - t_10ms) >= 10) {
			t_10ms = millis();
			if (hal->pollBaro()) {
				if (sendData) {
					sendBaro();
				}
			}
			if (hal->pollMag()) {
				if (sendData) {
					sendMag();
				}
			}
			//dm->putBaro50ms(hal->getBarometerAltitude());
			hal->getMotion6(axyzgxyz);
			dm->putMotion6(axyzgxyz);
			if (sendData) {
				sendMotion6();
			}
			int16_t buffer[3];
			hal->getHeading(buffer);
			dm->putMag(buffer);
			dm->calc10ms();
		}

		if ((millis() - t_20ms) >= 20) {
			t_20ms = millis();
			if (hal->pollGPS()) {
				if (sendData) {
					sendGPS();
				}
			} //dm->putGPS()...
		}

		if ((millis() - t_50ms) >= 50) {
			t_50ms = millis();
			if (sendData) {
				sendInput();
			}
			for (uint8_t i = hal->IN0; i <= hal->IN7; i++) {
				dm->putInput50ms(i, hal->getPmw(i));
			}
			dm->calc50ms();
		}
	}
} /* namespace arducopterNg */
