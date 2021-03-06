package ch.sharpsoft.arducopter.client.uart;

import static ch.sharpsoft.arducopter.client.view.Quaternion.conjugate;
import static ch.sharpsoft.arducopter.client.view.Quaternion.multiply;
import static ch.sharpsoft.arducopter.client.view.Quaternion.multiplyVec;
import static ch.sharpsoft.arducopter.client.view.Quaternion.normalize;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;

import ch.sharpsoft.arducopter.client.math.FpMath;
import ch.sharpsoft.arducopter.client.model.Kalman;
import ch.sharpsoft.arducopter.client.model.KalmanModel;
import ch.sharpsoft.arducopter.client.model.KalmanModel1D;
import ch.sharpsoft.arducopter.client.model.PID;
import ch.sharpsoft.arducopter.client.view.Quaternion;

public class DatenModel {
	private DatenModel() {
		for (int i = 0; i < 4; i++) {
			outputK[i] = new KalmanModel1D(0.002, 1.0, 0.00, 0);
		}
		for (int i = 0; i < 3; i++) {
			rollPitchYawPid[i] = new PID(0.6, 0.05, 0.02);
			rollPitchYawIntKalman[i] = new Kalman();
		}
	}

	private final static DatenModel instance = new DatenModel();

	private final List<NewDataListener> events = new ArrayList<>();

	public void addEventListener(final NewDataListener eventListener) {
		events.add(eventListener);
	}

	private final int[] accel = new int[3];
	private final int[] accelK = new int[3];
	private final int[] accelLast = new int[3];
	private final int[] accelDiff = new int[3];
	private final int[] rollPitchYawInt = new int[3];
	private final Kalman[] rollPitchYawIntKalman = new Kalman[3];
	private final int[] gyro = new int[3];
	private final double[] accelRelative = new double[3];
	private final double[] accelInMss = new double[3];
	private final double[] gyroInDegreeSec = new double[3];
	private final double[] quat = new double[4];
	private final double[] quatLevel = new double[] { 1.0, 0, 0, 0 };
	private final double[] quatDiff = new double[4];
	private final double[] thrust = new double[4];
	private final short[] mag = new short[3];
	private final double[] magScaled = new double[3];
	private final double[] magCompensated = new double[3];
	private final short[] magMax = new short[] { 710, 424, 435 };
	private final short[] magMin = new short[] { -503, -768, -553 };
	private final float[] baro = new float[1];
	private final GPSData gps = new GPSData();
	private final ConcurrentLinkedQueue<String> debugInfos = new ConcurrentLinkedQueue<>();
	private final long[] cycles = new long[1];
	private final double[] rollPitchYaw = new double[3];
	private final double[] rollPitchYawLevel = new double[3];
	private final double[] rollPitchYawLast = new double[3];
	private final double[] rollPitchYawDiff = new double[3];
	private final double[] rollPitchYawDiffLast = new double[3];
	private final double[] rollPitchYawDiffDiff = new double[3];
	private final double[] rollPitchYawKalman = new double[3];

	private final double[] posXYZLevel = new double[3];
	private final double[] posXYZRelative = new double[3];
	private final double[] posXYZ = new double[3];
	private final double[] posXYZLast = new double[3];
	private final double[] posXYZDiff = new double[3];
	private final double[] posXYZDiffLast = new double[3];
	private final double[] posXYZDiffDiff = new double[3];

	private final short[] input = new short[8];
	private final short[] output = new short[8];
	private final KalmanModel1D[] outputK = new KalmanModel1D[4];
	private final long[] lastTimestamp = new long[1];
	private final double[] pXYZ = new double[] { 0.5, 0.5, 0.5 };
	private final double[] pRollPitchYaw = new double[] { 0.5, 0.5, 0.5 };
	private final KalmanModel kalmanModel = new KalmanModel();
	private final KalmanModel1D kalmanModel1D = new KalmanModel1D(0.004, 8.0, 0.46, 500.0);
	private final KalmanModel1D kalmanModel1Dpitch = new KalmanModel1D(0.004, 1.0, 0.46, 1);
	private final KalmanModel1D kalmanModel1Droll = new KalmanModel1D(0.004, 1.0, 0.46, 1);
	private final PID[] rollPitchYawPid = new PID[3];

