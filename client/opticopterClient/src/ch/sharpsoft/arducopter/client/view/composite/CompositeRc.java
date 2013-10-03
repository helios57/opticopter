package ch.sharpsoft.arducopter.client.view.composite;

import org.eclipse.core.databinding.DataBindingContext;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.jface.databinding.swt.SWTObservables;
import org.eclipse.jface.databinding.swt.WidgetProperties;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Spinner;

import ch.sharpsoft.arducopter.client.model.prop.ModelRC;
import ch.sharpsoft.arducopter.client.uart.DatenModel;
import ch.sharpsoft.arducopter.client.uart.NewDataListener;

public class CompositeRc extends Composite {
	private final ModelRC modelRc = new ModelRC();
	private final Button btnUpdateMax;
	private final Button btnUpdateMin;

	public CompositeRc(final Composite parent, final int style) {
		super(parent, style);

		final DataBindingContext bindingContext = new DataBindingContext();

		setLayout(new GridLayout(8, false));
		new Label(this, SWT.NONE);

		Label lblSavedrcmax = new Label(this, SWT.NONE);
		lblSavedrcmax.setText("SavedMax");

		Label lblSavedmin = new Label(this, SWT.NONE);
		lblSavedmin.setText("SavedMin");

		Label lblSaveddefault = new Label(this, SWT.NONE);
		lblSaveddefault.setText("SavedDefault");

		Label lblCurrent = new Label(this, SWT.NONE);
		lblCurrent.setText("Current");

		Label lblNewLabel = new Label(this, SWT.NONE);
		lblNewLabel.setText("NewMax");

		Label lblNewmin = new Label(this, SWT.NONE);
		lblNewmin.setText("NewMin");

		Label lblNewdefault = new Label(this, SWT.NONE);
		lblNewdefault.setText("NewDefault");
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);

		btnUpdateMax = new Button(this, SWT.CHECK);
		btnUpdateMax.setText("update");

		btnUpdateMin = new Button(this, SWT.CHECK);
		btnUpdateMin.setText("update");

		Button btnSet = new Button(this, SWT.NONE);
		btnSet.setText("set");

		for (int i = 0; i < 8; i++) {
			new Label(this, SWT.NONE).setText("Ch " + i);
			final Spinner spinnerSavedMax = new Spinner(this, SWT.BORDER);
			spinnerSavedMax.setEnabled(false);
			spinnerSavedMax.setMaximum(Short.MAX_VALUE);
			spinnerSavedMax.setMinimum(Short.MIN_VALUE);
			spinnerSavedMax.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerSavedMax), modelRc.getRcMaxSavedOV(i), null, null);

			final Spinner spinnerSavedMin = new Spinner(this, SWT.BORDER);
			spinnerSavedMin.setEnabled(false);
			spinnerSavedMin.setMaximum(Short.MAX_VALUE);
			spinnerSavedMin.setMinimum(Short.MIN_VALUE);
			spinnerSavedMin.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerSavedMin), modelRc.getRcMinSavedOV(i), null, null);

			final Spinner spinnerSavedDefault = new Spinner(this, SWT.BORDER);
			spinnerSavedDefault.setEnabled(false);
			spinnerSavedDefault.setMaximum(Short.MAX_VALUE);
			spinnerSavedDefault.setMinimum(Short.MIN_VALUE);
			spinnerSavedDefault.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerSavedDefault), modelRc.getRcDefaultSavedOV(i), null, null);

			final Spinner spinnerCurrent = new Spinner(this, SWT.BORDER);
			spinnerCurrent.setEnabled(false);
			spinnerCurrent.setMaximum(Short.MAX_VALUE);
			spinnerCurrent.setMinimum(Short.MIN_VALUE);
			spinnerCurrent.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerCurrent), modelRc.getRcCurrentOV(i), null, null);

			final Spinner spinnerNewMax = new Spinner(this, SWT.BORDER);
			spinnerNewMax.setMaximum(Short.MAX_VALUE);
			spinnerNewMax.setMinimum(Short.MIN_VALUE);
			spinnerNewMax.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerNewMax), modelRc.getRcMaxNewOV(i), null, null);

			final Spinner spinnerNewMin = new Spinner(this, SWT.BORDER);
			spinnerNewMin.setMaximum(Short.MAX_VALUE);
			spinnerNewMin.setMinimum(Short.MIN_VALUE);
			spinnerNewMin.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerNewMin), modelRc.getRcMinNewOV(i), null, null);

			final Spinner spinnerNewDefault = new Spinner(this, SWT.BORDER);
			spinnerNewDefault.setMaximum(Short.MAX_VALUE);
			spinnerNewDefault.setMinimum(Short.MIN_VALUE);
			spinnerNewDefault.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerNewDefault), modelRc.getRcDefaultNewOV(i), null, null);
		}
		new Label(this, SWT.NONE);

		Button button = new Button(this, SWT.NONE);
		button.setText("reload");

		Button button_1 = new Button(this, SWT.NONE);
		button_1.setText("reload");

		Button btnReload_2 = new Button(this, SWT.NONE);
		btnReload_2.setText("reload");
		new Label(this, SWT.NONE);

		Button btnSave = new Button(this, SWT.NONE);
		btnSave.setText("save");

		Button btnSave_1 = new Button(this, SWT.NONE);
		btnSave_1.setText("save");

		Button btnSave_2 = new Button(this, SWT.NONE);
		btnSave_2.setText("save");
		DatenModel.getInstance().addEventListener(new NewDataListener() {

			@Override
			public void onNewData() {
				modelRc.updateRc(DatenModel.getInstance().getInput());
			}
		});
		initDataBindings();
	}

	public ModelRC getModelRc() {
		return modelRc;
	}

	@Override
	protected void checkSubclass() {
		// Disable the check that prevents subclassing of SWT components
	}

	protected DataBindingContext initDataBindings() {
		DataBindingContext bindingContext = new DataBindingContext();
		//
		IObservableValue observeSelectionBtnUpdateMaxObserveWidget = WidgetProperties.selection().observe(btnUpdateMax);
		bindingContext.bindValue(observeSelectionBtnUpdateMaxObserveWidget, modelRc.getUpdatingMax(), null, null);
		//
		IObservableValue observeSelectionBtnUpdateMinObserveWidget = WidgetProperties.selection().observe(btnUpdateMin);
		bindingContext.bindValue(observeSelectionBtnUpdateMinObserveWidget, modelRc.getUpdatingMin(), null, null);
		//
		return bindingContext;
	}
}
