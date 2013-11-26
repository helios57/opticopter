/*
 * Filter.cpp
 *
 *  Created on: Nov 26, 2013
 *      Author: helios
 */

#include "Filter.h"
#include "Arduino.h"

float Filter::update(float rate, float measurement, float dt, bool active) {
	value += (rate - integratedDiff * 0.01) * dt;
	if (value > PI) {
		value -= 2 * PI;
	}
	if (value < -PI) {
		value += 2 * PI;
	}
	float diff = value - measurement;
	//Leave early to avoid improper adjustments to the state and bias on turnover or bad input
	if (fabs(measurement) < 0.0001 || (active && fabs(diff) > PI / 4)) {
		return value;
	}
	integratedDiff += diff;
	value -= diff * 0.02;
	return value;
}
