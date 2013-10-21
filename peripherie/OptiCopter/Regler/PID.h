/*
 * PID.h
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#ifndef PID_H_
#define PID_H_

#include <Arduino.h>

class PID {
private:
	float errSum;
	float errSumMax;
	float lastInput;
	float kp;
	float ki;
	float kd;
public:
	PID() {
		lastInput = 0;
		errSum = 0;
		kp = 0.1;
		ki = 0.01;
		kd = 0.05;
		errSumMax = (PI / ki) / 16;
	}
	void resetI();
	float updatePID(float target, float cur);
	virtual ~PID() {
	}
};

#endif /* PID_H_ */
