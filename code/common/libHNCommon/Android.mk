LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../LibHNMarket)

LOCAL_MODULE := hn_common_static

LOCAL_MODULE_FILENAME := libHNCommon

LOCAL_ARM_MODE := arm

#=======================================
#LibHNLobby
MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/ChildLayer/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/command/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/component/delegate/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/component/logic/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/GameMenu/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/background/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/deskList/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/findPw/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameBank/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameLand/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameList/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameLogOn/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameMenu/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameRank/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameRegister/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameSet/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameGift/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameShop/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameType/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameUser/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/gameUserHead/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/initial/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/menu/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/progressBar/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/pattern/factory/roomList/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/PersionalCenter/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/Popularise/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/Shop/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/state/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/v1/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/v2/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/v3/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/v66/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/v88/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/v99/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/v100/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/model/one/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLobby/HNLobby/model/ninetyN/*.cpp)

#LibHNLogic
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLogic/HNLogic/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLogic/HNPlatformLogic/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLogic/HNRoomLogic/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNLogic/Platform/*.cpp)
#LibHNNet
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNCommon/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNData/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNExternal/MD5/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNGame/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNHttp/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNNetProtocol/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNPlatform/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNRoom/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNNet/HNSocket/*.cpp)
#LibHNOpen
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNOpen/UMeng/Cocos2dx/Android/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNOpen/UMeng/Cocos2dx/Common/*.cpp)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNOpen/UMeng/Cocos2dx/ShareButton/*.cpp)
#LibHNUI
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNUI/UI/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNUI/UI/Base/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNUI/Tool/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../LibHNUI/Sound/*.cpp)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
#
#========================================
LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../LibHNLobby \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../LibHNLogic \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../LibHNNet \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../LibHNOpen \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../LibHNUI \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../LibHNMarket \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../game/MixProject/Classes	\

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
						   $(LOCAL_PATH)/../../../cocos2d \
						   $(LOCAL_PATH)/../../../cocos2d/cocos \
						   
LOCAL_EXPORT_LDLIBS := -llog \
                        -lz \
						-landroid
                        
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_market_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
$(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,LibHNMarket)
