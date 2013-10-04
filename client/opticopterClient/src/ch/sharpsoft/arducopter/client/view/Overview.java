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

import ch.sharpsoft.arducopter.client.uart.SerialConnection;

public class Overview extends ViewPart {
	private Text port;
	private Text baud;

	public Overview() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		parent.setLayout(new GridLayout(2, false));

		Label lblPort = new Label(parent, SWT.NONE);
		lblPort.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblPort.setText("Port");

		port = new Text(parent, SWT.BORDER);
		port.setText("/dev/ttyACM0");
		port.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Label lblBaud = new Label(parent, SWT.NONE);
		lblBaud.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblBaud.setText("Baud");

		baud = new Text(parent, SWT.BORDER);
		baud.setText("115200");
		baud.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(parent, SWT.NONE);

		Button btnConnect = new Button(parent, SWT.NONE);
		btnConnect.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				SerialConnection.getInstance().connect(port.getText(), baud.getText());
			}
		});
		btnConnect.setText("Connect");

	}

	@Override
	public void setFocus() {

	}

}
