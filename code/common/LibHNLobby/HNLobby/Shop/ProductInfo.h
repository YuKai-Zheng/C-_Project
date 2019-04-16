#ifndef __ProductInfo_H__
#define __ProductInfo_H__

#include "HNNetExport.h"
#include <string>

namespace HN {

//{
//	"identifier": "11111111111111111111",
//	"number": 10000,
//	"price": 2
//}

#define PRODUCT_PAY_TYPE_NONE		-1	//无类型
#define PRODUCT_PAY_TYPE_WEIXIN		0	//微信支付
#define PRODUCT_PAY_TYPE_ZHIFUBAO	1	//支付宝支付
#define PRODUCT_PAY_TYPE_YINLIAN	2	//银联支付
#define PRODUCT_PAY_TYPE_XINYONGKA	3	//信用卡支付
#define PRODUCT_PAY_TYPE_IAP		4	//苹果内购

// 商品结构
struct PRODUCT_INFO {
public:
    PRODUCT_INFO()
        : number(0)
        , price(0.0)
        , payType(PRODUCT_PAY_TYPE_NONE)
        , orderID("")
        ,IP("") {
    }

    PRODUCT_INFO(LLONG number, DOUBLE price, const std::string& identifier)
        : number(number)
        , price(price)
        , identifier(identifier)
        , payType(PRODUCT_PAY_TYPE_NONE)
        , orderID("")
        ,IP("") {
    }

    // 序列化
    std::string serialize();

    // 反序列化
    void deserialize(std::string data);

public:
    // 购买数量
    LLONG					number;
    // 商品价格
    DOUBLE					price;
    // 商品ID
    std::string             identifier;
    // 支付方式(sdk辨别)
    INT						payType;
    // 订单号
    std::string             orderID;
    // 发送订单的IP地址
    std::string             IP;
};
}

#endif