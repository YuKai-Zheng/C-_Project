//
//  WechatMgr.h
//  LibHNOpen
//
//  Created by 黄百超 on 05/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef WechatMgr_h
#define WechatMgr_h

#ifdef __OBJC__
#include "../../Classes/WechatMgr/WXApi.h"
#import <UIKit/UIKit.h>
#endif


class WechatMgr
{
public:
    static bool initWechatAPI();
    static bool isAppInstalled();
    
    static void sendAuthRequestScope();
    static bool wechatAuthRequest();
    
    static void LoginWithWechat();
    
    static void shareTextToWechatSession(std::string text);
    static void shareScreenShotToWechatSession(std::string name);
    static void shareContentLinkToWechatSession(std::string title, std::string content);
    static void shareContentLinkToWechatOrTimeLineSession(std::string title, std::string content, int type);
};

#endif /* WechatMgr_h */
