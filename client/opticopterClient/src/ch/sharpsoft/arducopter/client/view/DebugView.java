package ch.sharpsoft.arducopter.client.view;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.part.ViewPart;
import org.joda.time.DateTime;

import ch.sharpsoft.arducopter.client.uart.DatenModel;

public class DebugView extends ViewPart implements Runnable {
	private Text accelX;
	private Text accelY;
	private Text accelZ;
	private Text accelGX;
	private Text accelGY;
	private Text accelGZ;
	private Text gyroX;
	private Text gyroY;
	private Text gyroZ;
	private Text gyroDX;
	private Text gyroDY;
	private Text gyroDZ;
	private Text magX;
	private Text magY;
	private Text magZ;
	private Text magMaxX;
	private Text magMaxY;
	private Text magMaxZ;
	private Text magMinX;
	private Text magMinY;
	private Text magMinZ;
	private Text magScaledX;
	private Text magScaledY;
	private Text magScaledZ;
	private Text roll;
	private Text pitch;
	private Text yaw;
	private Text quatW;
	private Text quatX;
	private Text quatY;
	private Text quatZ;
	private Text quatDiffW;
	private Text quatDiffX;
	private Text quatDiffY;
	private Text quatDiffZ;
	private Text thrustA;
	private Text thrustB;
	private Text thrustC;
	private Text thrustD;
	private Text input0;
	private Text input1;
	private Text input2;
	private Text input3;
	private Text baro;
	private Label lblLatlongalt;
	private Text latitude;
	private Text longitude;
	private Text altitude;

	private Text lat;
	private Text lon;
	private Text alt;

	private Text fix;
	private Label lblTime;
	private Text time;
	private Label lblNumSat;
	private Text numSat;
	private Label lblThrust;
	private Label lblLevel;
	private Button btnLevel;
	private Text emptyCycles;

	public DebugView() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		parent.setLayout(new GridLayout(9, false));

		Label lblAcceleration = new Label(parent, SWT.NONE);
		lblAcceleration.setText("Acceleration");

		Label lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		accelX = new Text(parent, SWT.BORDER);
		accelX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Label lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		accelY = new Text(parent, SWT.BORDER);
		accelY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Label lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		accelZ = new Text(parent, SWT.BORDER);
		accelZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		lblAcceleration = new Label(parent, SWT.NONE);
		lblAcceleration.setText("Acceleration G");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		accelGX = new Text(parent, SWT.BORDER);
		accelGX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		accelGY = new Text(parent, SWT.BORDER);
		accelGY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		accelGZ = new Text(parent, SWT.BORDER);
		accelGZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		Label label = new Label(parent, SWT.NONE);
		label.setText("Gyro");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		gyroX = new Text(parent, SWT.BORDER);
		gyroX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		gyroY = new Text(parent, SWT.BORDER);
		gyroY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		gyroZ = new Text(parent, SWT.BORDER);
		gyroZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Gyro D/s");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		gyroDX = new Text(parent, SWT.BORDER);
		gyroDX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		gyroDY = new Text(parent, SWT.BORDER);
		gyroDY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		gyroDZ = new Text(parent, SWT.BORDER);
		gyroDZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Magnetometer");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		magX = new Text(parent, SWT.BORDER);
		magX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		magY = new Text(parent, SWT.BORDER);
		magY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		magZ = new Text(parent, SWT.BORDER);
		magZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Magnetometer Max");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		magMaxX = new Text(parent, SWT.BORDER);
		magMaxX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		magMaxY = new Text(parent, SWT.BORDER);
		magMaxY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		magMaxZ = new Text(parent, SWT.BORDER);
		magMaxZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Magnetometer Min");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		magMinX = new Text(parent, SWT.BORDER);
		magMinX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		magMinY = new Text(parent, SWT.BORDER);
		magMinY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		magMinZ = new Text(parent, SWT.BORDER);
		magMinZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Magnetometer Scaled");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		magScaledX = new Text(parent, SWT.BORDER);
		magScaledX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		magScaledY = new Text(parent, SWT.BORDER);
		magScaledY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		magScaledZ = new Text(parent, SWT.BORDER);
		magScaledZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Roll Pitch Yaw");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("roll");

