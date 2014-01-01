package ch.sharpsoft.opticopterandroid;

import org.opencv.android.NativeCameraView;

import android.content.Context;
import android.util.AttributeSet;

public class NativeCameraHDRdy extends NativeCameraView {

	public NativeCameraHDRdy(Context context, int cameraId) {
		super(context, cameraId);
	}

	public NativeCameraHDRdy(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	@Override
	protected boolean connectCamera(int width, int height) {
		return super.connectCamera(1280, 720);
	}
}
