//
//  ClipboardHelper.cpp
//  MixProject
//
//  Created by Sunsong Ye on 16/10/17.
//
//

#include "ClipboardHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSClipboard.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif

void ClipboardHelper::copyStr(std::string str) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS平台
    iOSClipboard::copy(str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Android平台
#endif
}