	public void calculate() {
		backupLastValues();
		calculateAccel();
		calculateGyro();
		calculateMag();
		calculateAltitude();
		calculateDiffs();
		calculateQuaternion();
		calculateOutput();
		for (NewDataListener event : events) {
			event.onNewData();
		}
		lastTimestamp[0] = System.currentTimeMillis();
	}

	private void calculateAltitude() {
		kalmanModel1D.update(baro[0]);
		posXYZ[2] = kalmanModel1D.getValue();
	}

	private void calculateDiffs() {
		for (int i = 0; i < 3; i++) {
			accelDiff[i] = accel[i] - accelLast[i];
		}

		rollPitchYawDiff[0] = rollPitchYaw[0] - rollPitchYawLast[0];
		rollPitchYawDiff[1] = rollPitchYaw[1] - rollPitchYawLast[1];
		rollPitchYawDiff[2] = rollPitchYaw[2] - rollPitchYawLast[2];
		rollPitchYawDiffDiff[0] = rollPitchYawDiff[0] - rollPitchYawDiffLast[0];
		rollPitchYawDiffDiff[1] = rollPitchYawDiff[1] - rollPitchYawDiffLast[1];
		rollPitchYawDiffDiff[2] = rollPitchYawDiff[2] - rollPitchYawDiffLast[2];

		posXYZDiff[0] = posXYZ[0] - posXYZLast[0];
		posXYZDiff[1] = posXYZ[1] - posXYZLast[1];
		posXYZDiff[2] = posXYZ[2] - posXYZLast[2];
		posXYZDiffDiff[0] = posXYZDiff[0] - posXYZDiffLast[0];
		posXYZDiffDiff[1] = posXYZDiff[1] - posXYZDiffLast[1];
		posXYZDiffDiff[2] = posXYZDiff[2] - posXYZDiffLast[2];

		posXYZRelative[0] = posXYZ[0] - posXYZLevel[0];
		posXYZRelative[1] = posXYZ[1] - posXYZLevel[1];
		posXYZRelative[2] = posXYZ[2] - posXYZLevel[2];
	}

	private void backupLastValues() {
		rollPitchYawLast[0] = rollPitchYaw[0];
		rollPitchYawLast[1] = rollPitchYaw[1];
		rollPitchYawLast[2] = rollPitchYaw[2];
		rollPitchYawDiffLast[0] = rollPitchYawDiff[0];
		rollPitchYawDiffLast[1] = rollPitchYawDiff[1];
		rollPitchYawDiffLast[2] = rollPitchYawDiff[2];

		posXYZLast[0] = posXYZ[0];
		posXYZLast[1] = posXYZ[1];
		posXYZLast[2] = posXYZ[2];
		posXYZDiffLast[0] = posXYZDiff[0];
		posXYZDiffLast[1] = posXYZDiff[1];
		posXYZDiffLast[2] = posXYZDiff[2];

	}

	private void calculateOutput() {
		double altitudeDiff = posXYZLevel[2] - posXYZ[2];
		long timeDiff = System.currentTimeMillis() - lastTimestamp[0];
		/*
		 * double rollA = rollPitchYawPid[0].updatePID(rollPitchYawLevel[0], rollPitchYawK[0], timeDiff); double pitchA = rollPitchYawPid[1].updatePID(rollPitchYawLevel[1], rollPitchYaw[1], timeDiff); double yawA =
		 * rollPitchYawPid[2].updatePID(rollPitchYawLevel[2], rollPitchYaw[2], timeDiff);
		 */
		double rollA = rollPitchYawPid[0].updatePID(rollPitchYawLevel[0], rollPitchYawKalman[0], timeDiff);
		double pitchA = rollPitchYawPid[1].updatePID(rollPitchYawLevel[1], rollPitchYawKalman[1], timeDiff);
		double yawA = rollPitchYawPid[2].updatePID(rollPitchYawLevel[2], rollPitchYawKalman[2], timeDiff);

		// double thrustInput = (input[0] - 1105.0) / (1000.0); // not 100%; leave some reserve
		double thrustInput = 0.5;// test

		thrust[0] = rollA + yawA + thrustInput;
		thrust[1] = -rollA + yawA + thrustInput;
		thrust[2] = -pitchA - yawA + thrustInput;
		thrust[3] = pitchA - yawA + thrustInput;

		for (int i = 0; i < 4; i++) {
			if (thrust[i] > 1) {
				thrust[i] = 1;
			} else if (thrust[i] < 0) {
				thrust[i] = 0;
			}
			// outputK[i].update(thrust[i]);
			// thrust[i] = outputK[i].getValue();
		}

		output[0] = (short) (1105 + (900 * (thrust[0])));
		output[1] = (short) (1105 + (900 * (thrust[1])));
		output[2] = (short) (1105 + (900 * (thrust[2])));
		output[3] = (short) (1105 + (900 * (thrust[3])));
	}

