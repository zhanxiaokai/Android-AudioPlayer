LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
./packet_queue.cpp \
./packet_pool.cpp 

LOCAL_MODULE := libcommontool
include $(BUILD_STATIC_LIBRARY)