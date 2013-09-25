package ch.sharpsoft.arducopter.client.view;

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
import org.eclipse.ui.part.ViewPart;

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
			final CircularBufferDataProvider rollDataD = createTrace("RollD");
			final CircularBufferDataProvider rollDataDD = createTrace("RollDD");
			final CircularBufferDataProvider rollDataGyro = createTrace("RollGyro");
			final CircularBufferDataProvider rollDataKalman = createTrace("RollKalman");
			final CircularBufferDataProvider pitchData = createTrace("Pitch");
			final CircularBufferDataProvider pitchDataD = createTrace("PitchD");
			final CircularBufferDataProvider pitchDataDD = createTrace("PitchDD");
			final CircularBufferDataProvider pitchDataGyro = createTrace("PitchGyro");
			final CircularBufferDataProvider pitchDataKalman = createTrace("PitchKalman");
			final CircularBufferDataProvider yawData = createTrace("Yaw");
			final CircularBufferDataProvider yawDataD = createTrace("YawD");
			final CircularBufferDataProvider yawDataDD = createTrace("YawDD");
			final CircularBufferDataProvider yawDataGyro = createTrace("YawGyro");
			final CircularBufferDataProvider yawDataKalman = createTrace("YawKalman");
			final CircularBufferDataProvider posDataZ = createTrace("Z");
			final CircularBufferDataProvider posDataZD = createTrace("ZD");
			final CircularBufferDataProvider posDataZDD = createTrace("ZDD");
			final CircularBufferDataProvider posDataZRel = createTrace("ZRel");

			toolbarArmedXYGraph = new ToolbarArmedXYGraph(xyGraph);
			add(toolbarArmedXYGraph);
			xyGraph.setFocusTraversable(true);
			xyGraph.setRequestFocusEnabled(true);

			final DatenModel dm = DatenModel.getInstance();
			dm.addEventListener(new NewDataListener() {

				@Override
				public void onNewData() {
					final double[] rollPitchYaw = dm.getRollPitchYaw();
					final double[] rollPitchYawD = dm.getRollPitchYawDiff();
					final double[] rollPitchYawDD = dm.getRollPitchYawDiffDiff();
					final double[] rollPitchYawGyro = dm.getGyroInRadSec();
					final double[] rollPitchYawKalman = dm.getRollPitchYawKalman();
					final double[] posXYZ = dm.getPosXYZ();
					final double[] posXYZD = dm.getPosXYZDiff();
					final double[] posXYZDD = dm.getPosXYZDiffDiff();
					final double[] posXYZRel = dm.getPosXYZRelative();

					final long timestamp = System.currentTimeMillis();
					final Sample roll = new Sample(timestamp, rollPitchYaw[0]);
					final Sample pitch = new Sample(timestamp, rollPitchYaw[1]);
					final Sample yaw = new Sample(timestamp, rollPitchYaw[2]);

					final Sample rollD = new Sample(timestamp, rollPitchYawD[0]);
					final Sample pitchD = new Sample(timestamp, rollPitchYawD[1]);
					final Sample yawD = new Sample(timestamp, rollPitchYawD[2]);

					final Sample rollG = new Sample(timestamp, rollPitchYawGyro[0]);
					final Sample pitchG = new Sample(timestamp, rollPitchYawGyro[1]);
					final Sample yawG = new Sample(timestamp, rollPitchYawGyro[2]);

					final Sample rollK = new Sample(timestamp, rollPitchYawKalman[0]);
					final Sample pitchK = new Sample(timestamp, rollPitchYawKalman[1]);
					final Sample yawK = new Sample(timestamp, rollPitchYawKalman[2]);

					final Sample rollDD = new Sample(timestamp, rollPitchYawDD[0]);
					final Sample pitchDD = new Sample(timestamp, rollPitchYawDD[1]);
					final Sample yawDD = new Sample(timestamp, rollPitchYawDD[2]);

					final Sample posZ = new Sample(timestamp, posXYZ[2]);
					final Sample posZD = new Sample(timestamp, posXYZD[2]);
					final Sample posZDD = new Sample(timestamp, posXYZDD[2]);
					final Sample posZRel = new Sample(timestamp, posXYZRel[2]);

					Graph.this.getSite().getShell().getDisplay().asyncExec(new Runnable() {

						@Override
						public void run() {
							// rollData.addSample(roll);
							// rollDataD.addSample(rollD);
							// rollDataDD.addSample(rollDD);
							// rollDataGyro.addSample(rollG);
							// rollDataKalman.addSample(rollK);
							// pitchData.addSample(pitch);
							// pitchDataD.addSample(pitchD);
							// pitchDataDD.addSample(pitchDD);
							// pitchDataGyro.addSample(pitchG);
							// yawData.addSample(yaw);
							// yawDataD.addSample(yawD);
							// yawDataDD.addSample(yawDD);
							// yawDataGyro.addSample(yawG);
							posDataZ.addSample(posZ);
							posDataZD.addSample(posZD);
							posDataZDD.addSample(posZDD);
							posDataZRel.addSample(posZRel);
						}
					});
				}
			});
		}

		private CircularBufferDataProvider createTrace(final String desc) {
			final CircularBufferDataProvider rollData = new CircularBufferDataProvider(true);
			rollData.setBufferSize(10000);
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
