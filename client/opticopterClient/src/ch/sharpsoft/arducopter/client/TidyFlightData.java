package ch.sharpsoft.arducopter.client;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class TidyFlightData {

	public static void main(final String[] args) throws IOException {
		File in = new File("/home/helios/git/opticopter/client/opticopterClient/log/flightLogDataflash03.txt");
		File out = new File("/home/helios/git/opticopter/client/opticopterClient/log/flightLogDataflash03.txt_clean.txt");
		if (out.exists()) {
			out.delete();
			out.createNewFile();
		}
		try (FileWriter fw = new FileWriter(out, false); FileReader fr = new FileReader(in); BufferedReader br = new BufferedReader(fr)) {
			// final List<String> result = new ArrayList<>(lines.size());
			StringBuffer sb = new StringBuffer();
			String line;
			while ((line = br.readLine()) != null) {
				sb.append(line.trim().replaceAll("\n", ""));
				final String string = sb.toString();
				final String[] split = string.split(",");
				if (split.length > 23) {
					System.err.println("Error on line:" + sb.toString());
					sb = new StringBuffer();
				} else if (split.length == 23 && (split[22].equals("output3") || split[22].length() == 4)) {
					fw.write(string + "\n");
					sb = new StringBuffer();
				}
			}
		}
		// Files.write(out.toPath(), result, Charset.defaultCharset(), StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
	}
}
