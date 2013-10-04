/*
 * Persistence.h
 *
 *  Created on: Sep 16, 2013
 *      Author: helios
 */

#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_
#include <stdint.h>

static const uint16_t LOC_MAG_MAX = 0; //4
static const uint16_t LOC_MAG_MIN = 6; //10
static const uint16_t LOC_RC_IN_MAX = 12; //26
static const uint16_t LOC_RC_IN_MIN = 28; //44
static const uint16_t LOC_RC_IN_DEFAULT = 44; //60
static const uint16_t LOC_DECLINATION_ANGLE = 60; //64
static const uint16_t LOC_RC_IN_FUNCTION = 64; //72
static const uint16_t LOC_PID = 72; //84
static const uint16_t LOC_KALMAN = 84; //98

class Persistence {
private:
	union {
		uint16_t uint;
		int16_t sint;
		uint8_t byte[2];
	} conv2;
	union {
		uint32_t uint;
		int32_t sint;
		uint8_t byte[4];
		float floating;
	} conv4;
	void write(uint16_t loc, uint8_t value);
	uint8_t read(uint16_t loc);
	void readSInt(int16_t* data, uint16_t start, uint8_t len);
	void writeSInt(int16_t* data, uint16_t start, uint8_t len);
	void readUInt(uint16_t* data, uint16_t start, uint8_t len);
	void readUInt(uint8_t* data, uint16_t start, uint8_t len);
	void writeUInt(uint16_t* data, uint16_t start, uint8_t len);
	void writeUInt(uint8_t* data, uint16_t start, uint8_t len);

public:
	Persistence() {
	}
	virtual ~Persistence() {
	}
	void saveMagMax(int16_t *xyz);
	void saveMagMin(int16_t *xyz);
	void readMagMax(int16_t *xyz);
	void readMagMin(int16_t *xyz);
	void saveRcMax(uint16_t *max, uint8_t count);
	void saveRcMin(uint16_t *min, uint8_t count);
	void saveRcDefault(uint16_t *def, uint8_t count);
	void readRcMax(uint16_t *max, uint8_t count);
	void readRcMin(uint16_t *min, uint8_t count);
	void readRcDefault(uint16_t *def, uint8_t count);
	void saveDeclinationAngle(float angle);
	float readDeclinationAngle();
	void saveRcInputFunction(uint8_t *function, uint8_t count);
	void readRcInputFunction(uint8_t *function, uint8_t count);
	void savePID(float *rollPitchYaw);
	void readPID(float *rollPitchYaw);

};

#endif /* PERSISTENCE_H_ */
