/*
 * DCM.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: helios
 */

#include "DCM.h"

void DCM::init() {
	for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
		kinematicsAngle[axis] = 0.0;
		gyroAngle[axis] = 0.0;
	}

	q0 = 1.0;
	q1 = 0.0;
	q2 = 0.0;
	q3 = 0.0;
	exInt = 0.0;
	eyInt = 0.0;
	ezInt = 0.0;

	previousEx = 0;
	previousEy = 0;
	previousEz = 0;

	Kp = 0.2; // 2.0;
	Ki = 0.0005; //0.005;
}

void DCM::argUpdate(float gx, float gy, float gz, float ax, float ay, float az, float G_Dt) {
	float norm;
	float vx, vy, vz;
	float q0i, q1i, q2i, q3i;
	float ex, ey, ez;

	halfT = G_Dt / 2;

	// normalise the measurements
	norm = sqrt(ax * ax + ay * ay + az * az);
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	// estimated direction of gravity and flux (v and w)
	vx = 2 * (q1 * q3 - q0 * q2);
	vy = 2 * (q0 * q1 + q2 * q3);
	vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (vy * az - vz * ay);
	ey = (vz * ax - vx * az);
	ez = (vx * ay - vy * ax);

	// integral error scaled integral gain
	exInt = exInt + ex * Ki;
	if (isSwitched(previousEx, ex)) {
		exInt = 0.0;
	}
	previousEx = ex;

	eyInt = eyInt + ey * Ki;
	if (isSwitched(previousEy, ey)) {
		eyInt = 0.0;
	}
	previousEy = ey;

	ezInt = ezInt + ez * Ki;
	if (isSwitched(previousEz, ez)) {
		ezInt = 0.0;
	}
	previousEz = ez;

	// adjusted gyroscope measurements
	gx = gx + Kp * ex + exInt;
	gy = gy + Kp * ey + eyInt;
	gz = gz + Kp * ez + ezInt;

	// integrate quaternion rate and normalise
	q0i = (-q1 * gx - q2 * gy - q3 * gz) * halfT;
	q1i = (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2i = (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3i = (q0 * gz + q1 * gy - q2 * gx) * halfT;
	q0 += q0i;
	q1 += q1i;
	q2 += q2i;
	q3 += q3i;

	// normalise quaternion
	norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	correctedRateVector[XAXIS] = gx;
	correctedRateVector[YAXIS] = gy;
	correctedRateVector[ZAXIS] = gz;
}

void DCM::eulerAngles(float* xyz) {
	kinematicsAngle[XAXIS] = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
	kinematicsAngle[YAXIS] = asin(2 * (q0 * q2 - q1 * q3));
	kinematicsAngle[ZAXIS] = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));
}

void DCM::getGyroUnbias(float* xyz) {
	xyz[0] = correctedRateVector[0];
	xyz[1] = correctedRateVector[1];
	xyz[2] = correctedRateVector[2];
}
