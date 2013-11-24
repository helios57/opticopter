/*
 * Heading.h
 *
 *  Created on: Nov 24, 2013
 *      Author: helios
 */

#ifndef HEADING_H_
#define HEADING_H_
#include <Arduino.h>

class Heading {
private:
	static const uint8_t ringIndexMax = 10;
	float yawRing[ringIndexMax];
	float magCompensated[3];
	uint8_t ringIndex;
	float declinationAngle;
public:
	void updateHeading(float roll, float pitch, float gyroZ, float* magXYZ, float dt);
	float getYaw();
	Heading() :
			ringIndex(0) {
		for (uint8_t i = 0; i < ringIndexMax; i++) {
			yawRing[i] = 0;
		}
		declinationAngle = -0.02472549;
	}
	virtual ~Heading() {
	}
};

#endif /* HEADING_H_ */
