/*
 * DataModel.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#include "DataModel.h"
#include "Math.h"
#include "Arduino.h"

void DataModel::calculateActivation() {
	if (!active && input[3] > activateTop && tActivate == 0) {
		tActivate = millis();
	} else if (!active && input[3] > activateTop && (tActivate + 5000) < millis()) {
		active = true;
		tActivate = 0;
	}

	if (active && input[3] < activateBot && tActivate == 0) {
		tActivate = millis();
		rollPitchYawLevel[0] = rollPitchYaw[0];
		rollPitchYawLevel[1] = rollPitchYaw[1];
		rollPitchYawLevel[2] = rollPitchYaw[2];
	} else if (active && input[3] < activateBot && (tActivate + 5000) < millis()) {
		active = false;
		tActivate = 0;
	}
}

void DataModel::getInput() {
	for (unsigned char i = hal->IN0; i < hal->IN7; i++) {
		if (inputMin[i] == 0 && hal->getPmw(i) > 1000) {
			inputMin[i] = hal->getPmw(i);
		}
		input[i] = hal->getPmw(i);
	}
}

void DataModel::getYaw() {
	hal->getHeading(mag);
	//TODO persisieren
	for (int i = 0; i < 3; i++) {
		magMax[i] = max(magMax[i], mag[i]);
		magMin[i] = min(magMin[i], mag[i]);
	}
	for (int i = 0; i < 3; i++) {
		magScaled[i] = (((double) (mag[i]) - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
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
}

void DataModel::getRollPitch() {
	hal->getAccel(accel);
	rollPitchYaw[0] = atan2(-accel[0], accel[2]);
	rollPitchYaw[1] = atan2(accel[1], accel[2]);
}

void DataModel::calcutateOutput() {
	/**
	 * <br />
	 * +y---|-<-c---------<br/ >
	 * -x---a---0---b--+x<br/ >
	 * -y-------d->-|-----<br/ >
	 */
	long timeDiff = t0 - t1;
	double rollA = rollPitchYawPid[0].updatePID(rollPitchYawLevel[0], rollPitchYaw[0], timeDiff);
	double pitchA = rollPitchYawPid[1].updatePID(rollPitchYawLevel[1], rollPitchYaw[1], timeDiff);
	double yawA = rollPitchYawPid[2].updatePID(rollPitchYawLevel[2], rollPitchYaw[2], timeDiff);
	double thrustInput = (input[0] - 1105.0) / (1000.0); // not 100%; leave some reserve
	thrust[0] = rollA + yawA + thrustInput;
	thrust[1] = -rollA + yawA + thrustInput;
	thrust[2] = -pitchA - yawA + thrustInput;
	thrust[3] = pitchA - yawA + thrustInput;
	for (int i = 0; i < 4; i++) {
		if (thrust[i] > 1) {
			thrust[i] = 1;
		} else if (thrust[i] < 0) {
			thrust[i] = 0;
		}

		thrust[i] = outputKalman[i].kalman_update(thrust[i]);
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

void DataModel::calculate() {
	t0 = t1;
	t1 = millis();
	calculateActivation();
	getRollPitch();
	getYaw();
	//pressure = hal->getPressure();

	getInput();
	calcutateOutput();
}
