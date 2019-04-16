//
//  WechatHttpMgr.h
//  MixProject
//
//  Created by 黄百超 on 08/06/2017.
//
//

#ifndef WechatHttpMgr_h
#define WechatHttpMgr_h

#include "network/HttpClient.h"

#ifdef __OBJC__
#import "WXApi.h"
#import "WXApiObject.h"
#endif

class WechatHttpMgr {
    
public:
    static void sendAccessTokenRequest(SendAuthResp *resp);
    static void sendRefreshTokenRequest();
    static void sendCheckAccessTokenRequest();
    
    static void sendUserInfoRequest(NSString* ACCESS_TOKEN, NSString* openID);
    
};



#endif /* WechatHttpMgr_h */
