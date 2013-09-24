package ch.sharpsoft.arducopter.client.model;

import ch.sharpsoft.arducopter.client.view.Matrix;

public class KalmanModel {
	/**
	 * Predicted state k at k-1
	 */
	private final double[][] Xk = new double[3][3];

	/**
	 * New measured state
	 */
	private final double[][] Xnew = new double[3][3];

	/**
	 * rate k
	 */
	private final double[][] Uk = new double[3][3];

	/**
	 * Identity matrix
	 */
	private final double[][] I = new double[3][3];

	/**
	 * Covariance matrix
	 */
	private final double[][] Pk = new double[3][3];

	/**
	 * Measurement noise (linked to accelerometer)
	 */
	private final double[][] R = new double[3][3];

	/**
	 * Processing noise (linked to gyroscope)
	 */
	private final double[][] Q = new double[3][3];

	/**
	 * Innovation covariance / Temp
	 */
	private final double[][] S = new double[3][3];

	/**
	 * Kalman Gain
	 */
	private final double[][] K = new double[3][3];

	/**
	 * Observation
	 */
	private final double[][] Yk = new double[3][3];

	public KalmanModel() {
		// Kalman filter matrix initializations
		Pk[0][0] = 1;
		Pk[1][1] = 1;
		Pk[2][2] = 1;

		// Identity matrix
		I[0][0] = 1;
		I[1][1] = 1;
		I[2][2] = 1;

		// /////////////// COVARIANCE NOISE MATRICES /////////////////
		// Measurement noise (accelerometer)
		R[0][0] = 0.5;
		R[1][1] = 0.5;
		R[2][2] = 0.01;

		// Processing noise (gyro)
		Q[0][0] = 0.005;
		Q[1][1] = 0.005;
		Q[2][2] = 0.0001;
	}

	public void putData(final double[] pitchRollYaw, final double[] pitchRollYawGyro) {
		Xnew[0][0] = pitchRollYaw[0];
		Xnew[1][1] = pitchRollYaw[1];
		Xnew[2][2] = pitchRollYaw[2];

		Uk[0][0] = pitchRollYawGyro[0];
		Uk[1][1] = pitchRollYawGyro[1];
		Uk[2][2] = pitchRollYawGyro[2];

		// //////Variables declaration

		// //////PREDICTION
		// Predicted (a priori) state estimate:
		Matrix.plus(Xk, Uk, Xk);

		// Predicted (a priori) estimate covariance
		Matrix.plus(Pk, Q, Pk);

		// //////UPDATE
		// Innovation or measurement residual
		Matrix.minus(Xnew, Xk, Yk);

		// Innovation covariance
		Matrix.plus(Pk, R, S);
		// Optimal Kalman Gain
		Matrix.invert3x3(S, S);
		Matrix.multiply(Pk, S, K);
		// Updated state estimate
		Matrix.multiply(K, Yk, S);
		Matrix.plus(Xk, S, Xk);
		// Updated estimate covariance
		Matrix.minus(I, K, S);
		Matrix.multiply(S, Pk, Pk);

		System.err.println("Kalman pitch/roll/yaw " + Xk[0][0] + " " + Xk[1][1] + " " + Xk[2][2]);
	}
}
