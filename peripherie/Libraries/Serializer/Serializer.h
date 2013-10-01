/*
 * Serializer.h
 *
 *  Created on: Aug 29, 2013
 *      Author: helios
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_
#include <Stream.h>

class Serializer {
private:
	Stream *serial;
	uint8_t buffer[254];
	uint8_t id;
	uint8_t index;
	uint8_t checkAnd;
	uint8_t checkXor;
public:
	static const uint8_t PREEAMBLE_1 = 0xE5;
	static const uint8_t PREEAMBLE_2 = 0xE7;

	//TODO add Out Prefix
	//OUT-ID's
	static const uint8_t ID_ACCEL = 0x01;
	static const uint8_t ID_GYRO = 0x02;
	static const uint8_t ID_BARO = 0x03;
	static const uint8_t ID_QUAT = 0x04;
	static const uint8_t ID_MAG = 0x05;
	static const uint8_t ID_GPS = 0x06;
	static const uint8_t ID_INPUT = 0x07;
	static const uint8_t ID_OUTPUT = 0x08;
	static const uint8_t ID_DEBUG = 0x09;
	static const uint8_t ID_CYCLES = 0x0A;
	static const uint8_t ID_PARAM = 0x0B;

	//IN-ID's
	static const uint8_t ID_IN_SEND_DATA = 0x01;
	static const uint8_t ID_IN_OUTPUT = 0x02;
	static const uint8_t ID_IN_READ_PARAM = 0x02;

	//PARAM SUB_PARAM
	static const uint8_t ID_IN_PARAM_MAG_MAX = 0x01;
	static const uint8_t ID_IN_PARAM_MAG_MIN = 0x02;
	static const uint8_t ID_IN_PARAM_RC_IN_MAX = 0x03;
	static const uint8_t ID_IN_PARAM_RC_IN_MIN = 0x04;
	static const uint8_t ID_IN_PARAM_RC_IN_DEFAULT = 0x05;
	static const uint8_t ID_IN_PARAM_RC_IN_FUNCTION = 0x06;
	static const uint8_t ID_IN_PARAM_DECLINATION_ANGLE = 0x07;
	static const uint8_t ID_IN_PARAM_PID = 0x08;
	static const uint8_t ID_IN_PARAM_KALMAN = 0x09;

	static const uint8_t ID_IN_READ_PARAM_ = 0x01;
	static const uint8_t ID_IN_WRITE_PARAM = 0x03;

	Serializer(Stream *serial);
	virtual ~Serializer();
	void beginn(uint8_t id);
	void write(uint8_t bytes[], uint8_t len);
	void write(uint8_t byte);
	void end();
	uint8_t read(uint8_t *buffer);
};

#endif /* SERIALIZER_H_ */
