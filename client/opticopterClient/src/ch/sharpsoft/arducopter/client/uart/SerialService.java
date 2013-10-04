package ch.sharpsoft.arducopter.client.uart;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class SerialService {
	private final static SerialService instance = new SerialService();
	private final static byte ID_IN_SEND_DATA = (byte) 0x01;
	private final static byte ID_IN_READ_PARAM = (byte) 0x02;
	private final static byte ID_IN_WRITE_PARAM = (byte) 0x03;

	private boolean requestingData = false;

	public static SerialService getInstance() {
		return instance;
	}

	public void writeParam(final byte paramId, final short[] data) {
		final ByteBuffer bb = begin(ID_IN_WRITE_PARAM);
		final byte len = (byte) (1 + data.length * 2);
		bb.put(len);// 1
		bb.put(paramId);// 1
		for (short s : data) {
			bb.putShort(s);
		}
		end(bb, len);
	}

	public void requestParam(final byte paramId, final IParamReciever paramReciever) {
		SerialConnection.getInstance().getParser().addParamReciever(paramId, paramReciever);
		final ByteBuffer bb = begin(ID_IN_READ_PARAM);
		final byte len = (byte) 1;
		bb.put(len);// 1
		bb.put(paramId);// 1
		end(bb, len);
	}

	public void startRequestingThread() {
		if (requestingData) {
			return;
		}
		requestingData = true;
		new Thread(new Runnable() {

			@Override
			public void run() {
				while (true) {
					requestSensorData();
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}).start();
	}

	public void requestSensorData() {
		final ByteBuffer bb = begin(ID_IN_SEND_DATA);
		final byte len = (byte) 8;
		bb.put(len);// 1
		bb.putLong(6000L);// 8
		end(bb, len);
	}

	private void end(final ByteBuffer bb, final byte len) {
		byte and = 0;
		byte xor = 0;
		for (int i = 4; i < (4 + len); i++) {
			and += bb.array()[i];
			xor ^= bb.array()[i];
		}
		bb.put(and);
		bb.put(xor);
		try {
			SerialConnection.getInstance().getOutputStream().write(bb.array(), 0, 4 + len + 2);
		} catch (final IOException e) {
			e.printStackTrace();
		}
	}

	private ByteBuffer begin(final byte msg_id) {
		final ByteBuffer bb = ByteBuffer.allocate(14);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		bb.put((byte) SerialConnection.PREEAMBLE_1);// 1
		bb.put((byte) SerialConnection.PREEAMBLE_2);// 1
		bb.put(msg_id);// 1
		return bb;
	}
}
