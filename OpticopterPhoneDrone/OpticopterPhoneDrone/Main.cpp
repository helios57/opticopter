/*
 * Main.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: helios
 */

#include "PhoneDrohne/PhoneDrohne.h"
#include "Arduino/Arduino.h"

int main(void) {
	init();
	PhoneDrohne pd;
	return pd.main();
}
