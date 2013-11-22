/*
 * DataModel.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#include "DataModel.h"
#include "Arduino.h"
#include "Util/Quaternion.h"

void DataModel::calculateActivation() {
	if (!active && input[3] > activateTop && tActivate == 0) {
		tActivate = millis();
		for (int i = 0; i < 3; i++) {
			gyroBiasSum[i] = 0;
		}
		gyroBiasSumCount = 0;
	} else if (!active && input[3] > activateTop && (tActivate + 5000) < millis()) {
		active = true;
		tActivate = 0;
		for (int i = 0; i < 3; i++) {
			gyroBias[i] = gyroBiasSum[i] / gyroBiasSumCount;
		}
		for (int i = 0; i < 4; i++) {
			orientationLevel[i] = orientation[i];
		}
	} else if (!active && input[3] > activateTop && tActivate > 0) {
		for (int i = 0; i < 3; i++) {
			gyroBiasSum[i] = motion[3 + i];
			gyroBiasSumCount++;
		}
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
	calcOutput10ms();
}

void DataModel::putMotion6(int16_t *axyzgxyz) {
	for (uint8_t i = 0; i < 6; i++) {
		motion[i] = axyzgxyz[i];
	}
}

void DataModel::putMag(int16_t* imag) {
	for (uint8_t i = 0; i < 3; i++) {
		mag[i] = imag[i];
	}
}

void DataModel::calcOutput10ms() {
	if (active) {
		for (uint8_t i = 3; i < 6; i++) {
			motion[i] -= gyroBias[i - 3];
		}
	}
	for (int i = 0; i < 3; i++) {
		magScaled[i] = (((float) ((mag[i])) - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
	}
	magScaled[2] = -magScaled[2];
	float dt = 0.002; //TODO check
	filter.MadgwickAHRSupdateIMU( //
	        motion[4] * GYRO_TO_RAD_PER_S_FACTOR, //
	        motion[3] * GYRO_TO_RAD_PER_S_FACTOR, //
	        motion[5] * GYRO_TO_RAD_PER_S_FACTOR, //
	        motion[0], motion[1], motion[2], 0.01);
	/*filter.MadgwickAHRSupdate( //
	 motion[4] * GYRO_TO_RAD_PER_S_FACTOR, //
	 motion[3] * GYRO_TO_RAD_PER_S_FACTOR, //
	 motion[5] * GYRO_TO_RAD_PER_S_FACTOR, //
	 motion[0], motion[1], motion[2], //
	 magScaled[0], magScaled[1], magScaled[2], dt);*/

	filter.getQuaternion(orientation);
	Quaternion::diff(orientation, orientationLevel, orientationError);
	Quaternion::multiply(orientationError, orientationInput, orientationErrorInput);

	float q0 = orientationErrorInput[0];
	float q1 = orientationErrorInput[1];
	float q2 = orientationErrorInput[2];
	float q3 = orientationErrorInput[3];
	float roll = -0.5 * atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
	float pitch = -0.5 * asin(2 * (q0 * q2 - q1 * q3));
	float yaw = 0; //-0.01 * atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));

	thrust[0] = inputThrust + roll + yaw;
	thrust[1] = inputThrust - roll + yaw;
	thrust[2] = inputThrust - pitch - yaw;
	thrust[3] = inputThrust + pitch - yaw;

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

	if (count++ >= 1000) {
		Serial.print(millis());
		Serial.print(",");
		count = 0;
		/*for (uint8_t i = 0; i < 6; i++) {
		 Serial.print(motion[i]);
		 Serial.print(",");
		 }
		 for (uint8_t i = 0; i < 3; i++) {
		 Serial.print(magScaled[i]);
		 Serial.print(",");
		 }
		 for (uint8_t i = 0; i < 4; i++) {
		 Serial.print(orientation[i]);
		 Serial.print(",");
		 }
		 Serial.print(roll * 1000);
		 Serial.print(",");
		 Serial.print(pitch * 1000);
		 Serial.print(",");
		 Serial.print(yaw * 1000);*/
		Serial.println();
	}
}

void DataModel::putBaro50ms(float altitude) {
}

void DataModel::putInput50ms(uint8_t ch, uint16_t pwm) {
	input[ch] = pwm;
}

void DataModel::calc50ms() {
	inputRoll = ((float) input[0] - inputDefault[0]) / (float) (inputMax[0] - inputMin[0]);
	inputPitch = ((float) input[1] - inputDefault[1]) / (float) (inputMax[1] - inputMin[1]);
	inputThrust = ((float) input[2] - inputDefault[2]) / (float) (inputMax[2] - inputMin[2]);
	inputYaw = ((float) input[3] - inputDefault[3]) / (float) (inputMax[3] - inputMin[3]);
	Quaternion::fromEuler(inputRoll, inputPitch, inputYaw, orientationInput);
	leveling = input[4] < 1500;
}
