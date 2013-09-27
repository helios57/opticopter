/*
 * DataModel.h
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#ifndef DATAMODEL_H_
#define DATAMODEL_H_

#include "../Libraries/Hal/HalApm.h"
using namespace arducopterNg;

class DataModel {
private:
	typedef struct {
		double q; //process noise covariance
		double r; //measurement noise covariance
		double x; //value
		double p; //estimation error covariance
		double k; //kalman gain
	} kalman_state;
	kalman_state kalman_init(double q, double r, double p, double intial_value) {
		kalman_state result;
		result.q = q;
		result.r = r;
		result.p = p;
		result.x = intial_value;
		return result;
	}
	double kalman_update(kalman_state* state, double measurement) {
		//prediction update
		//omit x = x
		state->p = state->p + state->q;

		//measurement update
		state->k = state->p / (state->p + state->r);
		state->x = state->x + state->k * (measurement - state->x);
		state->p = (1 - state->k) * state->p;
		return state->x;
	}

	HalApm *hal;
	int32_t accel[3];
	int16_t mag[3];
	unsigned short input[8];
	unsigned short inputMax[8];
	unsigned short inputMin[8];
	unsigned short output[8];
	kalman_state outputK[8];
	double thrust[8];
	double magScaled[3];
	double magCompensated[3];
	int magMax[3]; // { 710, 424, 435 };
	int magMin[3]; // { -503, -768, -553 };
	double rollPitchYawLevel[3]; //Orientation - Level
	double rollPitchYaw[3]; //Orientation
	double rollPitchYawLast[3]; //Orientation
	double rollPitchYawDiff[3]; //Angular Speed
	double rollPitchYawDiffLast[3]; //Angular Speed
	double rollPitchYawDiffDiff[3]; //Angular Acceleration
	double pressure;
	double pressureLast;
	double pressureDiff;
	double pressureDiffLast;
	double pressureDiffDiff;
	long t0;
	long t1;
	bool active;
	unsigned long tActivate;
	static const uint16_t activateTop = 1600;
	static const uint16_t activateBot = 1400;

	static const double declinationAngle = -0.02472549;
public:
	DataModel(HalApm *hal) :
			hal(hal) {
		t0 = 0;
		t1 = 0;
		pressure = 0;
		pressureLast = 0;
		pressureDiff = 0;
		pressureDiffLast = 0;
		pressureDiffDiff = 0;
		for (uint8_t i = 0; i < 8; i++) {
			outputK[i] = kalman_init(0.002, 1.0, 0.00, 0);
		}
		active = false;
		tActivate = 0;
	}
	virtual ~DataModel() {
	}
	void calculate();
};
#endif /* DATAMODEL_H_ */

