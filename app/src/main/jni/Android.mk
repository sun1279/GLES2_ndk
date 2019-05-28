LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ssss
LOCAL_SRC_FILES := ssss.cpp
LOCAL_LDLIBS := -lGLESv2

include $(BUILD_SHARED_LIBRARY)