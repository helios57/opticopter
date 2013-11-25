/*
 * DataModel.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#include "DataModel.h"
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
		gyroBias[0] = motion[3];
		gyroBias[1] = motion[4];
		gyroBias[2] = motion[5];
	}

	if (active && input[3] < activateBot && tActivate == 0) {
		tActivate = millis();
	} else if (active && input[3] < activateBot && (tActivate + 5000) < millis()) {
		active = false;
		tActivate = 0;
	}
}
static long count = 0;

void DataModel::calc10ms() {
	calculateActivation();
	calcRollPitch10ms();
	calcMag10ms();
	calcOutput10ms();
}

void DataModel::putMotion6(int16_t *axyzgxyz) {
	for (uint8_t i = 0; i < 6; i++) {
		motion[i] = axyzgxyz[i];
	}
}

void DataModel::putMag(int16_t* mag) {
	for (uint8_t i = 0; i < 3; i++) {
		magRing[i][magRingIndex] = mag[i];
	}
	if (++magRingIndex >= magRingIndexMax) {
		magRingIndex = 0;
	}
}

void DataModel::calcRollPitch10ms() {
	rollPitchYaw[0] = atan2(-motion[0], motion[2]);
	rollPitchYaw[1] = atan2(motion[1], motion[2]);
	rollPitchYawFiltered[0] = rollPitchYawKalman[0].getAngle(rollPitchYaw[0], motion[4] * GYRO_TO_RAD_PER_S_FACTOR, 0.01);
	rollPitchYawFiltered[1] = rollPitchYawKalman[1].getAngle(rollPitchYaw[1], motion[3] * GYRO_TO_RAD_PER_S_FACTOR, 0.01);
}

void DataModel::calcOutput10ms() {
	if (leveling && (inputRoll > 0.1 || inputRoll < 0.1)) {
		rollPitchYawLevel[0] += inputRoll * 0.01;
	}
	if (leveling && (inputPitch > 0.1 || inputPitch < 0.1)) {
		rollPitchYawLevel[1] += inputPitch * 0.01;
	}
	if (active) {
		for (uint8_t i = 3; i < 6; i++) {
			motion[i] -= gyroBias[i - 3];
		}
	}
	float rollLevel = rollPitchYawLevel[0];
	float pitchLevel = rollPitchYawLevel[1];
	float yawLevel = rollPitchYawLevel[2];
	if (!leveling) {
		//float yawA = 0;
		rollLevel += inputRoll;
		pitchLevel += inputPitch;
		yawLevel += inputYaw;
	}
	float yawCurrent = rollPitchYawFiltered[2];
	while (yawLevel > PI) {
		yawLevel -= PI * 2;
	}
	while (yawLevel < -PI) {
		yawLevel += PI * 2;
	}
	double diff = yawLevel - yawCurrent;
	if (diff < -PI) {
		yawLevel += 2 * PI;
	}
	if (diff > PI) {
		yawLevel -= 2 * PI;
	}

	float rollA = rollPitchYawPid[0].updatePID(rollLevel, rollPitchYawFiltered[0], motion[4] * GYRO_TO_RAD_PER_S_FACTOR, 0.01);
	float pitchA = rollPitchYawPid[1].updatePID(pitchLevel, rollPitchYawFiltered[1], motion[3] * GYRO_TO_RAD_PER_S_FACTOR, 0.01);
	float yawA = 0.2 * rollPitchYawPid[2].updatePID(yawLevel, rollPitchYawFiltered[2], 0.0/* -motion[5] * GYRO_TO_RAD_PER_S_FACTOR*/, 0.01);

	thrust[0] = inputThrust + rollA + yawA;
	thrust[1] = inputThrust - rollA + yawA;
	thrust[2] = inputThrust - pitchA - yawA;
	thrust[3] = inputThrust + pitchA - yawA;

	/*float factor = 1.0;
	 thrust[0] = inputThrust * (1.0 + factor * (-rollA - yawA));
	 thrust[1] = inputThrust * (1.0 + factor * (rollA + yawA));
	 thrust[2] = inputThrust * (1.0 + factor * (rollA * SIN_60_COS_30 + pitchA * SIN_60_COS_30 - yawA));
	 thrust[3] = inputThrust * (1.0 + factor * (-rollA * SIN_60_COS_30 - pitchA * SIN_60_COS_30 + yawA));
	 thrust[4] = inputThrust * (1.0 + factor * (-rollA * SIN_60_COS_30 + pitchA * SIN_60_COS_30 + yawA));
	 thrust[5] = inputThrust * (1.0 + factor * (rollA * SIN_60_COS_30 - pitchA * SIN_60_COS_30 - yawA));
	 */
	for (int i = 0; i < 6; i++) {
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
	hal->setPmw(hal->OUT4, output[4]);
	hal->setPmw(hal->OUT5, output[5]);

	if (count++ >= 10) {
		Serial.print(millis());
		Serial.print(",");
		count = 0;
		for (uint8_t i = 0; i < 6; i++) {
			Serial.print(motion[i]);
			Serial.print(",");
		}
		for (uint8_t i = 0; i < 3; i++) {
			Serial.print(mag[i]);
			Serial.print(",");
		}
		for (uint8_t i = 0; i < 3; i++) {
			Serial.print(rollPitchYawFiltered[i] * 1000);
			Serial.print(",");
		}
		Serial.println();
	}
}

