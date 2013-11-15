package ch.sharpsoft.arducopter.client.view.composite;

import org.eclipse.core.databinding.DataBindingContext;
import org.eclipse.core.databinding.UpdateValueStrategy;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.jface.databinding.swt.WidgetProperties;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import ch.sharpsoft.arducopter.client.model.prop.ModelPID;
import ch.sharpsoft.arducopter.client.uart.IParamReciever;
import ch.sharpsoft.arducopter.client.uart.Parser;
import ch.sharpsoft.arducopter.client.uart.SerialService;

public class CompositePID extends Composite {
	private final ModelPID model = new ModelPID();
	private final IParamReciever paramReciever = new IParamReciever() {

		@Override
		public void recieved(final float[] data) {
			for (int i = 0; i < 9; i++) {
				setValueSafe(model.getPidSavedOV(i), data[i]);
			}
		}
	};

	public CompositePID(final Composite parent, final int style) {
		super(parent, style);

		final DataBindingContext bindingContext = new DataBindingContext();

		setLayout(new GridLayout(7, false));
		new Label(this, SWT.NONE);

		Label lbl = new Label(this, SWT.NONE);
		lbl.setText("Saved P      ");
		lbl = new Label(this, SWT.NONE);
		lbl.setText("Saved I      ");
		lbl = new Label(this, SWT.NONE);
		lbl.setText("Saved D      ");
		lbl = new Label(this, SWT.NONE);
		lbl.setText("New P        ");
		lbl = new Label(this, SWT.NONE);
		lbl.setText("New I        ");
		lbl = new Label(this, SWT.NONE);
		lbl.setText("New D        ");

		new Label(this, SWT.NONE).setText("Roll ");

		final Text textRollP = new Text(this, SWT.NONE);
		textRollP.setEnabled(false);
		applyBounds(textRollP);
		glue(bindingContext, textRollP, model.getPidSavedOV(0));

		final Text textRollI = new Text(this, SWT.NONE);
		textRollI.setEnabled(false);
		applyBounds(textRollI);
		glue(bindingContext, textRollI, model.getPidSavedOV(1));

		final Text textRollD = new Text(this, SWT.NONE);
		textRollD.setEnabled(false);
		applyBounds(textRollD);
		glue(bindingContext, textRollD, model.getPidSavedOV(2));

		final Text textRollPnew = new Text(this, SWT.NONE);
		textRollPnew.setEnabled(true);
		applyBounds(textRollPnew);
		glue(bindingContext, textRollPnew, model.getPidNewOV(0));

		final Text textRollInew = new Text(this, SWT.NONE);
		textRollInew.setEnabled(true);
		applyBounds(textRollInew);
		glue(bindingContext, textRollInew, model.getPidNewOV(1));

		final Text textRollDnew = new Text(this, SWT.NONE);
		textRollDnew.setEnabled(true);
		applyBounds(textRollDnew);
		glue(bindingContext, textRollDnew, model.getPidNewOV(2));

		new Label(this, SWT.NONE).setText("Pitch ");

		final Text textPitchP = new Text(this, SWT.NONE);
		textPitchP.setEnabled(false);
		applyBounds(textPitchP);
		glue(bindingContext, textPitchP, model.getPidSavedOV(3));

		final Text textPitchI = new Text(this, SWT.NONE);
		textPitchI.setEnabled(false);
		applyBounds(textPitchI);
		glue(bindingContext, textPitchI, model.getPidSavedOV(4));

		final Text textPitchD = new Text(this, SWT.NONE);
		textPitchD.setEnabled(false);
		applyBounds(textPitchD);
		glue(bindingContext, textPitchD, model.getPidSavedOV(5));

		final Text textPitchPnew = new Text(this, SWT.NONE);
		textPitchPnew.setEnabled(true);
		applyBounds(textPitchPnew);
		glue(bindingContext, textPitchPnew, model.getPidNewOV(3));

		final Text textPitchInew = new Text(this, SWT.NONE);
		textPitchInew.setEnabled(true);
		applyBounds(textPitchInew);
		glue(bindingContext, textPitchInew, model.getPidNewOV(4));

		final Text textPitchDnew = new Text(this, SWT.NONE);
		textPitchDnew.setEnabled(true);
		applyBounds(textPitchDnew);
		glue(bindingContext, textPitchDnew, model.getPidNewOV(5));

		new Label(this, SWT.NONE).setText("Yaw ");

		final Text textYawP = new Text(this, SWT.NONE);
		textYawP.setEnabled(false);
		applyBounds(textYawP);
		glue(bindingContext, textYawP, model.getPidSavedOV(6));

		final Text textYawI = new Text(this, SWT.NONE);
		textYawI.setEnabled(false);
		applyBounds(textYawI);
		glue(bindingContext, textYawI, model.getPidSavedOV(7));

		final Text textYawD = new Text(this, SWT.NONE);
		textYawD.setEnabled(false);
		applyBounds(textYawD);
		glue(bindingContext, textYawD, model.getPidSavedOV(8));

		final Text textYawPnew = new Text(this, SWT.NONE);
		textYawPnew.setEnabled(true);
		applyBounds(textYawPnew);
		glue(bindingContext, textYawPnew, model.getPidNewOV(6));

		final Text textYawInew = new Text(this, SWT.NONE);
		textYawInew.setEnabled(true);
		applyBounds(textYawInew);
		glue(bindingContext, textYawInew, model.getPidNewOV(7));

		final Text textYawDnew = new Text(this, SWT.NONE);
		textYawDnew.setEnabled(true);
		applyBounds(textYawDnew);
		glue(bindingContext, textYawDnew, model.getPidNewOV(8));

		Button load = new Button(this, SWT.NONE);
		load.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				SerialService.getInstance().requestParam(Parser.ID_IN_PARAM_PID, paramReciever);
			}
		});
		load.setText("load");
		Button save = new Button(this, SWT.NONE);
		save.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				float[] data = new float[9];
				for (int i = 0; i < 9; i++) {
					data[i] = ((Float) model.getPidNewOV(i).getValue()).floatValue();
				}
				SerialService.getInstance().writeParam(Parser.ID_IN_PARAM_PID, data);
				SerialService.getInstance().requestParam(Parser.ID_IN_PARAM_PID, paramReciever);
			}
		});
		save.setText("save");
	}

	private void glue(final DataBindingContext bindingContext, final Text textRollP, final IObservableValue ov) {
		UpdateValueStrategy viewToModel = new UpdateValueStrategy();
		viewToModel.setConverter(new TextToFloatConverter());
		UpdateValueStrategy modelToView = new UpdateValueStrategy();
		modelToView.setConverter(new FloatToTextConverter());
		IObservableValue observeTextTxtBlubberObserveWidget = WidgetProperties.text(SWT.Modify).observe(textRollP);
		bindingContext.bindValue(observeTextTxtBlubberObserveWidget, ov, viewToModel, modelToView);
	}

	private void applyBounds(final Text text) {
		text.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
	}

	public ModelPID getModelPID() {
		return model;
	}

	private void setValueSafe(final IObservableValue ov, final Object o) {
		ov.getRealm().asyncExec(new Runnable() {

			@Override
			public void run() {
				ov.setValue(o);
			}
		});
	}

	@Override
	protected void checkSubclass() {
		// Disable the check that prevents subclassing of SWT components
	}
}