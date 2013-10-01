package ch.sharpsoft.arducopter.client.uart;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class SerialConnection {
	private final DatenModel datenModel = DatenModel.getInstance();
	private CommPortIdentifier portIdentifier;
	private CommPort commPort;
	private SerialPort serialPort;
	private InputStream in;
	private OutputStream out;

	private static final int PREEAMBLE_1 = 0xE5;
	private static final int PREEAMBLE_2 = 0xE7;

	public void connect(final String port, final String baud) {
		try {
			portIdentifier = CommPortIdentifier.getPortIdentifier(port);
			if (portIdentifier.isCurrentlyOwned()) {
				System.out.println("Error: Port is currently in use");
			} else {
				commPort = portIdentifier.open(this.getClass().getName(), 2000);
				if (commPort instanceof SerialPort) {
					serialPort = (SerialPort) commPort;
					serialPort.setSerialPortParams(Integer.valueOf(baud), SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

					in = serialPort.getInputStream();
					out = serialPort.getOutputStream();

					new Thread(new Runnable() {

						@Override
						public void run() {
							while (true) {
								ByteBuffer bb = ByteBuffer.allocate(14);
								bb.order(ByteOrder.LITTLE_ENDIAN);
								bb.put((byte) PREEAMBLE_1);// 1
								bb.put((byte) PREEAMBLE_2);// 1
								bb.put((byte) 0x01);// 1
								bb.put((byte) 8);// 1
								bb.putLong(3000L);// 8
								byte and = 0;
								byte xor = 0;
								for (int i = 4; i < 12; i++) {
									and += bb.array()[i];
									xor ^= bb.array()[i];
								}
								bb.put(and);
								bb.put(xor);
								try {
									out.write(bb.array(), 0, 14);
									Thread.sleep(1000);
								} catch (IOException | InterruptedException e) {
									e.printStackTrace();
								}
							}
						}
					}).start();

					new Thread(new Runnable() {

						long good = 0;
						long bad = 0;

						@Override
						public void run() {
							while (true) {
								try {
									final int read = in.read();
									if (PREEAMBLE_1 == read) {
										final int read2 = in.read();
										if (PREEAMBLE_2 == read2) {
											final int id = in.read();
											if (id == 0) {
												bad++;
												System.err.println("good=" + good + " bad=" + bad + " id=" + id);
												continue;
											}
											final int length = in.read();
											final byte[] buffer = new byte[length + 2];
											int i = 0;
											for (; i < buffer.length; i++) {
												buffer[i] = (byte) in.read();
											}
											// int readbytes = in.read(buffer);
											if (i < (length + 2)) {
												bad++;
												System.err.println("good=" + good + " bad=" + bad + " buffer=" + buffer + " expected=" + (length + 2));
												continue;
											}
											byte and = 0;
											byte xor = 0;
											for (i = 0; i < length; i++) {
												and += buffer[i];
												xor ^= buffer[i];
											}
											if (and == buffer[length] && xor == buffer[length + 1]) {
												parse(id, buffer);
												good++;
											} else {
												bad++;
												System.err.println("id=" + id + " len=" + length + " and=" + and + " should=" + buffer[length] + " xor=" + xor + " should=" + buffer[length + 1]);
											}
										}
									}
								} catch (IOException e) {
									e.printStackTrace();
								}
							}

						}

					}).start();
				} else {
					System.out.println("Error: Only serial ports are handled by this example.");
				}
			}
		} catch (NoSuchPortException | IOException | PortInUseException | UnsupportedCommOperationException e1) {
			e1.printStackTrace();
		}
	}

	private void parse(final int id, final byte[] buffer) {
		datenModel.parse(id, buffer);
	}

	public InputStream getInputStream() {
		return in;
	}

	public OutputStream getOutputStream() {
		return out;
	}
}
