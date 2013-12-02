/*
 * Kalman.h
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#ifndef KALMAN_H_
#define KALMAN_H_

namespace opticopter {
	class Kalman {
	private:
		float angle; // The angle calculated by the Kalman filter - part of the 2x1 state matrix
		float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state matrix
		float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

		float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
		float K[2]; // Kalman gain - This is a 2x1 matrix
		float y; // Angle difference - 1x1 matrix
		float S; // Estimate error - 1x1 matrix
	public:
		float Q_angle; // Process noise variance for the accelerometer
		float Q_bias; // Process noise variance for the gyro bias
		float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

		Kalman() {
			Q_angle = 0.1; // Process noise variance for the accelerometer
			Q_bias = 0.01; // Process noise variance for the gyro bias
			R_measure = 0.2; // Measurement noise variance - this is actually the variance of the measurement noise
			angle = 0;
			bias = 0;
			rate = 0;
			y = 0;
			S = 0;
			P[0][0] = 0;
			P[0][1] = 0;
			P[1][0] = 0;
			P[1][1] = 0;
			K[0] = 0;
			K[1] = 0;
		}
		float getAngle(float newAngle, float newRate, float dt);
		float getLastRate();
	};
}
#endif /* KALMAN_H_ */
