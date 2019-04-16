#ifndef __ThirteenZhang_MessageHead_H__
#define __ThirteenZhang_MessageHead_H__

#include "HNBaseType.h"

namespace ThirteenZhang
{

	/*
	0x00 // Back image
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	// Diamond	2 3 4 5 6 7 8 9 10 J Q K A
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	// Club		2 - A
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	// Heart	2 - A
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	// Spade	2 - A
	0x4E,0x4F //small king, big king
	*/

	enum ThirteenZhang_CONFIG
	{
		GAME_NAME_ID = 12100004,	//名字ID
		PLAY_COUNT = 4,				//游戏人数
		USER_CARD_COUNT = 13,		//每个人所发扑克张数
		GAME_KIND_ID = 1,
	};

    
    enum Card_Type {
        UG_ERROR_KIND   = 0,    /**< 错误 */
    };
    
	//游戏状态定义
	enum ThirteenZhang_GS
	{
		GS_WAIT_SETGAME = 0,	//等待东家设置状态
		GS_WAIT_AGREE = 1,		//等待同意设置
		GS_ROB_NT = 19,			//叫庄状态
		GS_SEND_CARD = 20,		//发牌状态
		GS_OPEN_CARD = 21,		//开牌阶段
		GS_COMPARE_CARD = 22,	//比牌阶段
		GS_WAIT_NEXT = 23,		//等待下一盘开始
        GS_CLIENT_PLAY_BEGIN_ANIMATION = 25,
	};

	//游戏消息
	enum ThirteenZhang_ASS
	{
		ASS_OUT_CARD = 50,		//出牌
		ASS_WASHCARD = 51,		//洗牌
		ASS_SEND = 52,			//发牌
		ASS_PREPARE = 53,				//准备完成
		ASS_NORMALFINISH = 54,			//游戏正常结束消息
		ASS_OUT_CARD_RESULT = 55,		//出牌结果
		ASS_SUPER = 56,		//超端验证
		ASS_COMPARE = 57,	//比牌
		ASS_FIRE = 58,		//打枪
		ASS_AUTO = 59,		//托管
		ASS_AUTO_RESULT = 60,			//托管结果
		ASS_SUPER_CONTROL = 61,			//超端控制
		ASS_SUPER_CONTROLRES = 62,		//超端控制结果
	};
	
	//游戏指令
	enum ThirteenZhang_COMMAND
	{
		S_C_IS_SUPER_USER = 10,		//通知是超端
		C_S_AUTO = 20,				//托管消息(客户端没有做托管 这个直线机器人发送过来 系统代理开牌)

		S_C_NOTICE_ROB = 50,		//通知抢庄
		C_S_USER_ROB = 51,			//玩家抢庄
		S_C_USER_ROB_RESULT = 52,	//玩家抢庄结果
		S_C_MAKE_SUER_NT = 55,		//确定庄家消息	DWJ
		S_C_SEND_CARD = 60,			//发牌消息
        S_C_PLAY_BEGIN_ANIMATION = 61,
        C_S_OPEN_SPECIAL_CARD = 63, //客户端请求是否按照特殊牌型不参与比牌
		S_C_NOTICE_OPEN_CARD = 70,		//通知开牌消息
		C_S_USER_OPEN_CARD = 71,		//玩家开牌消息
		S_C_USER_OPEN_CARD_RESULT = 72,	//玩家开牌结果
		S_C_COMPARE_CARD = 80,			//比牌消息
		S_C_OPEN_FIRE = 90,             //打枪消息
        S_C_OPEN_SPECIAL_SHAPE = 98,    //特殊牌开牌消息
		S_C_GAME_END = 100,             //结算消息
        S_C_FORCEEXIT_LESSMONEY = 101,  //金币不足强退

		C_S_USER_MANUAL_OPEN_CARD = 65,			//请求一键摆牌
		S_C_USER_MANUAL_OPEN_CARD_RESULT = 66,	//服务端一键摆牌结果
	};

