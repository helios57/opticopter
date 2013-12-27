package ch.sharpsoft.opticopterandroid;

public interface INativeCallback {

	void onDetection(long /* MatOfRect */detected);
}
