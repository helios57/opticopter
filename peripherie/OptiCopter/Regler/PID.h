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
	double pgain, igain, dgain, imax;
	double pTerm, iTerm, dTerm;
	double iState;
	double last;
	double diff;
public:
	PID() {
		pgain = 0;
		igain = 0;
		dgain = 0;
		imax = 0;
		pTerm = iTerm = dTerm = iState = last = diff = 0;
	}
	void init(double p, double i, double d) {
		pgain = p;
		igain = i;
		dgain = d;
		imax = PI / i;
	}
	void resetI();
	double updatePID(double target, double cur, long deltaTime);
	virtual ~PID() {
	}
};

#endif /* PID_H_ */
