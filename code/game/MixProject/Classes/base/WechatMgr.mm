//
//  WechatMgr.m
//  LibHNOpen
//
//  Created by 黄百超 on 05/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "WechatMgr.h"
#include "HNNetProtocol/HNBaseCommand.h"
#include "VertifyCodeHelper.h"
#include "HNLobby/globel.h"
#include "WechatHttpMgr.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "WXApiManager.h"
#import "WXApiRequestHandler.h"
#import "SnapshotHelper.h"
#endif

    
bool WechatMgr::initWechatAPI()
{
    return [WXApi registerApp:@"wx1816398499b4ca25"];
}

bool WechatMgr::isAppInstalled()
{
    if ([WXApi isWXAppInstalled]) {
        if ([WXApi isWXAppSupportApi]) {
            return YES;
        }
    }
    return NO;
}

void WechatMgr::sendAuthRequestScope() {
    /*NSString* accessToken = [NSString stringWithFormat:@"%s", cocos2d::UserDefault::getInstance()->getStringForKey(WX_ACCESS_TOKEN).c_str()];
    NSString* openId = [NSString stringWithFormat:@"%s", cocos2d::UserDefault::getInstance()->getStringForKey(WX_OPEN_ID).c_str()];
    if (accessToken.length != 0 && openId.length != 0) {
        //请求刷新
        WechatHttpMgr::sendRefreshTokenRequest();
    }
    else {
        wechatAuthRequest();
    }*/
    wechatAuthRequest();
}

bool WechatMgr::wechatAuthRequest() {
    return [WXApiRequestHandler sendAuthRequestScope:@"snsapi_userinfo"
                                               State:@"m&9WVxiKcTaZPRgqwJhRD2"
                                              OpenID:@""
                                    InViewController:nil];
}

void WechatMgr::LoginWithWechat() {
    VertifyCodeHelper::getInstance()->loginWithWechat();
}

void WechatMgr::shareTextToWechatSession(std::string text) {
    if (isAppInstalled()) {
        [WXApiRequestHandler sendText:[NSString stringWithCString:text.c_str()
                                                         encoding:NSUTF8StringEncoding]
                              InScene:WXSceneSession];
    }
}

void WechatMgr::shareScreenShotToWechatSession(std::string name) {
    NSData* imgData = [SnapshotHelper getSnapshotData];
    if (isAppInstalled() && imgData) {
        [WXApiRequestHandler sendImageData:imgData
                                   TagName:[NSString stringWithCString:name.c_str()
                                                              encoding:NSUTF8StringEncoding]
                                MessageExt:@""
                                    Action:@""
                                ThumbImage:[SnapshotHelper compressImage:[UIImage imageWithData:imgData]
                                                                  toByte:WX_THUMB_IMAGE_MAX_SIZE]
                                   InScene:WXSceneSession];
    }
}

void WechatMgr::shareContentLinkToWechatSession(std::string title, std::string content) {
    if (isAppInstalled()) {
        NSDictionary *infoPlist = [[NSBundle mainBundle] infoDictionary];
        NSString *icon = [[infoPlist valueForKeyPath:@"CFBundleIcons.CFBundlePrimaryIcon.CFBundleIconFiles"] lastObject];
        UIImage* image = [UIImage imageNamed:icon];
        NSString* url = [NSString stringWithFormat:@"%s/share.aspx", getWebServerUrl().c_str()];
        [WXApiRequestHandler sendLinkURL:[NSString stringWithString:url]
                                 TagName:@""
                                   Title:[NSString stringWithCString:title.c_str()
                                                            encoding:NSUTF8StringEncoding]
                             Description:[NSString stringWithCString:content.c_str()
                                                            encoding:NSUTF8StringEncoding]
                              ThumbImage:[SnapshotHelper compressImage:image
                                                                toByte:WX_THUMB_IMAGE_MAX_SIZE]
                                 InScene:WXSceneSession];
    }
}

void WechatMgr::shareContentLinkToWechatOrTimeLineSession(std::string title, std::string content, int type) {
    if (isAppInstalled()) {
        NSDictionary *infoPlist = [[NSBundle mainBundle] infoDictionary];
        NSString *icon = [[infoPlist valueForKeyPath:@"CFBundleIcons.CFBundlePrimaryIcon.CFBundleIconFiles"] lastObject];
        UIImage* image = [UIImage imageNamed:icon];
        NSString* url = [NSString stringWithFormat:@"%s/share.aspx", getWebServerUrl().c_str()];
        [WXApiRequestHandler sendLinkURL:[NSString stringWithString:url]
                                 TagName:@""
                                   Title:[NSString stringWithCString:title.c_str()
                                                            encoding:NSUTF8StringEncoding]
                             Description:[NSString stringWithCString:content.c_str()
                                                            encoding:NSUTF8StringEncoding]
                              ThumbImage:[SnapshotHelper compressImage:image
                                                                toByte:WX_THUMB_IMAGE_MAX_SIZE]
                                 InScene:(type == 0 ? WXSceneTimeline : WXSceneSession)];
    }
}
