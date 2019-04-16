#pragma once
#ifndef __SHZ_COMMON_DEFINE_h__
#define __SHZ_COMMON_DEFINE_h__

#include <memory.h>
#include "HNBaseType.h"
#include <vector>



#pragma pack(push,1)

namespace SHZ
{

#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)



#define LINE_COUNT                      9
#define LOTTERY_TYPE_COUNT              9
#define LOTTERY_COUNT                   15
#define MARY_DATA_COUNT                 24
#define MARY_SLOT_COUNT                 4
#define SHZ_COLUMN                      5
static	int linesIndexs[9][5] = { { 5, 6, 7, 8, 9 }, { 0, 1, 2, 3, 4 }, { 10, 11, 12, 13, 14 }, { 0, 6, 12, 8, 4 }, { 10, 6, 2, 8, 14 }, { 0, 1, 7, 3, 4 }, { 10, 11, 7, 13, 14 }, { 5, 11, 12, 13, 9 }, { 5, 1, 2, 3, 9 } };//9条线坐标位置
//static  int rateNumDatas[8][4] = { { 7, 20, 100, 250 }, { 10, 30, 160, 400 }, { 15, 40, 200, 500 }, { 2, 5, 20, 50 }, { 3, 10, 40, 100 }, { 50, 200, 1000, 2500 }, { 20, 80, 400, 1000 } };


	//#define S2C_NOTIFY_MSG          149
	enum
	{
		S2C_NOTIFY_MSG = 149,
	};

	//////////////////////////////////////////////////////////////////////////
	//游戏信息
	enum
	{
		GAME_NAME_ID = 31000503,
		GAME_KIND_ID = 1,
		PLAYER_COUNT = 1,
	};


	//游戏状态定义
	enum Game_State
	{
		GS_FREE = 0,		//空闲
		GS_KAI_JIANG,		//开奖
		GS_GUESS_MODEL,		//猜大小模式
		GS_MARY_MODEL,		//玛丽模式
		GS_FULLSCREEN_MODEL,//全屏奖模式
		GS_RESULT,			//结算
	};

	//Client Timer
	enum Client_Timer
	{
		CLIENT_TIMER_FREE,
		CLIENT_TIMER_XIAZHU,
		CLIENT_TIMER_KAIJIANG,
		CLIENT_TIMER_SETTLEMENT,
		CLIENT_TIMER_WAIT_NEXT,
	};


	enum
	{
        S2C_GAME_STATE = 50,        //游戏状态
        S2C_BASE_INFO,              //基础信息
        S2C_GAME_START,             //开始游戏
        S2C_HAS_MARY,               //是否进入小玛丽
        S2C_DO_MARY,                //更新小玛丽开奖数据
        S2C_MARY_FINISH,            //小玛丽结束
        S2C_DO_CAI_DA_XIAO,         //是否猜大小
        S2C_CAI_DA_XIAO,            //猜大小
        S2C_CAI_RESULT,             //本轮猜结果
        S2C_CAI_FINIISH,            //猜大小结束
        S2C_TIP_EXIT_MSG,           //没钱离开
        
        C2S_GAME_START,             //启动游戏
        C2S_DO_CAI,                 //是否猜大小
        C2S_CAI,                    //猜大小
	};

	enum
	{
		SERVER_TIMER_KAI_JIIANG_FINISH,		//开奖结束
		SERVER_TIMER_DO_CAI_DA_XIAO,		//要做猜大小
		SERVER_TIMER_CAI_DA_XIAO,			//猜大小
		SERVER_TIMER_CAI_FINISH,			//猜结束
	};

	enum LOTTERY_TYPE
	{
		MAN_LZS,			//鲁智深
		MAN_LC,				//林冲
		MAN_SJ,				//宋江
		WEAPON_BF,			//板斧
		WEAPON_QJ,			//枪戟
		WEAPON_DHD,			//大环刀
		WORD_ZYT,			//忠义堂
		WORD_TTXD,			//替天行道
		WORD_SHZ,			//水浒传
        WORD_EXIT,          //小玛丽状态退出
	};

	enum RATE_TYPE
	{
		THREE_CONNECT,		//三连线
		FOUR_CONNECT,		//四连线
		FIVE_CONNECT,		//五连线
		ALL_CONNECT			//全屏
	};


	//////////////////////////////S2C消息结构体
	//基础信息
	struct struct_s2c_base
	{
		long long i64NoteUnit;						//下注单元
		long long i64NoteMin;						//每条线最少压注值
		long long i64NoteMax;						//每条线最大压注值
		long long i64Mymoney;

		int nLotteryRateArr[LOTTERY_TYPE_COUNT][4];	//各区域倍率
		struct_s2c_base()
		{
			i64NoteUnit = 0;
			i64NoteMin = 0;
			i64NoteMax = 0;
			i64Mymoney = 0;

			memset(nLotteryRateArr, 0, sizeof(nLotteryRateArr));
		}
	};

	//开始游戏开奖
	struct struct_s2c_startGame
	{
		long long i64NoteMoney;
		long long i64NoteTotalMoney;
		long long i64MyMoney;

