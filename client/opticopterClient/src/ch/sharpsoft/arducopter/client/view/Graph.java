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
import org.eclipse.swt.widgets.Shell;
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

			// final CircularBufferDataProvider rollData = createTrace("Roll");
			final CircularBufferDataProvider rollDataInt = createTrace("Roll");
			// final CircularBufferDataProvider rollDataD = createTrace("RollD");
			// final CircularBufferDataProvider rollDataDD = createTrace("RollDD");
			// final CircularBufferDataProvider rollDataGyro = createTrace("RollGyro");
			// final CircularBufferDataProvider rollDataKalman = createTrace("RollKalman");
			// final CircularBufferDataProvider pitchData = createTrace("Pitch");
			// final CircularBufferDataProvider pitchDataD = createTrace("PitchD");
			// final CircularBufferDataProvider pitchDataDD = createTrace("PitchDD");
			// final CircularBufferDataProvider pitchDataGyro = createTrace("PitchGyro");
			// final CircularBufferDataProvider pitchDataKalman = createTrace("PitchKalman");
			// final CircularBufferDataProvider yawData = createTrace("Yaw");
			// final CircularBufferDataProvider yawDataD = createTrace("YawD");
			// final CircularBufferDataProvider yawDataDD = createTrace("YawDD");
			// final CircularBufferDataProvider yawDataGyro = createTrace("YawGyro");
			// final CircularBufferDataProvider yawDataKalman = createTrace("YawKalman");
			// final CircularBufferDataProvider posDataZ = createTrace("Z");
			// final CircularBufferDataProvider posDataZD = createTrace("ZD");
			// final CircularBufferDataProvider posDataZDD = createTrace("ZDD");
			// final CircularBufferDataProvider posDataZRel = createTrace("ZRel");
			// final CircularBufferDataProvider posDataY = createTrace("Y");
			// final CircularBufferDataProvider posDataYD = createTrace("YD");
			// final CircularBufferDataProvider posDataYDD = createTrace("YDD");
			// final CircularBufferDataProvider posDataYRel = createTrace("YRel");
			// final CircularBufferDataProvider out1Data = createTrace("Out1");
			// final CircularBufferDataProvider out2Data = createTrace("Out2");
			// final CircularBufferDataProvider out3Data = createTrace("Out3");
			// final CircularBufferDataProvider out4Data = createTrace("Out4");
			// final CircularBufferDataProvider in1Data = createTrace("In1");
			// final CircularBufferDataProvider in2Data = createTrace("In2");
			// final CircularBufferDataProvider in3Data = createTrace("In3");
			// final CircularBufferDataProvider in4Data = createTrace("In4");
			// final CircularBufferDataProvider accelData0 = createTrace("Accel0");
			// final CircularBufferDataProvider accelData1 = createTrace("Accel1");
			// final CircularBufferDataProvider accelKData1 = createTrace("AccelK1");
			// final CircularBufferDataProvider accelDiffData0 = createTrace("AccelDiff0");
			// final CircularBufferDataProvider accelDiffData1 = createTrace("AccelDiff1");
			// final CircularBufferDataProvider gyroData0 = createTrace("Gyro0");
			// final CircularBufferDataProvider gyroData1 = createTrace("Gyro1");

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
					final int[] rollPitchYawInt = dm.getRollPitchYawInt();
					final double[] posXYZ = dm.getPosXYZ();
					final double[] posXYZD = dm.getPosXYZDiff();
					final double[] posXYZDD = dm.getPosXYZDiffDiff();
					final double[] posXYZRel = dm.getPosXYZRelative();
					final short[] output = dm.getOutput();
					final short[] input = dm.getInput();
					final int[] accel = dm.getAccel();
					final int[] accelK = dm.getAccelK();
					final int[] accelDiff = dm.getAccelDiff();
					final int[] gyro = dm.getGyro();

					final long timestamp = System.currentTimeMillis();
					final Sample roll = new Sample(timestamp, rollPitchYaw[0]);
					final Sample rollInt = new Sample(timestamp, rollPitchYawInt[0]);
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

					final Sample posY = new Sample(timestamp, posXYZ[1]);
					final Sample posYD = new Sample(timestamp, posXYZD[1]);
					final Sample posYDD = new Sample(timestamp, posXYZDD[1]);
					final Sample posYRel = new Sample(timestamp, posXYZRel[1]);

					final Sample out1 = new Sample(timestamp, output[0]);
					final Sample out2 = new Sample(timestamp, output[1]);
					final Sample out3 = new Sample(timestamp, output[2]);
					final Sample out4 = new Sample(timestamp, output[3]);

					final Sample in1 = new Sample(timestamp, input[0]);
					final Sample in2 = new Sample(timestamp, input[1]);
					final Sample in3 = new Sample(timestamp, input[2]);
					final Sample in4 = new Sample(timestamp, input[3]);

					final Sample accel0 = new Sample(timestamp, accel[0]);
					final Sample accel1 = new Sample(timestamp, accel[1]);
					final Sample accelK1 = new Sample(timestamp, accelK[1]);

					final Sample accelDiff0 = new Sample(timestamp, accelDiff[0]);
					final Sample accelDiff1 = new Sample(timestamp, accelDiff[1]);

					final Sample gyro0 = new Sample(timestamp, gyro[0]);
					final Sample gyro1 = new Sample(timestamp, gyro[1]);

					final Shell shell = Graph.this.getSite().getShell();
					if (shell == null || shell.isDisposed()) {
						return;
					}
					shell.getDisplay().asyncExec(new Runnable() {

						@Override
						public void run() {
							// rollData.addSample(roll);
							rollDataInt.addSample(rollInt);
							// rollDataD.addSample(rollD);
							// rollDataDD.addSample(rollDD);
							// rollDataGyro.addSample(rollG);
							// rollDataKalman.addSample(rollK);
							// pitchData.addSample(pitch);
							// pitchDataD.addSample(pitchD);
							// pitchDataDD.addSample(pitchDD);
							// pitchDataGyro.addSample(pitchG);
							// pitchDataKalman.addSample(pitchK);
							// yawData.addSample(yaw);
							// yawDataD.addSample(yawD);
							// yawDataDD.addSample(yawDD);
							// yawDataGyro.addSample(yawG);
							// posDataZ.addSample(posZ);
							// posDataZD.addSample(posZD);
							// posDataZDD.addSample(posZDD);
							// posDataZRel.addSample(posZRel);

							// posDataY.addSample(posY);
							// posDataYD.addSample(posYD);
							// posDataYDD.addSample(posYDD);
							// posDataYRel.addSample(posYRel);

							// out1Data.addSample(out1);
							// out2Data.addSample(out2);
							// out3Data.addSample(out3);
							// out4Data.addSample(out4);
							// in1Data.addSample(in1);
							// in2Data.addSample(in2);
							// in3Data.addSample(in3);
							// in4Data.addSample(in4);
							// accelData0.addSample(accel0);
							// accelData1.addSample(accel1);
							// accelKData1.addSample(accelK1);

							// accelDiffData0.addSample(accelDiff0);
							// accelDiffData1.addSample(accelDiff1);

							// gyroData0.addSample(gyro0);
							// gyroData1.addSample(gyro1);
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
