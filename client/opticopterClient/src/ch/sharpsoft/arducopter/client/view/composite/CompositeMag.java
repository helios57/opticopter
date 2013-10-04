package ch.sharpsoft.arducopter.client.view.composite;

import org.eclipse.core.databinding.DataBindingContext;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.jface.databinding.swt.SWTObservables;
import org.eclipse.jface.databinding.swt.WidgetProperties;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Spinner;

import ch.sharpsoft.arducopter.client.model.prop.ModelMag;
import ch.sharpsoft.arducopter.client.uart.DatenModel;
import ch.sharpsoft.arducopter.client.uart.IParamReciever;
import ch.sharpsoft.arducopter.client.uart.NewDataListener;
import ch.sharpsoft.arducopter.client.uart.Parser;
import ch.sharpsoft.arducopter.client.uart.SerialService;

public class CompositeMag extends Composite {
	private final ModelMag modelMag = new ModelMag();
	private final Button btnUpdateMax;
	private final Button btnUpdateMin;

	public CompositeMag(final Composite parent, final int style) {
		super(parent, style);
		final DataBindingContext bindingContext = new DataBindingContext();

		setLayout(new GridLayout(6, false));
		new Label(this, SWT.NONE);

		Label lblSavedrcmax = new Label(this, SWT.NONE);
		lblSavedrcmax.setText("SavedMax");

		Label lblSavedmin = new Label(this, SWT.NONE);
		lblSavedmin.setText("SavedMin");

		Label lblCurrent = new Label(this, SWT.NONE);
		lblCurrent.setText("CurrentValue");

		Label lblNewLabel = new Label(this, SWT.NONE);
		lblNewLabel.setText("NewMax");

		Label lblNewmin = new Label(this, SWT.NONE);
		lblNewmin.setText("NewMin");

		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);

		btnUpdateMax = new Button(this, SWT.CHECK);
		btnUpdateMax.setText("update");

		btnUpdateMin = new Button(this, SWT.CHECK);
		btnUpdateMin.setText("update");

		final String[] label = new String[] { "x", "y", "z" };

