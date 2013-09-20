/*
 * Persistence.h
 *
 *  Created on: Sep 16, 2013
 *      Author: helios
 */

#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_
#include <stdint.h>

static const uint16_t LOC_MAG_MAX = 0; //-4
static const uint16_t LOC_MAG_MIN = 6; //-10
static const uint16_t LOC_RC_IN_MAX = 12; //-26
static const uint16_t LOC_RC_IN_MIN = 28; //42

class Persistence {
public:
	Persistence() {
	}
	virtual ~Persistence() {
	}
	void saveMagMax(uint16_t *xyz);
	void saveMagMin(uint16_t *xyz);
	void readMagMax(uint16_t *xyz);
	void readMagMin(uint16_t *xyz);
	void saveRcMax(uint8_t ch, uint16_t max);
	void saveRcMin(uint8_t ch, uint16_t min);
	uint16_t readRcMax(uint8_t ch);
	uint16_t readRcMin(uint8_t ch);
};

#endif /* PERSISTENCE_H_ */
