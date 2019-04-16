//
//  WXApiManager.h
//  SDKSample
//
//  Created by Jeason on 16/07/2015.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "../../WXApi.h"

//#include "cocos2d.h"
@class RootViewController;

@protocol WXApiManagerDelegate <NSObject>
//@protocol WXApiManagerDelegate <UIViewController>


@optional

- (void)managerDidRecvGetMessageReq:(GetMessageFromWXReq *)request;

- (void)managerDidRecvShowMessageReq:(ShowMessageFromWXReq *)request;

- (void)managerDidRecvLaunchFromWXReq:(LaunchFromWXReq *)request;

- (void)managerDidRecvMessageResponse:(SendMessageToWXResp *)response;

- (void)managerDidRecvAuthResponse:(SendAuthResp *)response;

- (void)managerDidRecvAddCardResponse:(AddCardToWXCardPackageResp *)response;

- (void)managerDidRecvChooseCardResponse:(WXChooseCardResp *)response;

- (void)managerDidRecvChooseInvoiceResponse:(WXChooseInvoiceResp *)response;


@end

@interface WXApiManager : NSObject<WXApiDelegate>

//@property (nonatomic, assign) id<WXApiManagerDelegate> delegate;
@property (nonatomic, assign) RootViewController * delegate;

+ (instancetype)sharedManager;

@end
