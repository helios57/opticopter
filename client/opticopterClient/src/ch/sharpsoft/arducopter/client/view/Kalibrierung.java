package ch.sharpsoft.arducopter.client.view;

import org.eclipse.core.databinding.DataBindingContext;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.jface.databinding.swt.WidgetProperties;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.forms.widgets.FormToolkit;
import org.eclipse.ui.forms.widgets.Section;
import org.eclipse.ui.part.ViewPart;

import ch.sharpsoft.arducopter.client.model.prop.ModelMag;
import ch.sharpsoft.arducopter.client.uart.DatenModel;
import ch.sharpsoft.arducopter.client.view.composite.CompositeMag;
import ch.sharpsoft.arducopter.client.view.composite.CompositeRc;

public class Kalibrierung extends ViewPart {
	public static final String ID = "ch.sharpsoft.arducopter.client.view.Kalibrierung";
	private final ModelMag magCurrent = new ModelMag(DatenModel.getInstance().getMagMax(), DatenModel.getInstance().getMagMin());
	private final ModelMag magNew = new ModelMag(new short[3], new short[3]);
	private final FormToolkit formToolkit = new FormToolkit(Display.getDefault());
	private Button button_1;

	public Kalibrierung() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		parent.setLayout(new FillLayout(SWT.HORIZONTAL));

		ScrolledComposite scrolledComposite = new ScrolledComposite(parent, SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL);
		formToolkit.adapt(scrolledComposite);
		formToolkit.paintBordersFor(scrolledComposite);
		scrolledComposite.setExpandHorizontal(true);
		scrolledComposite.setExpandVertical(true);
		Composite container = new Composite(scrolledComposite, SWT.NONE);
		container.setLayout(new GridLayout(1, false));

		Section sctnMag = formToolkit.createSection(container, Section.TWISTIE | Section.TITLE_BAR);
		sctnMag.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		formToolkit.paintBordersFor(sctnMag);
		sctnMag.setText("Mag");
		sctnMag.setExpanded(true);

		Composite composite = new Composite(sctnMag, SWT.NONE);
		formToolkit.adapt(composite);
		formToolkit.paintBordersFor(composite);
		sctnMag.setClient(composite);
		composite.setLayout(new GridLayout(5, false));

		Label lblSaved = new Label(composite, SWT.NONE);
		lblSaved.setText("Saved");
		formToolkit.adapt(lblSaved, true, true);

		Button button = new Button(composite, SWT.NONE);
		button.setText("Reload");
		formToolkit.adapt(button, true, true);

		Label label_1 = new Label(composite, SWT.NONE);
		label_1.setText("New");
		formToolkit.adapt(label_1, true, true);

		button_1 = new Button(composite, SWT.CHECK);
		button_1.setText("update");
		formToolkit.adapt(button_1, true, true);

		Button button_2 = new Button(composite, SWT.NONE);
		button_2.setText("Save");
		formToolkit.adapt(button_2, true, true);

		CompositeMag compositeMagCurrent = new CompositeMag(composite, SWT.NONE);
		compositeMagCurrent.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false, 2, 1));
		compositeMagCurrent.setModelMag(magCurrent);
		formToolkit.adapt(compositeMagCurrent);
		formToolkit.paintBordersFor(compositeMagCurrent);

		CompositeMag compositeMagNew = new CompositeMag(composite, SWT.NONE);
		compositeMagNew.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false, 3, 1));
		compositeMagNew.setModelMag(magNew);
		formToolkit.adapt(compositeMagNew);
		formToolkit.paintBordersFor(compositeMagNew);

		Section sctnLevel = formToolkit.createSection(container, Section.TWISTIE | Section.TITLE_BAR);
		sctnLevel.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		formToolkit.paintBordersFor(sctnLevel);
		sctnLevel.setText("RC");
		sctnLevel.setExpanded(true);

		Composite composite_1 = new Composite(sctnLevel, SWT.NONE);
		formToolkit.adapt(composite_1);
		formToolkit.paintBordersFor(composite_1);
		sctnLevel.setClient(composite_1);

		CompositeRc compositeRc = new CompositeRc(composite_1, SWT.NONE);
		compositeRc.setBounds(0, 0, 495, 301);
		formToolkit.adapt(compositeRc);
		formToolkit.paintBordersFor(compositeRc);
		scrolledComposite.setContent(container);
		scrolledComposite.setMinSize(container.computeSize(SWT.DEFAULT, SWT.DEFAULT));
		magCurrent.getEditable().setValue(Boolean.FALSE);
		magNew.getEditable().setValue(Boolean.TRUE);
		initDataBindings();
	}

	@Override
	public void setFocus() {

	}

	protected DataBindingContext initDataBindings() {
		DataBindingContext bindingContext = new DataBindingContext();
		//
		IObservableValue observeSelectionButton_1ObserveWidget = WidgetProperties.selection().observe(button_1);
		bindingContext.bindValue(observeSelectionButton_1ObserveWidget, magNew.getUpdating(), null, null);
		//
		return bindingContext;
	}
}