	enum En_User_State
	{
		STATE_ERR = 0,			//错误状态  --中途加入游戏状态
		STATE_NORMAL = 1,		//正常状态
		//STATE_HAVE_ROB = 2,		//已经抢庄了--玩家处于这个状态表示已经抢庄了
		STATE_OPEN_CARD = 3,	//摆牌完成等待开牌
	};


	#pragma pack(1)

	//游戏数据包客户端和服务端共有的数据
	struct	TGameBaseData
	{
		BYTE			m_byBeginTime;			//准备时间
		BYTE			m_byRobTime;			//抢庄时间
		BYTE			m_bySendCardTime;		//发牌时间
		BYTE			m_byThinkTime;			//开牌时间
		BYTE			m_byCompareTime;		//比牌时间
		BYTE			m_byCountTime;			//结算时间--结算框显示时间
		BYTE			m_byRemaindTime;		//阶段剩余时间
		int				m_iBaseFen;				//基础分
        LLONG			m_iAIWinWaterCount;		//计算玩家赢的水数
        LLONG			m_iAIWinMoney;			//玩家实际赢的钱数
        int				m_iAIWinRateLevel1;		//
        int				m_iAIWinRateLevel2;		//
        int				m_iAIWinRateLevel3;		//
        int				m_iAIWinRateLevel4;		//
        int				m_iAIWinRateLevel5;		//
        LLONG			m_iAIWinMoneyLevel1;
        LLONG			m_iAIWinMoneyLevel2;
        LLONG			m_iAIWinMoneyLevel3;
        LLONG			m_iAIWinMoneyLevel4;
        
		En_User_State	m_EnUserState[PLAY_COUNT];	//玩家状态

		BYTE			m_byNowNtStation;		//当前庄家位置

		BYTE			m_byCurrRobUser;		//当前抢庄玩家

		BYTE			m_byUserCards[PLAY_COUNT][USER_CARD_COUNT];		//当局所洗的牌

		int				m_iHeapCount[PLAY_COUNT][3];	//玩家每一堆牌的数量
		BYTE			m_byHeapCard[PLAY_COUNT][3][5];	//玩家每一堆牌的数据	
		int				m_iHeapShape[PLAY_COUNT][3];	//玩家对应的牌型
        int             m_iSpecialShape[PLAY_COUNT];    //玩家的特殊牌类型  -- added by HBC
        bool			m_iHasSpecial[PLAY_COUNT];		//记录玩家是否有特殊类型的牌	added by HBC
        bool			m_iUseSpecialShape[PLAY_COUNT];	//玩家是否使用特殊牌，不参与比牌过程
        
		int				m_iComepareResult[PLAY_COUNT][3];       //比牌结果
        int             m_iSpecialCompareResult[PLAY_COUNT];    //特殊牌型比牌结果  -- added by HBC
        int				m_iCompareWaterCount[PLAY_COUNT];		//玩家比牌赢得的水数
        
		bool			m_bUserFireUser[PLAY_COUNT][PLAY_COUNT];	//打枪数据 false-标识不打抢  true-标识打枪
        int				m_bExtraWater[PLAY_COUNT];					//打枪产生的额外水数
        int				m_bExtraWaterToAll[PLAY_COUNT];				//全垒打产生的额外水数
        
		LLONG			m_i64UserFen[PLAY_COUNT];		//结算得分(扣税前)
		LLONG			m_i64UserMoney[PLAY_COUNT];		//结算得分(扣税后)

