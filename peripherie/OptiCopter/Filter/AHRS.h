/*
 * AHRS.h
 * Implementation of Madgwick's IMU and AHRS algorithms.
 * See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
 */

#ifndef AHRS_H_
#define AHRS_H_
#include <Arduino.h>

class AHRS {
private:
	float beta; // 2 * proportional gain (Kp)
	float q0, q1, q2, q3; // quaternion of sensor frame relative to auxiliary frame

	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
public:
	AHRS() :
			beta(0.03f), q0(1.0f), q1(0.0f), q2(0.0f), q3(0.0f) {
	}
	virtual ~AHRS() {
	}
	void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az, float dt);
	void getQuaternion(float* wxyz);
	void getEuler(float* rollPitchYaw);
};

#endif /* AHRS_H_ */
