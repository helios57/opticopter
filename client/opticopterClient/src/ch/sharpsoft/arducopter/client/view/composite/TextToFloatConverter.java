package ch.sharpsoft.arducopter.client.view.composite;

import org.eclipse.core.databinding.conversion.Converter;

public class TextToFloatConverter extends Converter {

	public TextToFloatConverter() {
		super(String.class, Float.class);
	}

	@Override
	public Object convert(final Object fromObject) {
		if (fromObject instanceof String) {
			String str = (String) fromObject;
			return Float.valueOf(str);
		}
		return Float.valueOf(0);
	}

}
