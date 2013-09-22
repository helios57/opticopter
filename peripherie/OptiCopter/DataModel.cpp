/*
 * DataModel.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#include "DataModel.h"
#include "Math.h"
#include "Arduino.h"

void DataModel::calculate() {
	t0 = t1;
	t1 = millis();
	hal->getAccel(accel);
	Math::normalizeVec(accel, accelInG);

	rollPitchYawLast[0] = rollPitchYaw[0];
	rollPitchYawLast[1] = rollPitchYaw[1];
	rollPitchYawLast[2] = rollPitchYaw[2];

	rollPitchYaw[0] = atan2(-accelInG[0], accelInG[2]);
	rollPitchYaw[1] = atan2(accelInG[1], accelInG[2]);
	hal->getHeading(mag);

	for (int i = 0; i < 3; i++) {
		magMax[i] = max(magMax[i], mag[i]);
		magMin[i] = min(magMin[i], mag[i]);
	}
	for (int i = 0; i < 3; i++) {
		magScaled[i] = (((double) mag[i] - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
	}

	double rollSin = sin(rollPitchYaw[0]);
	double rollCos = cos(rollPitchYaw[0]);
	double pitchSin = sin(rollPitchYaw[1]);
	double pitchCos = cos(rollPitchYaw[1]);
	if (rollCos < 0) {
		rollCos = -rollCos;
	}
	magCompensated[1] = magScaled[1] * pitchCos - magScaled[2] * pitchSin;
	magCompensated[2] = magScaled[1] * pitchSin + magScaled[2] * pitchCos;
	magCompensated[0] = magScaled[0] * rollCos + magCompensated[2] * rollSin;
	rollPitchYaw[2] = atan2(magCompensated[1], magCompensated[0]);
	rollPitchYaw[2] += declinationAngle;
	if (rollPitchYaw[2] > PI) {
		rollPitchYaw[2] -= 2 * PI;
	}
	if (rollPitchYaw[2] < -PI) {
		rollPitchYaw[2] += 2 * PI;
	}

	rollPitchYawDiffLast[0] = rollPitchYawDiff[0];
	rollPitchYawDiffLast[1] = rollPitchYawDiff[1];
	rollPitchYawDiffLast[2] = rollPitchYawDiff[2];

	rollPitchYawDiff[0] = rollPitchYaw[0] - rollPitchYawLast[0];
	rollPitchYawDiff[1] = rollPitchYaw[1] - rollPitchYawLast[1];
	rollPitchYawDiff[2] = rollPitchYaw[2] - rollPitchYawLast[2];

	rollPitchYawDiffDiff[0] = rollPitchYawDiff[0] - rollPitchYawDiffLast[0];
	rollPitchYawDiffDiff[1] = rollPitchYawDiff[1] - rollPitchYawDiffLast[1];
	rollPitchYawDiffDiff[2] = rollPitchYawDiff[2] - rollPitchYawDiffLast[2];

	pressureLast = pressure;
	pressure = hal->getPressure();
	pressureDiffLast = pressureDiff;
	pressureDiff = pressure - pressureLast;
	pressureDiffDiff = pressureDiff - pressureDiffLast;


	Math::fromEuler(rollPitchYaw[1], rollPitchYaw[2], rollPitchYaw[0], quatCurrent);
	if (quatLevel[0] == 0) {
		for (int i = 0; i < 4; i++) {
			quatLevel[i] = quatCurrent[i];
		}
	}
	Math::normalize(quatLevel);
	Math::conjugate(quatCurrent, quatDiff);
	Math::multiply(quatDiff, quatLevel, quatDiff);
	Math::normalize(quatDiff);
	for (unsigned char i = hal->IN0; i < hal->IN7; i++) {
		if (inputMin[i] == 0 && hal->getPmw(i) > 1000) {
			inputMin[i] = hal->getPmw(i);
		}
		input[i] = hal->getPmw(i);
	}

	/**
	 * <br />
	 * +y---|-<-c---------<br/ >
	 * -x---a---0---b--+x<br/ >
	 * -y-------d->-|-----<br/ >
	 */
	// x,y,z,clockwise (1 = cw, -1 = ccw)
	double a[] = { -1, 0, 0, 1 };
	// double[] b = new double[] { 1, 0, 0, 1 };
	double c[] = { 0, 1, 0, -1 };
	// double[] d = new double[] { 0, -1, 0, -1 };

	double diffA[3];
	//Math::multiplyVec(a, quatDiff, diffA);
	Math::multiplyVec(a, quatCurrent, diffA);
	double diffC[3];
	//Math::multiplyVec(c, quatDiff, diffC);
	Math::multiplyVec(c, quatCurrent, diffC);
	correctionThrust[0] = diffA[2]; // + diffA[1];
	correctionThrust[1] = -diffA[2]; // + diffA[1]; // multiplyVec(b, quatDiff)[2];
	correctionThrust[2] = diffC[2]; // - diffC[0]; // multiplyVec(c, quatDiff)[2];
	correctionThrust[3] = -diffC[2]; // - diffC[0]; // multiplyVec(d, quatDiff)[2];

	unsigned short thrustDiff = input[0] - 1106;
	hal->setPmw(hal->OUT0, input[0] + thrustDiff * correctionThrust[0]);
	hal->setPmw(hal->OUT1, input[0] + thrustDiff * correctionThrust[1]);
	hal->setPmw(hal->OUT2, input[0] + thrustDiff * correctionThrust[2]);
	hal->setPmw(hal->OUT3, input[0] + thrustDiff * correctionThrust[3]);
}
