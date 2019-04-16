#ifndef __BRNN_COMMON_DEFINE_h__
#define __BRNN_COMMON_DEFINE_h__

#include <memory.h>
#include "HNNetExport.h"
#include <vector>
#pragma pack(push,1)

namespace NN
{
#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)
    
    //游戏信息
    enum
    {
        GAME_TWO_NAME_ID	=	11105101,			//二人
        GAME_FOUR_NAME_ID	=	11105102,			//四人
        GAME_SIX_NAME_ID	=	11105103,			//六人
        GAME_KIND_ID		=	1,					// 名字 ID
    };
    
	enum 
	{
		PLAYER_COUNT_TWO	= 2,
		PLAYER_COUNT_FOUR	= 4,
		PLAYER_COUNT_SIX	= 6
	};

    /// 基本牌型
    typedef enum eBaseCardsType
    {
        EBCT_TYPE_NONE = 0,				    /// 无牌型
        //牛牛牌型
        EBCT_TYPE_NO_POINT = 1000,							///无点牌
        EBCT_TYPE_FALSE_BULL_ONE,
        EBCT_TYPE_BULL_ONE,                            ///牛一
        EBCT_TYPE_FALSE_BULL_TWO,
        EBCT_TYPE_BULL_TWO,                             ///牛二
        EBCT_TYPE_FALSE_BULL_THREE,
        EBCT_TYPE_BULL_THREE,                           ///牛三
        EBCT_TYPE_FALSE_BULL_FOUR,
        EBCT_TYPE_BULL_FOUR,                            ///牛四
        EBCT_TYPE_FALSE_BULL_FIVE,
        EBCT_TYPE_BULL_FIVE,                            ///牛五
        EBCT_TYPE_FALSE_BULL_SIX,
        EBCT_TYPE_BULL_SIX,                             ///牛六
        EBCT_TYPE_FALSE_BULL_SEVEN,
        EBCT_TYPE_BULL_SEVEN,                           ///牛七
        EBCT_TYPE_FALSE_BULL_EIGHT,
        EBCT_TYPE_BULL_EIGHT,                           ///牛八
        EBCT_TYPE_FALSE_BULL_NINE,
        EBCT_TYPE_BULL_NINE,                            ///牛九
        EBCT_TYPE_BULL_joker,                           ///小王牛牛
        EBCT_TYPE_BULL_JOKER,                           /// 大王牛牛
        EBCT_TYPE_BULL_BULL								///牛牛
    }E_BASE_CARDS_TYPE;
    
    enum
    {
		GS_FREE = 1,							//空闲状态
		GS_CATCH_BANKER = 2,					//抢庄状态
		GS_BET = 3,								//下注状态
		GS_SEND_CARD = 4,						//发牌状态
    };
    
	//玩家状态
	enum EN_PLAYER_STATE
	{
		PS_WATCH,		//旁观
		PS_PLAY			//游戏中
	};

    enum
    {
        CLIENT_TIMER_PREPARE,								//准备时钟id
        CLIENT_TIMER_CATCH_BANKER,							//抢庄时钟id
        CLIENT_TIMER_BET,									//下注id
        CLIENT_TIMER_OPEN_CARD								//开牌id
    };
    
    //消息id
    enum
    {
        ASS_PREPARE,								//C2S	准备
        ASS_CATCH_BANK,								//C2S	抢庄
        ASS_CONFIRM_BANKER,
        ASS_BET,
        ASS_SEND_CARD,								//C2S	发牌
        ASS_OPEN_CARD,								//C2S	开牌
        
        S2C_CONFIG_MSG,								//S2C	配置
        S2C_PREPARE_MSG,							//S2C	准备
        S2C_CATCH_BANK_MSG,							//S2C	抢庄
        S2C_CONFIRM_BANKER,
        S2C_BET_MSG,
        S2C_SEND_CARD_MSG,							//S2C	发牌
        S2C_OPEN_OK_MSG,
        S2C_OPEN_CARD_MSG,							//S2C	开牌
        S2C_RESULT_MSG,
		S2C_TIP_EXIT_MSG,
		S2C_REWARD_POOL,							//奖池金额
		S2C_PERSONAL_CONTRIBUTION,					//个人贡献
		S2C_REWARD_GET_RECORD,						//获奖记录
		S2C_PALYER_STATE,							//玩家状态

