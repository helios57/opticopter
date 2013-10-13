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
double PID::updatePID(double setpoint, double current) {
	double error = setpoint - current;
	errSum += error;
	if (errSum > errSumMax) {
		errSum = errSumMax;
	}
	if (errSum < -errSumMax) {
		errSum = -errSumMax;
	}
	double dInput = (current - lastInput);
	lastInput = current;
	return kp * error + ki * errSum - kd * dInput;
}
