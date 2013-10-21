package ch.sharpsoft.arducopter.client.model;

public class Manager {
	private final static Manager instance = new Manager();

	public static Manager getInstance() {
		return instance;
	}

	private static int count = 0;

	public void triggerAccelGyroRecieved5ms() {
		count++;
		DataCalculated dc = DataCalculated.getInstance();
		DataRaw dr = DataRaw.getInstance();
		Calculator.calc5ms(dr, dc);
		if (count % 4 == 0) {
			Calculator.calc20ms(dr, dc);
		}
		if (count > 1000) {
			// System.err.println(Calculator.getRollPitchYawVariance(dc)[0] + "," + Calculator.getRollPitchYawFilteredVariance(dc)[0]);
			System.err.println(dc.getRollPitchYaw()[0] + "," + dc.getRollPitchYaw()[1] + "," + dc.getRollPitchYaw()[2]);
			// System.err.println(dc.getRollPitchYaw()[0] + "," + dc.getGyroRps()[0] + "," + dc.getRollPitchYawFiltered()[0]);
			// System.err.println(dc.getRollPitchYaw()[0] + "," + dc.getRollPitchYaw()[1] + "," + dc.getRollPitchYaw()[2] + "," + dr.getGyro()[0] + "," + dr.getGyro()[1] + "," + dr.getGyro()[2] + "," + dc.getGyroRps()[0] + "," + dc.getGyroRps()[1] + ","
			// + dc.getGyroRps()[2]);
			// System.err.println(Calculator.getGyroRpsVariance(dc)[0]);
			// System.err.println(dr.getGyro()[0]);
			// System.err.println(dc.getGyroRps()[0]);
		}
	}
}
