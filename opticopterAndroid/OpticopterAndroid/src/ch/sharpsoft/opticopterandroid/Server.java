package ch.sharpsoft.opticopterandroid;

import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
	private ServerSocket ss;
	private Socket socket;
	private OutputStream os;
	private volatile boolean running = true;

	public void init() {
		if (ss != null) {
			return;
		}
		try {
			ss = new ServerSocket(1337);
		} catch (IOException e) {
			e.printStackTrace();
		}

		new Thread(new Runnable() {

			@Override
			public void run() {
				while (running) {
					try {
						final Socket s = ss.accept();
						System.err.println("Socket Connected");
						if (socket != null) {
							os.close();
							socket.close();
						}
						socket = s;
						// os = new GZIPOutputStream(socket.getOutputStream());
						os = socket.getOutputStream();
					} catch (IOException e) {
						break;
					}
				}
			}
		}).start();
	}

	public Socket getSocket() {
		return socket;
	}

	public OutputStream getOS() {
		return os;
	}

	public void close() {
		running = false;
		try {
			if (ss != null) {
				ss.close();
			}
			if (socket != null) {
				socket.close();
				os.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void reset() {
		if (socket != null) {
			try {
				socket.close();
				os.close();
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				socket = null;
				os = null;
			}
		}
	}
}