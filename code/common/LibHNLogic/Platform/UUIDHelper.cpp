#include "UUIDHelper.h"
#include "cocos2d.h"
USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Platform/iOS/MyUUIDManager.h"

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <objbase.h>
#include <stdio.h>
#define GUID_LEN 64

#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
#include <stdio.h>
#include <uuid/uuid.h>

#endif

namespace HN {
UUIDHelper::UUIDHelper() {
}

UUIDHelper::~UUIDHelper() {
}

std::string UUIDHelper::getUUID() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return  MyUUIDManager::getUUID("");

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return JniHelper::callStaticStringMethod("com/jlx/MixProject/JniTestHelper", "setUUID2CPlusplus", "test");

//     JniMethodInfo minfo;//定义Jni函数信息结构体
//     bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/jlx/MixProject/JniTestHelper", "setUUID2CPlusplus", "(Ljava/lang/String;)V");
//     if (!isHave) {
//     } else {
//         jobject para = minfo.env->NewStringUTF("test");
//         minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, para);
//     }
//     return "";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

    char buffer[GUID_LEN] = { 0 };
    GUID guid;

    if (CoCreateGuid(&guid))	{
        fprintf(stderr, "create guid error\n");
        return "";
    }
    _snprintf(buffer, sizeof(buffer),
              "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
              guid.Data1, guid.Data2, guid.Data3,
              guid.Data4[0], guid.Data4[1], guid.Data4[2],
              guid.Data4[3], guid.Data4[4], guid.Data4[5],
              guid.Data4[6], guid.Data4[7]);
    printf("guid: %s\n", buffer);
    return buffer;

#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
    uuid_t uu;
    int i;
    uuid_generate(uu);

    for (i = 0; i < 16; i++) {
        printf("%02X-", uu[i]);
    }
    printf("\n");

    return "";

#endif
    return "";
}
}
