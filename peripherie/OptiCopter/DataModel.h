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
	int32_t gyro[3];
	int16_t mag[3];
	uint16_t input[8];
	uint16_t inputMax[8];
	uint16_t inputMin[8];
	uint16_t inputDefault[8];
	uint16_t output[8];
	double thrust[8];
	double magScaled[3];
	double magCompensated[3];
	int16_t magMax[3];
	int16_t magMin[3];
	double rollPitchYawLevel[3]; //Orientation - Level
	double rollPitchYaw[3]; //Orientation
	double rollPitchYawFiltered[3]; //Orientation
	Kalman rollPitchYawKalman[3];
	PID rollPitchYawPid[3]; //PID
	const static double GYRO_TO_RAD_PER_S_FACTOR = -2491011.89227323; // magical factor
	double pressure;
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
		active = false;
		tActivate = 0;
		pressure = 0;

		persistence->readMagMax(magMax);
		persistence->readMagMin(magMin);

		persistence->readRcMin(inputMin, 4);
		persistence->readRcMax(inputMax, 4);
		persistence->readRcDefault(inputDefault, 4);
		//declinationAngle = persistence->readDeclinationAngle();
		declinationAngle = -0.02472549;
		activateTop = inputMax[3] - 100;
		activateBot = inputMin[3] + 100;
	}
	void putAccel5ms(int32_t* accel);
	void putGyro5ms(int32_t* gyro);
	void putBaro10ms(float altitude);
	void putMag10ms(int16_t* mag);
	void putInput50ms(uint8_t ch, uint16_t pwm);

	virtual ~DataModel() {
	}
	void calculate();
};
#endif /* DATAMODEL_H_ */

