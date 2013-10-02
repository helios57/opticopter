package ch.sharpsoft.arducopter.client.model.prop;

import java.util.Arrays;

import org.eclipse.core.databinding.beans.PojoObservables;
import org.eclipse.core.databinding.observable.value.IObservableValue;
import org.eclipse.core.databinding.observable.value.WritableValue;

public class ModelMag {
	private final short[] magMax;
	private final short[] magMin;
	private final IObservableValue editable = new WritableValue();
	private final IObservableValue updating = new WritableValue();
	private final IObservableValue magMaxXOV = PojoObservables.observeValue(this, "magMaxX");
	private final IObservableValue magMaxYOV = PojoObservables.observeValue(this, "magMaxY");
	private final IObservableValue magMaxZOV = PojoObservables.observeValue(this, "magMaxZ");
	private final IObservableValue magMinXOV = PojoObservables.observeValue(this, "magMinX");
	private final IObservableValue magMinYOV = PojoObservables.observeValue(this, "magMinY");
	private final IObservableValue magMinZOV = PojoObservables.observeValue(this, "magMinZ");

	public ModelMag(final short[] magMax, final short[] magMin) {
		this.magMax = magMax;
		this.magMin = magMin;
		updating.setValue(Boolean.FALSE);
		editable.setValue(Boolean.FALSE);
	}

	public void updateMinMax(final short[] mag) {
		updating.getRealm().asyncExec(new Runnable() {
			@Override
			public void run() {
				if ((Boolean) updating.getValue()) {
					for (int i = 0; i < 3; i++) {
						magMax[i] = (short) Math.max(magMax[i], mag[i]);
						magMin[i] = (short) Math.min(magMin[i], mag[i]);
					}
					magMaxXOV.setValue(magMax[0]);
					magMaxYOV.setValue(magMax[1]);
					magMaxZOV.setValue(magMax[2]);
					magMinXOV.setValue(magMin[0]);
					magMinYOV.setValue(magMin[1]);
					magMinZOV.setValue(magMin[2]);
				}
			}
		});
	}

	public final short getMagMaxX() {
		return magMax[0];
	}

	public final void setMagMaxX(final short magMaxX) {
		magMax[0] = magMaxX;
	}

	public final short getMagMaxY() {
		return magMax[1];
	}

	public final void setMagMaxY(final short magMaxY) {
		this.magMax[1] = magMaxY;
	}

	public final short getMagMaxZ() {
		return magMax[2];
	}

	public final void setMagMaxZ(final short magMaxZ) {
		this.magMax[2] = magMaxZ;
	}

	public final short getMagMinX() {
		return magMin[0];
	}

	public final void setMagMinX(final short magMinX) {
		this.magMin[0] = magMinX;
	}

	public final short getMagMinY() {
		return magMin[1];
	}

	public final void setMagMinY(final short magMinY) {
		this.magMin[1] = magMinY;
	}

	public final short getMagMinZ() {
		return magMin[2];
	}

	public final void setMagMinZ(final short magMinZ) {
		this.magMin[2] = magMinZ;
	}

	public IObservableValue getEditable() {
		return editable;
	}

	public IObservableValue getUpdating() {
		return updating;
	}

	public final short[] getMagMax() {
		return magMax;
	}

	public final short[] getMagMin() {
		return magMin;
	}

	public final IObservableValue getMagMaxXOV() {
		return magMaxXOV;
	}

	public final IObservableValue getMagMaxYOV() {
		return magMaxYOV;
	}

	public final IObservableValue getMagMaxZOV() {
		return magMaxZOV;
	}

	public final IObservableValue getMagMinXOV() {
		return magMinXOV;
	}

	public final IObservableValue getMagMinYOV() {
		return magMinYOV;
	}

	public final IObservableValue getMagMinZOV() {
		return magMinZOV;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + Arrays.hashCode(magMax);
		result = prime * result + Arrays.hashCode(magMin);
		return result;
	}

	@Override
	public boolean equals(final Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		ModelMag other = (ModelMag) obj;
		if (!Arrays.equals(magMax, other.magMax))
			return false;
		if (!Arrays.equals(magMin, other.magMin))
			return false;
		return true;
	}
}
