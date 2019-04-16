#pragma once
#ifndef __JPM_COMMON_DEFINE_h__
#define __JPM_COMMON_DEFINE_h__

#include <memory.h>
#include "HNBaseType.h"
#include <vector>

#pragma pack(push,1)

namespace JPM
{

#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)

#define LINE_COUNT                          9                   //最高连线数
//#define LOTTERY_TYPE_COUNT                  14                  //水果类型数
#define LOTTERY_COUNT_NL                       15                  //3行5列，共15格子
#define LOTTERY_COLUMN_COUNT                5                   //共5行
#define RATE_TYPE_COUNT                     4                   //倍率类型数，有2连、3连、4连、5连
//#define PRIZE_TYPE_COUNT                3                     //返奖类型：底注倍数、免费次数、奖池百分比

	enum
	{
		S2C_NOTIFY_MSG = 149,
	};
    
    //////////////////////////////////////////////////////////////////////////
    //游戏信息
    enum
    {
        GAME_NAME_ID = 31000520,
        GAME_KIND_ID = 1,
        PLAYER_COUNT = 1,
    };

    enum
    {
        GS_FREE = 0,         //空闲
        GS_KAI_JIANG,        //开奖
        GS_FINISH,           //结算
    };
    
    enum
    {
        S2C_GAME_STATE = 50,        //游戏状态
        S2C_BASE_INFO,              //基础信息
        S2C_GAME_BEGIN,             //开始游戏
        S2C_GAME_FINISH,            //游戏结束
        S2C_TIP_EXIT_MSG,           //没钱离开
        
        C2S_START_GAME,             //开始游戏
    };
    
    enum LOTTERY_TYPE
    {
        LT_Orange,              //橘子
        LT_Mango,               //芒果
        LT_Watermelon,          //西瓜
        LT_Apple,               //苹果
        LT_Cherry,              //樱桃
        LT_Grape,               //葡萄
        LT_Pineapple,           //菠萝
        LT_Bell,                //铃铛
        LT_BAR,                 //BAR
        LT_777,                 //777
        LT_Diamond,             //钻石
        LT_TreasureBox,         //宝箱
        
        LOTTERY_TYPE_COUNT_NL,
    };
    
    enum RATE_TYPE
    {
        RT_TWO_CONNECT,         //二连线
        RT_THREE_CONNECT,       //三连线
        RT_FOUR_CONNECT,        //四连线
        RT_FIVE_CONNECT,        //五连线
    };
    
    enum PRIZE_TYPE
    {
        PT_MONEY,           //底注倍数的金币
        PT_FREE,            //免费次数
        PT_JACKPOT,         //奖池百分比
    };
    
    //////////////////////////////S2C消息结构体
    struct struct_s2c_game_state
    {
        BYTE bGameState;
        struct_s2c_game_state()
        {
            bGameState = GS_FREE;
        }
    };
    
    //基础信息
    struct struct_s2c_base_info
    {
        long long i64NoteUnit;                      //加注单位
        long long i64NoteMin;                       //每条线最少压注值
        long long i64NoteMax;                       //每条线最大压注值
        long long i64MyMoney;                       //用户当前金币
        long long i64Jackpot;                       //当前奖池
        int  nLotteryTypeCount;                     //当前游戏的水果类型数
        int nLineConstitute[LINE_COUNT][5];         //每条线的连接规则
        struct_s2c_base_info()
        {
            i64NoteUnit = 0;
            i64NoteMin = 0;
            i64NoteMax = 0;
            i64MyMoney = 0;
            i64Jackpot = 0;
            nLotteryTypeCount = 0;
            memset(nLineConstitute, 0, sizeof(nLineConstitute));
        }
    };
    
    //开始游戏开奖
    struct struct_s2c_game_begin
    {
        long long i64MyMoney;                       //用户下注后的钱（中奖前）
        long long i64WinMoney;                      //本局中奖金额，方便结算的显示（这里是没扣本金和扣税）
        long long i64WinJackpot;                    // 本局中的奖池金额
        int       nFreeCount;                       //本局中奖的免费次数，方便结算的显示
        
        int nOpenRoundTime;                         //转动时间
        int nOpenLineTime;                          //每条线显示时间
        int nShowResultTime;                        //显示结算的时间（客户端可以不管该值，直接等到S2C_GAME_FINISH消息到来）
        
        int nOpenLotteryType[LOTTERY_COUNT_NL];        //每个格子的水果
        int nOpenLineLotteryType[LINE_COUNT];       //每条线的中奖水果，-1是不中奖       类型
        int nOpenLinePrizeType[LINE_COUNT];         //每条线的奖品类型：倍率、免费次数、奖池比率
        int nOpenLineCount[LINE_COUNT];             //每条线连格的个数：2、3、4、5连     *N
        int nOpenLineRate[LINE_COUNT];              //每条线的得奖倍率（or免费次数or奖池比率） 倍数
        
        struct_s2c_game_begin()
        {
            i64MyMoney = 0;
            i64WinMoney = 0;
            nFreeCount = 0;
            
            nOpenRoundTime = 0;
            nOpenLineTime = 0;
            nShowResultTime = 0;
            
            memset(nOpenLotteryType, INVALID_BYTE, sizeof(nOpenLotteryType));
            memset(nOpenLineLotteryType, INVALID_BYTE, sizeof(nOpenLineLotteryType));
            memset(nOpenLinePrizeType, 0, sizeof(nOpenLinePrizeType));
            memset(nOpenLineCount, 0, sizeof(nOpenLineCount));
            memset(nOpenLineRate, 0, sizeof(nOpenLineRate));
        }
    };
    
    //本轮结束
    struct struct_s2c_game_finish
    {
        long long i64MyMoney;               //当前的金币
        long long i64Jackpot;               //当前的奖池
        long long i64WinMoney;              //扣除本金和扣税后，本局真正赢的钱，输钱就是负数
        int       nFreeCount;               //当前累计的免费次数
        struct_s2c_game_finish()
        {
            i64MyMoney = 0;
            i64Jackpot = 0;
            i64WinMoney = 0;
            nFreeCount = 0;
        }
    };
    
    //没钱退出
    struct tag_s2c_tip_exit
    {
        BYTE       bIsExit;
        LLONG      i64Money;
        tag_s2c_tip_exit()
        {
            bIsExit = INVALID_BYTE;
            i64Money = 0;
        }
    };
    
    //////////////////////////////C2S消息结构体
    //开始
    struct struct_c2s_start
    {
        int       nNoteLines;           //下注的线数
        long long i64NoteMoney;         //每条线的底注
        long long i64Jackpot;
        struct_c2s_start()
        {
            nNoteLines = 0;
            i64NoteMoney = 0;
        }
    };

}
#pragma pack(pop)
#endif
