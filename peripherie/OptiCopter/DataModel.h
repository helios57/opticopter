/*
 * DataModel.h
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#ifndef DATAMODEL_H_
#define DATAMODEL_H_

#include "../Libraries/Hal/HalApm.h"
using namespace arducopterNg;

class DataModel {
private:
	HalApm *hal;
	int32_t accel[3];
	int16_t mag[3];
	double accelInG[3];
	double quatCurrent[4];
	double quatLevel[4];
	double quatShould[4];
	double quatDiff[4];
	unsigned short input[8];
	unsigned short inputMax[8];
	unsigned short inputMin[8];
	double correctionThrust[8];
	double magScaled[3];
	double magCompensated[3];
	int magMax[3]; // { 710, 424, 435 };
	int magMin[3]; // { -503, -768, -553 };
	double rollPitchYaw[3];

	static const double declinationAngle = -0.02472549;
public:
	DataModel(HalApm *hal) :
			hal(hal) {
	}
	virtual ~DataModel() {
	}
	void calculate();
};
#endif /* DATAMODEL_H_ */

