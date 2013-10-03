package ch.sharpsoft.arducopter.client.view.composite;

import org.eclipse.core.databinding.DataBindingContext;
import org.eclipse.jface.databinding.swt.SWTObservables;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Spinner;

import ch.sharpsoft.arducopter.client.model.prop.ModelMag;

public class CompositeMag extends Composite {

	private final ModelMag modelMag = new ModelMag();

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
		lblCurrent.setText("Current");

		Label lblNewLabel = new Label(this, SWT.NONE);
		lblNewLabel.setText("NewMax");

		Label lblNewmin = new Label(this, SWT.NONE);
		lblNewmin.setText("NewMin");

		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);

		Button btnUpdateMax = new Button(this, SWT.CHECK);
		btnUpdateMax.setText("update");

		Button btnUpdateMin = new Button(this, SWT.CHECK);
		btnUpdateMin.setText("update");

		final String[] label = new String[] { "x", "y", "z" };

		for (int i = 0; i < 3; i++) {
			new Label(this, SWT.NONE).setText(label[i]);
			final Spinner spinnerSavedMax = new Spinner(this, SWT.BORDER);
			spinnerSavedMax.setEnabled(false);
			spinnerSavedMax.setMaximum(Short.MAX_VALUE);
			spinnerSavedMax.setMinimum(Short.MIN_VALUE);
			spinnerSavedMax.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerSavedMax), modelMag.getMagMaxSavedOV(i), null, null);

			final Spinner spinnerSavedMin = new Spinner(this, SWT.BORDER);
			spinnerSavedMin.setEnabled(false);
			spinnerSavedMin.setMaximum(Short.MAX_VALUE);
			spinnerSavedMin.setMinimum(Short.MIN_VALUE);
			spinnerSavedMin.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerSavedMin), modelMag.getMagMinSavedOV(i), null, null);

			final Spinner spinnerCurrent = new Spinner(this, SWT.BORDER);
			spinnerCurrent.setEnabled(false);
			spinnerCurrent.setMaximum(Short.MAX_VALUE);
			spinnerCurrent.setMinimum(Short.MIN_VALUE);
			spinnerCurrent.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerCurrent), modelMag.getMagCurrentOV(i), null, null);

			final Spinner spinnerNewMax = new Spinner(this, SWT.BORDER);
			spinnerNewMax.setMaximum(Short.MAX_VALUE);
			spinnerNewMax.setMinimum(Short.MIN_VALUE);
			spinnerNewMax.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerNewMax), modelMag.getMagMaxNewOV(i), null, null);

			final Spinner spinnerNewMin = new Spinner(this, SWT.BORDER);
			spinnerNewMin.setMaximum(Short.MAX_VALUE);
			spinnerNewMin.setMinimum(Short.MIN_VALUE);
			spinnerNewMin.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false));
			bindingContext.bindValue(SWTObservables.observeSelection(spinnerNewMin), modelMag.getMagMinNewOV(i), null, null);
		}
		new Label(this, SWT.NONE);

		Button button = new Button(this, SWT.NONE);
		button.setText("reload");

		Button button_1 = new Button(this, SWT.NONE);
		button_1.setText("reload");

		new Label(this, SWT.NONE);

		Button btnSave = new Button(this, SWT.NONE);
		btnSave.setText("save");

		Button btnSave_2 = new Button(this, SWT.NONE);
		btnSave_2.setText("save");
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
		new Label(this, SWT.NONE);
	}
}