	private void calculateMag() {
		// Calculate heading when the magnetometer is level, then correct for
		// signs of axis.

		// Once you have your heading, you must then add your 'Declination
		// Angle', which is the 'Error' of the magnetic field in your location.
		// Find yours here: http://www.magnetic-declination.com/
		// Mine is: 2° 37' W, which is 2.617 Degrees, or (which we need)
		// 0.0456752665 radians, I will use 0.0457
		// Magnetic declination: 1° 25' EAST = -1.416666° = -0.02472549 rad
		// Declination is POSITIVE

		final double declinationAngle = -0.02472549;

		double rollSin = Math.sin(rollPitchYaw[0]);
		double rollCos = Math.cos(rollPitchYaw[0]);
		double pitchSin = Math.sin(rollPitchYaw[1]);
		double pitchCos = Math.cos(rollPitchYaw[1]);
		if (rollCos < 0) {
			rollCos = -rollCos;
		}

		magCompensated[1] = magScaled[1] * pitchCos - magScaled[2] * pitchSin;
		magCompensated[2] = magScaled[1] * pitchSin + magScaled[2] * pitchCos;
		magCompensated[0] = magScaled[0] * rollCos + magCompensated[2] * rollSin;
		rollPitchYaw[2] = Math.atan2(magCompensated[1], magCompensated[0]);

		rollPitchYaw[2] += declinationAngle;
		if (rollPitchYaw[2] > Math.PI) {
			rollPitchYaw[2] -= 2 * Math.PI;
		}
		if (rollPitchYaw[2] < -Math.PI) {
			rollPitchYaw[2] += 2 * Math.PI;
		}
	}

	private void calculateGyro() {
		//
		// <pre> <br/>
		// FS_SEL | Full Scale Range | LSB Sensitivity
		// -------+--------------------+----------------
		// 0 | +/- 250 degrees/s | 131 LSB/deg/s
		// 1 | +/- 500 degrees/s | 65.5 LSB/deg/s
		// 2 | +/- 1000 degrees/s | 32.8 LSB/deg/s
		// 3 | +/- 2000 degrees/s | 16.4 LSB/deg/s
		// </pre>
		// use: (float) gyro[0] / 16.4 / (1L << 16) to get deg/s
		// use: (float) (gyro[0] / 16.4) / (1L << 16) * 0.0174532925 to get
		// rad/s
		// */
		for (int i = 0; i < 3; i++) {
			gyroInDegreeSec[i] = (Math.PI * (gyro[i] / (16.4 * Math.pow(2.0, 16)))) / 180;
		}
	}

	// g = 9.81 m/s²
	private final static double accelToMssFactor = 9.81 / 5.147638312110519E8;

	private void calculateAccel() {
		rollPitchYawInt[0] = FpMath.arctan2(-accel[0], accel[2]);
		rollPitchYawInt[1] = FpMath.arctan2(accel[1], accel[2]);

		// rollPitchYawKalman[0] = rollPitchYawIntKalman[0].getNext(rollPitchYawInt[0], gyro[0]);
		// rollPitchYawKalman[1] = rollPitchYawIntKalman[1].getNext(rollPitchYawInt[1], gyro[1]);

		Quaternion.normalizeVec(accel, accelRelative);
		rollPitchYaw[0] = Math.atan2(-accel[0], accel[2]);
		rollPitchYaw[1] = Math.atan2(accel[1], accel[2]);

		kalmanModel1Droll.update(rollPitchYaw[0]);
		rollPitchYawKalman[0] = kalmanModel1Droll.getValue();
		kalmanModel1Dpitch.update(rollPitchYaw[1]);
		rollPitchYawKalman[1] = kalmanModel1Dpitch.getValue();

		// TODO optimize, rotate accel - vetctor with rollPitchYaw instead of
		// accelRelative
		accelInMss[0] = accel[0] * accelToMssFactor - accelRelative[0] * 9.81;
		accelInMss[1] = accel[1] * accelToMssFactor - accelRelative[1] * 9.81;
		accelInMss[2] = accel[2] * accelToMssFactor - accelRelative[2] * 9.81;
	}

