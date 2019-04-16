LOCAL_PATH := $(call my-dir)

#---libBugly.so---
include $(CLEAR_VARS)
LOCAL_MODULE := bugly_native_prebuilt
LOCAL_SRC_FILES :=prebuilt/$(TARGET_ARCH_ABI)/libBugly.so
include $(PREBUILT_SHARED_LIBRARY)
#---end---

$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/extensions)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/libiconv)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/cocos/audio/include)

$(call import-add-path,$(LOCAL_PATH)/../../../../common)

$(call import-add-path,$(LOCAL_PATH)/../../../BB)
$(call import-add-path,$(LOCAL_PATH)/../../../BJL)
$(call import-add-path,$(LOCAL_PATH)/../../../BRXJ)
$(call import-add-path,$(LOCAL_PATH)/../../../CSLL)
$(call import-add-path,$(LOCAL_PATH)/../../../FQZS)
$(call import-add-path,$(LOCAL_PATH)/../../../Fruit)
$(call import-add-path,$(LOCAL_PATH)/../../../FruitSlot)
$(call import-add-path,$(LOCAL_PATH)/../../../GoldenToad)
$(call import-add-path,$(LOCAL_PATH)/../../../HappyGoldenFlower)
$(call import-add-path,$(LOCAL_PATH)/../../../JSXS)
$(call import-add-path,$(LOCAL_PATH)/../../../NL)
$(call import-add-path,$(LOCAL_PATH)/../../../NN)
$(call import-add-path,$(LOCAL_PATH)/../../../OnePieceSlot)
$(call import-add-path,$(LOCAL_PATH)/../../../SHZ)
$(call import-add-path,$(LOCAL_PATH)/../../../Stud)
$(call import-add-path,$(LOCAL_PATH)/../../../ThirteenZhang)
$(call import-add-path,$(LOCAL_PATH)/../../../TTNN)
$(call import-add-path,$(LOCAL_PATH)/../../../Medusa)
$(call import-add-path,$(LOCAL_PATH)/../../../Tiger)
$(call import-add-path,$(LOCAL_PATH)/../../../Pubg)
$(call import-add-path,$(LOCAL_PATH)/../../../EightSlot)
$(call import-add-path,$(LOCAL_PATH)/../../../Honor)
$(call import-add-path,$(LOCAL_PATH)/../../../RedPacket)
$(call import-add-path,$(LOCAL_PATH)/../../../21D)
$(call import-add-path,$(LOCAL_PATH)/../../../NewYearActivity)

LOCAL_MODULE := MixProject_shared

LOCAL_MODULE_FILENAME := libMixProject

