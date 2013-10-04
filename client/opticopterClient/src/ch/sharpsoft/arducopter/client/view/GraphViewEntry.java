package ch.sharpsoft.arducopter.client.view;

import org.csstudio.swt.xygraph.dataprovider.CircularBufferDataProvider;
import org.csstudio.swt.xygraph.dataprovider.Sample;
import org.csstudio.swt.xygraph.figures.Trace;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.core.databinding.observable.value.IValueChangeListener;
import org.eclipse.core.databinding.observable.value.ValueChangeEvent;
import org.eclipse.core.databinding.observable.value.WritableValue;

public class GraphViewEntry {
	private final IObservableValue enabled;
	private final String desc;
	private CircularBufferDataProvider data;
	private Trace trace;

	public GraphViewEntry(final String desc, final IObservableValue sample) {
		this.desc = desc;
		enabled = new WritableValue(Boolean.FALSE, Boolean.class);
		sample.addValueChangeListener(new IValueChangeListener() {

			@Override
			public void handleValueChange(final ValueChangeEvent event) {
				if (data != null && trace != null && ((Boolean) enabled.getValue())) {
					data.addSample(new Sample(System.currentTimeMillis(), (Double) event.diff.getNewValue()));
				}
			}
		});
	}

	public String getDesc() {
		return desc;
	}

	public void setDataBuffer(final CircularBufferDataProvider data) {
		this.data = data;
	}

	public void setTrace(final Trace trace) {
		this.trace = trace;
	}

	public Trace getTrace() {
		return trace;
	}

	public final IObservableValue getEnabled() {
		return enabled;
	}
}
