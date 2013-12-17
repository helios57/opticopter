package ch.sharpsoft.arducopter.client;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;

public class GenerateNNDatasets {

	public static void main(final String[] args) throws IOException {
		File raw = new File("/home/helios/git/opticopter/client/opticopterClient/log/nn/raw00.csv");
		File input = new File("/home/helios/git/opticopter/client/opticopterClient/log/nn/input00.csv");
		File output = new File("/home/helios/git/opticopter/client/opticopterClient/log/nn/output00.csv");
		File nnDataset = new File("/home/helios/git/opticopter/client/opticopterClient/log/nn/nnDataset00.train");

		final List<String> rawLines = Files.readAllLines(raw.toPath(), Charset.defaultCharset());
		final List<String> inputLines = new ArrayList<>();
		final List<String> outputLines = new ArrayList<>();
		final List<String> nnDatasetLines = new ArrayList<>();
		inputLines.add("roll,pitch,yaw,output0,output1,output2,output3");
		outputLines.add("roll,pitch,yaw");

		boolean firstLine = true;
		boolean headerLine = true;
		for (String line : rawLines) {
			if (headerLine) {
				headerLine = false;
				continue;
			}
			// [0]=timestamp
			// [1]=accelX,accelY,accelZ,
			// [4] =gyroX,gyroY,gyroZ,
			// [7] = magX,magY,magZ,
			// [10] = roll,pitch,yaw,
			// [13] = rollLevel,pitchLevel,yawLevel,
			// [16] = rollPid,pitchPid,yawPid,
			// [19] = output0,output1,output2,output3
			final String[] split = line.split(",");
			float roll = Float.valueOf(split[10]) / 1000;
			float pitch = Float.valueOf(split[11]) / 1000;
			float yaw = Float.valueOf(split[12]) / 1000;
			int out0 = Integer.valueOf(split[19]);
			int out1 = Integer.valueOf(split[20]);
			int out2 = Integer.valueOf(split[21]);
			int out3 = Integer.valueOf(split[22]);

			inputLines.add(roll + "," + pitch + "," + yaw + "," + out0 + "," + out1 + "," + out2 + "," + out3);
			if (firstLine) {
				firstLine = false;
				continue;
			}
			outputLines.add(roll + "," + pitch + "," + yaw);
		}
		inputLines.remove(inputLines.size() - 1);
		Files.write(input.toPath(), inputLines, Charset.defaultCharset(), StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
		Files.write(output.toPath(), outputLines, Charset.defaultCharset(), StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);

		inputLines.remove(0);
		outputLines.remove(0);
		long samples = inputLines.size();
		long inputData = 7;
		long outputData = 3;
		nnDatasetLines.add(samples + " " + inputData + " " + outputData);
		for (int i = 0; i < samples; i++) {
			nnDatasetLines.add(inputLines.get(i).replaceAll(",", " "));
			nnDatasetLines.add(outputLines.get(i).replaceAll(",", " "));
		}
		Files.write(nnDataset.toPath(), nnDatasetLines, Charset.defaultCharset(), StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
	}
}
