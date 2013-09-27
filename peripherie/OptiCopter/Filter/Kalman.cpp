/*
 * Kalman.cpp
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#include "Kalman.h"

double Kalman::kalman_update(double measurement) {
	//prediction update
	//omit x = x
	p = p + q;
	//measurement update
	k = p / (p + r);
	x = x + k * (measurement - x);
	p = (1 - k) * p;
	return x;
}
