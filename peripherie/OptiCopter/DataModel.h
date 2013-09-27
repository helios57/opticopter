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

using namespace arducopterNg;

class DataModel {
private:
	HalApm *hal;
	int32_t accel[3];
	int16_t mag[3];
	unsigned short input[8];
	unsigned short inputMax[8];
	unsigned short inputMin[8];
	unsigned short output[8];
	Kalman outputKalman[8];
	double thrust[8];
	double magScaled[3];
	double magCompensated[3];
	int magMax[3]; // { 710, 424, 435 };
	int magMin[3]; // { -503, -768, -553 };
	double rollPitchYawLevel[3]; //Orientation - Level
	double rollPitchYaw[3]; //Orientation
	PID rollPitchYawPid[3]; //Pid
	double pressure;
	long t0;
	long t1;
	bool active;
	unsigned long tActivate;
	static const uint16_t activateTop = 1600;
	static const uint16_t activateBot = 1400;

	static const double declinationAngle = -0.02472549;

	void calculateActivation();
	void getInput();
	void getYaw();
	void getRollPitch();
	void calcutateOutput();

public:
	DataModel(HalApm *hal) :
			hal(hal) {
		t0 = 0;
		t1 = 0;
		pressure = 0;
		for (int i = 0; i < 8; i++) {
			outputKalman[i] = new Kalman(0.002, 1.0, 0.00, 0);
		}
		for (int i = 0; i < 3; i++) {
			rollPitchYawPid[i] = new PID(0.6, 0.05, 0.05, 0.5);
		}
		active = false;
		tActivate = 0;
	}
	virtual ~DataModel() {
	}
	void calculate();
};
#endif /* DATAMODEL_H_ */