		roll = new Text(parent, SWT.BORDER);
		roll.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("pitch");

		pitch = new Text(parent, SWT.BORDER);
		pitch.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("yaw");

		yaw = new Text(parent, SWT.BORDER);
		yaw.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Orientierung");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("w");

		quatW = new Text(parent, SWT.BORDER);
		quatW.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		quatX = new Text(parent, SWT.BORDER);
		quatX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		quatY = new Text(parent, SWT.BORDER);
		quatY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		quatZ = new Text(parent, SWT.BORDER);
		quatZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		label = new Label(parent, SWT.NONE);
		label.setText("Orientierung D");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("w");

		quatDiffW = new Text(parent, SWT.BORDER);
		quatDiffW.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("x");

		quatDiffX = new Text(parent, SWT.BORDER);
		quatDiffX.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("y");

		quatDiffY = new Text(parent, SWT.BORDER);
		quatDiffY.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("z");

		quatDiffZ = new Text(parent, SWT.BORDER);
		quatDiffZ.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblThrust = new Label(parent, SWT.NONE);
		lblThrust.setText("Thrust");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("a");

		thrustA = new Text(parent, SWT.BORDER);
		thrustA.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("b");

		thrustB = new Text(parent, SWT.BORDER);
		thrustB.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("c");

		thrustC = new Text(parent, SWT.BORDER);
		thrustC.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("d");

		thrustD = new Text(parent, SWT.BORDER);
		thrustD.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblThrust = new Label(parent, SWT.NONE);
		lblThrust.setText("Input");

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("0");

		input0 = new Text(parent, SWT.BORDER);
		input0.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblX = new Label(parent, SWT.NONE);
		lblX.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblX.setText("1");

		input1 = new Text(parent, SWT.BORDER);
		input1.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblY = new Label(parent, SWT.NONE);
		lblY.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblY.setText("2");

		input2 = new Text(parent, SWT.BORDER);
		input2.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("3");

		input3 = new Text(parent, SWT.BORDER);
		input3.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		lblLevel = new Label(parent, SWT.NONE);
		lblLevel.setText("Level");
		new Label(parent, SWT.NONE);

