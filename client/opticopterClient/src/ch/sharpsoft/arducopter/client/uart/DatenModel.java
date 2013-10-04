package ch.sharpsoft.arducopter.client.uart;

import static ch.sharpsoft.arducopter.client.view.Quaternion.conjugate;
import static ch.sharpsoft.arducopter.client.view.Quaternion.multiply;
import static ch.sharpsoft.arducopter.client.view.Quaternion.multiplyVec;
import static ch.sharpsoft.arducopter.client.view.Quaternion.normalize;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;

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
		}
	}

	private final static DatenModel instance = new DatenModel();

	private final List<NewDataListener> events = new ArrayList<>();

	public void addEventListener(final NewDataListener eventListener) {
		events.add(eventListener);
	}

	private static final int ID_ACCEL = 0x01;
	private static final int ID_GYRO = 0x02;
	private static final int ID_BARO = 0x03;
	private static final int ID_QUAT = 0x04;
	private static final int ID_MAG = 0x05;
	private static final int ID_GPS = 0x06;
	private static final int ID_INPUT = 0x07;
	private static final int ID_OUTPUT = 0x08;
	private static final int ID_DEBUG = 0x09;
	private static final int ID_CYCLES = 0x0A;
	public static final int ID_PARAM = 0x0B;

	private final int[] accel = new int[3];
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

	public void parse(final int id, final byte[] buffer) {
		ByteBuffer bb = ByteBuffer.wrap(buffer);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		if (id == ID_ACCEL) {
			accel[0] = bb.getInt(0);
			accel[1] = bb.getInt(4);
			accel[2] = bb.getInt(8);
			calculate();
		} else if (id == ID_GYRO) {
			gyro[0] = bb.getInt(0);
			gyro[1] = bb.getInt(4);
			gyro[2] = bb.getInt(8);
		} else if (id == ID_BARO) {
			baro[0] = bb.getFloat();
		} else if (id == ID_QUAT) {
			quat[0] = bb.getInt(0);
			quat[1] = bb.getInt(4);
			quat[2] = bb.getInt(8);
			quat[3] = bb.getInt(12);
			Quaternion.normalize(quat);
		} else if (id == ID_MAG) {
			mag[0] = bb.getShort(0);
			mag[1] = bb.getShort(2);
			mag[2] = (short) -bb.getShort(4);
			for (int i = 0; i < 3; i++) {
				magMax[i] = (short) Math.max(magMax[i], mag[i]);
				magMin[i] = (short) Math.min(magMin[i], mag[i]);
			}
			for (int i = 0; i < 3; i++) {
				magScaled[i] = (((double) mag[i] - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
			}
		} else if (id == ID_GPS) {
			/*
			 * int32_t latitude; int32_t longitude; int32_t altitude; int32_t ground_speed; int32_t ground_course; uint8_t satellites; uint8_t fix_type; uint32_t utc_date; uint32_t utc_time; uint16_t hdop;
			 */
			gps.setLatitude(bb.getInt());
			gps.setLongitude(bb.getInt());
			gps.setAltitude(bb.getInt());
			gps.setGround_speed(bb.getInt());
			gps.setGround_course(bb.getInt());
			gps.setNum_sats(bb.get());
			byte fixType = bb.get();
			gps.setFix(fixType == 3 || fixType == 7);
			gps.setDate(bb.getInt() & 0xFFFFFFFFL);
			gps.setTime(bb.getInt() & 0xFFFFFFFFL);
			gps.setHdop(bb.getShort());
			gps.calc();
		} else if (id == ID_INPUT) {
			int lenght = bb.get(0);
			for (int i = 0; i < lenght; i++) {
				final byte chan = bb.get(1 + i * 3);
				final short pmw = bb.getShort(2 + i * 3);
				input[chan] = pmw;
			}
		} else if (id == ID_OUTPUT) {
		} else if (id == ID_DEBUG) {
			debugInfos.add(new String(bb.array()));
		} else if (id == ID_CYCLES) {
			cycles[0] = bb.getLong();
		}
	}

	private void calculate() {
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
}
