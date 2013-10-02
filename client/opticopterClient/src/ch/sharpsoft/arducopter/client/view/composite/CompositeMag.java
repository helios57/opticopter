package ch.sharpsoft.arducopter.client.view.composite;

import org.eclipse.core.databinding.DataBindingContext;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.jface.databinding.swt.SWTObservables;
import org.eclipse.jface.databinding.swt.WidgetProperties;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Spinner;

public class CompositeMag extends Composite {

	private DataBindingContext m_bindingContext;
	private ch.sharpsoft.arducopter.client.model.prop.ModelMag modelMag;
	private final Spinner magMaxXSpinner;
	private final Spinner magMaxYSpinner;
	private final Spinner magMaxZSpinner;
	private final Spinner magMinXSpinner;
	private final Spinner magMinYSpinner;
	private final Spinner magMinZSpinner;

	public CompositeMag(final Composite parent, final int style, final ch.sharpsoft.arducopter.client.model.prop.ModelMag newModelMag) {
		this(parent, style);
		setModelMag(newModelMag);
	}

	public CompositeMag(final Composite parent, final int style) {
		super(parent, style);
		setLayout(new GridLayout(2, false));

		new Label(this, SWT.NONE).setText("MagMaxX:");

		magMaxXSpinner = new Spinner(this, SWT.BORDER);
		magMaxXSpinner.setMaximum(Short.MAX_VALUE);
		magMaxXSpinner.setMinimum(Short.MIN_VALUE);
		magMaxXSpinner.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		new Label(this, SWT.NONE).setText("MagMaxY:");

		magMaxYSpinner = new Spinner(this, SWT.BORDER);
		magMaxYSpinner.setMaximum(Short.MAX_VALUE);
		magMaxYSpinner.setMinimum(Short.MIN_VALUE);
		magMaxYSpinner.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		new Label(this, SWT.NONE).setText("MagMaxZ:");

		magMaxZSpinner = new Spinner(this, SWT.BORDER);
		magMaxZSpinner.setMaximum(Short.MAX_VALUE);
		magMaxZSpinner.setMinimum(Short.MIN_VALUE);
		magMaxZSpinner.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		new Label(this, SWT.NONE).setText("MagMinX:");

		magMinXSpinner = new Spinner(this, SWT.BORDER);
		magMinXSpinner.setMaximum(Short.MAX_VALUE);
		magMinXSpinner.setMinimum(Short.MIN_VALUE);
		magMinXSpinner.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		new Label(this, SWT.NONE).setText("MagMinY:");

		magMinYSpinner = new Spinner(this, SWT.BORDER);
		magMinYSpinner.setMaximum(Short.MAX_VALUE);
		magMinYSpinner.setMinimum(Short.MIN_VALUE);
		magMinYSpinner.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		new Label(this, SWT.NONE).setText("MagMinZ:");

		magMinZSpinner = new Spinner(this, SWT.BORDER);
		magMinZSpinner.setMaximum(Short.MAX_VALUE);
		magMinZSpinner.setMinimum(Short.MIN_VALUE);
		magMinZSpinner.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		if (modelMag != null) {
			m_bindingContext = initDataBindings();
		}
	}

	@Override
	protected void checkSubclass() {
		// Disable the check that prevents subclassing of SWT components
	}

	public ch.sharpsoft.arducopter.client.model.prop.ModelMag getModelMag() {
		return modelMag;
	}

	public void setModelMag(final ch.sharpsoft.arducopter.client.model.prop.ModelMag newModelMag) {
		setModelMag(newModelMag, true);
	}

	public void setModelMag(final ch.sharpsoft.arducopter.client.model.prop.ModelMag newModelMag, final boolean update) {
		modelMag = newModelMag;
		if (update) {
			if (m_bindingContext != null) {
				m_bindingContext.dispose();
				m_bindingContext = null;
			}
			if (modelMag != null) {
				m_bindingContext = initDataBindings();
			}
		}
	}

	protected DataBindingContext initDataBindings() {
		DataBindingContext bindingContext = new DataBindingContext();
		//
		IObservableValue magMaxXObserveWidget = SWTObservables.observeSelection(magMaxXSpinner);
		bindingContext.bindValue(magMaxXObserveWidget, modelMag.getMagMaxXOV(), null, null);
		//
		IObservableValue magMaxYObserveWidget = SWTObservables.observeSelection(magMaxYSpinner);
		bindingContext.bindValue(magMaxYObserveWidget, modelMag.getMagMaxYOV(), null, null);
		//
		IObservableValue magMaxZObserveWidget = SWTObservables.observeSelection(magMaxZSpinner);
		bindingContext.bindValue(magMaxZObserveWidget, modelMag.getMagMaxZOV(), null, null);
		//
		IObservableValue magMinXObserveWidget = SWTObservables.observeSelection(magMinXSpinner);
		bindingContext.bindValue(magMinXObserveWidget, modelMag.getMagMinXOV(), null, null);
		//
		IObservableValue magMinYObserveWidget = SWTObservables.observeSelection(magMinYSpinner);
		bindingContext.bindValue(magMinYObserveWidget, modelMag.getMagMinYOV(), null, null);
		//
		IObservableValue magMinZObserveWidget = SWTObservables.observeSelection(magMinZSpinner);
		bindingContext.bindValue(magMinZObserveWidget, modelMag.getMagMinZOV(), null, null);
		//
		IObservableValue observeEnabledMagMaxXSpinnerObserveWidget = WidgetProperties.enabled().observe(magMaxXSpinner);
		bindingContext.bindValue(observeEnabledMagMaxXSpinnerObserveWidget, modelMag.getEditable(), null, null);
		//
		IObservableValue observeEnabledMagMaxYSpinnerObserveWidget = WidgetProperties.enabled().observe(magMaxYSpinner);
		bindingContext.bindValue(observeEnabledMagMaxYSpinnerObserveWidget, modelMag.getEditable(), null, null);
		//
		IObservableValue observeEnabledMagMaxZSpinnerObserveWidget = WidgetProperties.enabled().observe(magMaxZSpinner);
		bindingContext.bindValue(observeEnabledMagMaxZSpinnerObserveWidget, modelMag.getEditable(), null, null);
		//
		IObservableValue observeEnabledMagMinXSpinnerObserveWidget = WidgetProperties.enabled().observe(magMinXSpinner);
		bindingContext.bindValue(observeEnabledMagMinXSpinnerObserveWidget, modelMag.getEditable(), null, null);
		//
		IObservableValue observeEnabledMagMinYSpinnerObserveWidget = WidgetProperties.enabled().observe(magMinYSpinner);
		bindingContext.bindValue(observeEnabledMagMinYSpinnerObserveWidget, modelMag.getEditable(), null, null);
		//
		IObservableValue observeEnabledMagMinZSpinnerObserveWidget = WidgetProperties.enabled().observe(magMinZSpinner);
		bindingContext.bindValue(observeEnabledMagMinZSpinnerObserveWidget, modelMag.getEditable(), null, null);
		//
		return bindingContext;
	}
}
