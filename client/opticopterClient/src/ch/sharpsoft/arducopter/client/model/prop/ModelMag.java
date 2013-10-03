package ch.sharpsoft.arducopter.client.model.prop;

import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.core.databinding.observable.value.WritableValue;

public class ModelMag {
	private final IObservableValue updatingMax = new WritableValue(Boolean.FALSE, Boolean.class);
	private final IObservableValue updatingMin = new WritableValue(Boolean.FALSE, Boolean.class);

	private final IObservableValue[] magMaxSavedOV = new IObservableValue[3];
	private final IObservableValue[] magMinSavedOV = new IObservableValue[3];
	private final IObservableValue[] magCurrentOV = new IObservableValue[3];
	private final IObservableValue[] magMaxNewOV = new IObservableValue[3];
	private final IObservableValue[] magMinNewOV = new IObservableValue[3];

	public ModelMag() {
		for (int i = 0; i < 3; i++) {
			magMaxSavedOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			magMinSavedOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			magCurrentOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			magMaxNewOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
			magMinNewOV[i] = new WritableValue(Short.valueOf((short) 0), Short.class);
		}
	}

	public void updateMinMax(final short[] mag) {
		magMaxNewOV[0].getRealm().asyncExec(new Runnable() {
			@Override
			public void run() {
				for (int i = 0; i < 3; i++) {
					if ((Boolean) updatingMax.getValue()) {
						magMaxNewOV[i].setValue((short) Math.max((Short) magMaxNewOV[i].getValue(), mag[i]));
					}
					if ((Boolean) updatingMin.getValue()) {
						magMinNewOV[i].setValue((short) Math.min((Short) magMinNewOV[i].getValue(), mag[i]));
					}
					magCurrentOV[i].setValue(mag[i]);
				}
			}
		});
	}

	public final IObservableValue getUpdatingMax() {
		return updatingMax;
	}

	public final IObservableValue getUpdatingMin() {
		return updatingMin;
	}

	public final IObservableValue getMagMaxSavedOV(final int i) {
		return magMaxSavedOV[i];
	}

	public final IObservableValue getMagMinSavedOV(final int i) {
		return magMinSavedOV[i];
	}

	public final IObservableValue getMagCurrentOV(final int i) {
		return magCurrentOV[i];
	}

	public final IObservableValue getMagMaxNewOV(final int i) {
		return magMaxNewOV[i];
	}

	public final IObservableValue getMagMinNewOV(final int i) {
		return magMinNewOV[i];
	}
}
