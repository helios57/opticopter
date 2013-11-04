/*
 * PID.cpp
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#include "PID.h"
void PID::resetI() {
	errSum = 0;
}
float PID::updatePID(float setpoint, float current, float rate, float dt) {
	float error = setpoint - current;
	return kp * error;
}
