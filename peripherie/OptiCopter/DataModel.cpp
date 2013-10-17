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
		rollPitchYawPid[0].resetI();
		rollPitchYawPid[1].resetI();
		rollPitchYawPid[2].resetI();
		rollPitchYawLevel[0] = rollPitchYawFiltered[0];
		rollPitchYawLevel[1] = rollPitchYawFiltered[1];
		rollPitchYawLevel[2] = rollPitchYawFiltered[2];
	}

	if (active && input[3] < activateBot && tActivate == 0) {
		tActivate = millis();
	} else if (active && input[3] < activateBot && (tActivate + 5000) < millis()) {
		active = false;
		tActivate = 0;
	}
}

void DataModel::calcutateOutput() {
	/**
	 * <br />
	 * +y---|-<-c---------<br/ >
	 * -x---a---0---b--+x<br/ >
	 * -y-------d->-|-----<br/ >
	 */
	float rollA = rollPitchYawPid[0].updatePID(rollPitchYawLevel[0], rollPitchYawFiltered[0]);
	float pitchA = rollPitchYawPid[1].updatePID(rollPitchYawLevel[1], rollPitchYawFiltered[1]);
	float yawA = rollPitchYawPid[2].updatePID(rollPitchYawLevel[2], rollPitchYawFiltered[2]);

	float inputThrust = ((float) input[0] - inputDefault[0]) / (float) (inputMax[0] - inputMin[0]);
	float inputRoll = ((float) input[1] - inputDefault[1]) / (float) (inputMax[1] - inputMin[1]);
	float inputPitch = ((float) input[2] - inputDefault[2]) / (float) (inputMax[2] - inputMin[2]);
	float inputYaw = ((float) input[3] - inputDefault[3]) / (float) (inputMax[3] - inputMin[3]);

	rollA += inputRoll;
	pitchA += inputPitch;
	yawA += inputYaw;
	yawA *= 0.1;

	thrust[0] = rollA + yawA + inputThrust;
	thrust[1] = -rollA + yawA + inputThrust;
	thrust[2] = -pitchA - yawA + inputThrust;
	thrust[3] = pitchA - yawA + inputThrust;

	for (int i = 0; i < 4; i++) {
		if (thrust[i] > 1) {
			thrust[i] = 1;
		} else if (thrust[i] < 0) {
			thrust[i] = 0;
		}
		if (active && inputThrust > 0.01) {
			output[i] = (inputMin[0] + ((inputMax[0] - inputMin[0]) * (thrust[i])));
		} else {
			output[i] = inputMin[0];
		}
	}

	hal->setPmw(hal->OUT0, output[0]);
	hal->setPmw(hal->OUT1, output[1]);
	hal->setPmw(hal->OUT2, output[2]);
	hal->setPmw(hal->OUT3, output[3]);
}

void DataModel::calculate10ms() {
	calculateActivation();
	calcutateOutput();
}

void DataModel::putAccel5ms(int32_t* accel) {
	rollPitchYaw[0] = atan2(-accel[0], accel[2]);
	rollPitchYaw[1] = atan2(accel[1], accel[2]);
}

void DataModel::putGyro5ms(int32_t* igyro) {
	rollPitchYawFiltered[0] = rollPitchYawKalman[0].getAngle(rollPitchYaw[0], igyro[0] / GYRO_TO_RAD_PER_S_FACTOR, 0.005);
	rollPitchYawFiltered[1] = rollPitchYawKalman[1].getAngle(rollPitchYaw[1], igyro[1] / GYRO_TO_RAD_PER_S_FACTOR, 0.005);
	gyro[2] = igyro[2];
}

void DataModel::putBaro50ms(float altitude) {
}

void DataModel::putMag10ms(int16_t* mag) {
	for (int i = 0; i < 3; i++) {
		magScaled[i] = (((float) (mag[i]) - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
	}
	float rollSin = sin(rollPitchYawFiltered[0]);
	float rollCos = cos(rollPitchYawFiltered[0]);
	float pitchSin = sin(rollPitchYawFiltered[1]);
	float pitchCos = cos(rollPitchYawFiltered[1]);
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
	rollPitchYawFiltered[2] = rollPitchYawKalman[2].getAngle(rollPitchYaw[2], gyro[2] / GYRO_TO_RAD_PER_S_FACTOR, 0.01);
}

void DataModel::putInput50ms(uint8_t ch, uint16_t pwm) {
	input[ch] = pwm;
}
