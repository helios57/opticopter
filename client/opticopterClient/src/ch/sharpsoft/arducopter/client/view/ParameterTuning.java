package ch.sharpsoft.arducopter.client.view;

import java.io.IOException;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.part.ViewPart;

import ch.sharpsoft.arducopter.client.model.Kalman;
import ch.sharpsoft.arducopter.client.uart.DatenModel;
import ch.sharpsoft.arducopter.client.uart.SerialConnection;

public class ParameterTuning extends ViewPart {

	public static final String ID = "ch.sharpsoft.arducopter.client.view.ParameterTuning"; //$NON-NLS-1$

	public ParameterTuning() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		Composite container = new Composite(parent, SWT.NONE);
		container.setLayout(new GridLayout(2, false));
		{
			Label lblKalman = new Label(container, SWT.NONE);
			lblKalman.setText("Kalman");
		}
		new Label(container, SWT.NONE);

		Label lblQ = new Label(container, SWT.NONE);
		lblQ.setText("q / process noise covariance");
		final Text q = new Text(container, SWT.NONE);
		q.setText(Double.toString(DatenModel.getInstance().getKalmanModel1Droll().getQ()));
		q.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				try {
					Double value = Double.valueOf(q.getText());
					DatenModel.getInstance().getKalmanModel1Droll().setQ(value.doubleValue());
					DatenModel.getInstance().getKalmanModel1Dpitch().setQ(value.doubleValue());
				} catch (Exception ex) {

				}
			}
		});

		lblQ = new Label(container, SWT.NONE);
		lblQ.setText("r / measurement noise covariance");
		final Text r = new Text(container, SWT.NONE);
		r.setText(Double.toString(DatenModel.getInstance().getKalmanModel1Droll().getR()));
		r.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				try {
					Double value = Double.valueOf(r.getText());
					DatenModel.getInstance().getKalmanModel1Droll().setR(value.doubleValue());
					DatenModel.getInstance().getKalmanModel1Dpitch().setR(value.doubleValue());
				} catch (Exception ex) {

				}
			}
		});

		lblQ = new Label(container, SWT.NONE);
		lblQ.setText("p / estimation error covariance");
		final Text p = new Text(container, SWT.NONE);
		p.setText(Double.toString(DatenModel.getInstance().getKalmanModel1Droll().getP()));
		p.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				try {
					Double value = Double.valueOf(p.getText());
					DatenModel.getInstance().getKalmanModel1Droll().setP(value.doubleValue());
					DatenModel.getInstance().getKalmanModel1Dpitch().setP(value.doubleValue());
				} catch (Exception ex) {

				}
			}
		});

		lblQ = new Label(container, SWT.NONE);
		lblQ.setText("k / kalman gain");
		final Text k = new Text(container, SWT.NONE);
		k.setText(Double.toString(DatenModel.getInstance().getKalmanModel1Droll().getK()));
		p.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				try {
					Double value = Double.valueOf(k.getText());
					DatenModel.getInstance().getKalmanModel1Droll().setK(value.doubleValue());
					DatenModel.getInstance().getKalmanModel1Dpitch().setK(value.doubleValue());
				} catch (Exception ex) {

				}
			}
		});

		lblQ = new Label(container, SWT.NONE);
		lblQ.setText("Q_angle");
		final Text Q_angle = new Text(container, SWT.NONE);
		Q_angle.setText(Double.toString(Kalman.Q_angle));
		Q_angle.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				try {
					Double value = Double.valueOf(Q_angle.getText());
					Kalman.Q_angle = value.doubleValue();
				} catch (Exception ex) {
				}
			}
		});
		lblQ = new Label(container, SWT.NONE);
		lblQ.setText("Q_bias");
		final Text Q_bias = new Text(container, SWT.NONE);
		Q_bias.setText(Double.toString(Kalman.Q_bias));
		Q_bias.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				try {
					Double value = Double.valueOf(Q_bias.getText());
					Kalman.Q_bias = value.doubleValue();
				} catch (Exception ex) {
				}
			}
		});
		lblQ = new Label(container, SWT.NONE);
		lblQ.setText("R_measure");
		final Text R_measure = new Text(container, SWT.NONE);
		R_measure.setText(Double.toString(Kalman.R_measure));
		R_measure.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				try {
					Double value = Double.valueOf(R_measure.getText());
					Kalman.R_measure = value.doubleValue();
				} catch (Exception ex) {
				}
			}
		});
		Button start = new Button(container, SWT.PUSH);
		start.setText("Start");
		start.addSelectionListener(new SelectionAdapter() {

			@Override
			public void widgetSelected(final SelectionEvent e) {
				try {
					SerialConnection.main("");
				} catch (IOException e1) {
					e1.printStackTrace();
				}
			}
		});
	}

	@Override
	public void setFocus() {
		// Set the focus
	}

}