		S2C_NOTIFY_MSG = 149						//牛牛广播
    };
    
    
    ////////////////////////////C2S消息结构体
    //准备
    struct tag_c2s_prepare
    {
        BYTE	bChairID;
        
        tag_c2s_prepare()
        {
            bChairID = INVALID_BYTE;
        }
    };
    
    //抢庄
    struct tag_c2s_catch_banker
    {
        BYTE	bIsCatch;							//0:不抢 1:抢
        
        tag_c2s_catch_banker()
        {
            bIsCatch = INVALID_BYTE;
        }
    };
    
	//押注
	struct tag_c2s_bet_two
	{
		long long	i64BetMoney;

		tag_c2s_bet_two()
		{
			i64BetMoney = 0;
		}
	};

    //开牌
    struct tag_c2s_open_card
    {
        BYTE	bChairID;
        
        tag_c2s_open_card()
        {
            bChairID = INVALID_BYTE;
        }
    };
    //////////////////////////////S2C消息结构体
	//配置包
	struct tag_s2c_config
	{
		BYTE	bPrepareTime;
		BYTE	bCatchBankerTime;
		BYTE	bBetTime;
		BYTE	bSendCardTime;

		BYTE	bCardTypeMul[11];

		tag_s2c_config()
		{
			bPrepareTime = 0;
			bCatchBankerTime = 0;
			bBetTime = 0;
			bSendCardTime = 0;
			memset(bCardTypeMul, 0, sizeof(bCardTypeMul));
		}
	};

	//奖池总额
	struct tag_s2c_reward_pool
	{
		long long i64RewardPoolMoney;

		tag_s2c_reward_pool()
		{
			i64RewardPoolMoney = 0;
		}
	};

	//奖池个人贡献
	struct tag_s2c_personal_contribution
	{
		long long i64PersonalContribution;

		tag_s2c_personal_contribution()
		{
			i64PersonalContribution = 0;
		}
	};

	//获奖记录
	struct tag_s2c_reward_get_record
	{
		BYTE		bCount;
		char		nickName[61];	//玩家昵称
		char		byCard[5];		//牌型
		long long	i64RewardMoney;	//奖励的金币

		tag_s2c_reward_get_record()
		{
			bCount = 0;
			memset(nickName, 0, sizeof(nickName));
			memset(byCard, 0, sizeof(byCard));
			i64RewardMoney = 0;
		}
	};

    //准备
    struct tag_s2c_prepare
    {
        BYTE	bChairID;
        
        tag_s2c_prepare()
        {
            bChairID = INVALID_BYTE;
        }
    };
    
    //抢庄
    struct tag_s2c_catch_banker
    {
        BYTE	bCurChairID;
        BYTE	bCurIsCatch;
        BYTE	bNextChairID;
        
        tag_s2c_catch_banker()
        {
            bCurChairID = INVALID_BYTE;
            bCurIsCatch = INVALID_BYTE;
            bNextChairID = INVALID_BYTE;
        }
    };
    
	//确定庄家
	struct tag_s2c_confirm_banker_two
	{
		BYTE		bBankerID;
		long long	i64BetArr[4];

		tag_s2c_confirm_banker_two()
		{
			bBankerID = INVALID_BYTE;
			memset(i64BetArr, 0, sizeof(i64BetArr));
		}
	};

	//押注
	struct tag_s2c_bet_two
	{
		BYTE		bBetChairID;
		long long	i64BetMoney;

		tag_s2c_bet_two()
		{
			bBetChairID = INVALID_BYTE;
			i64BetMoney = 0;
		}
	};

    //发牌
    struct tag_s2c_send_card
    {
        BYTE	bCards[5];
        
        tag_s2c_send_card()
        {
            memset(bCards, 0, sizeof(bCards));
        }
    };
    
	//开牌ok
    struct tag_s2c_open_card_ok
    {
        BYTE bChairID;
        
        tag_s2c_open_card_ok()
        {
            bChairID = INVALID_BYTE;
        }
    };
    
    //开牌
    struct tag_s2c_open_card
    {
        BYTE bChairID;
        BYTE bCardPoint;
        BYTE bCard[5];		//重排序
        
