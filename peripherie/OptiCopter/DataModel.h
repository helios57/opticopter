/*
 * DataModel.h
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#ifndef DATAMODEL_H_
#define DATAMODEL_H_

#include "../Libraries/Hal/HalApm.h"
#include "Regler/PID.h"
#include "Persistence/Persistence.h"
#include "Filter/Gyro.h"
#include "Filter/AHRS.h"

using namespace arducopterNg;

class DataModel {
private:
	HalApm *hal;
	Persistence *persistence;
	AHRS ahrs;
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
	float rollPitchYawLevel[3]; //Orientation - Level
	float rollPitchYaw[3]; //Orientation
	Gyro gyro;
	PID rollPitchYawPid[3]; //PID
	float rollPitchYawPidParams[9];
	const static float SIN_60_COS_30 = 0.866025403784439;
	float pressure;
	bool active;
	unsigned long tActivate;
	uint16_t activateTop;
	uint16_t activateBot;
	float declinationAngle;
	void calculateActivation();
	void getInput();
	void getYaw();
	void getRollPitch();
	void calcutateOutput();
	void calcAhrs10ms();
	void calcRollPitch10ms();

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
		//declinationAngle = persistence->readDeclinationAngle();
		declinationAngle = -0.02472549;
		activateTop = inputMax[3] - 100;
		activateBot = inputMin[3] + 100;
		for (int i = 0; i < 3; i++) {
			rollPitchYawLevel[i] = 0;
			rollPitchYaw[i] = 0;
		}
		persistence->readPID(rollPitchYawPidParams);
		initPID(rollPitchYawPidParams);
	}
	void putMotion6(int16_t *axyzgxyz);
	void calc2ms();
	void calcOutput10ms();
	void putBaro50ms(float altitude);
	void putMag(int16_t* mag);
	void calc10ms();
	void putInput50ms(uint8_t ch, uint16_t pwm);
	void calc50ms();
	void initPID(float *rollPitchYawPidParams);
	virtual ~DataModel() {
	}
};
#endif /* DATAMODEL_H_ */

