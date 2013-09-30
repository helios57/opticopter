/*
 * PID.cpp
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#include "PID.h"
void PID::resetI() {
	iState = 0;
}
double PID::updatePID(double target, double cur, long deltaTime) {
	diff = target - cur;

	// the pTerm is the view from now, the pgain judges
	// how much we care about error at this instant.
	pTerm = pgain * diff;

	// integrated iState
	iState += diff * (deltaTime / 10000.0);

	// limit iTerm
	if (iState > imax)
		iState = imax;
	else if (iState < -imax)
		iState = -imax;

	iTerm = igain * iState;

	// speed / Differential
	dTerm = (dgain * (cur - last)) / deltaTime;
	last = cur;
	return pTerm + iTerm - dTerm;
}
