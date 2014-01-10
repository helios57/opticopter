package ch.sharpsoft.arducopter.client.view;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.net.Socket;

import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.part.ViewPart;

public class LiveVideo extends ViewPart {

	public static final String ID = "ch.sharpsoft.arducopter.client.view.LiveVideo"; //$NON-NLS-1$
	private Text ipAdress;
	private Text port;
	private Canvas canvas;
	private Image img;

	public LiveVideo() {
	}

	/**
	 * Create contents of the view part.
	 * 
	 * @param parent
	 */
	@Override
	public void createPartControl(final Composite parent) {
		Composite container = new Composite(parent, SWT.NONE);
		container.setLayout(new GridLayout(5, false));
		{
			Label lblIp = new Label(container, SWT.NONE);
			lblIp.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
			lblIp.setText("ip:");
		}
		{
			ipAdress = new Text(container, SWT.BORDER);
			ipAdress.setText("192.168.1.176");
			ipAdress.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		}
		{
			Label lblPort = new Label(container, SWT.NONE);
			lblPort.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
			lblPort.setText("port:");
		}
		{
			port = new Text(container, SWT.BORDER);
			port.setText("1337");
			port.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, true, false, 1, 1));
		}
		{
			Button btnConnect = new Button(container, SWT.NONE);
			btnConnect.addSelectionListener(new SelectionAdapter() {
				@Override
				public void widgetSelected(final SelectionEvent e) {
					final String ip = ipAdress.getText();
					final String portText = port.getText();
					new Thread(new Runnable() {

						@Override
						public void run() {
							byte[] jpgBuffer = new byte[262144];
							int index = 0;
							try (Socket android = new Socket(ip, Integer.valueOf(portText))) {
								InputStream is = android.getInputStream();
								byte[] preamble = new byte[] { 111, 22, 33, 44, 55 };
								boolean valid = false;
								while (true) {
									if (!valid) {
										int pre0 = is.read();
										while (pre0 != preamble[0]) {
											pre0 = is.read();
										}
										valid = true;
										for (int i = 1; i < 5; i++) {
											if (preamble[i] != is.read()) {
												valid = false;
												break;
											}
										}
										if (!valid) {
											continue;
										}
									}
									while (index < jpgBuffer.length) {
										final int read = is.read();
										if (read == -1) {
											throw new RuntimeException("Stream closed");
										}
										jpgBuffer[index++] = (byte) read;
										if (index > 5) {
											boolean end = true;
											for (int i = 0; i < 5; i++) {
												if (!(preamble[i] == jpgBuffer[index - 5 + i])) {
													end = false;
													break;
												}
											}
											if (end) {
												final ImageData id = new ImageData(new ByteArrayInputStream(jpgBuffer));
												canvas.getDisplay().asyncExec(new Runnable() {

													@Override
													public void run() {
														img = new Image(canvas.getDisplay(), id);
														canvas.redraw();
													}
												});
												valid = true;
												break;
											}
										}
									}
									index = 0;
								}
							} catch (Exception e1) {
								throw new RuntimeException(e1);
							}
						}
					}).start();

				}
			});
			btnConnect.setText("Connect");
		}
		{
			canvas = new Canvas(container, SWT.NONE);
			GridData gd_canvas = new GridData(SWT.FILL, SWT.FILL, true, true, 5, 1);
			gd_canvas.widthHint = 1280;
			gd_canvas.heightHint = 720;
			canvas.setLayoutData(gd_canvas);
			canvas.setSize(720, 1280);
			canvas.addPaintListener(new PaintListener() {

				@Override
				public void paintControl(final PaintEvent e) {
					if (img != null && !img.isDisposed()) {
						e.gc.drawImage(img, 0, 0);
						img.dispose();
					}
				}
			});
		}

		createActions();
		initializeToolBar();
		initializeMenu();
	}

	/**
	 * Create the actions.
	 */
	private void createActions() {
		// Create the actions
	}

	/**
	 * Initialize the toolbar.
	 */
	private void initializeToolBar() {
		IToolBarManager toolbarManager = getViewSite().getActionBars().getToolBarManager();
	}

	/**
	 * Initialize the menu.
	 */
	private void initializeMenu() {
		IMenuManager menuManager = getViewSite().getActionBars().getMenuManager();
	}

	@Override
	public void setFocus() {
		// Set the focus
	}

}
