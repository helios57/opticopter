package ch.sharpsoft.arducopter.client.uart;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.Map;

import ch.sharpsoft.arducopter.client.model.DataRaw;
import ch.sharpsoft.arducopter.client.model.Manager;

public class Parser {
	private static final int ID_MOTION6 = 0x01;
	private static final int ID_BARO = 0x03;
	private static final int ID_MAG = 0x05;
	private static final int ID_GPS = 0x06;
	private static final int ID_INPUT = 0x07;
	private static final int ID_OUTPUT = 0x08;
	private static final int ID_DEBUG = 0x09;
	private static final int ID_CYCLES = 0x0A;
	private static final int ID_PARAM = 0x0B;

	public static final byte ID_IN_PARAM_MAG_MAX = 0x01;
	public static final byte ID_IN_PARAM_MAG_MIN = 0x02;
	public static final byte ID_IN_PARAM_RC_IN_MAX = 0x03;
	public static final byte ID_IN_PARAM_RC_IN_MIN = 0x04;
	public static final byte ID_IN_PARAM_RC_IN_DEFAULT = 0x05;
	public static final byte ID_IN_PARAM_RC_IN_FUNCTION = 0x06;
	public static final byte ID_IN_PARAM_DECLINATION_ANGLE = 0x07;
	public static final byte ID_IN_PARAM_PID = 0x08;
	public static final byte ID_IN_PARAM_KALMAN = 0x09;

	private final DatenModel dm = DatenModel.getInstance();
	private final DataRaw rd = DataRaw.getInstance();
	private final Manager m = Manager.getInstance();

	private final Map<Byte, IParamReciever> paramReciever = new HashMap<>();

	public void parse(final int id, final byte[] buffer) {
		try {
			ByteBuffer bb = ByteBuffer.wrap(buffer);
			bb.order(ByteOrder.LITTLE_ENDIAN);
			if (id == ID_MOTION6) {
				short[] input = new short[6];
				for (int i = 0; i < 6; i++) {
					input[i] = bb.getShort(i * 2);
				}
				System.arraycopy(input, 0, rd.getMotion6(), 0, 6);
				dm.getAccel()[0] = input[0];
				dm.getAccel()[1] = input[1];
				dm.getAccel()[2] = input[2];
				dm.getGyro()[0] = input[3];
				dm.getGyro()[1] = input[4];
				dm.getGyro()[2] = input[5];
				m.triggerAccelGyroRecieved5ms();
				dm.triggerEvents();
			} else if (id == ID_BARO) {
				float baro = bb.getFloat();
				rd.getBaro()[0] = baro;
				dm.getBaro()[0] = baro;
			} else if (id == ID_MAG) {
				short[] input = new short[3];
				input[0] = bb.getShort(0);
				input[1] = bb.getShort(2);
				input[2] = bb.getShort(4);
				System.arraycopy(input, 0, dm.getMag(), 0, 3);
				System.arraycopy(input, 0, rd.getMag(), 0, 3);
			} else if (id == ID_GPS) {
				GPSData gps = new GPSData();
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
				dm.getGps().setInput(gps);
				rd.getGps().setInput(gps);
			} else if (id == ID_INPUT) {
				int lenght = bb.get(0);
				for (int i = 0; i < lenght; i++) {
					final byte chan = bb.get(1 + i * 3);
					final short pmw = bb.getShort(2 + i * 3);
					dm.getInput()[chan] = pmw;
					rd.getRcIn()[chan] = pmw;
				}
			} else if (id == ID_OUTPUT) {
				int lenght = bb.get(0);
				for (int i = 0; i < lenght; i++) {
					final byte chan = bb.get(1 + i * 3);
					final short pmw = bb.getShort(2 + i * 3);
					dm.getOutput()[chan] = pmw;
					rd.getOut()[chan] = pmw;
				}
			} else if (id == ID_DEBUG) {
				dm.getDebugInfos().add(new String(bb.array()));
			} else if (id == ID_CYCLES) {
				dm.getCycles()[0] = bb.getLong();
			} else if (id == ID_PARAM) {
				byte paramID = bb.get();
				IParamReciever reciever = paramReciever.get(Byte.valueOf(paramID));
				if (reciever == null) {
					return;
				}
				paramReciever.remove(Byte.valueOf(paramID));
				switch (paramID) {
				case ID_IN_PARAM_MAG_MAX:
				case ID_IN_PARAM_MAG_MIN:
					short[] mag = new short[3];
					mag[0] = bb.getShort();
					mag[1] = bb.getShort();
					mag[2] = bb.getShort();
					reciever.recieved(mag);
					break;
				case ID_IN_PARAM_RC_IN_MAX:
				case ID_IN_PARAM_RC_IN_MIN:
				case ID_IN_PARAM_RC_IN_DEFAULT:
					short[] rc = new short[8];
					for (int i = 0; i < 8; i++) {
						rc[i] = bb.getShort();
					}
					reciever.recieved(rc);
					break;
				case ID_IN_PARAM_PID:
					float[] pid = new float[9];
					for (int i = 0; i < 9; i++) {
						pid[i] = bb.getFloat();
					}
					reciever.recieved(pid);
					break;
				}
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public void addParamReciever(final byte paramId, final IParamReciever paramReciever) {
		this.paramReciever.put(paramId, paramReciever);
	}
}
