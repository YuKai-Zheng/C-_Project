//
//  ExtraReward.hpp
//  LibHNLobby
//
//  Created by cocos on 2018/1/26.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef ExtraReward_hpp
#define ExtraReward_hpp

#include <stdio.h>

#define ASS_SHOW_LUCKYMONEY        181                  ///通知前端出现红包
#define ASS_LUCKYMONEY_RESULT      182                  ///通知前端红包打开结果
#define ASS_OPEN_LUCKYMONEY        191                  ///客户端打开红包

#pragma pack(push,1)
//红包出现通知
struct LuckyMoneyNotify
{
    int        nStayTime;      //红包停留时长
};

//红包打开结果
struct LuckyMoneyResult
{
    int         nType;          //打开结果（-1打开失败，0已被抢完，1成功抢到）
    int         nShowTime;      //红包打开后显示时长
    long long   i64LuckyMoney;    //本次红包的金额
    long long   i64MyMoney;      //打开后钱包新的总数
};
#pragma pack(pop)

#endif /* ExtraReward_hpp */