	private void calculateQuaternion() {
		double[] fromEuler = Quaternion.fromEuler(rollPitchYaw[1], rollPitchYaw[2], rollPitchYaw[0]);
		for (int i = 0; i < 4; i++) {
			quat[i] = fromEuler[i];
		}
		double[] diff = normalize(multiply(normalize(conjugate(quat)), normalize(quatLevel)));
		for (int i = 0; i < 4; i++) {
			quatDiff[i] = diff[i];
		}
		/**
		 * <br />
		 * +y---|-<-c---------<br/ >
		 * -x---a---0---b--+x<br/ >
		 * -y-------d->-|-----<br/ >
		 */
		// x,y,z,clockwise (1 = cw, -1 = ccw)
		double[] a = new double[] { -1, 0, 0, 1 };
		// double[] b = new double[] { 1, 0, 0, 1 };
		double[] c = new double[] { 0, 1, 0, -1 };
		// double[] d = new double[] { 0, -1, 0, -1 };

		double[] diffA = multiplyVec(a, quatDiff);
		double[] diffC = multiplyVec(c, quatDiff);
		thrust[0] = diffA[2] + diffA[1];
		thrust[1] = -diffA[2] + diffA[1]; // multiplyVec(b, quatDiff)[2];
		thrust[2] = diffC[2] - diffC[0];// multiplyVec(c, quatDiff)[2];
		thrust[3] = -diffC[2] - diffC[0];// multiplyVec(d, quatDiff)[2];
	}

	public static DatenModel getInstance() {
		return instance;
	}

	public int[] getAccel() {
		return accel;
	}

	public int[] getAccelDiff() {
		return accelDiff;
	}

	public int[] getGyro() {
		return gyro;
	}

	public double[] getQuat() {
		return quat;
	}

	public double[] getLevel() {
		return quatLevel;
	}

	public double[] getDiff() {
		return quatDiff;
	}

	public double[] getThrust() {
		return thrust;
	}

	public short[] getMag() {
		return mag;
	}

	public short[] getMagMax() {
		return magMax;
	}

	public short[] getMagMin() {
		return magMin;
	}

	public double[] getMagScaled() {
		return magScaled;
	}

	public float[] getBaro() {
		return baro;
	}

	public long[] getCycles() {
		return cycles;
	}

	public GPSData getGps() {
		return gps;
	}

	public final ConcurrentLinkedQueue<String> getDebugInfos() {
		return debugInfos;
	}

	public void level() {
		for (int i = 0; i < 4; i++) {
			quatLevel[i] = quat[i];
		}

		for (int i = 0; i < 3; i++) {
			posXYZLevel[i] = posXYZ[i];
		}
		for (int i = 0; i < 3; i++) {
			rollPitchYawLevel[i] = rollPitchYaw[i];
		}

	}

	public double[] getAccelRelative() {
		return accelRelative;
	}

	public double[] getGyroInRadSec() {
		return gyroInDegreeSec;
	}

	public double[] getRollPitchYaw() {
		return rollPitchYaw;
	}

	public short[] getInput() {
		return input;
	}

	public final double[] getRollPitchYawDiff() {
		return rollPitchYawDiff;
	}

	public final double[] getRollPitchYawDiffDiff() {
		return rollPitchYawDiffDiff;
	}

	public final double[] getRollPitchYawKalman() {
		return rollPitchYawKalman;
	}

	public final double[] getPosXYZ() {
		return posXYZ;
	}

	public final double[] getPosXYZRelative() {
		return posXYZRelative;
	}

	public final double[] getPosXYZDiff() {
		return posXYZDiff;
	}

	public final double[] getPosXYZDiffDiff() {
		return posXYZDiffDiff;
	}

	public final short[] getOutput() {
		return output;
	}

	public final KalmanModel1D getKalmanModel1Dpitch() {
		return kalmanModel1Dpitch;
	}

	public final KalmanModel1D getKalmanModel1Droll() {
		return kalmanModel1Droll;
	}

	public int[] getAccelK() {
		return accelK;
	}

	public int[] getRollPitchYawInt() {
		return rollPitchYawInt;
	}

	public void triggerEvents() {
		calculate();
	}
}
