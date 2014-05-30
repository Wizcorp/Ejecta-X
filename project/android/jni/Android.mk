LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

subdirs := $(LOCAL_PATH)/libraries/android/JavaScriptCore/Android.mk \
           $(LOCAL_PATH)/Android_ejecta_lib.mk \
           $(LOCAL_PATH)/Android_ejecta.mk \

include $(subdirs)

# Copy the shaders from sources to assets
$(shell rm -r $(LOCAL_PATH)/../assets/www/shaders)
$(shell cp -r $(LOCAL_PATH)/source/ejecta/EJCanvas/2D/Shaders $(LOCAL_PATH)/../assets/www/shaders)