LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3

LOCAL_STATIC_LIBRARIES := libcommontool

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../libcommon \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include 
    
LOCAL_SRC_FILES := \
./accompany_decoder_controller.cpp \
./accompany_decoder.cpp 

LOCAL_MODULE := libmusicdecoder
include $(BUILD_STATIC_LIBRARY)