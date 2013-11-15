package ch.sharpsoft.arducopter.client.model.prop;

import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.core.databinding.observable.value.WritableValue;

public class ModelPID {
	private final IObservableValue[] pidSavedOV = new IObservableValue[9];
	private final IObservableValue[] pidNewOV = new IObservableValue[9];

	public ModelPID() {
		for (int i = 0; i < 9; i++) {
			pidSavedOV[i] = new WritableValue(Float.valueOf(0), Float.class);
			pidNewOV[i] = new WritableValue(Float.valueOf(0), Float.class);
		}
	}

	public IObservableValue getPidSavedOV(final int i) {
		return pidSavedOV[i];
	}

	public IObservableValue getPidNewOV(final int i) {
		return pidNewOV[i];
	}
}