		TGameBaseData()
		{
            m_byThinkTime	= 255;			//游戏思考时间
            m_byRobTime		= 255;			//抢庄时间
            m_byBeginTime	= 255;			//游戏开始时间
            m_byRemaindTime	= 255;
            m_iBaseFen		= 0;
            m_iAIWinMoney = 0;
            m_iAIWinWaterCount = 0;
            m_iAIWinRateLevel1 = 0;
            m_iAIWinRateLevel2 = 0;
            m_iAIWinRateLevel3 = 0;
            m_iAIWinRateLevel4 = 0;
            m_iAIWinRateLevel5 = 0;
            m_iAIWinMoneyLevel1 = 0;
            m_iAIWinMoneyLevel2 = 0;
            m_iAIWinMoneyLevel3 = 0;
            m_iAIWinMoneyLevel4 = 0;
            
            memset(m_EnUserState,STATE_ERR,sizeof(m_EnUserState));
            m_byNowNtStation= 255;
            m_byCurrRobUser	= 255;
            
            memset(m_byUserCards,0,sizeof(m_byUserCards));
            memset(m_iHeapCount,0,sizeof(m_iHeapCount));
            
            memset(m_iSpecialShape,0,sizeof(m_iSpecialShape));
            memset(m_iHasSpecial,false,sizeof(m_iHasSpecial));
            memset(m_iUseSpecialShape, false, sizeof(m_iUseSpecialShape));
            
            memset(m_byHeapCard,255,sizeof(m_byHeapCard));
            memset(m_iHeapShape,0,sizeof(m_iHeapShape));
            memset(m_iComepareResult,0,sizeof(m_iComepareResult));
            memset(m_iSpecialCompareResult,0,sizeof(m_iSpecialCompareResult));
            memset(m_iCompareWaterCount, 0, sizeof(m_iCompareWaterCount));
            
            memset(m_bUserFireUser,false,sizeof(m_bUserFireUser));
            memset(m_bExtraWater, 0, sizeof(m_bExtraWater));
            memset(m_bExtraWaterToAll, 0, sizeof(m_bExtraWaterToAll));
            
            memset(m_i64UserFen,false,sizeof(m_i64UserFen));
            memset(m_i64UserMoney,false,sizeof(m_i64UserMoney));
		}

		//初始所有数据
		void	InitAllData()
		{
			m_byThinkTime = 255;			//游戏思考时间
			m_byRobTime = 255;			//抢庄时间
			m_byBeginTime = 255;			//游戏开始时间
			m_byRemaindTime = 255;
			m_iBaseFen = 0;
			memset(m_EnUserState, STATE_ERR, sizeof(m_EnUserState));
			m_byNowNtStation = 255;
			m_byCurrRobUser = 255;

			memset(m_byUserCards, 0, sizeof(m_byUserCards));
			memset(m_iHeapCount, 0, sizeof(m_iHeapCount));

			memset(m_byHeapCard, 255, sizeof(m_byHeapCard));
			memset(m_iHeapShape, 0, sizeof(m_iHeapShape));
			memset(m_iComepareResult, 0, sizeof(m_iComepareResult));

			memset(m_bUserFireUser, false, sizeof(m_bUserFireUser));

			memset(m_i64UserFen, false, sizeof(m_i64UserFen));
			memset(m_i64UserMoney, false, sizeof(m_i64UserMoney));
		}

		//初始化部分数据
		void	InitPartData()
		{
			memset(m_EnUserState, STATE_ERR, sizeof(m_EnUserState));
			m_byNowNtStation = 255;
			m_byCurrRobUser = 255;

			memset(m_byUserCards, 0, sizeof(m_byUserCards));
			memset(m_iHeapCount, 0, sizeof(m_iHeapCount));
			memset(m_byHeapCard, 255, sizeof(m_byHeapCard));
			memset(m_iHeapShape, 0, sizeof(m_iHeapShape));
			memset(m_iComepareResult, 0, sizeof(m_iComepareResult));

			memset(m_bUserFireUser, false, sizeof(m_bUserFireUser));

			memset(m_i64UserFen, false, sizeof(m_i64UserFen));
			memset(m_i64UserMoney, false, sizeof(m_i64UserMoney));
		}
	};
	/*---------------------------------------------------------------------------*/
	//超端验证消息包
	struct	S_C_SuperUser
	{
		BYTE byDeskStation;      //超端位置
		bool bIsSuper;           // 是否开通

		S_C_SuperUser()
		{
			memset(this, 0, sizeof(S_C_SuperUser));
		}
	};

	/*---------------------------------------------------------------------------*/
	//游戏状态基础数据包
	struct	S_C_TGameStation_Base
	{
		BYTE			byGameStation;			//游戏状态

