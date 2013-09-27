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

	rollPitchYawLast[0] = rollPitchYaw[0];
	rollPitchYawLast[1] = rollPitchYaw[1];
	rollPitchYawLast[2] = rollPitchYaw[2];

	rollPitchYaw[0] = atan2(-accel[0], accel[2]);
	rollPitchYaw[1] = atan2(accel[1], accel[2]);
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

	long timeDiff = t0 - t1;
	double rollDiff = rollPitchYawLevel[0] - rollPitchYaw[0];
	double pitchDiff = rollPitchYawLevel[1] - rollPitchYaw[1];
	double yawDiff = rollPitchYawLevel[2] - rollPitchYaw[2];

	double rollA = (2 * rollDiff) / (timeDiff * timeDiff * 0.4) - (2 * rollPitchYawDiff[0]) / timeDiff - rollPitchYawDiffDiff[0];
	double pitchA = (2 * pitchDiff) / (timeDiff * timeDiff * 0.4) - (2 * rollPitchYawDiff[1]) / timeDiff - rollPitchYawDiffDiff[1];
	double yawA = (2 * yawDiff) / (timeDiff * timeDiff * 0.4) - (2 * rollPitchYawDiff[2]) / timeDiff - rollPitchYawDiffDiff[2];

	double thrustInput = (input[0] - 1105.0) / (1000.0); // not 100%; leave some reserve

	thrust[0] = rollA + yawA + thrustInput;
	thrust[1] = -rollA + yawA + thrustInput;
	thrust[2] = -pitchA - yawA + thrustInput;
	thrust[3] = pitchA - yawA + thrustInput;

	for (uint8_t i = 0; i < 4; i++) {
		if (thrust[i] > 1) {
			thrust[i] = 1;
		} else if (thrust[i] < 0) {
			thrust[i] = 0;
		}
		thrust[i] = kalman_update(&outputK[i], thrust[i]);
	}

	if (!active && input[3] > 1600 && tActivate == 0) {
		tActivate = millis();
	} else if (!active && input[3] > activateTop && (tActivate + 5000) < millis()) {
		active = true;
		tActivate = 0;
	}
	if (active && input[3] < 1400 && tActivate == 0) {
		tActivate = millis();
		rollPitchYawLevel[0] = rollPitchYaw[0];
		rollPitchYawLevel[1] = rollPitchYaw[1];
		rollPitchYawLevel[2] = rollPitchYaw[2];
	} else if (active && input[3] < activateBot && (tActivate + 5000) < millis()) {
		active = false;
		tActivate = 0;
	}

	if (active && thrustInput > 0.01) {
		hal->setPmw(hal->OUT0, 1105 + (900 * (thrust[0])));
		hal->setPmw(hal->OUT1, 1105 + (900 * (thrust[1])));
		hal->setPmw(hal->OUT2, 1105 + (900 * (thrust[2])));
		hal->setPmw(hal->OUT3, 1105 + (900 * (thrust[3])));
	} else {
		hal->setPmw(hal->OUT0, 1105);
		hal->setPmw(hal->OUT1, 1105);
		hal->setPmw(hal->OUT2, 1105);
		hal->setPmw(hal->OUT3, 1105);
	}
}
