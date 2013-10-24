package ch.sharpsoft.arducopter.client.model;

import ch.sharpsoft.arducopter.client.uart.GPSData;

public class DataRaw {

	private final static DataRaw instance = new DataRaw();

	private final short[] axyzgxyz = new short[6];
	private final short[] mag = new short[3];
	private final float[] baro = new float[1];
	private final GPSData gps = new GPSData();
	private final short[] rcIn = new short[8];
	private final short[] out = new short[8];

	public static DataRaw getInstance() {
		return instance;
	}

	public final short[] getMag() {
		return mag;
	}

	public final float[] getBaro() {
		return baro;
	}

	public final GPSData getGps() {
		return gps;
	}

	public final short[] getRcIn() {
		return rcIn;
	}

	public final short[] getOut() {
		return out;
	}

	public short[] getMotion6() {
		return axyzgxyz;
	}
}
