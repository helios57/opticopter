package ch.sharpsoft.arducopter.client.math;

import static java.lang.Math.abs;

public class FpMath {
	static int factor = 1 << 29;// (int) Math.pow(2, 29);
	static int coeff_1 = (int) ((Math.PI / 4) * factor);
	static int coeff_2 = 3 * coeff_1;

	/**
	 * @return in rad * 2^29
	 */
	public static int arctan2(final int y, final int x) {
		int angle;
		int abs_y = abs(y);
		if (x >= 0) {
			if (((x + abs_y) == 0)) {
				abs_y++;
			}
			int r = (int) ((long) coeff_1 * (x - abs_y)) / (x + abs_y);
			angle = coeff_1 - coeff_1 * r;
		} else {
			if (((abs_y - x) == 0)) {
				abs_y++;
			}
			int r = (int) ((long) coeff_1 * (x + abs_y)) / (abs_y - x);
			angle = coeff_2 - r;
		}
		if (y < 0)
			return (-angle); // negate if in quad III or IV
		else
			return (angle);
	}
}
