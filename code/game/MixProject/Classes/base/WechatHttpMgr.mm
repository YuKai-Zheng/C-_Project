
//
//  WechatHttpMgr.m
//  MixProject
//
//  Created by 黄百超 on 08/06/2017.
//
//

#include "WechatHttpMgr.h"

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "HNNetProtocol/HNBaseCommand.h"
#include "HNLobby/globel.h"
#include "WechatMgr.h"
    
void WechatHttpMgr::sendAccessTokenRequest(SendAuthResp *resp) {
    NSString* appID = [NSString stringWithFormat:@"%s", getWXAppID().c_str()];
    NSString* secretKey = [NSString stringWithFormat:@"%s", getWXSecretKey().c_str()];
    NSString* URL = [NSString stringWithFormat:@"%@%@%@%@%@%@%@", @"https://api.weixin.qq.com/sns/oauth2/access_token?appid=", appID, @"&secret=", secretKey, @"&code=", resp.code, @"&grant_type=authorization_code"];
    
    cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl([URL UTF8String]);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            std::vector<char>* buffer = response->getResponseData();
            std::string data;
            data.assign(buffer->begin(), buffer->end());
            
            if (data.empty()) {
                return;
            }
            NSString* dataString = [NSString stringWithFormat:@"%s", data.c_str()];
            NSData* jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
            NSMutableDictionary* resultDic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                                             options:NSJSONReadingMutableContainers
                                                                               error:nil];
            
            NSString* accessToken = [resultDic valueForKey:@"access_token"];
            if (accessToken.length != 0) {
                cocos2d::UserDefault::getInstance()->setStringForKey(WX_ACCESS_TOKEN, [accessToken UTF8String]);
                NSTimeInterval time = [[NSDate date] timeIntervalSince1970];
                LLONG accessTime = [[NSNumber numberWithDouble:time] longLongValue];
                cocos2d::UserDefault::getInstance()->setDoubleForKey(WX_ACCESS_TOKEN_TIME, accessTime);
            }
            NSString* openID = [resultDic valueForKey:@"openid"];
            if (openID.length != 0) {
                cocos2d::UserDefault::getInstance()->setStringForKey(WX_OPEN_ID, [openID UTF8String]);
            }
            NSString* refreshToken = [resultDic valueForKey:@"refresh_token"];
            if (refreshToken.length != 0) {
                cocos2d::UserDefault::getInstance()->setStringForKey(WX_REFRESH_TOKEN, [refreshToken UTF8String]);
            }
            NSString* unionID = [resultDic valueForKey:@"unionid"];
            if (unionID.length != 0) {
                cocos2d::UserDefault::getInstance()->setStringForKey(WX_UNION_ID, [unionID UTF8String]);
            }
            
            cocos2d::UserDefault::getInstance()->flush();
            
            sendUserInfoRequest(accessToken, openID);
        }
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

//返回true，刷新成功；返回false，需要重新授权
void WechatHttpMgr::sendRefreshTokenRequest()
{
    NSString* freshToken = [NSString stringWithFormat:@"%s", cocos2d::UserDefault::getInstance()->getStringForKey(WX_REFRESH_TOKEN).c_str()];
    NSString* appID = [NSString stringWithFormat:@"%s", getWXAppID().c_str()];
    NSString* URL = [NSString stringWithFormat:@"%@%@%@%@", @"https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=", appID, @"&grant_type=refresh_token&refresh_token=", freshToken];
    
    cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl([URL UTF8String]);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            std::vector<char>* buffer = response->getResponseData();
            std::string data;
            data.assign(buffer->begin(), buffer->end());
            
            if (data.empty()) {
                return;
            }
            NSString* dataString = [NSString stringWithFormat:@"%s", data.c_str()];
            NSData* jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
            NSMutableDictionary* resultDic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                                             options:NSJSONReadingMutableContainers
                                                                               error:nil];
            NSString* errmsg = [resultDic valueForKey:@"errmsg"];
            if (errmsg != nil || errmsg.length != 0) {
                //需要重新授权
                WechatMgr::wechatAuthRequest();
            }
            else {
                NSString* nAccessToken = [resultDic valueForKey:@"access_token"];
                if (nAccessToken.length != 0) {
                    cocos2d::UserDefault::getInstance()->setStringForKey(WX_ACCESS_TOKEN, [nAccessToken UTF8String]);
                    NSTimeInterval time = [[NSDate date] timeIntervalSince1970];
                    LLONG accessTime = [[NSNumber numberWithDouble:time] longLongValue];
                    cocos2d::UserDefault::getInstance()->setDoubleForKey(WX_ACCESS_TOKEN_TIME, accessTime);
                }
                NSString* openID = [resultDic valueForKey:@"openid"];
                if (openID.length != 0) {
                    cocos2d::UserDefault::getInstance()->setStringForKey(WX_OPEN_ID, [openID UTF8String]);
                }
                NSString* nRefreshToken = [resultDic valueForKey:@"refresh_token"];
                if (nRefreshToken.length != 0) {
                    cocos2d::UserDefault::getInstance()->setStringForKey(WX_REFRESH_TOKEN, [nRefreshToken UTF8String]);
                }
                cocos2d::UserDefault::getInstance()->flush();
                
                sendUserInfoRequest(nAccessToken, openID);
            }
        }
        else {
            //http通信失败，重新授权
            WechatMgr::wechatAuthRequest();
        }
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

