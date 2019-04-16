#pragma once
#ifndef __FruitSlotMessageHead_h__
#define __FruitSlotMessageHead_h__

#pragma pack(push,1)

namespace FruitSlot
{
	//游戏信息
	enum
	{
		GAME_NAME_ID = 31000504,
		GAME_KIND_ID = 1,
		PLAYER_COUNT = 1,
	};


#define OPEN_TYPE_COUNT_FRUIT				10
#define LOTTERY_TYPE_COUNT_FRUIT			8
#define LOTTERY_COUNT_FRUIT				3

	enum
	{
		GS_FREE = 0,		//空闲
		GS_KAI_JIANG,		//开奖
		GS_SELECT_GUESS_MODEL,//选择猜大小
		GS_GUESS_MODEL,		//猜大小模式
		GS_RESULT,			//结算
	};

	enum
	{
		S2C_GAME_STATE = 50,//游戏状态
		S2C_BASE_INFO,		//基础信息
		S2C_GAME_START,		//开始游戏
		S2C_DO_CAI_DA_XIAO,	//是否猜大小
		S2C_CAI_DA_XIAO,	//猜大小
		S2C_CAI_RESULT,		//本轮猜结果
		S2C_CAI_FINIISH,	//猜大小结束
		S2C_TIP_EXIT_MSG,	//没钱离开

		C2S_GAME_START,		//启动游戏
		C2S_DO_CAI,			//是否猜大小
		C2S_CAI,			//猜大小
	};

	enum LOTTERY_TYPE
	{
		THING_APPLE,			//苹果
		THING_WATERMELON,		//西瓜
		THING_RING,			//铃铛
		THING_BAR,			//BAR
		THING_TWO_BAR,		//2BAR
		THING_THREE_BAR,	//3BAR
		THING_TWO_SEVEN,	//27
		THING_THREE_SEVEN,	//37

		LOTTERY_TYPE_MAX,
	};

	enum OPEN_TYPE
	{
		ONE_APPLE,
		TWO_APPLE,
		THREE_APPLE,
		THREE_WATERMELON,
		THREE_RING,
		THREE_BAR,
		SIX_BAR,
		NINE_BAR,
		SIX_SEVEN,
		NINE_SEVEN,
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
		long long i64NoteUnit;						//下注单元
		long long i64MyMoney;

		struct_s2c_base()
		{
			i64NoteUnit = 0;
			i64MyMoney = 0;
		}
	};

	//开始游戏开奖
	struct struct_s2c_startGame
	{
		long long i64MyMoney;
		int	nOpenShowTime;					//展示时间
		float fOpenTurnUnitTime;			//转动一个icon时间
		int nOpenLineType;					//开类型
		int nOpenLotteryType[LOTTERY_COUNT_FRUIT];//开的icon
		int nTurnUnitCount[LOTTERY_COUNT_FRUIT];	//开奖转的icon单元数

		struct_s2c_startGame()
		{
			i64MyMoney = 0;

			nOpenShowTime = 0;
			fOpenTurnUnitTime = 0;
			nOpenLineType = -1;
			memset(nOpenLotteryType, -1, sizeof(nOpenLotteryType));
			memset(nTurnUnitCount, 0, sizeof(nTurnUnitCount));
		}
	};

	//是否猜大小
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

	//猜大小
	struct struct_s2c_caiDaXiao
	{
		int nFlag;
		struct_s2c_caiDaXiao()
		{
			nFlag = 0;
		}
	};



	//猜大小结果
	struct struct_s2c_caiResult
	{
		float fTime;
		int nPoint;
		int nSuccess;
		long long i64WinMoney;

		struct_s2c_caiResult()
		{
			fTime = 0;
			nPoint = 0;
			nSuccess = 0;
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
		struct_c2s_start()
		{
			i64NoteMul = 0;
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
			nCaiType = -1;
		}
	};
	
}
#pragma pack(pop)
#endif
