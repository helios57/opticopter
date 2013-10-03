package ch.sharpsoft.arducopter.client.view;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.forms.widgets.FormToolkit;
import org.eclipse.ui.forms.widgets.Section;
import org.eclipse.ui.part.ViewPart;

import ch.sharpsoft.arducopter.client.view.composite.CompositeMag;
import ch.sharpsoft.arducopter.client.view.composite.CompositeRc;

public class Kalibrierung extends ViewPart {
	public static final String ID = "ch.sharpsoft.arducopter.client.view.Kalibrierung";
	private final FormToolkit formToolkit = new FormToolkit(Display.getDefault());

	public Kalibrierung() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		parent.setLayout(new FillLayout(SWT.HORIZONTAL));

		final ScrolledComposite scrolledComposite = new ScrolledComposite(parent, SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL);
		formToolkit.adapt(scrolledComposite);
		formToolkit.paintBordersFor(scrolledComposite);
		scrolledComposite.setExpandHorizontal(true);
		scrolledComposite.setExpandVertical(true);
		final Composite container = new Composite(scrolledComposite, SWT.NONE);
		container.setLayout(new GridLayout(1, false));

		final Section sctnMag = formToolkit.createSection(container, Section.TWISTIE | Section.TITLE_BAR);
		sctnMag.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		formToolkit.paintBordersFor(sctnMag);
		sctnMag.setText("Mag");
		sctnMag.setExpanded(true);

		final Composite composite = new Composite(sctnMag, SWT.NONE);
		formToolkit.adapt(composite);
		formToolkit.paintBordersFor(composite);
		sctnMag.setClient(composite);
		composite.setLayout(new GridLayout(2, false));

		final CompositeMag compositeMagCurrent = new CompositeMag(composite, SWT.NONE);
		compositeMagCurrent.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false, 2, 1));
		formToolkit.adapt(compositeMagCurrent);
		formToolkit.paintBordersFor(compositeMagCurrent);

		final Section sectionRc = formToolkit.createSection(container, Section.TWISTIE | Section.TITLE_BAR);
		sectionRc.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		formToolkit.paintBordersFor(sectionRc);
		sectionRc.setText("RC");
		sectionRc.setExpanded(true);

		final CompositeRc compositeRc = new CompositeRc(sectionRc, SWT.NONE);
		formToolkit.adapt(compositeRc);
		formToolkit.paintBordersFor(compositeRc);
		sectionRc.setClient(compositeRc);
		scrolledComposite.setContent(container);
		scrolledComposite.setMinSize(container.computeSize(SWT.DEFAULT, SWT.DEFAULT));
	}

	@Override
	public void setFocus() {

	}
}
