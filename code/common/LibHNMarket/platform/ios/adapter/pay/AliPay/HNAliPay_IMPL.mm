//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#import "HNAliPay_IMPL.h"
#include <UIKit/UIKit.h>
#import <AlipaySDK/AlipaySDK.h>
//#import "Util/DataSigner.h"
#import "Order.h"
#import "Util/RSADataSigner.h"

@interface HNAliPay_IMPL()
{
    NSString *_appID;
    NSString *_privateKey;
    NSString *_handleUrl;
    HNPayDelegate *_delegate;
}
@end

@implementation HNAliPay_IMPL

- (void)dealloc
{
    [super dealloc];
    [_appID release];
    [_privateKey release];
    [_handleUrl release];
    _appID = nil;
    _privateKey = nil;
    _handleUrl = nil;
}

- (id)initWithAppID:(NSString *)appID
         PrivateKey:(NSString *)privateKey
           Delegate:(HNPayDelegate *)delegate
          HandleURL:(NSString *)handleUrl {
    if (self = [super init]) {
        _appID = [appID retain];
        _privateKey = [privateKey retain];
        _handleUrl = [handleUrl retain];
        _delegate = delegate;
    }
    return self;
}

- (void)pay:(NSString *)orderID Price:(float)price Description:(NSString *)description {
    /*
     *商户的唯一的parnter和seller。
     *签约后，支付宝会为每个商户分配一个唯一的 parnter 和 seller。
     *生成订单信息及签名
     */
    //将商品信息赋予AlixPayOrder的成员变量
    Order *order = [Order new];
    // NOTE: app_id设置
    order.app_id = _appID;
    // NOTE: 支付接口名称
    order.method = @"alipay.trade.app.pay";
    // NOTE: 参数编码格式
    order.charset = @"utf-8";
    // NOTE: 当前时间点
    NSDateFormatter* formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    order.timestamp = [formatter stringFromDate:[NSDate date]];
    // NOTE: 支付版本
    order.version = @"1.0";
    // NOTE: sign_type 根据商户设置的私钥来决定
    order.sign_type = @"RSA";
    
    order.notify_url = _handleUrl;
        
    // NOTE: 商品数据
    order.biz_content = [BizContent new];
    order.biz_content.body = description;
    order.biz_content.subject = description;
    
    if (orderID.length == 0){
        //客户端生成订单
        order.biz_content.out_trade_no = [self generateTradeNO]; //订单ID（由商家自行制定）
    } else {
        //服务端生成订单
        order.biz_content.out_trade_no = orderID; //订单ID（由商家自行制定）
    }
    order.biz_content.timeout_express = @"30m"; //超时时间设置
    order.biz_content.total_amount = [NSString stringWithFormat:@"%.2f", price]; //商品价格
//    order.biz_content.seller_id = @"2088421611688731";
    
    
    //将商品信息拼接成字符串
    NSString *orderInfo = [order orderInfoEncoded:NO];
    NSString *orderInfoEncoded = [order orderInfoEncoded:YES];
    NSLog(@"orderSpec = %@",orderInfo);
    
    // NOTE: 获取私钥并将商户信息签名，外部商户的加签过程请务必放在服务端，防止公私钥数据泄露；
    //       需要遵循RSA签名规范，并将签名字符串base64编码和UrlEncode
    NSString *signedString = nil;
    RSADataSigner* signer = [[RSADataSigner alloc] initWithPrivateKey:_privateKey];
    signedString = [signer signString:orderInfo];
    
    // NOTE: 如果加签成功，则继续执行支付
    if (signedString != nil) {
        //应用注册scheme,在AliSDKDemo-Info.plist定义URL types
        NSString *appScheme = @"gcenterAlipay";
        // NOTE: 将签名成功字符串格式化为订单字符串,请严格按照该格式
        NSString *orderString = [NSString stringWithFormat:@"%@&sign=%@",
                                 orderInfoEncoded, signedString];
        // NOTE: 调用支付结果开始支付
        [[AlipaySDK defaultService] payOrder:orderString fromScheme:appScheme callback:^(NSDictionary *resultDic) {
            NSString* resultCode = [resultDic objectForKey:@"resultStatus"];
            bool status = false;
            if ([resultCode isEqualToString:@"9000"]) {
                status = true;
            }
            _delegate->HandlePayResult(status, 2);
        }];
    }
}

- (NSString *)generateTradeNO
{
    static int kNumber = 15;
    
    NSString *sourceStr = @"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    NSMutableString *resultStr = [[NSMutableString alloc] init];
    srand((unsigned)time(0));
    for (int i = 0; i < kNumber; i++)
    {
        unsigned index = rand() % [sourceStr length];
        NSString *oneStr = [sourceStr substringWithRange:NSMakeRange(index, 1)];
        [resultStr appendString:oneStr];
    }
    return resultStr;
}

- (bool)handleURL:(NSURL *)url
{
    if ([url.host isEqualToString:@"safepay"]) {
        //支付结果url，传入后由SDK解析，统一在支付时的pay方法的callback中回调
        [[AlipaySDK defaultService] processOrderWithPaymentResult:url standbyCallback:^(NSDictionary *resultDic) {}];
        return true;
    }
    return false;
}
@end
