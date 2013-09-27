package ch.sharpsoft.arducopter.client.model;

public class KalmanModel1D {
	double q; // process noise covariance
	double r; // measurement noise covariance
	double x; // value
	double p; // estimation error covariance
	double k; // kalman gain

	public KalmanModel1D(final double q, final double r, final double p, final double intial_value) {
		this.q = q;
		this.r = r;
		this.p = p;
		this.x = intial_value;
	}

	public void update(final double measurement) {
		if (measurement > -4000 && measurement < 4000) {
			// prediction update
			// omit x = x
			p = p + q;

			// measurement update
			k = p / (p + r);
			x = x + k * (measurement - x);
			p = (1 - k) * p;
		}
	}

	public double getValue() {
		return x;
	}
}
