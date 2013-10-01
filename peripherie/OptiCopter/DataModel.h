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
	HalApm *hal;
	Persistence *persistence;
	int32_t accel[3];
	int16_t mag[3];
	uint16_t input[8];
	uint16_t inputMax[8];
	uint16_t inputMin[8];
	uint16_t inputDefault[8];
	uint16_t output[8];
	Kalman outputKalman[8];
	double thrust[8];
	double magScaled[3];
	double magCompensated[3];
	int16_t magMax[3];
	int16_t magMin[3];
	double rollPitchYawLevel[3]; //Orientation - Level
	double rollPitchYaw[3]; //Orientation
	PID rollPitchYawPid[3]; //PID
	double pressure;
	long t0;
	long t1;
	bool active;
	unsigned long tActivate;
	uint16_t activateTop;
	uint16_t activateBot;
	double declinationAngle;
	void calculateActivation();
	void getInput();
	void getYaw();
	void getRollPitch();
	void calcutateOutput();

public:
	DataModel(HalApm *hal, Persistence *persistence) :
			hal(hal), persistence(persistence) {
		t0 = 0;
		t1 = 0;
		active = false;
		tActivate = 0;
		pressure = 0;

		for (int i = 0; i < 8; i++) {
			outputKalman[i].init(0.002, 1.0, 0.00, 0.0);
		}
		for (int i = 0; i < 3; i++) {
			rollPitchYawPid[i].init(0.6, 0.05, 0.02);
		}
		/*magMax[0] = 773;magMax[1] = 369; magMax[2] = 565;*/
		persistence->readMagMax(magMax);
		//magMin[0] = -451;magMin[1] = -828;magMin[2] = -468;
		persistence->readMagMin(magMin);

		//inputMin[0] = 1105;inputMin[1] = 1105;inputMin[2] = 1105;inputMin[3] = 1105;
		persistence->readRcMin(inputMin, 4);
		//inputMax[0] = 1879;inputMax[1] = 1870;inputMax[2] = 1870;inputMax[3] = 1870;
		persistence->readRcMax(inputMax, 4);
		//inputDefault[0] = 1105;inputDefault[1] = 1488;inputDefault[2] = 1470;inputDefault[3] = 1488;
		persistence->readRcDefault(inputDefault, 4);
		//declinationAngle = -0.02472549;
		declinationAngle = persistence->readDeclinationAngle();
		activateTop = inputMax[3] - 100;
		activateBot = inputMin[3] + 100;
	}
	virtual ~DataModel() {
	}
	void calculate();
};
#endif /* DATAMODEL_H_ */