        tag_s2c_open_card()
        {
            bChairID = INVALID_BYTE;
            bCardPoint = 0;
            memset(bCard, 0, sizeof(bCard));
        }
    };
    
    //结算
    struct tag_s2c_result
    {
        BYTE	  bChairID;
        long long i64ResultMoney;
        
        tag_s2c_result()
        {
            bChairID = INVALID_BYTE;
            i64ResultMoney = 0;
        }
    };
    
	//没钱退出
	struct tag_s2c_tip_exit
	{
        BYTE        bChairID;
		BYTE		bIsExit;
		long long	i64Money;
		tag_s2c_tip_exit()
		{
            bChairID = INVALID_BYTE;
			bIsExit = INVALID_BYTE;
			i64Money = 0;
		}
	};

	//通知
	struct tag_s2c_notify
	{
		int				nUserID;
		BYTE			bGameID;
		char			cRoomName[64];
		BYTE			bDeskID;
		char			cNickName[64];
		long long		i64WinMoney;
		tag_s2c_notify()
		{
			nUserID = 0;
			bGameID = INVALID_BYTE;
			memset(cRoomName, 0, sizeof(cRoomName));
			bDeskID = INVALID_BYTE;
			memset(cNickName, 0, sizeof(cNickName));
			i64WinMoney = 0;
		}
	};

	//玩家状态
	struct tag_s2c_player_state
	{
		BYTE bChairID;
		BYTE bPlayerState;
		tag_s2c_player_state()
		{
			bChairID = INVALID_BYTE;
			bPlayerState = PS_WATCH;
		}
	};
    /////////////
    //重连包
    //抢庄阶段
    struct tag_s2c_station_catch_banker
    {
        BYTE	bLeftTime;
        BYTE	bPrepareTime;
        BYTE	bCatchBankerTime;
        BYTE	bBetTime;
        BYTE	bSendCardTime;
        BYTE	bCardTypeMul[11];
        
        BYTE	bCurChairID;
        BYTE	bCurIsCatch;
        BYTE	bNextChairID;
        
        tag_s2c_station_catch_banker()
        {
            bLeftTime = 0;
            bPrepareTime = 0;
            bCatchBankerTime = 0;
            bBetTime = 0;
            bSendCardTime = 0;
            memset(bCardTypeMul, 0, sizeof(bCardTypeMul));
            
            bCurChairID = INVALID_BYTE;
            bCurIsCatch = INVALID_BYTE;
            bNextChairID = INVALID_BYTE;
        }
    };
    
    // 押注阶段
    struct tag_s2c_station_bet
    {
        BYTE	bLeftTime;
        BYTE	bPrepareTime;
        BYTE	bCatchBankerTime;
        BYTE	bBetTime;
        BYTE	bSendCardTime;
        BYTE	bCardTypeMul[11];
        
        BYTE	bBankerID;
        
        long long i64BetValue[4];
        long long i64BetMoney[6];
        
        tag_s2c_station_bet()
        {
            bLeftTime = 0;
            bPrepareTime = 0;
            bCatchBankerTime = 0;
            bBetTime = 0;
            bSendCardTime = 0;
            memset(bCardTypeMul, 0, sizeof(bCardTypeMul));
            
            bBankerID = INVALID_BYTE;
            memset(i64BetValue, 0, sizeof(i64BetValue));
            memset(i64BetMoney, 0, sizeof(i64BetMoney));
        }
    };
    
	//发牌阶段
    struct tag_s2c_station_send
    {
        BYTE	bLeftTime;
        BYTE	bPrepareTime;
        BYTE	bCatchBankerTime;
        BYTE	bBetTime;
        BYTE	bSendCardTime;
        BYTE	bCardTypeMul[11];
        
        BYTE	bBankerID;
        
        long long i64BetMoney[6];
        
        BYTE bCard[5];
        
        tag_s2c_station_send()
        {
            bLeftTime = 0;
            bPrepareTime = 0;
            bCatchBankerTime = 0;
            bBetTime = 0;
            bSendCardTime = 0;
            memset(bCardTypeMul, 0, sizeof(bCardTypeMul));
            
            bBankerID = INVALID_BYTE;
            memset(i64BetMoney, 0, sizeof(i64BetMoney));
            memset(bCard, 0, sizeof(bCard));
        }
    };
}
#pragma pack(pop)
#endif
