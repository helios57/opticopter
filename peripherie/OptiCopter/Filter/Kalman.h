/*
 * Kalman.h
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#ifndef KALMAN_H_
#define KALMAN_H_

class Kalman {
private:
	double Q_angle; // Process noise variance for the accelerometer
	double Q_bias; // Process noise variance for the gyro bias
	double R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

	double angle; // The angle calculated by the Kalman filter - part of the 2x1 state matrix
	double bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state matrix
	double rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

	double P[2][2]; // Error covariance matrix - This is a 2x2 matrix
	double K[2]; // Kalman gain - This is a 2x1 matrix
	double y; // Angle difference - 1x1 matrix
	double S; // Estimate error - 1x1 matrix
public:
	Kalman() {
		Q_angle = 0.001; // Process noise variance for the accelerometer
		Q_bias = 0.003; // Process noise variance for the gyro bias
		R_measure = 0.0005; // Measurement noise variance - this is actually the variance of the measurement noise
		angle = 0;
		bias = 0;
		rate = 0;
		y = 0;
		S = 0;
	}
	double getAngle(double newAngle, double newRate, double dt);
}
;

#endif /* KALMAN_H_ */
