/*
 * Main.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: helios
 */

#include "OptiCopter/OptiCopter.h"
#include "Arduino/Arduino.h"

int main(void) {
	init();
	opticopter::OptiCopter ac;
	return ac.main();
}
