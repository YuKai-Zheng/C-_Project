#include "iOSVertifyCode.h"

#import <TCWebCodesSDK/TCWebCodesBridge.h>
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#include "base/VertifyCodeHelper.h"

@implementation iOSVertifyCode : NSObject

+(void)vertify{
        [m_viewController.view setHidden:NO];    
    
//        NSString *JSUrl=@"";
        NSString* JSUrl=[NSString stringWithUTF8String: VertifyCodeHelper::getInstance()->getJSUrl().c_str()];
    
    [[TCWebCodesBridge sharedBridge] setShowHeader:YES];
    
//    UIView* view = ;
    //
    [m_viewController.view addSubview:[[TCWebCodesBridge sharedBridge] startLoad:JSUrl webFrame:m_viewController.view.bounds]];
    //
    
    [[TCWebCodesBridge sharedBridge] setCallback:^(NSDictionary *resultJSON, UIView *webView) {
        NSString* jsonStr = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:resultJSON options:0 error:NULL] encoding:NSUTF8StringEncoding];
        if (0 == [resultJSON[@"ret"] intValue]) {
//            [[[UIAlertView alloc] initWithTitle:@"验证成功" message:jsonStr delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
        } else {
//            [[[UIAlertView alloc] initWithTitle:@"验证失败" message:jsonStr delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
        }
        NSLog(@"ticket:%@",[resultJSON objectForKey:@"ticket"]);
        
//        VertifyCodeHelper::getInstance()->setJSUrl([_url UTF8String]);
        VertifyCodeHelper::getInstance()->vertifyTicket([[resultJSON objectForKey:@"ticket"] UTF8String]);
        
        [m_viewController.view willRemoveSubview:webView];
        [webView setHidden:YES];
    }];    
    
//        NSMutableURLRequest* urlRequest = [[NSMutableURLRequest alloc] initWithURL:[[NSURL alloc] initWithString:JSUrl]];
//        urlRequest.cachePolicy = NSURLRequestReloadIgnoringCacheData; //不做任何cache
//        urlRequest.timeoutInterval = 10;
//        [urlRequest setHTTPMethod:@"GET"];
//        [[[NSURLSession sharedSession] dataTaskWithRequest:urlRequest completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
//    
//            dispatch_async(dispatch_get_main_queue(), ^{
//                
//                if (nil == error && 0 < data) {
//                    NSDictionary* dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:NULL];
//                    if (0 < dic.count) {
//                        int code = [dic[@"code"] intValue];
//                        if (0 == code) {
//                            _url = dic[@"url"];
//                            
////                            _url = @"https://captcha.guard.qcloud.com/template/TCapIframeApi.js?appid=1252422255&clientype=1&lang=2052&asig=7SLUC70vLrM15fJXhkOgplPfythJJIUmNN39NS_OqgBBo_6xLwHdsQJ4teOpI6DAV-dmSZJ1iLsI7y7ERRi8L2yG3d92HqDf";
//                            
//                            [[TCWebCodesBridge sharedBridge] setShowHeader:YES];
//                            
//                            UIView* view = [[TCWebCodesBridge sharedBridge] startLoad:_url webFrame:m_viewController.view.bounds];
////
//                            [m_viewController.view addSubview:view];
////
//                            
//                            [[TCWebCodesBridge sharedBridge] setCallback:^(NSDictionary *resultJSON, UIView *webView) {
//                                NSString* jsonStr = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:resultJSON options:0 error:NULL] encoding:NSUTF8StringEncoding];
//                                if (0 == [resultJSON[@"ret"] intValue]) {
//                                    [[[UIAlertView alloc] initWithTitle:@"验证成功" message:jsonStr delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
//                                } else {
//                                    [[[UIAlertView alloc] initWithTitle:@"验证失败" message:jsonStr delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
//                                }
//                                NSLog(@"ticket:%@",[resultJSON objectForKey:@"ticket"]);
//                                
//                                VertifyCodeHelper::getInstance()->setJSUrl([_url UTF8String]);
//                                VertifyCodeHelper::getInstance()->vertifyTicket([_url UTF8String]);
//                                
//                                [m_viewController.view willRemoveSubview:webView];
//                                [webView setHidden:YES];
//                            }];
//
//                            
//                            NSLog(@"GET URL : %@", _url);
//                        }
//                    }
//                }
//                else {
//                    [[[UIAlertView alloc] initWithTitle:nil message:error.localizedDescription delegate:NSMetadataUbiquitousItemURLInLocalContainerKey cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
//                }
//            });
//    
//        }] resume];
    
}

+(void)setUIView:(UIViewController*) control{
    m_viewController = control;
}

@end
