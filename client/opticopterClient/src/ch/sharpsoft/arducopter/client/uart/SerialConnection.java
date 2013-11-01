package ch.sharpsoft.arducopter.client.uart;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.Iterator;
import java.util.List;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

public class SerialConnection {
	private SerialConnection() {

	}

	private final static SerialConnection instance = new SerialConnection();
	private final Parser parser = new Parser();
	private CommPortIdentifier portIdentifier;
	private CommPort commPort;
	private SerialPort serialPort;
	private InputStream in;
	private OutputStream out;

	public static final int PREEAMBLE_1 = 0xE5;
	public static final int PREEAMBLE_2 = 0xE7;

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

					SerialService.getInstance().startRequestingThread();

					new Thread(new Runnable() {

						@Override
						public void run() {
							readFromInputStream();

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

	public static void main(final String... args) throws FileNotFoundException, IOException {
		// final File file = Paths.get("/home/helios/git/opticopter/client/opticopterClient/log/steady.bin").toFile();
		final File file = Paths.get("/home/helios/git/opticopter/client/opticopterClient/log/flight_text.bin").toFile();
		final File out = Paths.get("/home/helios/git/opticopter/client/opticopterClient/log/flight_text.txt").toFile();
		SerialConnection sc = new SerialConnection();
		sc.in = new GZIPInputStream(new FileInputStream(file));
		FileOutputStream fos = new FileOutputStream(out);
		int i = sc.in.read();
		int j = 0;
		try {
			while (sc.in.available() > 0) {
				if (j++ > 100) {
					fos.write(i);
				}
				i = sc.in.read();
			}
		} catch (Exception e) {

		}
		fos.close();
		final List<String> lines = Files.readAllLines(out.toPath(), Charset.defaultCharset());
		final Iterator<String> iterator = lines.iterator();
		while (iterator.hasNext()) {
			final int length = iterator.next().split(",").length;
			System.err.println(length);
			if (!(length == 16)) {
				iterator.remove();
			}
		}
		Files.write(out.toPath(), lines, Charset.defaultCharset(), StandardOpenOption.TRUNCATE_EXISTING);

		// sc.readFromInputStream();
	}

	public static void record() {
		try {
			final CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier("/dev/ttyUSB0");
			// final CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier("/dev/ttyACM0");
			if (portIdentifier.isCurrentlyOwned()) {
				System.out.println("Error: Port is currently in use");
			} else {
				CommPort port;
				port = portIdentifier.open("Blubber", 2000);

				if (port instanceof SerialPort) {
					final SerialPort serialPort = (SerialPort) port;
					serialPort.setSerialPortParams(Integer.valueOf("115200"), SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
					final File file = Paths.get("/home/helios/git/opticopter/client/opticopterClient/log/flight_text.bin").toFile();
					final FileOutputStream fos = new FileOutputStream(file);
					final GZIPOutputStream gso = new GZIPOutputStream(fos, true);
					SerialConnection.getInstance().in = serialPort.getInputStream();
					SerialConnection.getInstance().out = serialPort.getOutputStream();
					// SerialService.getInstance().startRequestingThread();
					new Thread(new Runnable() {

						@Override
						public void run() {
							while (true) {
								try {
									gso.write(serialPort.getInputStream().read());
									gso.flush();
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
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void readFromInputStream() {

		long good = 0;
		long bad = 0;
		while (true) {
			try {
				final int read = in.read();
				if (read == -1) {
					break;
				}
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
				if (e instanceof EOFException) {
					break;
				}
				e.printStackTrace();
			}
		}
	}

	private void parse(final int id, final byte[] buffer) {
		parser.parse(id, buffer);
	}

	public InputStream getInputStream() {
		return in;
	}

	public OutputStream getOutputStream() {
		return out;
	}

	public static SerialConnection getInstance() {
		return instance;
	}

	public Parser getParser() {
		return parser;
	}
}