		for (int i = 0; i < 3; i++) {
			new Label(this, SWT.NONE).setText(label[i]);
			final Spinner spinnerSavedMax = new Spinner(this, SWT.BORDER);
			spinnerSavedMax.setEnabled(false);
			spinnerSavedMax.setMaximum(Short.MAX_VALUE);
			spinnerSavedMax.setMinimum(Short.MIN_VALUE);
			spinnerSavedMax.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerSavedMax), modelMag.getMagMaxSavedOV(i), null, null);

			final Spinner spinnerSavedMin = new Spinner(this, SWT.BORDER);
			spinnerSavedMin.setEnabled(false);
			spinnerSavedMin.setMaximum(Short.MAX_VALUE);
			spinnerSavedMin.setMinimum(Short.MIN_VALUE);
			spinnerSavedMin.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerSavedMin), modelMag.getMagMinSavedOV(i), null, null);

			final Spinner spinnerCurrent = new Spinner(this, SWT.BORDER);
			spinnerCurrent.setEnabled(false);
			spinnerCurrent.setMaximum(Short.MAX_VALUE);
			spinnerCurrent.setMinimum(Short.MIN_VALUE);
			spinnerCurrent.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerCurrent), modelMag.getMagCurrentOV(i), null, null);

			final Spinner spinnerNewMax = new Spinner(this, SWT.BORDER);
			spinnerNewMax.setMaximum(Short.MAX_VALUE);
			spinnerNewMax.setMinimum(Short.MIN_VALUE);
			spinnerNewMax.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerNewMax), modelMag.getMagMaxNewOV(i), null, null);

			final Spinner spinnerNewMin = new Spinner(this, SWT.BORDER);
			spinnerNewMin.setMaximum(Short.MAX_VALUE);
			spinnerNewMin.setMinimum(Short.MIN_VALUE);
			spinnerNewMin.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerNewMin), modelMag.getMagMinNewOV(i), null, null);
		}
		new Label(this, SWT.NONE);

		Button reloadSavedMax = new Button(this, SWT.NONE);
		reloadSavedMax.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				SerialService.getInstance().requestParam(Parser.ID_IN_PARAM_MAG_MAX, new IParamReciever() {

					@Override
					public void recieved(final short[] data) {
						for (int i = 0; i < 3; i++) {
							setValueSafe(modelMag.getMagMaxSavedOV(i), data[i]);
						}
					}
				});
			}
		});
		reloadSavedMax.setText("reload");

		Button reloadSavedMin = new Button(this, SWT.NONE);
		reloadSavedMin.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				SerialService.getInstance().requestParam(Parser.ID_IN_PARAM_MAG_MIN, new IParamReciever() {

					@Override
					public void recieved(final short[] data) {
						for (int i = 0; i < 3; i++) {
							setValueSafe(modelMag.getMagMinSavedOV(i), data[i]);
						}
					}
				});
			}
		});
		reloadSavedMin.setText("reload");

		new Label(this, SWT.NONE);

		Button saveMax = new Button(this, SWT.NONE);
		saveMax.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				short[] data = new short[3];
				for (int i = 0; i < 3; i++) {
					data[i] = ((Short) modelMag.getMagMaxNewOV(i).getValue()).shortValue();
				}
				SerialService.getInstance().writeParam(Parser.ID_IN_PARAM_MAG_MAX, data);
				SerialService.getInstance().requestParam(Parser.ID_IN_PARAM_MAG_MAX, new IParamReciever() {

					@Override
					public void recieved(final short[] data) {
						for (int i = 0; i < 3; i++) {
							setValueSafe(modelMag.getMagMaxSavedOV(i), data[i]);
						}
					}
				});
			}
		});
		saveMax.setText("save");

		Button saveMin = new Button(this, SWT.NONE);
		saveMin.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				short[] data = new short[3];
				for (int i = 0; i < 3; i++) {
					data[i] = ((Short) modelMag.getMagMinNewOV(i).getValue()).shortValue();
				}
				SerialService.getInstance().writeParam(Parser.ID_IN_PARAM_MAG_MIN, data);
				SerialService.getInstance().requestParam(Parser.ID_IN_PARAM_MAG_MIN, new IParamReciever() {

					@Override
					public void recieved(final short[] data) {
						for (int i = 0; i < 3; i++) {
							setValueSafe(modelMag.getMagMinSavedOV(i), data[i]);
						}
					}
				});
			}
		});
		saveMin.setText("save");

		initDataBindings();

		DatenModel.getInstance().addEventListener(new NewDataListener() {

			@Override
			public void onNewData() {
				modelMag.updateMinMax(DatenModel.getInstance().getMag());
			}
		});
	}

	private void setValueSafe(final IObservableValue ov, final Object o) {
		ov.getRealm().asyncExec(new Runnable() {

			@Override
			public void run() {
				ov.setValue(o);
			}
		});
	}

	public final ModelMag getModelMag() {
		return modelMag;
	}

	protected DataBindingContext initDataBindings() {
		DataBindingContext bindingContext = new DataBindingContext();
		//
		IObservableValue observeSelectionBtnUpdateMaxObserveWidget = WidgetProperties.selection().observe(btnUpdateMax);
		bindingContext.bindValue(observeSelectionBtnUpdateMaxObserveWidget, modelMag.getUpdatingMax(), null, null);
		//
		IObservableValue observeSelectionBtnUpdateMinObserveWidget = WidgetProperties.selection().observe(btnUpdateMin);
		bindingContext.bindValue(observeSelectionBtnUpdateMinObserveWidget, modelMag.getUpdatingMin(), null, null);
		//
		return bindingContext;
	}
}
