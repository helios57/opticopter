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
	arducopterNg::ArducopterNg ac;
	return ac.main();
}
