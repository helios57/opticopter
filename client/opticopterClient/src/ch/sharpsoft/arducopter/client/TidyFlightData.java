package ch.sharpsoft.arducopter.client;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class TidyFlightData {

	public static void main(final String[] args) throws IOException {
		File in = new File("/home/helios/git/opticopter/client/opticopterClient/log/flightLogDataflash00.txt");
		File out = new File("/home/helios/git/opticopter/client/opticopterClient/log/flightLogDataflash00.txt_clean.txt");
		final List<String> lines = Files.readAllLines(in.toPath(), Charset.defaultCharset());
		final List<String> result = new ArrayList<>();
		final Iterator<String> iter = lines.iterator();
		StringBuffer sb = new StringBuffer();
		while (iter.hasNext()) {
			final String line = iter.next();
			sb.append(line.trim().replaceAll("\n", ""));
			final String string = sb.toString();
			final String[] split = string.split(",");
			if (split.length == 23 && (split[22].equals("output3") || split[22].length() == 4)) {
				result.add(string);
				sb = new StringBuffer();
			}
		}
		Files.write(out.toPath(), result, Charset.defaultCharset(), StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
	}
}