void WechatHttpMgr::sendCheckAccessTokenRequest() {
    NSString* accessToken = [NSString stringWithFormat:@"%s", cocos2d::UserDefault::getInstance()->getStringForKey(WX_ACCESS_TOKEN).c_str()];
    NSString* openID = [NSString stringWithFormat:@"%s", cocos2d::UserDefault::getInstance()->getStringForKey(WX_OPEN_ID).c_str()];
    
    NSString* URL = [NSString stringWithFormat:@"%@%@%@%@", @"https://api.weixin.qq.com/sns/auth?access_token=", accessToken, @"&openid=", openID];
    
    cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl([URL UTF8String]);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            std::vector<char>* buffer = response->getResponseData();
            std::string data;
            data.assign(buffer->begin(), buffer->end());
            
            if (data.empty()) {
                return;
            }
            NSString* dataString = [NSString stringWithFormat:@"%s", data.c_str()];
            NSData* jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
            NSMutableDictionary* resultDic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                                             options:NSJSONReadingMutableContainers
                                                                               error:nil];
            int errCode = [[resultDic valueForKey:@"errcode"] intValue];
            if (errCode != 0) {
                //需要重新授权
                WechatMgr::wechatAuthRequest();
            }
            else {
                sendRefreshTokenRequest();
            }
        }
        else {
            //http通信失败，重新授权
            WechatMgr::wechatAuthRequest();
        }
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

void WechatHttpMgr::sendUserInfoRequest(NSString *ACCESS_TOKEN, NSString* openID) {
    NSString* URL = [NSString stringWithFormat:@"%@%@%@%@", @"https://api.weixin.qq.com/sns/userinfo?access_token=", ACCESS_TOKEN, @"&openid=", openID];
    
    cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl([URL UTF8String]);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            std::vector<char>* buffer = response->getResponseData();
            std::string data;
            data.assign(buffer->begin(), buffer->end());
            
            if (data.empty()) {
                return;
            }
            NSString* dataString = [NSString stringWithUTF8String:data.c_str()];
            NSData* jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
            NSMutableDictionary* resultDic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                                             options:NSJSONReadingMutableContainers
                                                                               error:nil];

            
            NSString* nickName = [resultDic valueForKey:@"nickname"];
            
            if (nickName.length != 0) {
                //去除微信昵称中表情
                NSString *regex = @"^[a-zA-Z0-9_\u4e00-\u9fa5]+$";
                NSPredicate *predicate = [NSPredicate predicateWithFormat:@"SELF MATCHES %@", regex];
                NSString *temp = nil;
                
                for(int i = 0; i < [nickName length]; i++)
                {
                    temp = [nickName substringWithRange:NSMakeRange(i, 1)];
                    if ([predicate evaluateWithObject:temp]) {
                    } else {
                        NSRange range = NSMakeRange(i, 1);
                        nickName = [nickName stringByReplacingCharactersInRange:range
                                                                     withString:@" "];
                    }
                }
                
                NSString *withoutEmojiUsername = [nickName stringByReplacingOccurrencesOfString:@" "
                                                                                     withString:@""];

                cocos2d::UserDefault::getInstance()->setStringForKey(WX_NICK_NAME, [withoutEmojiUsername UTF8String]);
            }
            NSString* headImgUrl = [resultDic valueForKey:@"headimgurl"];
            if (headImgUrl.length != 0) {
                cocos2d::UserDefault::getInstance()->setStringForKey(WX_HEAD_IMG_URL, [headImgUrl UTF8String]);
            }
            
            WechatMgr::LoginWithWechat();
        }
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

