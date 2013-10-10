/*
 * FpMath.cpp
 *
 *  Created on: Oct 6, 2013
 *      Author: helios
 */

#include "FpMath.h"

FpMath::FpMath() {

}

FpMath::~FpMath() {
}

float FpMath::arctan2(int32_t y, int32_t x) {
	coeff_1 = pi / 4;
	coeff_2 = 3 * coeff_1;
	abs_y = abs(y) + 1e-10      // kludge to prevent 0/0 condition
	if (x >= 0) {
		r = (x - abs_y) / (x + abs_y);
		angle = coeff_1 - coeff_1 * r;
	} else {
		r = (x + abs_y) / (abs_y - x);
		angle = coeff_2 - coeff_1 * r;
	}
	if (y < 0)
		return (-angle);     // negate if in quad III or IV
	else
		return (angle);
}
