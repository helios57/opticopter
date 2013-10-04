package ch.sharpsoft.arducopter.client.uart;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.Map;

public class Parser {

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
	private final Map<Byte, IParamReciever> paramReciever = new HashMap<>();

	public void parse(final int id, final byte[] buffer) {
		if (id == DatenModel.ID_PARAM) {
			ByteBuffer bb = ByteBuffer.wrap(buffer);
			bb.order(ByteOrder.LITTLE_ENDIAN);
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
			}
		} else {
			dm.parse(id, buffer);
		}
	}

	public void addParamReciever(final byte paramId, final IParamReciever paramReciever) {
		this.paramReciever.put(paramId, paramReciever);
	}
}
