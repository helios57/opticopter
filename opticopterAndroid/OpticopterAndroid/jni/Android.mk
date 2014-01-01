LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
#OPENCV_INSTALL_MODULES:=off
#OPENCV_LIB_TYPE:=SHARED
include ${OPENCV_SDK}/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := OpticopterAndroid_jni.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -lm -llog -ldl -landroid

LOCAL_MODULE     := opticopter_android

include $(BUILD_SHARED_LIBRARY)
