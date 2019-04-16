//
//  ChangeRootViewController.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 07/08/2017.
//
//

#include "ChangeRootViewController.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "AppController.h"
#endif
#include "HNLobby/globel.h"

void ChangeRootViewController::changeRootViewControllerH() {
    [AppController changeRootViewControllerH];
    cocos2d::UserDefault::getInstance()->setBoolForKey(ROOT_VIEW_IS_VERTICAL, false);
    cocos2d::UserDefault::getInstance()->flush();
}

void ChangeRootViewController::changeRootViewControllerV() {
    [AppController changeRootViewControllerV];
    cocos2d::UserDefault::getInstance()->setBoolForKey(ROOT_VIEW_IS_VERTICAL, true);
    cocos2d::UserDefault::getInstance()->flush();
}
