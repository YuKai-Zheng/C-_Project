//
//  SaveImageIntoAlbumHelper.cpp
//  MixProject_189
//
//  Created by 黄百超 on 26/06/2017.
//
//

#include "SaveImageIntoAlbumHelper.h"
#include "HNLobby/globel.h"
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSSameImageHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif

void SaveImageIntoAlbumHelper::saveImageFromURL(std::string url) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS平台
    auto link = cocos2d::UserDefault::getInstance()->getStringForKey(WX_HEAD_IMG_URL);
    iOSSameImageHelper::sameImage(link);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Android平台
#endif
}