#
MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/base/*.cpp)
#
LOCAL_SRC_FILES := hellocpp/main.cpp \
				   hellocpp/test.cpp \
                   
				   
LOCAL_SRC_FILES += $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes	\
					$(LOCAL_PATH)/../../../../common/LibHNNet	\
					$(LOCAL_PATH)/../../../../common/LibHNUI	\
					$(LOCAL_PATH)/../../../../common/LibHNLobby	\
					$(LOCAL_PATH)/../../../../common/LibHNLogic	\
					$(LOCAL_PATH)/../../../../common/LibHNMarket	\
					$(LOCAL_PATH)/../../../../common/LibHNOpen	\
					$(LOCAL_PATH)/../../../BB/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../BB/Classes/GameTable		\
					$(LOCAL_PATH)/../../../BJL/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../BJL/Classes/GameTable	\
					$(LOCAL_PATH)/../../../BRXJ/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../BRXJ/Classes/GameTable	\
					$(LOCAL_PATH)/../../../CSLL/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../CSLL/Classes/GameTable	\
					$(LOCAL_PATH)/../../../FQZS/Classes/GameMessage \
					$(LOCAL_PATH)/../../../FQZS/Classes/GameTable \
					$(LOCAL_PATH)/../../../Fruit/Classes/GameMessage \
					$(LOCAL_PATH)/../../../Fruit/Classes/GameTable \
					$(LOCAL_PATH)/../../../FruitSlot/Classes/GameMessage \
					$(LOCAL_PATH)/../../../FruitSlot/Classes/GameTable \
					$(LOCAL_PATH)/../../../GoldenToad/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../GoldenToad/Classes/GameTable	\
					$(LOCAL_PATH)/../../../HappyGoldenFlower/Classes/GameMessage \
					$(LOCAL_PATH)/../../../HappyGoldenFlower/Classes/GameTable \
					$(LOCAL_PATH)/../../../JSXS/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../JSXS/Classes/GameTable	\
					$(LOCAL_PATH)/../../../NL/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../NL/Classes/GameTable		\
					$(LOCAL_PATH)/../../../NN/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../NN/Classes/GameTable	\
					$(LOCAL_PATH)/../../../OnePieceSlot/Classes/GameMessage \
					$(LOCAL_PATH)/../../../OnePieceSlot/Classes/GameTable \
					$(LOCAL_PATH)/../../../SHZ/Classes/GameMessage \
					$(LOCAL_PATH)/../../../SHZ/Classes/GameTable \
					$(LOCAL_PATH)/../../../Stud/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../Stud/Classes/GameTable	\
					$(LOCAL_PATH)/../../../ThirteenZhang/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../ThirteenZhang/Classes/GameTable	\
					$(LOCAL_PATH)/../../../TTNN/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../TTNN/Classes/GameTable \
					$(LOCAL_PATH)/../../../Medusa/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../Medusa/Classes/GameTable \
					$(LOCAL_PATH)/../../../Tiger/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../Tiger/Classes/GameTable\
					$(LOCAL_PATH)/../../../Honor/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../Honor/Classes/GameTable\
                    $(LOCAL_PATH)/../../../Pubg/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../Pubg/Classes/GameTable\
                    $(LOCAL_PATH)/../../../EightSlot/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../EightSlot/Classes/GameTable\
					$(LOCAL_PATH)/../../../RedPacket/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../RedPacket/Classes/GameTable\
					$(LOCAL_PATH)/../../../21D/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../21D/Classes/GameTable\
					$(LOCAL_PATH)/../../../NewYearActivity/Classes/GameMessage	\
					$(LOCAL_PATH)/../../../NewYearActivity/Classes/GameTable\
					$(LOCAL_PATH)/../../../NewYearActivity/Classes/v88	\

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

LOCAL_STATIC_LIBRARIES += bugly_crashreport_cocos_static

LOCAL_WHOLE_STATIC_LIBRARIES += libiconv_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_common_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

# hn_game_static
LOCAL_WHOLE_STATIC_LIBRARIES += BB_static
LOCAL_WHOLE_STATIC_LIBRARIES += BJL_static
LOCAL_WHOLE_STATIC_LIBRARIES += BRXJ_static
LOCAL_WHOLE_STATIC_LIBRARIES += CSLL_static
LOCAL_WHOLE_STATIC_LIBRARIES += FQZS_static
LOCAL_WHOLE_STATIC_LIBRARIES += Fruit_static
LOCAL_WHOLE_STATIC_LIBRARIES += FruitSlot_static
LOCAL_WHOLE_STATIC_LIBRARIES += GoldenToad_static
LOCAL_WHOLE_STATIC_LIBRARIES += HappyGoldenFlower_static
LOCAL_WHOLE_STATIC_LIBRARIES += JSXS_static
LOCAL_WHOLE_STATIC_LIBRARIES += NL_static
LOCAL_WHOLE_STATIC_LIBRARIES += NN_static
LOCAL_WHOLE_STATIC_LIBRARIES += OnePieceSlot_static
LOCAL_WHOLE_STATIC_LIBRARIES += SHZ_static
LOCAL_WHOLE_STATIC_LIBRARIES += Stud_static
LOCAL_WHOLE_STATIC_LIBRARIES += ThirteenZhang_static
LOCAL_WHOLE_STATIC_LIBRARIES += TTNN_static
LOCAL_WHOLE_STATIC_LIBRARIES += Medusa_static
LOCAL_WHOLE_STATIC_LIBRARIES += Tiger_static
LOCAL_WHOLE_STATIC_LIBRARIES += Pubg_static
LOCAL_WHOLE_STATIC_LIBRARIES += EightSlot_static
LOCAL_WHOLE_STATIC_LIBRARIES += Honor_static
LOCAL_WHOLE_STATIC_LIBRARIES += RedPacket_static
LOCAL_WHOLE_STATIC_LIBRARIES += 21D_static
LOCAL_WHOLE_STATIC_LIBRARIES += NewYearActivity_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,editor-support/cocostudio)
$(call import-module,extensions)
$(call import-module,libiconv)
$(call import-module,libHNCommon)
$(call import-module,BB)
$(call import-module,BJL)
$(call import-module,BRXJ)
$(call import-module,CSLL)
$(call import-module,FQZS)
$(call import-module,Fruit)
$(call import-module,FruitSlot)
$(call import-module,GoldenToad)
$(call import-module,HappyGoldenFlower)
$(call import-module,JSXS)
$(call import-module,NewYearActivity)
$(call import-module,NL)
$(call import-module,NN)
$(call import-module,OnePieceSlot)
$(call import-module,SHZ)
$(call import-module,Stud)
$(call import-module,ThirteenZhang)
$(call import-module,TTNN)
$(call import-module,Medusa)
$(call import-module,Tiger)
$(call import-module,Pubg)
$(call import-module,EightSlot)
$(call import-module,Honor)
$(call import-module,RedPacket)
$(call import-module,21D)
$(call import-module,external/bugly)


# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
