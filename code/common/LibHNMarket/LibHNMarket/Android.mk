LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := hn_market_static

LOCAL_MODULE_FILENAME := libHNMarket

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := ../platform/android/HNJNIHelper.cpp \
                   ../platform/android/HNMarket.cpp \
                   ../HNOperator.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
                           $(LOCAL_PATH)/../../../game/MixProject/cocos2d \
                           $(LOCAL_PATH)/../../../game/MixProject/cocos2d/cocos \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../.. \

#LOCAL_EXPORT_LDLIBS := -llog \
#                        -lz \
#                        -landroid
                        
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)