		int	nOpenRoundTime;					//转的时间
		int	nOpenLineTime;					//开的线时间
		int nOpenLotteryType[LOTTERY_COUNT]; //15个图标
		int nOpenLineResult[LINE_COUNT]; // 开奖的线第几条
		int nOpenLineType[LINE_COUNT];  // 开奖的类型0-8 宋江类型2
		int nOpenLineRate[LINE_COUNT];  //中奖的倍数
		int nOpenLineCount[LINE_COUNT];//中奖的个数

		struct_s2c_startGame()
		{
			i64NoteMoney = 0;
			i64NoteTotalMoney = 0;
			i64MyMoney = 0;

			nOpenRoundTime = 0;
			nOpenLineTime = 0;
			memset(nOpenLotteryType, INVALID_BYTE, sizeof(nOpenLotteryType));
			memset(nOpenLineResult, 0, sizeof(nOpenLineResult));
			memset(nOpenLineType, INVALID_BYTE, sizeof(nOpenLineType));
			memset(nOpenLineRate, 0, sizeof(nOpenLineRate));
			memset(nOpenLineCount, 0, sizeof(nOpenLineCount));
		}
	};

	struct struct_game_state
	{
		BYTE bGameState;
		struct_game_state()
		{
			bGameState = GS_FREE;
		}
	};

    //是否有小玛丽
    struct struct_s2c_hasMary
    {
        int     maryTimes;        //小玛丽次数，默认0，水浒传3-5个分别对应1-3
        LLONG   m_nWalletMoney;                         //身上金币
        int     m_nTotalRate;                           //总倍数
        int     m_nTotalNote;                           //总下注
        struct_s2c_hasMary() {
            maryTimes = 0;
            m_nWalletMoney = 0;
            m_nTotalRate = 0;
            m_nTotalNote = 0;
        }
    };
    
    //小玛丽开奖结果通知
    struct struct_s2c_maryData
    {
        int     maryTimes;                              //小玛丽剩余次数
        int     m_nMaryLoopOpenIndex;                   //小玛丽圆盘上开奖的位置
        int     m_nMaryLoopOpenType;                    //小玛丽圆盘上开奖类型
        int     m_nMarySlotData[MARY_SLOT_COUNT];       //slot位置每个转盘类型
        int     m_nMaryExtraRate;                       //通过小玛丽获得的额外奖励倍数
        bool    m_nMaryZhongJiang;                      //单次开玛丽是否有中奖
        LLONG   m_nWalletMoney;                         //身上金币
        int     m_nTotalRate;                           //总倍数
        int     m_nTotalNote;                           //总下注
        struct_s2c_maryData() {
            maryTimes = 0;
            m_nMaryLoopOpenIndex = 0;
            m_nMaryLoopOpenType = WORD_EXIT;
            memset(m_nMarySlotData, 0, sizeof(m_nMarySlotData));
            m_nMaryExtraRate = 0;
            m_nMaryZhongJiang = false;
            m_nWalletMoney = 0;
            m_nTotalRate = 0;
            m_nTotalNote = 0;
        }
    };
    
    //小玛丽结束通知
    struct struct_s2c_maryFinish
    {
        int     m_nMaryExtraRate;   //玛丽环节获得的额外中奖倍数
        int     m_nTotalRate;       //总倍数
        struct_s2c_maryFinish() {
            m_nMaryExtraRate = 0;
            m_nTotalRate = 0;
        }
    };
    
	//猜大小
	struct struct_s2c_caiDaXiao
	{
		int nFlag;
		long long i64WinMoney;
		struct_s2c_caiDaXiao()
		{
			nFlag = 0;
			i64WinMoney = 0;
		}
	};

	//猜大小结果
	struct struct_s2c_caiResult
	{
		float fTime;
		int nPoint1;
		int nPoint2;
		int nSuccess;
		int nRate;
		long long i64WinMoney;


		struct_s2c_caiResult()
		{
			fTime = 0;
			nPoint1 = 0;
			nPoint2 = 0;
			nSuccess = 0;
			nRate = 0;
			i64WinMoney = 0;
		}
	};

	//猜大小结束
	struct struct_s2c_caiFinish
	{
		long long i64MyMoney;
		long long i64WinMoney;
		struct_s2c_caiFinish()
		{
			i64MyMoney = 0;
			i64WinMoney = 0;
		}
	};

	//没钱退出
	struct tag_s2c_tip_exit
	{
		BYTE	bIsExit;
		long long	i64Money;
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
		long long i64NoteMoney;
		struct_c2s_start()
		{
			i64NoteMoney = 0;
		}
	};
	//是否做猜大小
	struct struct_c2s_doCaiDaXiao
	{
		int nDo;

		struct_c2s_doCaiDaXiao()
		{
			nDo = 0;
		}
	};

	struct struct_c2s_caiDaXiao
	{
		int nDo;
		int nCaiType;
		struct_c2s_caiDaXiao()
		{
			nDo = 0;
			nCaiType = INVALID_BYTE;
		}
	};

	struct struct_s2c_doCaiDaXiao
	{
		int nFlag;
		long long i64WinMoney;
		struct_s2c_doCaiDaXiao()
		{
			nFlag = 0;
			i64WinMoney = 0;
		}
	};
}
#pragma pack(pop)
#endif
