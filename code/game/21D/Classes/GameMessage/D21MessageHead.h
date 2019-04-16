#ifndef __D21_MessageHead_H__
#define __D21_MessageHead_H__

//#include "HNNetExport.h"
#include "HNBaseType.h"

typedef int                 BOOL;

namespace D21
{
	//游戏ID
	enum Game_ID
	{
		NAME_ID = 12100404,
		KIND_ID = 1,
	};

	////游戏版本
	//enum Game_Version
	//{
	//	GAME_MAX_VER = 1,			//现在最高版本
	//	GAME_LESS_VER = 1,			//现在最低版本
	//	GAME_CHANGE_VER = 0,		//修改版本
	//  DEV_HEIGHT_VERSION = 3，		//内部开发高版本号
	//  DEV_LOW_VERSION = 10，		//内部开发低版本号
	//};

	//游戏状态
	enum Game_State
	{
		GS_WAIT_SETGAME			=	0,				//等待东家设置状态
		GS_WAIT_ARGEE				=	1,				//等待同意设置
		GS_BET								=	20,			//下注状态
		GS_SEND							=	21,			//发牌状态
		GS_PLAY_GAME					=	22,			//游戏中状态
		GS_WAIT_NEXT					=	23,			//等待下一盘开始阶段
		//wzx 2017 4  5		新添加字段
		GS_CATCH						=	19,			//抢庄
		GS_INSURANCE					=	24,			//购买保险
	};

	const int iNote[18]=
	{
		1,2,5,10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000
	};

	const int iDouble[12]={1,5,10,50,100,500,1000,5000,10000,50000,100000,500000};

	enum TIME_ID
	{
		TIME_WAIT = 0,
		TIME_CATCH,
		TIME_BET,
		TIME_SEND,
		TIME_INSURANCE,
		TIME_OPERATE,
		TIME_WAIT_NEXT,
	};

	//游戏消息
	enum MSG_ID
	{
		GAME_BUTTON = 100,				//按钮消息

		ASS_AGREE = 101,				//准备
		ASS_CATCH = 102,				//通知抢庄
		ASS_NT = 103,				//庄家
		ASS_BET = 104,				//下注通知
		ASS_BETRESULT = 105,				//下注结果
		ASS_SEND = 106,				//发牌消息
		ASS_INSURANCE = 107,				//有没保险
		ASS_OPERATE = 108,				//操作通知
		ASS_DIVIDE = 109,				//分牌结果
		ASS_PLAYER_STATE = 110,				//同步玩家状态

		ASS_SAFEEND = 121,				//游戏安全结束
		ASS_CALL = 122,				//叫牌消息
		ASS_CALLFINISH = 123,				//叫分消息
		ASS_NORMALFINISH = 124,				//游戏正常结束
		ASS_RESTART = 125,				//重新开始游戏
	};

	enum
	{
		//玩家状态定义
		PLAYER_READY  =						299,				//用户准备状态
		PLAYER_GIVEUP =						301,				//放弃
		PLAYER_JOIN	  =						302				//中途加入状态
	};

	enum Game_Const
	{
		PLAY_COUNT								=	4,					//游戏人数
		CARD_COUNT								=	5,					//每人游戏张数
	};

	//点击消息
	enum Game_CLICKMSG
	{
		CLICK_START								    =  500,				//开始
		CLICK_OPERATE							=  501,				//操作
		CLICK_BET										=	502,				//下注
		CLICK_BETOPERATE						=	503,				//下注操作
		CLICK_PLAYOPERATE						=	504,				//游戏中操作
		CLICK_GAMESET							=	505,				//游戏设置
		//wzx 2017 4  5		新添加字段
		CLICK_CATCH								=	506,				//抢庄
		CLICK_INSURANCE							=	507				//通知是否购买保险
	};

	//服务器发送消息的辅助标识
	enum Game_AssisentSign
	{
		SERVRE_EXTRA_START_OPERATE      =		499,			//开始操作
		SERVER_EXTRA_DOUBLE				=		500,			//加倍
		SERVER_EXTRA_CLICK_GIVEUP       =       501,			//放弃
		SERVER_EXTRA_CLICK_NEED			=		502,			//要牌
		SERVER_EXTRA_CLICK_STOP			=       503,			//停牌 
		SERVER_EXTRA_CLICK_TIMEEND    =       504,			//时间到下一个人叫分
		
