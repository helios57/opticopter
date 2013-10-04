package ch.sharpsoft.arducopter.client.model;

public class KalmanModel1D {
	private double q; // process noise covariance
	private double r; // measurement noise covariance
	private double x; // value
	private double p; // estimation error covariance
	private double k; // kalman gain

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

	public final double getQ() {
		return q;
	}

	public final void setQ(final double q) {
		this.q = q;
	}

	public final double getR() {
		return r;
	}

	public final void setR(final double r) {
		this.r = r;
	}

	public final double getX() {
		return x;
	}

	public final void setX(final double x) {
		this.x = x;
	}

	public final double getP() {
		return p;
	}

	public final void setP(final double p) {
		this.p = p;
	}

	public final double getK() {
		return k;
	}

	public final void setK(final double k) {
		this.k = k;
	}
}
