package ch.sharpsoft.arducopter.client.model;

public class Kalman {
	/* Kalman filter variables */
	public static double Q_angle = 0.001;// Process noise variance for the accelerometer
	public static double Q_bias = 0.003; // Process noise variance for the gyro bias
	public static double R_measure = 0.0005; // Measurement noise variance - this is actually the variance of the measurement noise

	double angle; // The angle calculated by the Kalman filter - part of the 2x1 state matrix
	double bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state matrix
	double rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

	double[][] P = new double[2][2]; // Error covariance matrix - This is a 2x2 matrix
	double[] K = new double[2]; // Kalman gain - This is a 2x1 matrix
	double y; // Angle difference - 1x1 matrix
	double S; // Estimate error - 1x1 matrix

	// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
	double getAngle(final double newAngle, final double newRate, final double dt) {
		// KasBot V2 - Kalman filter module - http://www.x-firm.com/?page_id=145
		// Modified by Kristian Lauszus
		// See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

		// Discrete Kalman filter time update equations - Time Update ("Predict")
		// Update xhat - Project the state ahead
		/* Step 1 */
		rate = newRate - bias;
		angle += dt * rate;

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
	};
}
