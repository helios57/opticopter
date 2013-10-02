package ch.sharpsoft.arducopter.client.model.prop;

import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.core.databinding.observable.value.WritableValue;

public class ModelRC {
	private final IObservableValue updating = new WritableValue();
	private final IObservableValue[] rcMaxSavedOV = new IObservableValue[8];
	private final IObservableValue[] rcMinSavedOV = new IObservableValue[8];
	private final IObservableValue[] rcDefaultSavedOV = new IObservableValue[8];
	private final IObservableValue[] rcMaxNewOV = new IObservableValue[8];
	private final IObservableValue[] rcMinNewOV = new IObservableValue[8];
	private final IObservableValue[] rcDefaultNewOV = new IObservableValue[8];
	private final IObservableValue[] rcCurrentOV = new IObservableValue[8];

	public ModelRC() {
		for (int i = 0; i < 8; i++) {
			rcMaxSavedOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			rcMinSavedOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			rcDefaultSavedOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			rcMaxNewOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			rcMinNewOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			rcDefaultNewOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			rcCurrentOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
		}
	}

	public void setDefault() {
		rcMaxSavedOV[0].getRealm().asyncExec(new Runnable() {

			@Override
			public void run() {
				for (int i = 0; i < 8; i++) {
					rcDefaultNewOV[i].setValue(rcCurrentOV[i].getValue());
				}
			}
		});
	}

	public void updateRc(final short[] rc) {
		rcMaxSavedOV[0].getRealm().asyncExec(new Runnable() {

			@Override
			public void run() {
				for (int i = 0; i < 8; i++) {
					rcCurrentOV[i].setValue(rc[i]);
					if ((Boolean) updating.getValue()) {
						rcMaxNewOV[i].setValue((short) Math.max(rc[i], (Short) rcMaxNewOV[i].getValue()));
						rcMinNewOV[i].setValue((short) Math.min(rc[i], (Short) rcMinNewOV[i].getValue()));
					}
				}
			}
		});
	}

	public IObservableValue getRcMaxSavedOV(final int i) {
		return rcMaxSavedOV[i];
	}

	public IObservableValue getRcMinSavedOV(final int i) {
		return rcMinSavedOV[i];
	}

	public IObservableValue getRcDefaultSavedOV(final int i) {
		return rcDefaultSavedOV[i];
	}

	public IObservableValue getRcMaxNewOV(final int i) {
		return rcMaxNewOV[i];
	}

	public IObservableValue getRcMinNewOV(final int i) {
		return rcMinNewOV[i];
	}

	public IObservableValue getRcDefaultNewOV(final int i) {
		return rcDefaultNewOV[i];
	}

	public IObservableValue getRcCurrentOV(final int i) {
		return rcCurrentOV[i];
	}

	public IObservableValue getUpdating() {
		return updating;
	}
}
