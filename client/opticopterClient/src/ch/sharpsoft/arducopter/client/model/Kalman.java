package ch.sharpsoft.arducopter.client.model;

public class Kalman {
	// static int Q_angle = 303715; // variance accel // Process noise variance for the accelerometer
	// static int Q_bias = 10962;// Process noise variance for the gyro bias
	// static int R_measure = 10962; // variance gyro // Measurement noise variance - this is actually the variance of the measurement noise
	public static int Q_angle = 1; // variance accel // Process noise variance for the accelerometer
	public static int Q_bias = 1;// Process noise variance for the gyro bias
	public static int R_measure = 0; // variance gyro // Measurement noise variance - this is actually the variance of the measurement noise
	public static int TIME = 1;

	long bias = 0; // Reset bias
	long P[][] = new long[2][2];

	long rate = 0;
	int angle = 0;
	long[] K = new long[2]; // Kalman gain - This is a 2x1 matrix
	long y; // Angle difference - 1x1 matrix
	long S; // Estimate error - 1x1 matrix

	public Kalman() {
		P[0][0] = 0; // Since we assume tha the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
		P[0][1] = 0;
		P[1][0] = 0;
		P[1][1] = 0;
	}

	public int getNext(final int newValue, final int diff) {
		// KasBot V2 - Kalman filter module - http://www.x-firm.com/?page_id=145
		// Modified by Kristian Lauszus
		// See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

		// Discrete Kalman filter time update equations - Time Update ("Predict")
		// Update xhat - Project the state ahead
		/* Step 1 */
		rate = diff - bias;
		angle += rate;
		angle += rate / TIME;

		// Update estimation error covariance - Project the error covariance ahead
		/* Step 2 */
		P[0][0] += (P[1][1] / TIME - P[0][1] - P[1][0] + Q_angle) / TIME;
		P[0][1] -= P[1][1] / TIME;
		P[1][0] -= P[1][1] / TIME;
		P[1][1] += Q_bias / TIME;

		// Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
		// Calculate Kalman gain - Compute the Kalman gain
		/* Step 4 */
		S = P[0][0] + R_measure;
		/* Step 5 */
		K[0] = P[0][0] / S;
		K[1] = P[1][0] / S;

		// Calculate angle and bias - Update estimate with measurement zk (newAngle)
		/* Step 3 */
		y = newValue - angle;
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
