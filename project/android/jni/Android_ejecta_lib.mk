BASE_LOCAL_PATH := $(call my-dir)
BASE_PATH := ../../../library/android

include $(CLEAR_VARS)

# Define our compiler flags
LOCAL_CFLAGS += -Wno-endif-labels -Wno-import -Wno-format
LOCAL_CFLAGS += -fno-strict-aliasing
LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -DALWAYS_INLINE=inline
LOCAL_CPPFLAGS := -Wno-sign-promo

JAVASCRIPT_ENGINE := false
# Enable JSC JIT if JSC is used and ENABLE_JSC_JIT environment
# variable is set to true
ifeq ($(JAVASCRIPT_ENGINE),jsc)
ifeq ($(ENABLE_JSC_JIT),true)
LOCAL_CFLAGS += -DENABLE_ANDROID_JSC_JIT=1
endif
endif

ifeq ($(TARGET_ARCH),arm)
LOCAL_CFLAGS += -Darm
# remove this warning: "note: the mangling of 'va_list' has changed in GCC 4.4"
LOCAL_CFLAGS += -Wno-psabi
endif

# need a flag to tell the C side when we're on devices with large memory
# budgets (i.e. larger than the low-end devices that initially shipped)
ifeq ($(ARCH_ARM_HAVE_VFP),true)
LOCAL_CFLAGS += -DANDROID_LARGE_MEMORY_DEVICE
endif

ifeq ($(ENABLE_WTF_USE_ACCELERATED_COMPOSITING),false)
LOCAL_CFLAGS += -DWTF_USE_ACCELERATED_COMPOSITING=0
endif

ifeq ($(ENABLE_WTF_USE_ACCELERATED_COMPOSITING),true)
LOCAL_CFLAGS += -DWTF_USE_ACCELERATED_COMPOSITING=1
endif


LOCAL_CFLAGS += -DENABLE_SINGLE_THREADED=1 -D__LINUX__=1 -DCOMPATIBLE_GCC4=1 -D__LITTLE_ENDIAN__=1

#LOCAL_PATH := $(BASE_PATH)/libJavaScriptCore/lib

#LOCAL_MODULE := libJavaScriptCore
#LOCAL_MODULE_FILENAME := libJavaScriptCore

LOCAL_MODULE := libJavaScriptCore

# Build libjs

include $(BUILD_SHARED_LIBRARY)
