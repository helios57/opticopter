package ch.sharpsoft.arducopter.client.model;

import static java.lang.Math.atan2;
import static java.lang.Math.cos;
import static java.lang.Math.sin;

public class Calculator {
	// private final static double GYRO_TO_RPS_FACTOR = 16.4 * (1L << 16) * 0.0174532925;
	// private final static double GYRO_TO_RAD_PER_5MS_FACTOR = 498202378.5;// magical factor
	private final static double GYRO_TO_RAD_PER_S_FACTOR = 0.00106413;// magical factor

	/**
	 * accel input
	 */
	public static void calc5ms(final DataRaw dr, final DataCalculated dc) {
		calcRollPitchYaw(dr, dc);
		calcGyroRps(dr, dc);
		calcKalman(dr, dc);
	}

	public static void calc20ms(final DataRaw dr, final DataCalculated dc) {
		calcOutput(dr, dc);

	}

	private static void calcOutput(final DataRaw dr, final DataCalculated dc) {
		for (int i = 0; i < 3; i++) {
			dc.getRollPitchYawOutput()[i] = dc.getPid()[i].update(dc.getRollPitchYawLevel()[i], dc.getRollPitchYawFiltered()[i]);
		}
	}

	private static void calcKalman(final DataRaw dr, final DataCalculated dc) {
		dc.getRollPitchYawFiltered()[0] = dc.getRollPitchYawKalman()[0].getAngle(dc.getRollPitchYaw()[0], dc.getGyroRps()[1], 0.005);
		dc.getRollPitchYawFiltered()[1] = dc.getRollPitchYawKalman()[1].getAngle(dc.getRollPitchYaw()[1], dc.getGyroRps()[0], 0.005);
		dc.getRollPitchYawFiltered()[2] = dc.getRollPitchYawKalman()[2].getAngle(dc.getRollPitchYaw()[2], dc.getGyroRps()[2], 0.005);
		dc.saveRollPitchYawFiltered();
	}

	private static void calcGyroRps(final DataRaw dr, final DataCalculated dc) {
		for (int i = 0; i < 3; i++) {
			dc.getGyroRps()[i] = dr.getMotion6()[3 + i] * GYRO_TO_RAD_PER_S_FACTOR;
		}
		dc.saveGyroRps();
	}

	private static void calcRollPitchYaw(final DataRaw dr, final DataCalculated dc) {
		dc.getRollPitchYaw()[0] = atan2(-dr.getMotion6()[0], dr.getMotion6()[2]);
		dc.getRollPitchYaw()[1] = atan2(dr.getMotion6()[1], dr.getMotion6()[2]);
		// Scale Mag to [-1;1]
		for (int i = 0; i < 3; i++) {
			dc.getMagScaled()[i] = (((double) (dr.getMag()[i]) - dc.getMagMin()[i]) / (dc.getMagMax()[i] - dc.getMagMin()[i])) * 2 - 1.0;
		}
		dc.getMagScaled()[2] = -dc.getMagScaled()[2];
		double rollSin = sin(dc.getRollPitchYaw()[0]);
		double rollCos = cos(dc.getRollPitchYaw()[0]);
		double pitchSin = sin(dc.getRollPitchYaw()[1]);
		double pitchCos = cos(dc.getRollPitchYaw()[1]);
		if (rollCos < 0) {
			rollCos = -rollCos;
		}
		dc.getMagCompensated()[1] = dc.getMagScaled()[1] * pitchCos - dc.getMagScaled()[2] * pitchSin;
		dc.getMagCompensated()[2] = dc.getMagScaled()[1] * pitchSin + dc.getMagScaled()[2] * pitchCos;
		dc.getMagCompensated()[0] = dc.getMagScaled()[0] * rollCos + dc.getMagScaled()[2] * rollSin;// check ob compensated or scaled
		dc.getRollPitchYaw()[2] = atan2(dc.getMagCompensated()[1], dc.getMagCompensated()[0]);
		dc.saveRollPitchYaw();
	}

	public static double[] getRollPitchYawVariance(final DataCalculated dc) {
		return getVariance(dc.getRollPitchYawHistory());
	}

	public static double[] getRollPitchYawFilteredVariance(final DataCalculated dc) {
		return getVariance(dc.getRollPitchYawFilteredHistory());
	}

	public static double[] getGyroRpsVariance(final DataCalculated dc) {
		return getVariance(dc.getGyroRpsHistory());
	}

	private static double[] getVariance(final double[][] input) {
		double sum[] = new double[3];
		double sum2[] = new double[3];
		double variance[] = new double[3];
		for (double[] sample : input) {
			for (int i = 0; i < 3; i++) {
				sum[i] += sample[i];
				sum2[i] += sample[i] * sample[i];
			}
		}
		int length = input.length;
		for (int i = 0; i < 3; i++) {
			variance[i] = (sum2[i] - ((sum[i] * sum[i])) / length) / (length - 1);
		}
		return variance;
	}
}