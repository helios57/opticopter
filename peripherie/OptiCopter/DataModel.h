/*
 * DataModel.h
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#ifndef DATAMODEL_H_
#define DATAMODEL_H_

#include "../Libraries/Hal/HalApm.h"
#include "Persistence/Persistence.h"
#include "Filter/AHRS.h"
#include "Regler/PID.h"
#include "Filter/Heading.h"

using namespace arducopterNg;

class DataModel {
private:
	HalApm *hal;
	Persistence *persistence;
	AHRS filter;
	Heading heading;
	int16_t motion[6];
	int16_t mag[3];
	uint16_t input[8];
	uint16_t inputMax[8];
	uint16_t inputMin[8];
	uint16_t inputDefault[8];
	float inputRoll;
	float inputPitch;
	float inputThrust;
	float inputYaw;
	bool leveling;
	uint16_t output[8];
	float thrust[8];
	float magScaled[3];
	int16_t magMax[3];
	int16_t magMin[3];
	int16_t gyroBias[3];
	float rollPitchYawLevel[3]; //Orientation - Level
	float rollPitchYaw[3]; //Orientatio
	int32_t gyroBiasSum[3];
	int32_t gyroBiasSumCount;
	float orientation[4]; //Orientation
	PID rollPitchYawPid[3]; //PID
	float rollPitchYawPidParams[9];
	const static float GYRO_TO_RAD_PER_S_FACTOR = 0.000106413;
	const static float SIN_60_COS_30 = 0.866025403784439;
	float pressure;
	bool active;
	unsigned long tActivate;
	uint16_t activateTop;
	uint16_t activateBot;
	void calculateActivation();
	void calcutateOutput();

public:
	DataModel(HalApm *hal, Persistence *persistence) :
			hal(hal), persistence(persistence) {
		active = false;
		tActivate = 0;
		pressure = 0;
		persistence->readMagMax(magMax);
		persistence->readMagMin(magMin);
		persistence->readRcMin(inputMin, 4);
		persistence->readRcMax(inputMax, 4);
		persistence->readRcDefault(inputDefault, 4);
		inputRoll = 0;
		inputPitch = 0;
		inputThrust = 0;
		inputYaw = 0;
		leveling = false;
		activateTop = inputMax[3] - 100;
		activateBot = inputMin[3] + 100;

		gyroBiasSumCount = 0;
		for (int i = 0; i < 3; i++) {
			gyroBias[i] = 0;
			gyroBiasSum[i] = 0;
			orientation[i + 1] = 0;
		}
		orientation[0] = 1.0f;
	}
	virtual ~DataModel() {
	}
	void putMotion6(int16_t *axyzgxyz);
	void calcOutput10ms();
	void calcMag10ms();
	void putBaro50ms(float altitude);
	void putMag(int16_t* mag);
	void calc10ms();
	void putInput50ms(uint8_t ch, uint16_t pwm);
	void calc50ms();
	void initPID(float *rollPitchYawPidParams);

};
#endif /* DATAMODEL_H_ */

