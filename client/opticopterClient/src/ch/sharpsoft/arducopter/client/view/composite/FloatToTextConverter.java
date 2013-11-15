package ch.sharpsoft.arducopter.client.view.composite;

import java.text.DecimalFormat;

import org.eclipse.core.databinding.conversion.Converter;

public class FloatToTextConverter extends Converter {

	public FloatToTextConverter() {
		super(Float.class, String.class);
	}

	@Override
	public Object convert(final Object fromObject) {
		if (fromObject instanceof Float) {
			return new DecimalFormat("#.########").format(fromObject);
		}
		return "";
	}

}
