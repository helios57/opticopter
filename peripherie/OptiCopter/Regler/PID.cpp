/*
 * PID.cpp
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#include "PID.h"

namespace opticopter {
	void PID::resetI() {
		diffSum = 0;
	}
	float PID::updatePID(float setpoint, float current, float rate, float dt) {
		float diff = setpoint - current;
		//diffSum += diff * dt;
		if (diffSum > windupGuard) {
			diffSum = windupGuard;
		} else if (diffSum < -windupGuard) {
			diffSum = -windupGuard;
		}
		//float dTerm = rate;
		float dTerm = (current - lastPos) / dt;
		lastPos = current;

		float newValue = kp * diff + ki * diffSum + kd * dTerm;

		if (newValue > max) {
			newValue = max;
		} else if (newValue < -max) {
			newValue = -max;
		}
		oldValue = newValue; // (newValue - oldValue) / (smoothing / dt);
		lastRate = rate;
		return oldValue;
	}
}