		BYTE			byBeginTime;		//准备时间   
		BYTE			byRobTime;			//抢庄时间
		BYTE			bySendCardTime;		//发牌时间
		BYTE			byThinkTime;		//开牌时间
		BYTE			byCompareTime;		//比牌时间
		BYTE			byCountTime;		//结算时间--结算框显示时间

		int				iBaseFen;				//基础分


		S_C_TGameStation_Base()
		{
			memset(this, 0, sizeof(S_C_TGameStation_Base));
		}
	};
	/*---------------------------------------------------------------------------*/
	//空闲阶段
	struct	S_C_TGameStation_Free : public S_C_TGameStation_Base
	{
		bool            bUserReady[PLAY_COUNT];			//准备状态
		S_C_TGameStation_Free()
		{
			memset(this, 0, sizeof(S_C_TGameStation_Free));
		}
	};
	/*---------------------------------------------------------------------------*/
	//抢庄阶段
	struct	S_C_GameStation_RobNt : public S_C_TGameStation_Base
	{
		BYTE	byNowNtStation;			//当前庄家位置
		int		iUserState[PLAY_COUNT];	//玩家状态
		BYTE	byRemaindTime;			//阶段剩余时间
		BYTE	byCurrRobUser;			//当前操作玩家
		S_C_GameStation_RobNt()
		{
			memset(this, 0, sizeof(S_C_GameStation_RobNt));
			byNowNtStation = 255;
		}
	};
	/*---------------------------------------------------------------------------*/
	//发牌阶段
	struct	S_C_GameStation_SendCard : public S_C_TGameStation_Base
	{
		BYTE	byNowNtStation;			//当前庄家位置
		int		iUserState[PLAY_COUNT];	//玩家状态
		BYTE	byRemaindTime;			//阶段剩余时间

		BYTE	byUserCards[USER_CARD_COUNT];		//玩家自己的牌数据 只需自己的牌数据即可 

		S_C_GameStation_SendCard()
		{
			memset(this, 0, sizeof(S_C_GameStation_SendCard));
			byNowNtStation = 255;
		}
	};
	/*---------------------------------------------------------------------------*/
	//开牌阶段
	struct	S_C_GameStation_OpenCard : public S_C_TGameStation_Base
	{
		BYTE	byNowNtStation;			//当前庄家位置
		int		iUserState[PLAY_COUNT];	//玩家状态
		BYTE	byRemaindTime;			//阶段剩余时间


		BYTE	byUserCards[USER_CARD_COUNT];		//玩家自己的牌数据 只需自己的牌数据即可 

		BYTE	byMyHeapCard[3][5];		//玩家摆牌每一堆牌的数据		DWJ
		S_C_GameStation_OpenCard()
		{
			memset(this, 0, sizeof(S_C_GameStation_OpenCard));
			byNowNtStation = 255;
		}
	};
	/*---------------------------------------------------------------------------*/
	//比牌阶段
	struct	S_C_GameStation_CompareCard : public S_C_TGameStation_Base
	{
		BYTE	byNowNtStation;			//当前庄家位置
		int		iUserState[PLAY_COUNT];	//玩家状态
		BYTE	byRemaindTime;			//阶段剩余时间

		BYTE	byHeapCard[PLAY_COUNT][3][5];	//玩家摆牌每一堆牌的数据	
		int		iHeapShape[PLAY_COUNT][3];		//玩家对应的牌型
		int		iComepareResult[PLAY_COUNT][3];	//比牌结果

		S_C_GameStation_CompareCard()
		{
			memset(this, 0, sizeof(S_C_GameStation_CompareCard));
			byNowNtStation = 255;
		}
	};
	/*---------------------------------------------------------------------------*/
	//通知抢庄消息
	struct	S_C_TNoticeRobNt
	{
		BYTE			byCurrRobDesk;			//当前抢庄的玩家
		int				iUserState[PLAY_COUNT];	//玩家状态

