/*
 * AHRS.h
 *  Implementation of Madgwick's IMU and AHRS algorithms.
 * See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
 *
 * Date			Author          Notes
 * 29/09/2011	SOH Madgwick    Initial release
 * 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
 *
 */

#ifndef AHRS_H_
#define AHRS_H_

class AHRS {
private:
	float beta; // algorithm gain
	float q0, q1, q2, q3; // quaternion of sensor frame relative to auxiliary frame
public:
	AHRS() :
			beta(0.1f), q0(1.0), q1(0.0), q2(0.0), q3(0.0) {
	}
	virtual ~AHRS() {
	}
	void update(float gx, float gy, float gz, float ax, float ay, float az,
			float dt);
	void update(float gx, float gy, float gz, float ax, float ay, float az,
			float mx, float my, float mz, float dt);
	float getRoll();
	float getPitch();
	float getYaw();
};

#endif /* AHRS_H_ */
