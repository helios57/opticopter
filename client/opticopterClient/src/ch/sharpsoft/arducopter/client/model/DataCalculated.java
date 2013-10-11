package ch.sharpsoft.arducopter.client.model;

public class DataCalculated {
	private final static DataCalculated instance = new DataCalculated();
	private final short[] magMax = new short[] { 710, 424, 435 };
	private final short[] magMin = new short[] { -503, -768, -553 };
	private final double[] magScaled = new double[3];
	private final double[] magCompensated = new double[3];

	private final double[][] rollPitchYawHistory = new double[50][3];
	private int rollPitchYawHistoryIndex = 0;
	private final double[][] rollPitchYawFilteredHistory = new double[50][3];
	private int rollPitchYawFilteredHistoryIndex = 0;
	private final double[][] gyroRpsHistory = new double[50][3];
	private int gyroRpsHistoryIndex = 0;

	private final double[] rollPitchYaw = new double[3];
	private final double[] rollPitchYawFiltered = new double[3];

	private final double[] gyroRps = new double[3];

	public static DataCalculated getInstance() {
		return instance;
	}

	public void saveRollPitchYaw() {
		System.arraycopy(rollPitchYaw, 0, rollPitchYawHistory[rollPitchYawHistoryIndex++], 0, 3);
		if (rollPitchYawHistoryIndex >= rollPitchYawHistory.length) {
			rollPitchYawHistoryIndex = 0;
		}
	}

	public void putRollPitchYawFiltered(final double[] rollPitchYawFiltered) {
		System.arraycopy(rollPitchYawFiltered, 0, rollPitchYawFilteredHistory[rollPitchYawFilteredHistoryIndex++], 0, 3);
		if (rollPitchYawFilteredHistoryIndex >= rollPitchYawFilteredHistory.length) {
			rollPitchYawFilteredHistoryIndex = 0;
		}
	}

	public void saveGyroRps() {
		System.arraycopy(gyroRps, 0, gyroRpsHistory[gyroRpsHistoryIndex++], 0, 3);
		if (gyroRpsHistoryIndex >= gyroRpsHistory.length) {
			gyroRpsHistoryIndex = 0;
		}
	}

	public double[] getRollPitchYaw() {
		return rollPitchYaw;
	}

	public double[] getRollPitchYawFiltered() {
		return rollPitchYawFiltered;
	}

	public double[][] getRollPitchYawHistory() {
		return rollPitchYawHistory;
	}

	public double[][] getRollPitchYawFilteredHistory() {
		return rollPitchYawFilteredHistory;
	}

	public double[][] getGyroRpsHistory() {
		return gyroRpsHistory;
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

	public double[] getMagCompensated() {
		return magCompensated;
	}

	public double[] getGyroRps() {
		return gyroRps;
	}
}
