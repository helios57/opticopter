package ch.sharpsoft.arducopter.client.view;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.part.ViewPart;

import ch.sharpsoft.arducopter.client.uart.DatenModel;

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

	}

	@Override
	public void setFocus() {
		// Set the focus
	}

}
