LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_STATIC_LIBRARIES := libmusicdecoder

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include 

LOCAL_SRC_FILES += \
	sound_service.cpp \
	opensl_es_context.cpp 
		
# Link with OpenSL ES
LOCAL_LDLIBS += -lOpenSLES

LOCAL_MODULE := libmedia
include $(BUILD_STATIC_LIBRARY)