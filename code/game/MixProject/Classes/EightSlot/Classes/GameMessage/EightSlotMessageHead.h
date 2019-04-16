#pragma once
#ifndef __EightSlotMessageHead_h__
#define __EightSlotMessageHead_h__

#pragma pack(push,1)

namespace EightSlot
{
	//游戏信息
	enum
	{
		GAME_NAME_ID = 31000512,
		GAME_KIND_ID = 1,
		PLAYER_COUNT = 1,
	};

#define LOTTERY_TYPE_COUNT_EIGHT			15
#define LOTTERY_COUNT_EIGHT				1

	enum
	{
        GS_FREE = 0,        //空闲
        GS_CONTINUE,         //空闲（等待旋转或收分）
        GS_KAI_JIANG,        //开奖
        GS_FINISH,            //结算
	};

	enum
	{
        S2C_GAME_STATE = 50,    //游戏状态
        S2C_BASE_INFO,            //基础信息
        S2C_GAME_RESULT,        //开奖结果
        S2C_UPDATE_MONEY,        //更新金钱和分数
        S2C_TIP_EXIT_MSG,        //没钱离开
        
        C2S_START_GAME = 60,    //开始游戏
        C2S_GET_SCORE,            //收分(不用带结构体)
	};

	enum LOTTERY_TYPE
	{
        LT_Bang,            //清零
        LT_OneCoin,            //1个硬币
        LT_ThreeCoin,        //3个硬币
        LT_OnePaper,        //1叠纸币
        LT_ThreePaper,        //3叠纸币
        LT_GoldBar,            //金条
        LT_OneDollar,        //1个美元
        LT_ThreeDollar,        //3个美元
        LT_TwoTimes,        //2倍
        LT_ThreeTimes,        //3倍
        LT_FourTimes,        //4倍
        LT_FiveTimes,        //5倍
        LT_TenTimes,        //10倍
        LT_Bonus,            //免费
        LT_888,                //奖池
	};

    enum PRIZE_TYPE
    {
        PT_BASE_NOTE,        //基于本局投注金额
        PT_BASE_SCORE,        //基于当前累计赢分
        PT_JACKPOT,            //基于奖池
    };
	//////////////////////////////S2C消息结构体
	struct struct_game_state
	{
		BYTE bGameState;
		struct_game_state()
		{
			bGameState = GS_FREE;
		}
	};

	//基础信息
	struct struct_s2c_base
	{
        long long i64NoteUnit;                        //加注单位
        long long i64NoteMin;                        //每条线最少压注值
        long long i64NoteMax;                        //每条线最大压注值
        long long i64MyMoney;                        //用户当前金币
        long long i64Jackpot;                        //当前奖池
        int nLotteryRate[LT_ThreeDollar];            //7种基础图案对应的倍率(百分比)
        struct_s2c_base()
        {
            i64NoteUnit = 0;
            i64NoteMin = 0;
            i64NoteMax = 0;
            i64MyMoney = 0;
            i64Jackpot = 0;
            memset(nLotteryRate, 0, sizeof(nLotteryRate));
        }
	};

	//开始游戏开奖
	struct struct_s2c_startGame
	{
        long long i64MyMoney;            //用户投注后的钱
        long long i64WinScore;            //本局中奖的得分
        long long i64AllScore;            //当前累计的得分
        long long i64Jackpot;            //当前剩余的奖池
        int nFreeCount;                    //免费次数
        int nOpenLotteryType;            //本局中奖的图案，-1是没中图案
        
        struct_s2c_startGame()
        {
            i64MyMoney = 0;
            i64WinScore = 0;
            i64AllScore = 0;
            i64Jackpot = 0;
            nFreeCount = 0;
            nOpenLotteryType = 0;
        }
	};
    //下分后更新金钱
    struct struct_s2c_update_money
    {
        long long i64MyMoney;            //下分后的金币
        long long i64WinScore;            //下分后的得分
        struct_s2c_update_money()
        {
            i64MyMoney = 0;
            i64WinScore = 0;
        }
    };
	//没钱退出
	struct tag_s2c_tip_exit
	{
		BYTE	bIsExit;
		LLONG	i64Money;
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
		long long i64NoteMul;
        long long i64jackpot;
		struct_c2s_start()
		{
			i64NoteMul = 0;
            i64jackpot = 0;
		}
	};
    struct struct_c2s_score
    {
        long long i64null;
        struct_c2s_score()
        {
            i64null = 0;
        }
    };
}
#pragma pack(pop)
#endif
