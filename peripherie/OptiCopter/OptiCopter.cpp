/*
 * ArducopterNg.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: helios
 */

#include "OptiCopter.h"
#include "../Arduino/Arduino.h"
#include "../Libraries/Hal/HalApm.h"

using namespace opticopter;

namespace opticopter {

	OptiCopter::OptiCopter() {
		persistence = 0;
		hal = 0;
		serializer = 0;
		debug = 0;
		dm = 0;
		logging = 0;
		t_10ms = 0;
		t_20ms = 0;
		t_50ms = 0;
		lastMicros = 0;
		t_sendData = 0;
		sendData = false;
	}

	OptiCopter::~OptiCopter() {
		delete hal;
		delete serializer;
		delete debug;
		delete dm;
		delete persistence;
		delete logging;
	}

	int OptiCopter::main() {
		this->setup();
		for (;;) {
			this->loop();
			if (serialEventRun)
				serialEventRun();
		}
		return 0;
	}

	void OptiCopter::setup() {
		Serial.begin(115200);
		Serial1.begin(38400);
		Serial.flush();
		Serial1.flush();
		persistence = new Persistence();
		serializer = new Serializer(&Serial);
		debug = new DebugStream(serializer);
		hal = new HalApm(&Serial, debug);
		logging = new Logging(hal, serializer);
		dm = new DataModel(hal, persistence, logging);
		t_10ms = millis();
		t_20ms = millis();
		t_50ms = millis();
		lastMicros = micros();
	}

	void OptiCopter::sendMotion6() {
		serializer->beginn(Serializer::ID_MOTION6);
		for (uint8_t i = 0; i < 6; i++) {
			conv2.sint = axyzgxyz[i];
			serializer->write(conv2.byte, 2);
		}
		serializer->end();
	}

	void OptiCopter::sendBaro() {
		conv4.floating = hal->getBarometerAltitude();
		serializer->beginn(Serializer::ID_BARO);
		serializer->write(conv4.byte, 4);
		serializer->end();
	}

	void OptiCopter::sendMag() {
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

	void OptiCopter::sendGPS() {
		serializer->beginn(Serializer::ID_GPS);
		serializer->write(hal->getGPSDataBuffer(), 32);
		serializer->end();
	}

	void OptiCopter::sendInput() {
		serializer->beginn(serializer->ID_INPUT);
		serializer->write(hal->IN7 - hal->IN0);
		for (uint8_t i = hal->IN0; i <= hal->IN7; i++) {
			conv2.uint = hal->getPmw(i);
			serializer->write(i);
			serializer->write(conv2.byte, 2);
		}
		serializer->end();
	}

	void OptiCopter::handleInSendData() {
		sendData = true;
		for (uint8_t i = 0; i < 8; i++) {
			conv8.byte[i] = commandBuffer[i];
		}
		t_sendData = millis() + conv8.ulong;
	}

	void OptiCopter::handleInReadParam() {
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

	void OptiCopter::handleInWriteData() {
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

	void OptiCopter::handleIn(uint8_t id) {
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

	void OptiCopter::sendLoggedData() {
		Serial.read();
		Serial.println("timestamp,accelX,accelY,accelZ,gyroX,gyroY,gyroZ,magX,magY,magZ,roll,pitch,yaw,rollLevel,pitchLevel,yawLevel,rollPid,pitchPid,yawPid,output0,output1,output2,output3");
		logging->setPos(0);
		while (logging->getNext()) {
			Serial.print(logging->entry.timestamp);
			Serial.print(",");
			Serial.print(logging->entry.accelX);
			Serial.print(",");
			Serial.print(logging->entry.accelY);
			Serial.print(",");
			Serial.print(logging->entry.accelZ);
			Serial.print(",");
			Serial.print(logging->entry.gyroX * 1000);
			Serial.print(",");
			Serial.print(logging->entry.gyroY * 1000);
			Serial.print(",");
			Serial.print(logging->entry.gyroZ * 1000);
			Serial.print(",");
			Serial.print(logging->entry.magX * 1000);
			Serial.print(",");
			Serial.print(logging->entry.magY * 1000);
			Serial.print(",");
			Serial.print(logging->entry.magZ * 1000);
			Serial.print(",");
			Serial.print(logging->entry.roll * 1000);
			Serial.print(",");
			Serial.print(logging->entry.pitch * 1000);
			Serial.print(",");
			Serial.print(logging->entry.yaw * 1000);
			Serial.print(",");
			Serial.print(logging->entry.rollLevel * 1000);
			Serial.print(",");
			Serial.print(logging->entry.pitchLevel * 1000);
			Serial.print(",");
			Serial.print(logging->entry.yawLevel * 1000);
			Serial.print(",");
			Serial.print(logging->entry.rollPid * 1000);
			Serial.print(",");
			Serial.print(logging->entry.pitchPid * 1000);
			Serial.print(",");
			Serial.print(logging->entry.yawPid * 1000);
			Serial.print(",");
			Serial.print(logging->entry.output0);
			Serial.print(",");
			Serial.print(logging->entry.output1);
			Serial.print(",");
			Serial.print(logging->entry.output2);
			Serial.print(",");
			Serial.println(logging->entry.output3);
		}
	}

	void OptiCopter::loop() {
		uint8_t id = serializer->read(commandBuffer);
		if (id > 0) {
			handleIn(id);
		}
		if (sendData && millis() > t_sendData) {
			sendData = false;
		}

		if (hal->pollMotion()) {
			unsigned long currentMicros = micros();
			unsigned long microsDiff = currentMicros - lastMicros;
			lastMicros = currentMicros;
			hal->getMotion6(axyzgxyz);
			dm->putMotion6(axyzgxyz);
			dm->calc(microsDiff / 1000000.0);
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

			if (sendData) {
				sendMotion6();
			}
			hal->getHeading(buffer_16t);
			dm->putMag(buffer_16t);

			dm->calculateActivation();
			if ((millis() - t_10ms) < 10) {
				//dm->debugLog();
			}
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
