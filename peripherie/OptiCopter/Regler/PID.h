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
	double errSum;
	double errSumMax;
	double lastInput;
	double kp;
	double ki;
	double kd;
public:
	PID() {
		lastInput = 0;
		errSum = 0;
		kp = 0.6;
		ki = 0.5 * 0.02;
		kd = 0.02 / 0.02;
		errSumMax = (PI / ki) / 16;
	}
	void resetI();
	double updatePID(double target, double cur);
	virtual ~PID() {
	}
};

#endif /* PID_H_ */