		S_C_TNoticeRobNt()
		{
			memset(this, 0, sizeof(S_C_TNoticeRobNt));
			byCurrRobDesk = 255;
		}
	};
	//玩家抢庄消息
	struct	C_S_UserRobNt
	{
		BYTE			byDeskStation;		//抢庄的玩家
		bool			byRob;				//是否抢庄	true-抢庄 false-不抢

		C_S_UserRobNt()
		{
			memset(this, 0, sizeof(C_S_UserRobNt));
			byDeskStation = 255;
		}
	};

	struct	S_C_UserRobNt_Result
	{
		BYTE			byDeskStation;		//抢庄的玩家
		bool			byRobResult;		//抢庄结果	true-抢庄 false-不抢
		BYTE			byNowNtStation;		//当前庄家	DWJ
		S_C_UserRobNt_Result()
		{
			memset(this, 0, sizeof(S_C_UserRobNt_Result));
			byDeskStation = 255;
			byNowNtStation = 255;
		}
	};
	//轮庄确定庄家  DWJ
	struct	S_C_MakeSure_NtStation
	{
		BYTE			byNowNtStation;		//当前庄家位置 
		int				iUserState[PLAY_COUNT];	//玩家状态
		S_C_MakeSure_NtStation()
		{
			memset(this, 255, sizeof(S_C_MakeSure_NtStation));
		}
	};
	/*---------------------------------------------------------------------------*/
	struct	S_C_SendCard
	{
		BYTE		byNowNtStation;									//当前庄家的位置
		BYTE		byUserCards[USER_CARD_COUNT];					//玩家的牌数据--只发送自己的牌数据 防止看牌外挂  
		BYTE		bySendTurn[PLAY_COUNT*USER_CARD_COUNT];			//发牌顺序
		int			iUserState[PLAY_COUNT];							//玩家状态
        int         iSpecialShape[PLAY_COUNT];                      //玩家特殊牌类型
		S_C_SendCard()
		{
			memset(this, 255, sizeof(S_C_SendCard));
			memset(iUserState, 0, sizeof(iUserState));
			memset(byUserCards, 0, sizeof(byUserCards));
            memset(iSpecialShape, 0, sizeof(iSpecialShape));
		}
	};
    /*---------------------------------------------------------------------------*/
    struct C_S_OpenSpecialCard
    {
        BYTE		byDeskStation;		//玩家位置
        int         iSpecialShape;      //特殊牌类型
        bool        iOpenBySpecialShape;//是否使用特殊牌型开牌 false 正常比牌
        C_S_OpenSpecialCard()
        {
            memset(this, 0, sizeof(C_S_OpenSpecialCard));
        }
    };
	/*---------------------------------------------------------------------------*/
	//通知开牌消息
	struct	S_C_NoticeOpenCard
	{
		int			iUserState[PLAY_COUNT];							//玩家状态
		S_C_NoticeOpenCard()
		{
			memset(this, 0, sizeof(S_C_NoticeOpenCard));
		}
	};

	//玩家开牌
	struct	C_S_UserOpenCard
	{
		BYTE		byDeskStation;		//玩家位置
		int			iHeapCount[3];		//玩家每一堆牌的数量	
		BYTE		byHeapCard[3][5];	//玩家每一堆牌的数据	
		C_S_UserOpenCard()
		{
			memset(this, 0, sizeof(C_S_UserOpenCard));
			byDeskStation = 255;
		}
	};

	//玩家手动开牌
	struct C_S_MANUAL_UserOpenCard
	{
		BYTE byDeskStation;		//玩家位置
		bool isManualOpenCard;	//是否请求手动摆牌
		C_S_MANUAL_UserOpenCard()
		{
			memset(this, 0, sizeof(C_S_MANUAL_UserOpenCard));
			byDeskStation = 255;
		}
	};

    //玩家开牌结果
    struct	C_S_UserOpenCard_Result
    {
        BYTE		byDeskStation;					//玩家位置
        BYTE		byHeapCard[3][5];				//玩家自己的摆牌数据
        int			iUserState[PLAY_COUNT];			//玩家状态
        bool		iUseSpecialShape[PLAY_COUNT];	//是否使用特殊牌	默认false
        