		ASS_SUPER_USER							=		70,			//超端用户消息
		ASS_SUPER_RESULT						=		71,			//超端用户控制结果消息包
	};
	
	#pragma pack(1)
	//游戏数据包
	/********************************************************************************/
	//游戏状态数据包 （游戏等待开始状态）
	struct GameStation_1
	{

		//游戏版本
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		BYTE				bGameStation;					//游戏状态
		BYTE				bGameBeginWaitTime;				//游戏开始等待时间
		BYTE				bGameCatchTime;					//游戏抢庄时间
		BYTE				bGameBetTime;					//游戏下注时间
		BYTE				bGameInsuranceTime;				//游戏买保险时间
		BYTE                bCallCardTime;					//游戏叫牌时间
		int					bUserReady[PLAY_COUNT];		//玩家准备状态
		long long			i64BetUnit[4];					//筹码单元
		GameStation_1()
		{
			memset(this, 0, sizeof(GameStation_1));
		}
	};
// 	struct GameStation_1	
// 	{
// 		//游戏版本
// 		BYTE				iVersion;								//游戏版本号
// 		BYTE				iVersion2;								//游戏版本号
// 		BYTE				bGameStation;							//游戏状态
// 		BYTE				bGameBeginWaitTime;						//游戏开始等待时间
// 		BYTE				bGameCatchTime;							//游戏抢庄时间
// 		BYTE				bGameBetTime;							//游戏下注时间
// 		BYTE				bGameInsuranceTime;						//游戏买保险时间
// 		BYTE				bCallCardTime;							//游戏叫牌时间
// 		int					bUserReady[PLAY_COUNT];					//玩家准备状态
// 		GameStation_1()
// 		{
// 			memset(this,0,sizeof(GameStation_1));
// 		}
// 	};

	//游戏状态数据包	（ 游戏下注状态 ）
	struct GameStation_2
	{
		BYTE				iVersion;								//游戏版本号
		BYTE				iVersion2;								//游戏版本号
		BYTE				bGameStation;							//游戏状态
		BYTE				bGameBeginWaitTime;						//游戏开始等待时间
		BYTE				bGameCatchTime;							//游戏抢庄时间
		BYTE				bGameBetTime;							//游戏下注时间
		BYTE				bGameInsuranceTime;						//游戏买保险时间
		BYTE				bCallCardTime;							//游戏叫牌时间
		int					iNtPeople;								//庄家位置
		int	                bUserReady[PLAY_COUNT];					//玩家准备状态
		GameStation_2()
		{
			memset(this,0,sizeof(GameStation_2));
		}
	};

	//游戏状态数据包	（ 游戏中状态 ）
	struct GameStation_3
	{
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		BYTE				bGameStation;					//游戏状态
		BYTE				bGameBeginWaitTime;				//游戏开始等待时间
		BYTE				bGameCatchTime;					//游戏抢庄时间
		BYTE				bGameBetTime;					//游戏下注时间
		BYTE				bGameInsuranceTime;				//游戏买保险时间
		BYTE                bCallCardTime;					//游戏叫牌时间
		int					iNtPeople;						//庄家位置
		int	                bUserReady[PLAY_COUNT];			//玩家准备状态

		GameStation_3()
		{
			memset(this,0,sizeof(GameStation_3));
		}
	};

	//游戏状态数据包	（ 等待下一盘开始状态 ）
	struct GameStation_4	
	{
		BYTE				iVersion;								//游戏版本号
		BYTE				iVersion2;								//游戏版本号
		BYTE				bGameStation;							//游戏状态
		BYTE				bGameBeginWaitTime;						//游戏开始等待时间
		BYTE				bGameCatchTime;							//游戏抢庄时间
		BYTE				bGameBetTime;							//游戏下注时间
		BYTE				bGameInsuranceTime;						//游戏买保险时间
		BYTE				bCallCardTime;							//游戏叫牌时间
		int	                bUserReady[PLAY_COUNT] ;				//玩家准备状态
		long long			i64BetUnit[4];					//筹码单元
		GameStation_4()
		{
			memset(this,0,sizeof(GameStation_4));
		}

	};

