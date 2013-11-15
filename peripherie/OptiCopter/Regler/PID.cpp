/*
 * PID.cpp
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#include "PID.h"
void PID::resetI() {
	diffSum = 0;
}
float PID::updatePID(float setpoint, float current, float rate, float dt) {
	float diff = setpoint - current - rate * dt * 100;
	diffSum += diff * (dt / 2);
	if (diffSum > windupGuard) {
		diffSum = windupGuard;
	} else if (diffSum < -windupGuard) {
		diffSum = -windupGuard;
	}
	float dTerm = (current - lastPos) * dt;
	lastPos = current;

	float result = kp * diff - ki * diffSum - kd * dTerm;

	if (result > max) {
		result = max;
	} else if (result < -max) {
		result = -max;
	}
	lastResult = result;
	lastRate = rate;
	return result;
}
