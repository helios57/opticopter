/*
 * PID.h
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#ifndef PID_H_
#define PID_H_

class PID {
private:
	double pgain, igain, dgain, imax;
	double pTerm, iTerm, dTerm;
	double iState;
	double last;
	double diff;
public:
	PID(double p, double i, double d, double imax) :
			pgain(p), igain(i), dgain(d) {
		pTerm = iTerm = dTerm = iState = last = diff = 0;
	}
	double updatePID(double target, double cur, long deltaTime);
	virtual ~PID() {
	}
};

#endif /* PID_H_ */
