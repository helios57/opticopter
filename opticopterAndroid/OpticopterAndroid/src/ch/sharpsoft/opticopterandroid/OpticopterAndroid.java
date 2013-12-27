package ch.sharpsoft.opticopterandroid;

import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;

public class OpticopterAndroid {
	public OpticopterAndroid(String cascadeName, int minObjSize) {
		mNativeObj = nativeCreateObject(cascadeName, minObjSize);
	}

	public void detect(Mat imageGray, MatOfRect faces) {
		nativeDetect(mNativeObj, imageGray.getNativeObjAddr(), faces.getNativeObjAddr());
	}

	public void release() {
		nativeDestroyObject(mNativeObj);
		mNativeObj = 0;
	}

	private long mNativeObj = 0;

	private static native long nativeCreateObject(String cascadeName, int minFaceSize);

	private static native void nativeDestroyObject(long thiz);

	private static native void nativeDetect(long thiz, long inputImage, long faces);
}
