package ch.sharpsoft.arducopter.client.view;

public class Quaternion {

	public static double[] vector_cross(final double[] xyz_a, final double[] xyz_b) {
		double[] result = new double[3];
		result[0] = xyz_a[1] * xyz_b[2] - xyz_a[2] * xyz_b[1];
		result[1] = xyz_a[2] * xyz_b[0] - xyz_a[0] * xyz_b[2];
		result[0] = xyz_a[0] * xyz_b[1] - xyz_a[1] * xyz_b[0];
		return result;
	}

	public static double vector_dot(final double[] xyz_a, final double[] xyz_b) {
		return xyz_a[0] * xyz_b[0] + xyz_a[1] * xyz_b[1] + xyz_a[2] * xyz_b[2];
	}

	public static double norm(final double[] wxyz) {
		return Math.sqrt(wxyz[0] * wxyz[0] + wxyz[1] * wxyz[1] + wxyz[2] * wxyz[2] + wxyz[3] * wxyz[3]);
	}

	public static double normVec(final double[] xyz) {
		return Math.sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
	}

	public static double normVec(final int[] xyz) {
		return Math.sqrt((double) xyz[0] * (double) xyz[0] + (double) xyz[1] * (double) xyz[1] + (double) xyz[2] * (double) xyz[2]);
	}

	public static final double[] normalize(final double[] wxyz) {
		double norm = norm(wxyz);
		final double[] tmp = new double[4];
		for (int i = 0; i < 4; i++) {
			tmp[i] = wxyz[i] / norm;
		}
		return tmp;
	}

	public static final double[] normalizeVec(final double[] xyz) {
		double norm = normVec(xyz);
		final double[] tmp = new double[3];
		for (int i = 0; i < 3; i++) {
			tmp[i] = xyz[i] / norm;
		}
		return tmp;
	}

	public static final void normalizeVec(final int[] in_xyz, final double[] out_xyz) {
		double norm = normVec(in_xyz);
		for (int i = 0; i < 3; i++) {
			out_xyz[i] = in_xyz[i] / norm;
		}
	}

	public static double[] conjugate(final double[] wxyz) {
		return new double[] { wxyz[0], -wxyz[1], -wxyz[2], -wxyz[3] };
	}

	public static double[] plus(final double[] a_wxyz, final double[] b_wxyz) {
		return new double[] { a_wxyz[0] + b_wxyz[0], a_wxyz[1] + b_wxyz[1], a_wxyz[2] + b_wxyz[2], a_wxyz[3] + b_wxyz[3] };
	}

	public static double[] multiply(final double[] a_wxyz, final double[] b_wxyz) {
		double w0 = a_wxyz[0] * b_wxyz[0] - a_wxyz[1] * b_wxyz[1] - a_wxyz[2] * b_wxyz[2] - a_wxyz[3] * b_wxyz[3];
		double x0 = a_wxyz[0] * b_wxyz[1] + a_wxyz[1] * b_wxyz[0] + a_wxyz[2] * b_wxyz[3] - a_wxyz[3] * b_wxyz[2];
		double y0 = a_wxyz[0] * b_wxyz[2] - a_wxyz[1] * b_wxyz[3] + a_wxyz[2] * b_wxyz[0] + a_wxyz[3] * b_wxyz[1];
		double z0 = a_wxyz[0] * b_wxyz[3] + a_wxyz[1] * b_wxyz[2] - a_wxyz[2] * b_wxyz[1] + a_wxyz[3] * b_wxyz[0];
		return new double[] { w0, x0, y0, z0 };
	}

	public static double[] multiplyVec(final double[] xyz, final double[] wxyz) {
		double[] vec = new double[] { 0, xyz[0], xyz[1], xyz[2] };
		double[] res = multiply(multiply(wxyz, vec), conjugate(wxyz));
		return new double[] { res[1], res[2], res[3] };
	}

	public static double[] diff(final double[] a_wxyz, final double[] b_wxyz) {
		return normalize(multiply(normalize(conjugate(a_wxyz)), normalize(b_wxyz)));
	}

	public static double[] fromAxis(final double[] xyz, final double angle) {
		normalizeVec(xyz);
		double sinAngle = Math.sin(angle / 2);
		double x = (xyz[0] * sinAngle);
		double y = (xyz[1] * sinAngle);
		double z = (xyz[2] * sinAngle);
		double w = Math.cos(angle / 2);
		return new double[] { w, x, y, z };
	}

	/**
	 * 
	 * @param pitch
	 *            in radiants
	 * @param yaw
	 *            in radiants
	 * @param roll
	 *            in radiants
	 * @return
	 */
	public static double[] fromEuler(final double pitch, final double yaw, final double roll) {
		// Basically we create 3 Quaternions, one for pitch, one for yaw, one
		// for roll
		// and multiply those together.
		// the calculation below does the same, just shorter

		double p = (pitch / 2.0);
		double y = (yaw / 2.0);
		double r = (roll / 2.0);

		double sinp = Math.sin(p);
		double siny = Math.sin(y);
		double sinr = Math.sin(r);
		double cosp = Math.cos(p);
		double cosy = Math.cos(y);
		double cosr = Math.cos(r);

		double _x = sinr * cosp * cosy - cosr * sinp * siny;
		double _y = cosr * sinp * cosy + sinr * cosp * siny;
		double _z = cosr * cosp * siny - sinr * sinp * cosy;
		double _w = cosr * cosp * cosy + sinr * sinp * siny;

		double[] result = new double[] { _w, _x, _y, _z };
		normalize(result);
		return result;
	}
}