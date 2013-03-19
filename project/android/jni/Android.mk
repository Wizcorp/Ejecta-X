LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

subdirs := $(LOCAL_PATH)/../../../library/android/JavaScriptCore/Android.mk \
           $(LOCAL_PATH)/Android_ejecta_lib.mk \
           $(LOCAL_PATH)/Android_ejecta.mk \

include $(subdirs)
