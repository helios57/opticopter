package ch.sharpsoft.arducopter.client.view;

import java.util.concurrent.atomic.AtomicLong;

import org.csstudio.swt.xygraph.dataprovider.CircularBufferDataProvider;
import org.csstudio.swt.xygraph.dataprovider.Sample;
import org.csstudio.swt.xygraph.figures.Axis;
import org.csstudio.swt.xygraph.figures.ToolbarArmedXYGraph;
import org.csstudio.swt.xygraph.figures.Trace;
import org.csstudio.swt.xygraph.figures.Trace.PointStyle;
import org.csstudio.swt.xygraph.figures.Trace.TraceType;
import org.csstudio.swt.xygraph.figures.XYGraph;
import org.csstudio.swt.xygraph.linearscale.AbstractScale.LabelSide;
import org.csstudio.swt.xygraph.util.XYGraphMediaFactory;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.LightweightSystem;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.part.ViewPart;

import ch.sharpsoft.arducopter.client.model.DataCalculated;
import ch.sharpsoft.arducopter.client.uart.DatenModel;
import ch.sharpsoft.arducopter.client.uart.NewDataListener;

public class Graph extends ViewPart {

	public static final String ID = "ch.sharpsoft.arducopter.client.view.Graph"; //$NON-NLS-1$

	public Graph() {
	}

	@Override
	public void createPartControl(final Composite parent) {
		Composite container = new Composite(parent, SWT.NONE);
		container.setLayout(new FillLayout(SWT.HORIZONTAL));
		Canvas c = new Canvas(container, SWT.NONE);
		final LightweightSystem lws = new LightweightSystem(c);
		final XYGraphTest testFigure = new XYGraphTest();
		lws.setContents(testFigure);
	}

	@Override
	public void setFocus() {
	}

	class XYGraphTest extends Figure {
		public XYGraph xyGraph;
		private final ToolbarArmedXYGraph toolbarArmedXYGraph;

		public XYGraphTest() {
			xyGraph = new XYGraph();
			xyGraph.setTitle("Rot-Data");
			xyGraph.setFont(XYGraphMediaFactory.getInstance().getFont(XYGraphMediaFactory.FONT_TAHOMA));
			xyGraph.primaryXAxis.setTitle("Time");
			xyGraph.primaryYAxis.setTitle("Amplitude");
			xyGraph.primaryXAxis.setDateEnabled(true);
			xyGraph.primaryXAxis.setAutoScale(true);
			xyGraph.primaryXAxis.setShowMajorGrid(true);
			xyGraph.primaryYAxis.setShowMajorGrid(true);
			xyGraph.primaryYAxis.setRange(-Math.PI, Math.PI);
			xyGraph.primaryXAxis.setAutoScaleThreshold(0);

			final Axis xAxis = new Axis("timestamp", false);
			xAxis.setTickLableSide(LabelSide.Primary);
			xAxis.setAutoScale(true);
			xyGraph.addAxis(xAxis);

			final Axis yAxis = new Axis("Degree", true);
			yAxis.setRange(-2 * Math.PI, 2 * Math.PI);
			yAxis.setAutoScale(true);
			yAxis.setForegroundColor(XYGraphMediaFactory.getInstance().getColor(XYGraphMediaFactory.COLOR_PINK));
			yAxis.setTickLableSide(LabelSide.Primary);
			xyGraph.addAxis(yAxis);

			final CircularBufferDataProvider rollData = createTrace("Roll");
			final CircularBufferDataProvider rollDataGyro = createTrace("RollGyro");
			final CircularBufferDataProvider rollDataKalman = createTrace("RollKalman");
			final CircularBufferDataProvider pitchData = createTrace("Pitch");
			final CircularBufferDataProvider pitchDataGyro = createTrace("PitchGyro");
			final CircularBufferDataProvider pitchDataKalman = createTrace("PitchKalman");

			toolbarArmedXYGraph = new ToolbarArmedXYGraph(xyGraph);
			add(toolbarArmedXYGraph);
			xyGraph.setFocusTraversable(true);
			xyGraph.setRequestFocusEnabled(true);

			final DatenModel dm = DatenModel.getInstance();
			final DataCalculated dc = DataCalculated.getInstance();
			final AtomicLong cunter = new AtomicLong();
			dm.addEventListener(new NewDataListener() {

				@Override
				public void onNewData() {
					final double[] rollPitchYaw = dc.getRollPitchYaw();
					final double[] rollPitchYawGyro = dc.getGyroRps();
					final double[] rollPitchYawKalman = dc.getRollPitchYawFiltered();
					final long timestamp = cunter.incrementAndGet();

					final Sample roll = new Sample(timestamp, rollPitchYaw[0]);
					final Sample pitch = new Sample(timestamp, rollPitchYaw[1]);

					final Sample rollG = new Sample(timestamp, rollPitchYawGyro[0]);
					final Sample pitchG = new Sample(timestamp, rollPitchYawGyro[1]);

					final Sample rollK = new Sample(timestamp, rollPitchYawKalman[0]);
					final Sample pitchK = new Sample(timestamp, rollPitchYawKalman[1]);

					final Shell shell = Graph.this.getSite().getShell();
					if (shell == null || shell.isDisposed()) {
						return;
					}
					shell.getDisplay().asyncExec(new Runnable() {

						@Override
						public void run() {
							rollData.addSample(roll);
							// rollDataGyro.addSample(rollG);
							rollDataKalman.addSample(rollK);
							// pitchData.addSample(pitch);
							// pitchDataGyro.addSample(pitchG);
							// pitchDataKalman.addSample(pitchK);
						}
					});
				}
			});
		}

		private CircularBufferDataProvider createTrace(final String desc) {
			final CircularBufferDataProvider rollData = new CircularBufferDataProvider(true);
			rollData.setBufferSize(5000);
			final Trace rotTraceRoll = new Trace(desc, xyGraph.primaryXAxis, xyGraph.primaryYAxis, rollData);
			rotTraceRoll.setTraceType(TraceType.SOLID_LINE);
			rotTraceRoll.setLineWidth(1);
			rotTraceRoll.setPointStyle(PointStyle.POINT);
			rotTraceRoll.setPointSize(2);
			rotTraceRoll.setAntiAliasing(true);
			xyGraph.addTrace(rotTraceRoll);
			return rollData;
		}

		@Override
		protected void layout() {
			toolbarArmedXYGraph.setBounds(bounds.getCopy().shrink(5, 5));
			super.layout();
		}
	}
}
