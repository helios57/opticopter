/*
 * DCM.h
 *
 *  Created on: Nov 18, 2013
 *      Author: helios
 */

#ifndef DCM_H_
#define DCM_H_

#include "AQMath.h"

class DCM {
private:
	static const byte XAXIS = 0;
	static const byte YAXIS = 1;
	static const byte ZAXIS = 2;

	float Kp;                   					// proportional gain governs rate of convergence to accelerometer/magnetometer
	float Ki;                   					// integral gain governs rate of convergence of gyroscope biases
	float halfT;                					// half the sample period
	float q0, q1, q2, q3;       				    // quaternion elements representing the estimated orientation
	float exInt, eyInt, ezInt;  		            // scaled integral error

	float previousEx;
	float previousEy;
	float previousEz;

	byte kinematicsType;
	float kinematicsAngle[3];
	float gyroAngle[3];
	float correctedRateVector[3];
	float earthAccel[3];

	float accelCutoff;
public:
	DCM() {
		Kp = 0.0;
		Ki = 0.0;
		halfT = 0.0;
		q0 = 0.0, q1 = 0.0, q2 = 0.0, q3 = 0.0;
		exInt = 0.0, eyInt = 0.0, ezInt = 0.0;

		previousEx = 0.0;
		previousEy = 0.0;
		previousEz = 0.0;

		kinematicsType = 0;
		for (int i = 0; i < 3; i++) {
			kinematicsAngle[i] = 0.0;
			gyroAngle[i] = 0.0;
			correctedRateVector[i] = 0.0;
			earthAccel[i] = 0.0;
		}
		accelCutoff = 0.0;
	}
	~DCM() {

	}
	void init();
	void argUpdate(float gx, float gy, float gz, float ax, float ay, float az, float G_Dt);
	void eulerAngles(float *xyz);
	void getGyroUnbias(float *xyz);
};

#endif /* DCM_H_ */
