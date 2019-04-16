#pragma once
#ifndef __Pubg_COMMON_DEFINE_h__
#define __Pubg_COMMON_DEFINE_h__

#include <memory.h>
#include "HNBaseType.h"
#include <vector>

#pragma pack(push,1)

namespace Pubg
{

#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)

#define LINE_COUNT              40                   //最高连线数
//#define LOTTERY_TYPE_COUNT                  14                  //水果类型数
#define LOTTERY_COUNT_Pubg      20                  //4行5列，共20格子
#define LOTTERY_COLUMN_COUNT    5                   //共5行
#define RATE_TYPE_COUNT         4                   //倍率类型数，有2连、3连、4连、5连
//#define PRIZE_TYPE_COUNT                3                     //返奖类型：底注倍数、免费次数、奖池百分比
#define FINAL_KILL_COUNT       9  //大逃杀最大击杀数
	enum
	{
		S2C_NOTIFY_MSG = 149,
	};
    
    //////////////////////////////////////////////////////////////////////////
    //游戏信息
    enum
    {
        GAME_NAME_ID = 31000511,
        GAME_KIND_ID = 1,
        PLAYER_COUNT = 1,
    };

    enum
    {
        GS_FREE = 0,         //空闲
        GS_KAI_JIANG,        //开奖
        GS_FINAL_MODE,        //大逃杀模式
        GS_FINISH,           //结算
    };
    
    enum
    {
        S2C_GAME_STATE = 50,        //游戏状态
        S2C_BASE_INFO,              //基础信息
        S2C_GAME_BEGIN,             //开始游戏
        S2C_FINAL_MODE_RESULT,    //大逃杀结果
        S2C_TIP_EXIT_MSG,           //没钱离开
        
        C2S_START_GAME = 70,     //开始游戏
        C2S_START_FINAL_MODE,    //开始大逃杀,
    };
    
    enum
    {
        S2C_FREE_COUNT = 60       //免费次数
        
    };
    enum LOTTERY_TYPE
    {
        LT_WORD_J,            //J
        LT_WORD_Q,            //Q
        LT_WORD_L,            //K
        LT_WORD_A,            //A
        LT_PAN,                //平底锅
        LT_BACKPACK,        //三级包
        LT_GRENADE,            //手榴弹
        LT_CASQUE,            //三级头
        LT_AWM,                //AWM
        LT_WILD_BOX,        //WILD盒子
        LT_WILD_GUN,        //WILD信号枪
        LT_WILD_ENEMY,        //WILD敌人
        LT_JACKPOT,            //绝地鸡王
        
        LOTTERY_TYPE_COUNT_Pubg,
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
        int nLineConstitute[LINE_COUNT][5];         //每条线的连接规则
        int nFinalKillRate[FINAL_KILL_COUNT + 1];  //大逃杀击杀数对应的奖励
        struct_s2c_base_info()
        {
            i64NoteUnit = 0;
            i64NoteMin = 0;
            i64NoteMax = 0;
            i64MyMoney = 0;
            i64Jackpot = 0;
            memset(nLineConstitute, 0, sizeof(nLineConstitute));
            memset(nFinalKillRate,0,sizeof(nFinalKillRate));
        }
    };
    
    //开始游戏开奖
    struct struct_s2c_game_begin
    {
        long long i64WinMoney;                       //本局中奖金额，方便结算的显示
        long long i64WinJackpot;                    //本局中奖的奖池总金额(已经包含到winMoney里面了)
        
        long long i64LatestMoney;                  //用户最新的钱数
        long long i64LatestJackpot;                //本房间最新的奖池金额(中奖后)
        
        int nGunsignPosition;                      //瞄准镜位置
        
        int nOpenLotteryType[LOTTERY_COUNT_Pubg];        //每个格子的水果
        int nOpenLineLotteryType[LINE_COUNT];       //每条线的中奖水果，-1是不中奖       类型
        int nOpenLineCount[LINE_COUNT];             //每条线连格的个数：2、3、4、5连     *N
        int nOpenLineRate[LINE_COUNT];              //每条线的得奖倍率（or免费次数or奖池比率） 倍数
        
        struct_s2c_game_begin()
        {
            i64WinMoney = 0;
            i64WinJackpot = 0;
            nGunsignPosition = 0;
            
            memset(nOpenLotteryType, INVALID_BYTE, sizeof(nOpenLotteryType));
            memset(nOpenLineLotteryType, INVALID_BYTE, sizeof(nOpenLineLotteryType));
            memset(nOpenLineCount, 0, sizeof(nOpenLineCount));
            memset(nOpenLineRate, 0, sizeof(nOpenLineRate));
        }
    };
    
    //大逃杀开奖结果
    struct struct_s2c_finalmode_result
    {
        long long i64MyMoney;                    //玩家金币（中奖后）
        long long i64WinMoney;                    //本局中奖金额
        
        int nGunsightPosition;                    //瞄准镜(中心)位置
        int nKillCount;                            //本局击杀个数
        
        BYTE bGameOver;                            //大逃杀模式是否结束 非0结束大逃杀 0为大逃杀
        int nAllKillCount;                        //当前累计的总击杀数
        
        int nOpenLotteryType[LOTTERY_COUNT_Pubg];    //本局的开奖图案
        
        struct_s2c_finalmode_result()
        {
            i64MyMoney = 0;
            i64WinMoney = 0;
            
            nGunsightPosition = 0;
            nKillCount = 0;
            
            bGameOver = 0;
            nAllKillCount = 0;
            
            memset(nOpenLotteryType, INVALID_BYTE, sizeof(nOpenLotteryType));
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
        struct_c2s_start()
        {
            nNoteLines = 0;
            i64NoteMoney = 0;
        }
    };
    
    //免费次数
    struct struct_s2c_free_count
    {
        int       nFreeCount;      //剩余免费次数
        int       nLines;         //可免费的线数
        long long i64Notes;       //可免费的底注
        
        struct_s2c_free_count()
        {
            nFreeCount = 0;
            nLines = 0;
            i64Notes = 0;
        }
    };

}
#pragma pack(pop)
#endif
