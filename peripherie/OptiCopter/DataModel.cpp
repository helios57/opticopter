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
		rollPitchYawLevel[0] = rollPitchYaw[0];
		rollPitchYawLevel[1] = rollPitchYaw[1];
		rollPitchYawLevel[2] = rollPitchYaw[2];
	}

	if (active && input[3] < activateBot && tActivate == 0) {
		tActivate = millis();
	} else if (active && input[3] < activateBot && (tActivate + 5000) < millis()) {
		active = false;
		tActivate = 0;
	}
}
static long count = 0;
void DataModel::calcutateOutput() {
	/**
	 * <br />
	 * +y---|-<-c---------<br/ >
	 * -x---a---0---b--+x<br/ >
	 * -y-------d->-|-----<br/ >
	 */

	float inputThrust = ((float) input[0] - inputDefault[0]) / (float) (inputMax[0] - inputMin[0]);
	float inputRoll = ((float) input[1] - inputDefault[1]) / (float) (inputMax[1] - inputMin[1]);
	float inputPitch = ((float) input[2] - inputDefault[2]) / (float) (inputMax[2] - inputMin[2]);
	float inputYaw = ((float) input[3] - inputDefault[3]) / (float) (inputMax[3] - inputMin[3]);

	float rollA = rollPitchYawPid[0].updatePID(rollPitchYawLevel[0] + inputRoll * 0.5, rollPitchYawFiltered[0]);
	float pitchA = rollPitchYawPid[1].updatePID(rollPitchYawLevel[1] + inputPitch * 0.5, rollPitchYawFiltered[1]);
	float yawA = rollPitchYawPid[2].updatePID(rollPitchYawLevel[2] + inputYaw * 0.5, rollPitchYawFiltered[2]);
	yawA = 0; //*= 0.5;

	/**
	 //quad X
	 thrust[0] = rollA + yawA + inputThrust;
	 thrust[1] = -rollA + yawA + inputThrust;
	 thrust[2] = -pitchA - yawA + inputThrust;
	 thrust[3] = pitchA - yawA + inputThrust;
	 */

	//hexa X
	thrust[0] = inputThrust * (1 + (rollA + yawA));
	thrust[1] = inputThrust * (1 + (-rollA - yawA));
	thrust[2] = inputThrust * (1 + (-rollA * SIN_60_COS_30 + pitchA * SIN_60_COS_30 + yawA));
	thrust[3] = inputThrust * (1 + (+rollA * SIN_60_COS_30 - pitchA * SIN_60_COS_30 - yawA));
	thrust[4] = inputThrust * (1 + (-rollA * SIN_60_COS_30 + pitchA * SIN_60_COS_30 + yawA));
	thrust[5] = inputThrust * (1 + (+rollA * SIN_60_COS_30 - pitchA * SIN_60_COS_30 - yawA));

	if (false && count++ > 0) {
		count = 0;
		for (uint8_t i = 0; i < 6; i++) {
			Serial.print(motion[i]);
			Serial.print(",");
		}
		Serial.print(rollPitchYaw[0]);
		Serial.print(",");
		Serial.print(rollPitchYaw[1]);
		Serial.print(",");
		Serial.print(rollPitchYawFiltered[0]);
		Serial.print(",");
		Serial.print(rollPitchYawFiltered[1]);
		Serial.print(",");
		Serial.print(rollA);
		Serial.print(",");
		Serial.print(pitchA);
		Serial.print(",");
		Serial.print(thrust[0]);
		Serial.print(",");
		Serial.print(thrust[1]);
		Serial.print(",");
		Serial.print(thrust[2]);
		Serial.print(",");
		Serial.print(thrust[3]);
		Serial.print(",");
		Serial.print(thrust[4]);
		Serial.print(",");
		Serial.println(thrust[5]);
	}
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
}

void DataModel::calculate10ms() {
	calculateActivation();
	calcutateOutput();
}

void DataModel::putMotion6_5ms(int16_t *axyzgxyz) {
	for (uint8_t i = 0; i < 6; i++) {
		motion[i] = axyzgxyz[i];
	}
	rollPitchYaw[0] = atan2(-motion[0], motion[2]);
	rollPitchYaw[1] = atan2(motion[1], motion[2]);
	rollPitchYawFiltered[0] = rollPitchYawKalman[0].getAngle(rollPitchYaw[0], motion[4] * GYRO_TO_RAD_PER_S_FACTOR, 0.005);
	rollPitchYawFiltered[1] = rollPitchYawKalman[1].getAngle(rollPitchYaw[1], motion[3] * GYRO_TO_RAD_PER_S_FACTOR, 0.005);
}

void DataModel::putBaro50ms(float altitude) {
}

void DataModel::putMag10ms(int16_t* mag) {
	for (int i = 0; i < 3; i++) {
		magScaled[i] = (((float) (mag[i]) - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
	}
	magScaled[2] = -magScaled[2];
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
	rollPitchYawFiltered[2] = rollPitchYawKalman[2].getAngle(rollPitchYaw[2], motion[5] * GYRO_TO_RAD_PER_S_FACTOR, 0.01);
}

void DataModel::putInput50ms(uint8_t ch, uint16_t pwm) {
	input[ch] = pwm;
}
