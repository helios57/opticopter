package ch.sharpsoft.arducopter.client.view;

import gnu.io.CommPortIdentifier;

import java.util.Enumeration;
import java.util.concurrent.ConcurrentLinkedQueue;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.part.ViewPart;

import ch.sharpsoft.arducopter.client.uart.DatenModel;

public class Console extends ViewPart implements Runnable {
	private Text text;
	private StyledText styledText;

	public Console() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		parent.setLayout(new GridLayout(2, false));

		styledText = new StyledText(parent, SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL);
		styledText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 2, 1));

		text = new Text(parent, SWT.BORDER);
		text.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Button btnSend = new Button(parent, SWT.NONE);
		btnSend.setText("Send");

		@SuppressWarnings("unchecked")
		Enumeration<CommPortIdentifier> ports = CommPortIdentifier.getPortIdentifiers();
		while (ports.hasMoreElements()) {
			styledText.setText(styledText.getText() + "\n" + ports.nextElement().getName());
		}
		parent.getDisplay().timerExec(1000, this);
	}

	@Override
	public void setFocus() {

	}

	@Override
	public void run() {
		if (styledText.isDisposed()) {
			return;
		}
		styledText.getDisplay().timerExec(1000, this);
		ConcurrentLinkedQueue<String> debugInfos = DatenModel.getInstance().getDebugInfos();
		StringBuffer sb = new StringBuffer();
		while (!debugInfos.isEmpty()) {
			sb.append("\n").append(debugInfos.poll());
		}
		sb.append(styledText.getText());
		styledText.setText(sb.toString());
	}
}
