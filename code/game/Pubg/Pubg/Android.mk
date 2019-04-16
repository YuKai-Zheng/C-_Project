LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../common)

LOCAL_MODULE := Pubg_static

LOCAL_MODULE_FILENAME := libPubg

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/../Classes/GameTable/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../Classes/GameMessage/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../Classes/v88/*.cpp)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%) 
                                  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Classes/GameMessage \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/GameTable \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/v88 \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../ \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../common/LibHNNet \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../common/LibHNUI \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../common/LibHNLobby \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../common/LibHNLogic \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../common/LibHNMarket	\

LOCAL_EXPORT_LDLIBS := -llog \
                       -lz \
                       -landroid     
                   
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_common_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,editor-support/cocostudio)
$(call import-module,libHNCommon)