void DataModel::putBaro50ms(float altitude) {
}

void DataModel::calcMag10ms() {
	for (uint8_t i = 0; i < magRingIndexMax; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			magSummed[j] += magRing[j][i];
		}
	}
	for (uint8_t j = 0; j < 3; j++) {
		mag[j] = magSummed[j] / magRingIndexMax;
		magSummed[j] = 0;
	}

	for (int i = 0; i < 3; i++) {
		magScaled[i] = (((float) ((mag[i])) - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
	}
	magScaled[2] = -magScaled[2];
	float rollSin = sin(rollPitchYawFiltered[0] * 0.5);
	float rollCos = cos(rollPitchYawFiltered[0] * 0.5);
	float pitchSin = sin(rollPitchYawFiltered[1] * 0.2);
	float pitchCos = cos(rollPitchYawFiltered[1] * 0.2);
	if (rollCos < 0) {
		rollCos = -rollCos;
	}
	magCompensated[1] = magScaled[1] * pitchCos - magScaled[2] * pitchSin;
	magCompensated[2] = magScaled[1] * pitchSin + magScaled[2] * pitchCos;
	magCompensated[0] = magScaled[0] * rollCos + magCompensated[2] * rollSin;
	//Filter input-data
	magCompensated[0] = magCompensatedKalman[0].getAngle(magCompensated[0], 0.0, 0.01);
	magCompensated[1] = magCompensatedKalman[1].getAngle(magCompensated[1], 0.0, 0.01);
	magCompensated[2] = magCompensatedKalman[2].getAngle(magCompensated[2], 0.0, 0.01);
	rollPitchYaw[2] = atan2(magCompensated[1], magCompensated[0]);
	rollPitchYaw[2] += declinationAngle;
	if (rollPitchYaw[2] > PI) {
		rollPitchYaw[2] -= 2 * PI;
	}
	if (rollPitchYaw[2] < -PI) {
		rollPitchYaw[2] += 2 * PI;
	}
	//Filter Input-Data to avoid Euler-Angels-Turnover
	//TODO fix with quaternion
	//rollPitchYawFiltered[2] = rollPitchYawKalman[2].getAngle(rollPitchYaw[2], 0.0/*-motion[5] * GYRO_TO_RAD_PER_S_FACTOR*/, 0.01);
	rollPitchYawFiltered[2] = rollPitchYaw[2];
}

void DataModel::putInput50ms(uint8_t ch, uint16_t pwm) {
	input[ch] = pwm;
}

void DataModel::calc50ms() {
	inputRoll = ((float) input[0] - inputDefault[0]) / (float) (inputMax[0] - inputMin[0]);
	inputPitch = ((float) input[1] - inputDefault[1]) / (float) (inputMax[1] - inputMin[1]);
	inputThrust = ((float) input[2] - inputDefault[2]) / (float) (inputMax[2] - inputMin[2]);
	inputYaw = ((float) input[3] - inputDefault[3]) / (float) (inputMax[3] - inputMin[3]);
	leveling = input[4] < 1500;
}

void DataModel::initPID(float* rollPitchYawPidParams) {
	rollPitchYawPid[0].init(rollPitchYawPidParams[0], rollPitchYawPidParams[1], rollPitchYawPidParams[2]);
	rollPitchYawPid[1].init(rollPitchYawPidParams[3], rollPitchYawPidParams[4], rollPitchYawPidParams[5]);
	rollPitchYawPid[2].init(rollPitchYawPidParams[6], rollPitchYawPidParams[7], rollPitchYawPidParams[8]);
}