	//游戏状态数据包	（ 游戏下注状态 ）
	struct GameStation_5
	{
		BYTE				iVersion;								//游戏版本号
		BYTE				iVersion2;								//游戏版本号
		BYTE				bGameStation;							//游戏状态
		BYTE				bGameBeginWaitTime;						//游戏开始等待时间
		BYTE				bGameCatchTime;							//游戏抢庄时间
		BYTE				bGameBetTime;							//游戏下注时间
		BYTE				bGameInsuranceTime;						//游戏买保险时间
		BYTE				bCallCardTime;							//游戏叫牌时间
		int					iNtPeople;								//庄家位置
		int	                bUserReady[PLAY_COUNT] ;				//玩家准备状态
		GameStation_5()
		{
			memset(this,0,sizeof(GameStation_5));
		}
	};

	//玩家状态包
	struct GamePlayerStation
	{
		BYTE bPlayerStation[PLAY_COUNT];
		GamePlayerStation()
		{
			memset(bPlayerStation, -1, sizeof(bPlayerStation));
		}
	};

	struct AgreeGame
	{
		BOOL 				 flag;													//包标识
		BOOL 				 bUserReady[PLAY_COUNT];				//玩家是否准备
		AgreeGame()
		{
			memset(this,0,sizeof(AgreeGame));
		}
	};

	//按钮数据包
	struct GameStationButton
	{
		BYTE			bDeskStation;						//点击者
		int				iBtType;									//按钮类型
		int				iBtParam;								//按钮参数
		LLONG		i64Note;								//下注数量
		int    iGroupIndex;        //牌墩      
		GameStationButton()
		{
			memset(this,0,sizeof(GameStationButton));
		}
	};

	//抢庄消息包
	struct GameMessageCatch
	{
		BYTE bPrevChairID;								//上一家抢庄着 如果为INVALID_BYTE说明刚开始
		BYTE bIsPrevCatch;								//上一家是否抢0：不抢1:抢
		BYTE bCurChairID;									//当前抢庄者

		GameMessageCatch()
		{
			bPrevChairID = INVALID_BYTE;
			bIsPrevCatch = INVALID_BYTE;
			bCurChairID = INVALID_BYTE;
		}
	};

	//庄家消息包
	struct GameMessageNt
	{
		BOOL 			flag;												//发送庄家标识
		BYTE		bNtStation;									//庄家位置
		GameMessageNt()
		{
			memset(this,0,sizeof(GameMessageNt));
			bNtStation=255;
		}
	};

	//发牌消息包
	struct GameMessageSendCard
	{
		int				flag;								//发牌标识
		BYTE			bSendCardTime;						//发牌时间
		BYTE			bFirstPeople;						//第一个发牌者
		BYTE			bUserCard[PLAY_COUNT][2];			//用户扑克
		GameMessageSendCard()
		{
			memset(this,0,sizeof(GameMessageSendCard));
			bFirstPeople=255;
		}
	};
	//发牌消息包
	struct GameMessageStart
	{
		int					flag;											//发牌标识
		BYTE				bFirstScoreStation;						//第一个叫分者
		GameMessageStart()
		{
			memset(this,0,sizeof(GameMessageStart));
			bFirstScoreStation=255;
		}
	};
	//游戏安全结束数据包
	struct GameMessageEnd
	{
		int flag;												//发牌标识
		GameMessageEnd()
		{
			memset(this,0,sizeof(GameMessageEnd));
		}
	};
	//游戏正常结束数据包
	struct GameMessageNormalFinish
	{
		BOOL							flag;												//发牌标识
		BYTE						byCardList[PLAY_COUNT][13];		//结算扑克
		BYTE						byCardListSecond[PLAY_COUNT][13];    //结算扑克(第二幅)
		int							bIsRuning[PLAY_COUNT];				//参加本局结算的人	
		int							iUserPoint[PLAY_COUNT][2];				//玩家点数
		LLONG					i64UserScore[PLAY_COUNT];			//玩家输赢金币
		BYTE    bNTCard;            //庄家第一张牌,用于翻开庄家第一张牌
		GameMessageNormalFinish()
		{
			memset(this,0,sizeof(GameMessageNormalFinish));
		}
	};

