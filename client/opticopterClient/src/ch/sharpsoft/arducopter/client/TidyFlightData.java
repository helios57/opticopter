package ch.sharpsoft.arducopter.client;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.util.Iterator;
import java.util.List;

public class TidyFlightData {

	public static void main(final String[] args) throws IOException {
		File in = new File("/home/helios/git/opticopter/client/opticopterClient/log/realFlight1.txt");
		File out = new File("/home/helios/git/opticopter/client/opticopterClient/log/realFlight1_clean.txt");
		final List<String> lines = Files.readAllLines(in.toPath(), Charset.defaultCharset());
		final Iterator<String> iter = lines.iterator();
		while (iter.hasNext()) {
			final String line = iter.next();
			if (!(line.split(",").length == 7)) {
				iter.remove();
			}
		}
		Files.write(out.toPath(), lines, Charset.defaultCharset(), StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
	}

}