        C_S_UserOpenCard_Result()
        {
            memset(this,0,sizeof(C_S_UserOpenCard_Result));
            memset(iUseSpecialShape, false, sizeof(iUseSpecialShape));
            byDeskStation = 255;
        }
    };
	/*---------------------------------------------------------------------------*/
	//通知比牌消息
	struct	S_C_CompareCard
	{
		BYTE		byHeapCard[PLAY_COUNT][3][5];			//玩家每一堆牌的数据
		int			iHeapShape[PLAY_COUNT][3];				//对应的牌堆牌型
		int			iUserState[PLAY_COUNT];					//玩家状态
		int			iComepareResult[PLAY_COUNT][3];			//比牌结果 1-表示赢了1倍  -1标识输了1倍  2-表示赢了2倍  -2表示输了两倍 以此类推
        bool		iCommonCompare[PLAY_COUNT];				//是否参与正常比牌。特殊牌类型玩家有权不参与普通比牌  默认所有玩家均参与普通比牌
		S_C_CompareCard()
		{
			memset(this, 0, sizeof(S_C_CompareCard));
            memset(iCommonCompare, true, sizeof(iCommonCompare));
		}
	};
	/*---------------------------------------------------------------------------*/
	//打枪
	struct	S_C_OpenFire
	{
        bool	bUserFireUser[PLAY_COUNT][PLAY_COUNT];	//打枪数据 false-标识不打抢  true-标识打枪
        int		bExtraWater[PLAY_COUNT];
        int		bExtraWaterToAll[PLAY_COUNT];				//全垒打产生的额外水数
        S_C_OpenFire()
        {
            memset(this,0,sizeof(S_C_OpenFire));
            memset(bExtraWater, 0, sizeof(bExtraWater));
            memset(bExtraWaterToAll, 0, sizeof(bExtraWaterToAll));
        }
	};
    /*---------------------------------------------------------------------------*/
    //特殊牌开牌消息
    struct S_C_OpenSpecialShape {
        int bSpecialShape[PLAY_COUNT];  //玩家特殊牌类型
        BYTE bCards[PLAY_COUNT][USER_CARD_COUNT];	//玩家手牌信息，发送时只发送自己的
        int bSpecialResult[PLAY_COUNT]; //玩家因为特殊牌的输赢
        S_C_OpenSpecialShape()
        {
            memset(this, 0, sizeof(S_C_OpenSpecialShape));
            memset(bSpecialShape, 0, sizeof(bSpecialShape));
            memset(bSpecialResult, 0, sizeof(bSpecialShape));
        }
    };
    //没钱退出提醒
    struct S_C_ForceExit_LessMonye
    {
        BYTE	bIsExit;
        long long i64Money;
        S_C_ForceExit_LessMonye()
        {
            bIsExit = 255;
            i64Money = 0;
        }
    };
    /*--------------------------------------------------------------------*/
	struct	S_C_GameResult
	{
		LLONG		i64UserFen[PLAY_COUNT];			//得分(扣税前)
		LLONG		i64UserMoney[PLAY_COUNT];		//改变的金币(扣税后)
		S_C_GameResult()
		{
			memset(this, 0, sizeof(S_C_GameResult));
		}
	};
	/*---------------------------------------------------------------------------*/
	//托管消息包
	struct	C_S_UserAuto
	{
		BYTE byDeskStation;      //玩家位置 
		bool bAuto;				// 是否托管

		C_S_UserAuto()
		{
			memset(this, 0, sizeof(C_S_UserAuto));
		}
	};

    //通知客户端播放开始动画
    struct S_C_PlayBeginAnimation
    {
        BYTE bDeskStation;
        bool canPlay;       //true,播放
        S_C_PlayBeginAnimation() {
            bDeskStation = BYTE(-1);
            canPlay = false;
        }
    };
    
	#pragma pack()
}


/********************************************************************************/
#endif
