/*
 * Kalman.cpp
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#include "Kalman.h"
#include "Arduino.h"

float Kalman::getAngle(float newAngle, float newRate, float dt) {
	// KasBot V2 - Kalman filter module - http://www.x-firm.com/?page_id=145
	// Modified by Kristian Lauszus
	// See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

	// Discrete Kalman filter time update equations - Time Update ("Predict")
	// Update xhat - Project the state ahead
	/* Step 1 */
	rate = newRate - bias;
	angle += dt * rate;

	while (angle > PI) {
		angle -= PI * 2;
	}
	while (angle < -PI) {
		angle += PI * 2;
	}

	// Update estimation error covariance - Project the error covariance ahead
	/* Step 2 */
	P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
	P[0][1] -= dt * P[1][1];
	P[1][0] -= dt * P[1][1];
	P[1][1] += Q_bias * dt;

	// Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
	// Calculate Kalman gain - Compute the Kalman gain
	/* Step 4 */
	S = P[0][0] + R_measure;
	/* Step 5 */
	K[0] = P[0][0] / S;
	K[1] = P[1][0] / S;

	// Calculate angle and bias - Update estimate with measurement zk (newAngle)
	/* Step 3 */
	y = newAngle - angle;
	/* Step 6 */
	angle += K[0] * y;
	bias += K[1] * y;

	// Calculate estimation error covariance - Update the error covariance
	/* Step 7 */
	P[0][0] -= K[0] * P[0][0];
	P[0][1] -= K[0] * P[0][1];
	P[1][0] -= K[1] * P[0][0];
	P[1][1] -= K[1] * P[0][1];
	return angle;
}

float Kalman::getLastRate() {
	return rate;
}
