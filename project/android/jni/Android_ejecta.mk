# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ejecta

LOCAL_CFLAGS += -DENABLE_SINGLE_THREADED=1 -DUSE_FILE32API -D__LINUX__=1 -DCOMPATIBLE_GCC4=1 -D__LITTLE_ENDIAN__=1 -DGL_GLEXT_PROTOTYPES=1 -DEJECTA_DEBUG=1

LOCAL_C_INCLUDES := \
                    $(LOCAL_PATH) \
                    $(LOCAL_PATH)/source/lib/lodefreetype \
                    $(LOCAL_PATH)/source/lib/lodepng \
                    $(LOCAL_PATH)/source/lib/lodejpeg \
                    $(LOCAL_PATH)/source/lib/uriparser \
                    $(LOCAL_PATH)/source/ejecta \
                    $(LOCAL_PATH)/source/ejecta/EJCanvas \
                    $(LOCAL_PATH)/source/ejecta/EJCanvas/2D \
                    $(LOCAL_PATH)/source/ejecta/EJCocoa \
                    $(LOCAL_PATH)/source/ejecta/EJCocoa/support \
                    $(LOCAL_PATH)/source/ejecta/EJUtils \
                    $(LOCAL_PATH)/libraries/android/libfreetype/include \
                    $(LOCAL_PATH)/libraries/android/libpng/include \
                    $(LOCAL_PATH)/libraries/android/libjpeg/include \
                    $(LOCAL_PATH)/libraries/android/libcurl/include \
                    $(LOCAL_PATH)/libraries/android/JavaScriptCore/include 

LOCAL_SRC_FILES := \
                    source/lib/lodefreetype/lodefreetype.cpp \
                    source/lib/lodepng/lodepng.cpp \
                    source/lib/lodejpeg/lodejpeg.cpp \
                    source/ejecta/EJCocoa/support/nsCArray.cpp \
                    source/ejecta/EJCocoa/NSObject.cpp \
                    source/ejecta/EJCocoa/NSObjectFactory.cpp \
                    source/ejecta/EJCocoa/NSGeometry.cpp \
                    source/ejecta/EJCocoa/NSAutoreleasePool.cpp \
                    source/ejecta/EJCocoa/NSArray.cpp \
                    source/ejecta/EJCocoa/CGAffineTransform.cpp \
                    source/ejecta/EJCocoa/NSDictionary.cpp \
                    source/ejecta/EJCocoa/NSNS.cpp \
                    source/ejecta/EJCocoa/NSSet.cpp \
                    source/ejecta/EJCocoa/NSString.cpp \
                    source/ejecta/EJCocoa/NSValue.cpp \
                    source/ejecta/EJCocoa/NSZone.cpp \
                    source/ejecta/EJCocoa/NSCache.cpp \
                    source/ejecta/EJApp.cpp \
                    source/ejecta/EJConvert.cpp \
                    source/ejecta/EJBindingBase.cpp \
                    source/ejecta/EJBindingEjectaCore.cpp \
                    source/ejecta/EJBindingEventedBase.cpp \
                    source/ejecta/EJSharedOpenGLContext.cpp \
                    source/ejecta/EJTimer.cpp \
                    source/ejecta/EJAudio/EJBindingAudio.cpp \
                    source/ejecta/EJCanvas/EJBindingImage.cpp \
                    source/ejecta/EJCanvas/EJBindingCanvas.cpp \
                    source/ejecta/EJCanvas/EJCanvasContext.cpp \
                    source/ejecta/EJCanvas/EJTexture.cpp \
                    source/ejecta/EJCanvas/2D/EJBindingImageData.cpp \
                    source/ejecta/EJCanvas/2D/EJCanvasContextScreen.cpp \
                    source/ejecta/EJCanvas/2D/EJCanvasContextTexture.cpp \
                    source/ejecta/EJCanvas/2D/EJFont.cpp \
                    source/ejecta/EJCanvas/2D/EJGLProgram2D.cpp \
                    source/ejecta/EJCanvas/2D/EJImageData.cpp \
                    source/ejecta/EJCanvas/2D/EJPath.cpp \
                    source/ejecta/EJCanvas/2D/EJCanvasPattern.cpp \
                    source/ejecta/EJCanvas/2D/EJBindingCanvasPattern.cpp \
                    source/ejecta/EJUtils/EJBindingHttpRequest.cpp \
                    source/ejecta/EJUtils/EJBindingLocalStorage.cpp \
                    source/ejecta/EJUtils/EJBindingTouchInput.cpp \
                    ejecta.cpp \

LOCAL_LDLIBS :=  -landroid -lz -llog -lGLESv2 -lGLESv1_CM \
                    -L$(LOCAL_PATH)/libraries/android/libfreetype/libs/$(TARGET_ARCH_ABI) -lfreetype \
                    -L$(LOCAL_PATH)/libraries/android/libpng/libs/$(TARGET_ARCH_ABI) -lpng \
                    -L$(LOCAL_PATH)/libraries/android/libjpeg/libs/$(TARGET_ARCH_ABI) -ljpeg \
                    -L$(LOCAL_PATH)/libraries/android/libcurl/libs/$(TARGET_ARCH_ABI) -lcurl \

LOCAL_SHARED_LIBRARIES := libJavaScriptCore

include $(BUILD_SHARED_LIBRARY)