	//下注通知包
	struct GameMessageBet
	{
		int      	  flag;			//下注标识
		BYTE		bDeskStation;		//座位号
		LLONG		i64BetLimit;		//下注上限
		LLONG		i64Bet[4];		//下注筹码
		GameMessageBet()
		{
			memset(this, 0, sizeof(GameMessageBet));
			bDeskStation = 255;
		}
	};


	//下注结果
	struct GameMessageBetResult
	{
		BYTE		bStartBet;			//0：下注结果 1：通知下注
		BYTE		bDeskStation;		//当前下注位置
		LLONG	i64Note;			//下注金币
		LLONG	i64TotalNote;
		LLONG	i64UserMoney;
		LLONG		i64BetLimit;		//下注上限
		LLONG		i64Bet[4];		//下注筹码
		GameMessageBetResult()
		{
			bStartBet = 0;
			bDeskStation = INVALID_BYTE;
			i64Note = 0;
			i64TotalNote = 0;
			i64UserMoney = 0;
		}
	};

	//保险包
	struct GameMessageInsurance
	{
		BYTE bIsInsurance;					//0:有1:没有
		BYTE bDeskStation;
		BYTE bIsBuy;						//0:不买1:买
		GameMessageInsurance()
		{
			bIsInsurance = INVALID_BYTE;
			bDeskStation = INVALID_BYTE;
			bIsBuy = INVALID_BYTE;
		}
	};

	//无操作类型
	enum NO_OPERATE
	{
		NO_OPERATE_ERROR,	//错误
		STOP_CARD,			//停牌
		OVER_POINT,			//爆点
		BLACK_JACK,			//黑杰克
		FIVE_SMALL_DRAGON	//五小龙
	};

	struct GameMessageOperateNotify
	{
		bool bHasOperate;					//是否有操作
		BYTE bDeskStaion;					//操作者
		bool bCanDivide;					//是否可以分牌
		bool bCanDouble;					//是否可以加倍
		BYTE bNoOperateType;				//无操作类型
		LLONG i64UserMoney;				//玩家钱
		LLONG i64Changeoney;				//输赢的钱
		BYTE iGroupIndex;				//牌墩

		GameMessageOperateNotify()
		{
			bHasOperate = false;
			bDeskStaion = INVALID_BYTE;
			bCanDivide = false;
			bCanDouble = false;
			bNoOperateType = NO_OPERATE_ERROR;
			i64UserMoney = 0;
			i64Changeoney = 0;
			iGroupIndex = INVALID_BYTE;
		}
	};

	struct GameMessageDivideResult
	{
		BYTE	bDeskStation;				//谁分牌
		BYTE	bCard[2];					//分牌
		LLONG i64Note;					//分牌完要下注
		LLONG i64TotalNote;				//总的下注
		LLONG i64UserMoney;				//玩家钱
		GameMessageDivideResult()
		{
			memset(bCard, 0, sizeof(bCard));
			i64Note = 0;
			i64TotalNote = 0;
			i64UserMoney = 0;
		}
	};

	//要牌消息包
	struct GameMessageNeedCard
	{
		bool		bFlag;									//能否要牌
		bool		bCanDivide;								//能否分牌
		bool bCanDouble;					//能否加倍
		LLONG		i64Note;								//加倍的下注
		int			iStyle;									//要牌类型
		int			iPoints;								//要牌点数
		BYTE		bDeskStation;							//要牌位置
		BYTE		bNextStation;							//下一个要牌位置
		BYTE		bCard;									//所要牌的数据
		int   iGroupIndex;        //牌墩
		GameMessageNeedCard()
		{
			memset(this,0,sizeof(GameMessageNeedCard));
			bFlag=true;
		}
	};

	//-------------------------------------------------------------------------------
	/// 通知客户端是否为超级客户端状态消息结构
	typedef struct SuperUserState
	{
		BYTE		byDeskStation;						//玩家位置
		bool		bEnable;								//是否开通 
		SuperUserState()
		{
			memset(this,0,sizeof(SuperUserState));
		}
	}SUPERSTATE;

	//超端用户控制的结果
	struct	SuperUserControlReslut
	{
		bool    bySuperReslut;					//超端用户控制
		BYTE	byDeskStation;						//超端用户座位号
		SuperUserControlReslut()
		{
			memset(this,0,sizeof(SuperUserControlReslut));

		}
	};
	/********************************************************************************/

	#pragma	pack()
}


/********************************************************************************/
#endif
