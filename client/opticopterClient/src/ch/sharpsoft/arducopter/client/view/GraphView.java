package ch.sharpsoft.arducopter.client.view;

import org.csstudio.swt.xygraph.dataprovider.CircularBufferDataProvider;
import org.csstudio.swt.xygraph.figures.ToolbarArmedXYGraph;
import org.csstudio.swt.xygraph.figures.Trace;
import org.csstudio.swt.xygraph.figures.Trace.PointStyle;
import org.csstudio.swt.xygraph.figures.Trace.TraceType;
import org.csstudio.swt.xygraph.figures.XYGraph;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.core.databinding.observable.value.WritableValue;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.LightweightSystem;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.ViewPart;

import swing2swt.layout.FlowLayout;
import ch.sharpsoft.arducopter.client.uart.DatenModel;
import ch.sharpsoft.arducopter.client.uart.NewDataListener;

public class GraphView extends ViewPart {

	public static final String ID = "ch.sharpsoft.arducopter.client.view.GraphView"; //$NON-NLS-1$
	private Composite checkboxes;
	private GraphFigure gf;

	public GraphView() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		final Composite container = new Composite(parent, SWT.NONE);
		container.setLayout(new GridLayout(1, false));
		checkboxes = new Composite(container, SWT.NONE);
		checkboxes.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		GridData gd_composite = new GridData(SWT.LEFT, SWT.CENTER, false, false, 1, 1);
		gd_composite.heightHint = 40;
		checkboxes.setLayoutData(gd_composite);
		final Canvas c = new Canvas(container, SWT.NONE);
		c.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		gf = new GraphFigure();
		new LightweightSystem(c).setContents(gf);
		final IObservableValue roll = new WritableValue(0.0, Double.class);
		final IObservableValue pitch = new WritableValue(0.0, Double.class);
		final IObservableValue yaw = new WritableValue(0.0, Double.class);
		final IObservableValue rollK = new WritableValue(0.0, Double.class);
		final IObservableValue pitchK = new WritableValue(0.0, Double.class);
		final IObservableValue yawK = new WritableValue(0.0, Double.class);

		final IObservableValue[] input = new IObservableValue[8];
		final IObservableValue[] output = new IObservableValue[8];
		for (int i = 0; i < 8; i++) {
			input[i] = new WritableValue(0.0, Double.class);
			output[i] = new WritableValue(0.0, Double.class);
		}

		DatenModel.getInstance().addEventListener(new NewDataListener() {

			@Override
			public void onNewData() {
				roll.getRealm().asyncExec(new Runnable() {

					@Override
					public void run() {
						roll.setValue(DatenModel.getInstance().getRollPitchYaw()[0]);
						pitch.setValue(DatenModel.getInstance().getRollPitchYaw()[1]);
						yaw.setValue(DatenModel.getInstance().getRollPitchYaw()[2]);
						rollK.setValue(DatenModel.getInstance().getRollPitchYawKalman()[0]);
						pitchK.setValue(DatenModel.getInstance().getRollPitchYawKalman()[1]);
						yawK.setValue(DatenModel.getInstance().getRollPitchYawKalman()[2]);
						for (int i = 0; i < 8; i++) {
							input[i].setValue(Double.valueOf(DatenModel.getInstance().getInput()[i]));
							output[i].setValue(Double.valueOf(DatenModel.getInstance().getOutput()[i]));
						}
					}
				});

			}
		});
		addObservableSample("Roll", roll);
		addObservableSample("Pitch", pitch);
		addObservableSample("Yaw", yaw);
		addObservableSample("RollK", rollK);
		addObservableSample("PitchK", pitchK);
		addObservableSample("YawK", yawK);
		for (int i = 0; i < 8; i++) {
			addObservableSample("IN" + i, input[i]);
		}
		for (int i = 0; i < 8; i++) {
			addObservableSample("OUT" + i, output[i]);
		}
	}

	public void addObservableSample(final String desc, final IObservableValue value) {
		final GraphViewEntry gve = new GraphViewEntry(desc, value);
		final Button check = new Button(checkboxes, SWT.CHECK);
		check.setText(desc);
		check.addSelectionListener(new SelectionAdapter() {

			@Override
			public void widgetSelected(final SelectionEvent e) {
				if (check.getSelection()) {
					gve.getEnabled().setValue(Boolean.TRUE);
					gf.addEntry(gve);
				} else {
					gve.getEnabled().setValue(Boolean.FALSE);
					gf.removeEntry(gve);
				}
			}
		});
		checkboxes.getParent().pack();
	}

	@Override
	public void setFocus() {
	}

	class GraphFigure extends Figure {
		public XYGraph xyGraph;
		private final ToolbarArmedXYGraph toolbarArmedXYGraph;

		public GraphFigure() {
			xyGraph = new XYGraph();
			xyGraph.setTitle("Debug");
			xyGraph.primaryXAxis.setTitle("Time");
			xyGraph.primaryYAxis.setTitle("Amplitude");
			xyGraph.primaryXAxis.setDateEnabled(true);
			xyGraph.primaryXAxis.setAutoScale(true);
			xyGraph.primaryXAxis.setShowMajorGrid(true);
			xyGraph.primaryYAxis.setShowMajorGrid(true);
			xyGraph.primaryYAxis.setAutoScale(true);
			toolbarArmedXYGraph = new ToolbarArmedXYGraph(xyGraph);
			add(toolbarArmedXYGraph);
			xyGraph.setFocusTraversable(true);
			xyGraph.setRequestFocusEnabled(true);
		}

		private void addEntry(final GraphViewEntry e) {
			final CircularBufferDataProvider dataBuffer = new CircularBufferDataProvider(true);
			e.setDataBuffer(dataBuffer);
			dataBuffer.setBufferSize(1000);
			final Trace trace = new Trace(e.getDesc(), xyGraph.primaryXAxis, xyGraph.primaryYAxis, dataBuffer);
			trace.setTraceType(TraceType.SOLID_LINE);
			trace.setLineWidth(1);
			trace.setPointStyle(PointStyle.POINT);
			trace.setPointSize(2);
			trace.setAntiAliasing(true);
			e.setTrace(trace);
			xyGraph.addTrace(trace);
		}

		private void removeEntry(final GraphViewEntry e) {
			xyGraph.removeTrace(e.getTrace());
		}

		@Override
		protected void layout() {
			toolbarArmedXYGraph.setBounds(bounds.getCopy().shrink(5, 5));
			super.layout();
		}
	}
}
