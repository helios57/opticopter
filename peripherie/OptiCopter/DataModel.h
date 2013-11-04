/*
 * DataModel.h
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#ifndef DATAMODEL_H_
#define DATAMODEL_H_

#include "../Libraries/Hal/HalApm.h"
#include "Filter/Kalman.h"
#include "Regler/PID.h"
#include "Persistence/Persistence.h"

using namespace arducopterNg;

class DataModel {
private:
	const static uint8_t motionRingIndexMax = 3;
	const static uint8_t magRingIndexMax = 10;
	HalApm *hal;
	Persistence *persistence;
	int16_t motionRing[6][motionRingIndexMax];
	int32_t motionSummed[6];
	uint8_t motionRingIndex;
	int16_t motion[6];
	int16_t magRing[3][magRingIndexMax];
	uint8_t magRingIndex;
	int32_t magSummed[3];
	int16_t mag[3];
	uint16_t input[8];
	uint16_t inputMax[8];
	uint16_t inputMin[8];
	uint16_t inputDefault[8];
	uint16_t output[8];
	float thrust[8];
	float magScaled[3];
	float magCompensated[3];
	int16_t magMax[3];
	int16_t magMin[3];
	float rollPitchYawLevel[3]; //Orientation - Level
	float rollPitchYaw[3]; //Orientation
	float rollPitchYawFiltered[3]; //Orientation
	Kalman rollPitchYawKalman[3];
	Kalman magCompensatedKalman[3];
	PID rollPitchYawPid[3]; //PID
	const static float GYRO_TO_RAD_PER_S_FACTOR = 0.00106413;
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
	void calcMag();

public:
	DataModel(HalApm *hal, Persistence *persistence) :
			hal(hal), persistence(persistence) {
		active = false;
		tActivate = 0;
		pressure = 0;
		magRingIndex = 0;
		motionRingIndex = 0;
		persistence->readMagMax(magMax);
		persistence->readMagMin(magMin);

		persistence->readRcMin(inputMin, 4);
		persistence->readRcMax(inputMax, 4);
		persistence->readRcDefault(inputDefault, 4);
		//declinationAngle = persistence->readDeclinationAngle();
		declinationAngle = -0.02472549;
		activateTop = inputMax[3] - 100;
		activateBot = inputMin[3] + 100;
		for (int i = 0; i < 3; i++) {
			rollPitchYawLevel[i] = 0;
			rollPitchYaw[i] = 0;
			rollPitchYawFiltered[i] = 0;
		}
		rollPitchYawKalman[2].Q_angle = 1;
		rollPitchYawKalman[2].Q_bias = 1;
		rollPitchYawKalman[2].R_measure = 1;

	}
	void putMotion6(int16_t *axyzgxyz);
	void calc2ms();
	void calc5ms();
	void putBaro50ms(float altitude);
	void putMag(int16_t* mag);
	void calc10ms();
	void putInput50ms(uint8_t ch, uint16_t pwm);

	virtual ~DataModel() {
	}
};
#endif /* DATAMODEL_H_ */

