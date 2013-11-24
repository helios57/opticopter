/*
 * Heading.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: helios
 */

#include "Heading.h"
#include "Arduino.h"

void Heading::updateHeading(float roll, float pitch, float gyroZ, float* magXYZ, float dt) {
	float rollSin = sin(roll);
	float rollCos = cos(roll);
	float pitchSin = sin(pitch);
	float pitchCos = cos(pitch);
	if (rollCos < 0) {
		rollCos = -rollCos;
	}
	magCompensated[1] = magXYZ[1] * pitchCos - magXYZ[2] * pitchSin;
	magCompensated[2] = magXYZ[1] * pitchSin + magXYZ[2] * pitchCos;
	magCompensated[0] = magXYZ[0] * rollCos + magCompensated[2] * rollSin;
	yawRing[ringIndex] = atan2(magCompensated[1], magCompensated[0]);
	yawRing[ringIndex] += declinationAngle;
	//Filter Input-Data to avoid Euler-Angels-Turnover
	//TODO fix with quaternion
	if (++ringIndex >= ringIndexMax) {
		ringIndex = 0;
	}
}

float Heading::getYaw() {
	float result = 0.0f;
	for (uint8_t i = 0; i < ringIndexMax; i++) {
		result += yawRing[i];
	}
	result /= ringIndexMax;
	if (result > PI) {
		result -= 2 * PI;
	}
	if (result < -PI) {
		result += 2 * PI;
	}
	return result;
}