		btnLevel = new Button(parent, SWT.NONE);
		btnLevel.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				DatenModel.getInstance().level();
			}
		});
		btnLevel.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));
		btnLevel.setText("Level");
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		label = new Label(parent, SWT.NONE);
		label.setText("Barometer");

		lblZ = new Label(parent, SWT.NONE);
		lblZ.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblZ.setText("p");

		baro = new Text(parent, SWT.BORDER);
		baro.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		lblLatlongalt = new Label(parent, SWT.NONE);
		lblLatlongalt.setText("Lat/Long/Alt");
		new Label(parent, SWT.NONE);

		latitude = new Text(parent, SWT.BORDER);
		latitude.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);

		longitude = new Text(parent, SWT.BORDER);
		longitude.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);

		altitude = new Text(parent, SWT.BORDER);
		altitude.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);

		fix = new Text(parent, SWT.BORDER);
		fix.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Label ll = new Label(parent, SWT.NONE);
		ll.setText("Lat/Long/Alt");
		new Label(parent, SWT.NONE);

		lat = new Text(parent, SWT.BORDER);
		lat.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);

		lon = new Text(parent, SWT.BORDER);
		lon.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);

		alt = new Text(parent, SWT.BORDER);
		alt.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		lblTime = new Label(parent, SWT.NONE);
		lblTime.setText("Time");
		new Label(parent, SWT.NONE);

		time = new Text(parent, SWT.BORDER);
		time.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		lblNumSat = new Label(parent, SWT.NONE);
		lblNumSat.setText("Num Sat");
		new Label(parent, SWT.NONE);

		numSat = new Text(parent, SWT.BORDER);
		numSat.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		lblNumSat = new Label(parent, SWT.NONE);
		lblNumSat.setText("Empty Cycles");
		new Label(parent, SWT.NONE);

		emptyCycles = new Text(parent, SWT.BORDER);
		emptyCycles.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);
		new Label(parent, SWT.NONE);

		parent.getDisplay().timerExec(100, this);
	}

	@Override
	public void setFocus() {

	}

	@Override
	public void run() {
		if (baro.isDisposed()) {
			return;
		}
		baro.getDisplay().timerExec(100, this);
		DatenModel dm = DatenModel.getInstance();
		accelX.setText(Integer.toString(dm.getAccel()[0]));
		accelY.setText(Integer.toString(dm.getAccel()[1]));
		accelZ.setText(Integer.toString(dm.getAccel()[2]));

		accelGX.setText(Double.toString(dm.getAccelRelative()[0]));
		accelGY.setText(Double.toString(dm.getAccelRelative()[1]));
		accelGZ.setText(Double.toString(dm.getAccelRelative()[2]));

		gyroX.setText(Integer.toString(dm.getGyro()[0]));
		gyroY.setText(Integer.toString(dm.getGyro()[1]));
		gyroZ.setText(Integer.toString(dm.getGyro()[2]));

		gyroDX.setText(Double.toString(dm.getGyroInRadSec()[0]));
		gyroDY.setText(Double.toString(dm.getGyroInRadSec()[1]));
		gyroDZ.setText(Double.toString(dm.getGyroInRadSec()[2]));

		quatW.setText(Double.toString(dm.getQuat()[0]));
		quatX.setText(Double.toString(dm.getQuat()[1]));
		quatY.setText(Double.toString(dm.getQuat()[2]));
		quatZ.setText(Double.toString(dm.getQuat()[3]));

		quatDiffW.setText(Double.toString(dm.getDiff()[0]));
		quatDiffX.setText(Double.toString(dm.getDiff()[1]));
		quatDiffY.setText(Double.toString(dm.getDiff()[2]));
		quatDiffZ.setText(Double.toString(dm.getDiff()[3]));

		thrustA.setText(Double.toString(dm.getThrust()[0]));
		thrustB.setText(Double.toString(dm.getThrust()[1]));
		thrustC.setText(Double.toString(dm.getThrust()[2]));
		thrustD.setText(Double.toString(dm.getThrust()[3]));

		input0.setText(Short.toString(dm.getInput()[0]));
		input1.setText(Short.toString(dm.getInput()[1]));
		input2.setText(Short.toString(dm.getInput()[2]));
		input3.setText(Short.toString(dm.getInput()[3]));

		magX.setText(Integer.toString(dm.getMag()[0]));
		magY.setText(Integer.toString(dm.getMag()[1]));
		magZ.setText(Integer.toString(dm.getMag()[2]));
		magMaxX.setText(Integer.toString(dm.getMagMax()[0]));
		magMaxY.setText(Integer.toString(dm.getMagMax()[1]));
		magMaxZ.setText(Integer.toString(dm.getMagMax()[2]));
		magMinX.setText(Integer.toString(dm.getMagMin()[0]));
		magMinY.setText(Integer.toString(dm.getMagMin()[1]));
		magMinZ.setText(Integer.toString(dm.getMagMin()[2]));
		magScaledX.setText(Double.toString(dm.getMagScaled()[0]));
		magScaledY.setText(Double.toString(dm.getMagScaled()[1]));
		magScaledZ.setText(Double.toString(dm.getMagScaled()[2]));

		roll.setText(Double.toString(dm.getRollPitchYaw()[0]));
		pitch.setText(Double.toString(dm.getRollPitchYaw()[1]));
		yaw.setText(Double.toString(dm.getRollPitchYaw()[2]));

		baro.setText(Float.toString(dm.getBaro()[0]));

		latitude.setText(Integer.toString(dm.getGps().getLatitude()));
		longitude.setText(Integer.toString(dm.getGps().getLongitude()));
		altitude.setText(Integer.toString(dm.getGps().getAltitude()));
		fix.setText(dm.getGps().isFix() ? "Fix" : "No Fix");

		lat.setText(Double.toString(dm.getGps().getLat()));
		lon.setText(Double.toString(dm.getGps().getLon()));
		alt.setText(Double.toString(dm.getGps().getAlt()));

		time.setText(new DateTime(dm.getGps().getMillis()).toString("yyMMdd hh:mm:ss:S"));
		numSat.setText(Byte.toString(dm.getGps().getNum_sats()));
		emptyCycles.setText(Long.toString(dm.getCycles()[0]));
	}
}