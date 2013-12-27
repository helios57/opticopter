#include "OpticopterAndroid_jni.h"
#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <vector>
#include <android/log.h>

#define LOG_TAG "OpticopterAndroid"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

//Callback native to java
/*void sendDetected(JNIEnv * jenv, jclass callback, Mat* detectedObjs) {
 // First get the class that contains the method you need to call
 jclass clazz = jenv->FindClass(
 "ch/sharpsoft/opticopterandroid/INativeCallback");

 // Get the method that you want to call
 jmethodID onDetection = jenv->GetMethodID(clazz, "onDetection", "(J)");

 // Call the method on the object
 jenv->CallObjectMethod(callback, onDetection, detectedObjs);
 }*/

/*JNIEXPORT void JNICALL Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeInit(
 JNIEnv *, jclass, jclass callback) {
 }*/

inline void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat) {
	mat = Mat(v_rect, true);
}

JNIEXPORT jlong JNICALL Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeCreateObject(
		JNIEnv * jenv, jclass, jstring jFileName, jint faceSize) {
	LOGD(
			"Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeCreateObject enter");
	const char* jnamestr = jenv->GetStringUTFChars(jFileName, NULL);
	string stdFileName(jnamestr);
	jlong result = 0;

	try {
		CascadeClassifier* face_cascade = new CascadeClassifier();
		/*DetectionBasedTracker::Parameters DetectorParams;
		 if (faceSize > 0)
		 DetectorParams.minObjectSize = faceSize;
		 result = (jlong) new DetectionBasedTracker(stdFileName, DetectorParams);*/
		face_cascade->load(stdFileName);
		result = (jlong) face_cascade;
	} catch (cv::Exception& e) {
		LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
		jclass je = jenv->FindClass("org/opencv/core/CvException");
		if (!je)
			je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je, e.what());
	} catch (...) {
		LOGD("nativeCreateObject caught unknown exception");
		jclass je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je,
				"Unknown exception in JNI code of DetectionBasedTracker.nativeCreateObject()");
		return 0;
	}

	LOGD(
			"Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeCreateObject exit");
	return result;
}

JNIEXPORT void JNICALL Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeDestroyObject(
		JNIEnv * jenv, jclass, jlong thiz) {
	LOGD(
			"Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeDestroyObject enter");
	try {
		if (thiz != 0) {
			delete (CascadeClassifier*) thiz;
		}
	} catch (cv::Exception& e) {
		LOGD("nativeestroyObject caught cv::Exception: %s", e.what());
		jclass je = jenv->FindClass("org/opencv/core/CvException");
		if (!je)
			je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je, e.what());
	} catch (...) {
		LOGD("nativeDestroyObject caught unknown exception");
		jclass je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je,
				"Unknown exception in JNI code of DetectionBasedTracker.nativeDestroyObject()");
	}
	LOGD(
			"Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeDestroyObject exit");
}

JNIEXPORT void JNICALL Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeDetect(
		JNIEnv * jenv, jclass, jlong thiz, jlong imageGray,
		jlong detectedObjs) {
	LOGD(
			"Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeDetect enter");
	try {
		vector<Rect> objs;
		CascadeClassifier face_cascade = *((CascadeClassifier*) thiz);
		Mat img = *((Mat*) imageGray);
		for (int i = 1024; i > 16; i /= 2) {
			//-- Detect faces
			face_cascade.detectMultiScale(img, objs, 1.1, 2, 0, Size(i, i));
			if (objs.size() > 0) {
				break;
			}
		}
		vector_Rect_to_Mat(objs, *((Mat*) detectedObjs));
	} catch (cv::Exception& e) {
		LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
		jclass je = jenv->FindClass("org/opencv/core/CvException");
		if (!je)
			je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je, e.what());
	} catch (...) {
		LOGD("nativeDetect caught unknown exception");
		jclass je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je,
				"Unknown exception in JNI code DetectionBasedTracker.nativeDetect()");
	}
	LOGD(
			"Java_ch_sharpsoft_opticopterandroid_OpticopterAndroid_nativeDetect exit");
}
