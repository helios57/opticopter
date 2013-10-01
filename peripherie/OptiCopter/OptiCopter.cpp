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
		t_sendData = 0;
		sendData = false;
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
		for (uint8_t i = hal->IN0; i < hal->IN7; i++) {
			conv2.uint = hal->getPmw(i);
			serializer->write(i);
			serializer->write(conv2.byte, 2);
		}
		serializer->end();
	}

	void ArducopterNg::loop() {
		emptyCycles++;
		uint8_t id = serializer->read(commandBuffer);
		if (id > 0) {
			switch (id) {
			case Serializer::ID_IN_SEND_DATA:
				sendData = true;
				t_sendData = millis();
				break;
			case Serializer::ID_IN_SEND_DATA:
				sendData = true;
				for (uint8_t i = 0; i < 8; i++) {
					conv8.byte[i] = commandBuffer[i];
				}
				t_sendData = millis() + conv8.ulong;
				break;
			case Serializer::ID_IN_READ_PARAM:
				uint8_t id_param = commandBuffer[0];
				serializer->beginn(Serializer::ID_PARAM);
				serializer->write(id_param);
				switch (id_param) {
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
					for (uint8_t i = 0; i < 3; i++) {
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
				break;

			case Serializer::ID_IN_WRITE_PARAM:
				uint8_t id_param = commandBuffer[0];
				switch (id_param) {
				case Serializer::ID_IN_PARAM_MAG_MAX:
					for (uint8_t i = 0; i < 3; i++) {
						conv2.byte[0] = commandBuffer[1 + i * 2];
						conv2.byte[1] = commandBuffer[1 + i * 2 + 1];
						buffer_16t[i] = conv2.sint;
					}
					persistence->writeMagMax(buffer_16t);
					break;
				case Serializer::ID_IN_PARAM_MAG_MIN:
					for (uint8_t i = 0; i < 3; i++) {
						conv2.byte[0] = commandBuffer[1 + i * 2];
						conv2.byte[1] = commandBuffer[1 + i * 2 + 1];
						buffer_16t[i] = conv2.sint;
					}
					persistence->writeMagMin(buffer_16t);
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
					persistence->readRcInputFunction(buffer_u8t, 8);
					break;
				case Serializer::ID_IN_PARAM_DECLINATION_ANGLE:
					conv4.floating = persistence->readDeclinationAngle();
					serializer->write(conv4.byte, 4);
					break;
				case Serializer::ID_IN_PARAM_PID:
					persistence->readPID(buffer_float);
					for (uint8_t i = 0; i < 3; i++) {
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
				break;
			default:
				break;
			}

			if (sendData && (millis() - t_sendData) >= 0) {
				sendData = false;
			}

			if ((millis() - t_5ms) >= 5) {
				t_5ms = millis();
				resetEmptyCycles();
				if (hal->pollAccel()) {
					newData = true;
					if (sendData) {
						sendAccel();
						sendGyro();
						//sendQuat();
					}
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
			if ((millis() - t_1000ms) >= 50) {
				t_1000ms = millis();
				if (sendData) {
					sendInput();
				}
			}
		}
	} /* namespace arducopterNg */
