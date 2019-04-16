#ifndef __Fruit_COMMON_DEFINE_h__
#define __Fruit_COMMON_DEFINE_h__

#include <memory.h>
#include "HNBaseType.h"
#include <vector>



#pragma pack(push,1)

namespace Fruit
{

#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)

    //服务端向客户端发送消息ID
#define ASS_USER_SIT 			55
//#define ASS_GAME_FREE 			56
#define ASS_NOTE_RESPONSE		57
#define ASS_NOTE_FAIL			68
#define ASS_START_FAIL			69
#define ASS_KAIJIANG_SINGLE		60		//游戏开奖
#define ASS_GUESS_NUM			61		//允许猜大小
#define ASS_GUESS_NUM_RESPONSE	62		//猜大小结果
#define ASS_SWAP_RESPONSE		63		//划分结果通知到前端
    
#define ASS_LUCKY_NOTIFY		65		//
#define ASS_LUCKY_OPEN			66		//
#define ASS_UPDATE_REWARDS		67
    
#define ASS_GAME_SETTLEMENT		70
#define ASS_LACK_MONEY_OUT		72		//判断金币是否低于房间限额
    
#define ASS_SYNC_DATA           75		//刚进入游戏同步数据用
    
    //客户端请求消息ID
#define GS_C_NOTE				80	//客户端下注
#define GS_C_START				81	//客户端开始
#define GS_C_AUTO				82	//客户端请求自动
#define GS_C_GUESS_NUM			83	//客户端请求猜大小
#define GS_C_SWAP_SCORE			84	//客户端请求上下分
    
//#define S2C_NOTIFY_MSG          149
	enum
	{
		S2C_NOTIFY_MSG = 149,
	};
    
    //////////////////////////////////////////////////////////////////////////
    //游戏信息
    enum
    {
        GAME_NAME_ID	=	31000502,
        GAME_KIND_ID	=	1,
        PLAYER_COUNT	=   1,
    };
    
    
    //游戏状态定义
    enum Game_State
    {
        GS_FREE_NOTE		= 10,		//等待下注状态
		GS_XIAZHU,
        GS_KAIJIANG			= 20,		//开奖
        GS_GUESS			= 21,		//猜大小
        GS_LUCKY			= 22,		//触发lucky游戏状态
        GS_GAME_MACHINE		= 23,		//游戏机模式
        GS_SETTLEMENT		= 26,		//结算
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
    
    
    //游戏数据设定，转盘loop上24种
    enum {
        BetA_Bar = 0,
        BetA_Seven = 1,
        BetA_Star = 2,
        BetA_Watermelon = 3,
        BetA_Bell = 4,
        BetA_Cherry = 5,
        BetA_Orange = 6,
        BetA_Apple = 7,
        BetA_Count = 8,					//下注可选区域有8个类型
        Open_History_Logo_Count = 9,	//开奖记录记录数量
        BetA_Lucky_Big = 11,			//转盘上水果类型Lucky，额外再开4-6个
        BetA_Lucky_Small = 12,			//转盘上水果类型Lucky，额外再开1-3个
        Guess_Number_Count = 14,		//猜大小范围
        Loop_Sprite_Count = 24			//转盘上共有24个水果
    };

	const int posKindArr[24] = { -1, 0, 3, 1, 3, 7, 7, 0, 0, 2, 4, 4, -1, 0, 1, 1, 3, 6, 6, 0, 2, 2, 5, 5 };
    
    //转盘上24种水果类型
    const int g_Logos[Loop_Sprite_Count] = {
        BetA_Lucky_Big, BetA_Apple, BetA_Bell, BetA_Orange, BetA_Bell, BetA_Bar, BetA_Bar, BetA_Apple, BetA_Apple, BetA_Cherry, BetA_Watermelon, BetA_Watermelon,
        BetA_Lucky_Small, BetA_Apple, BetA_Orange, BetA_Orange, BetA_Bell, BetA_Seven, BetA_Seven, BetA_Apple, BetA_Cherry, BetA_Cherry, BetA_Star, BetA_Star,
    };
    
    //转盘上24种水果对应的赔率
    const int g_Logos_Rate[Loop_Sprite_Count] = {
        0, 5, 3, 10, 20, 50, 120, 5, 3, 15, 20, 3,
        0, 5, 3, 10, 20, 3, 40, 5, 3, 15, 3, 30
    };
    
    //转盘上每个位置的结构体信息
    struct g_Logos_Struct {
        unsigned int	logoIndex;
        int				logoType;
        int				logoRate;
        bool			hasOpened;
        bool			hasLuckyEffected;	//Lucky类型专用
        g_Logos_Struct() {
            logoIndex = 8;
            logoType = BetA_Apple;
            logoRate = 3;
            hasOpened = false;
            hasLuckyEffected = false;
        }
    };
    
    
    //游戏状态消息定义
    enum {
        //文字
//        GSTATE_XIAZHU_FAIL_BANKER_LACK_MONEY = 0,   //庄家不够赔，下注失败
//        GSTATE_XIAZHU_FAILE_LACK_MONEY,             //金币不足，下注失败
//        GSTATE_XIAZHU_FAILE_OVER,                   //超过个人下注限额，下注失败
//        GSTATE_XIAZHU_SUCCEED,                      //
//        GSTATE_SHANGZHUANG_FAIL_LACK_MONEY,
//        GSTATE_SHANGZHUANG_SUCCEED,
//        GSTATE_BE_BANKER_SUCCEED,
        //图片
//        GSTATE_BE_BANKER_TWO,
//        GSTATE_BE_BANKER_THREE,
//        GSTATE_BE_BANKER_FOUR,
//        GSTATE_BE_BANKER_FIVE,
//        GSTATE_XIAZHUANG_OVER_FIVE,         //任庄满5局下庄
//        GSTATE_XIAZHUANG_LACK_MONEY,        //金币不足，被下庄
    };


    
    
    /*******游戏数据包******/
    //////////////s2c/////////////////
 
    //同步数据包
    struct SyncDataStruct
    {
        int			gameState;
        LLONG		i64MyMoney;
        LLONG       iBasicCoin;
        SyncDataStruct() {
            gameState = GS_FREE_NOTE;
            i64MyMoney = 0;
            iBasicCoin = 0;
        }
    };
    
//    //玩家坐下消息，坐下后玩家就可以选择下注了，之后点击开始结束下注进入下一个状态
//    struct PlayerSitStruct
//    {
//        BYTE	bDeskStation;
//        int		gameState;
//        LLONG	i64MyMoney;
//        LLONG i64SingleNoteLimit;
//        //int		imultiplying[LoopK_A_BetA_Count];   //每个区域压中后庄家的赔率
//        int		iHistory[Open_History_Logo_Count];		//路子，最近9局开出的记录
//        //int		bCurrentStateLeftTimes;
//        //int		bCurrentBankerID;
//        //__int64 iCurrentBankerMoney;
//        //char	bCurrentBankerNickName[61];
//        PlayerSitStruct()
//        {
//            bDeskStation = BYTE(-1);
//            gameState = GS_FREE_NOTE;
//            i64MyMoney = 0;
//            i64SingleNoteLimit = 0;
//            memset(iHistory, -1, sizeof(iHistory));
//        }
//    };
    
    
	//游戏下注响应，每次下注都要以服务端数据为准，前段负责对数据进行同步
	struct GameNoteResponse
	{
		BYTE	bDeskStation;					//玩家座位号
		int		gameState;						//游戏状态
		LLONG iOwnAllCountNote;				//每个玩家自己下注总数（总押分）
		int		iOwnNotesNumber[BetA_Count];	//自己每个框下注数
		LLONG i64MyMoney;
		bool	canStartGame;					//true，下注成功，可以开始游戏
		GameNoteResponse()
		{
			bDeskStation = BYTE(-1);
			gameState = GS_XIAZHU;
			iOwnAllCountNote = 0;
			memset(iOwnNotesNumber, 0, sizeof(iOwnNotesNumber));
			i64MyMoney = 0;
			canStartGame = false;
		}
	};

    
    
    //游戏下注失败响应
    struct GameNoteFailedMsg
    {
        BYTE	bDeskStation;
        bool	bSucceed;
        int		bFailCode;	//0-自己金币不足	1-超过个人下注限额
        GameNoteFailedMsg() {
            bDeskStation = BYTE(-1);
            bSucceed = true;
            bFailCode = 0;
        }
    };
    
	//无法按照上局压注开始游戏
	struct GameBeginFailLackCoin
	{
		BYTE	bDeskStation;
		int		gameState;		//游戏状态
		bool	canStart;
		int		bFailCode;		//0-金币不足
		GameBeginFailLackCoin() {
			bDeskStation = BYTE(-1);
			gameState = GS_XIAZHU;
			canStart = false;
			bFailCode = 0;
		}
	};
    
    //发送开奖消息
    struct GameLoopResult
    {
        int			gameState;
        int			iStarPos;       ///开始位置
        int			iEndPos;        ///结束位置
        int			iOpenLogo;		//开奖类型，为8个下注区域中的一个及额外的lucky
        int			iTimes;			//圈数，客户端开奖前转动的圈数
        LLONG		i64Reward;		//当局奖金
        LLONG		i64MyMoney;		//背包金币
        GameLoopResult()
        {
            gameState = GS_KAIJIANG;
            iStarPos = -1;
            iEndPos = -1;
            iOpenLogo = BetA_Apple;
            iTimes = 1;
            i64Reward = 0;
            i64MyMoney = 0;
        }
    };
    
    //通知可以进行猜大小
    struct GameNotifyGuessNumber
    {
        int		gameState;
        bool	canGuess;
        GameNotifyGuessNumber() {
            gameState = GS_GUESS;
            canGuess = false;
        }
    };
    
    //猜大小操作超时
    struct GameGuessOverTime
    {
        int gameState;			//超时，则游戏结束，结算并进入下一轮
        LLONG		i64Reward;		//当局奖金
        LLONG		i64MyMoney;		//背包金币
        GameGuessOverTime() {
            gameState = GS_FREE_NOTE;
            i64Reward = 0;
            i64MyMoney = 0;
        }
    };
    
    //发送猜大小结果
    struct GameGuessResult
    {
        int				gameState;
        unsigned int	guessNumber;		//上次的值，1-7代表小，8-14代表大
        bool			canContinueGuess;	//如果上一次大小猜中，则可以继续猜大小
        LLONG			i64Reward;			//当局奖金
        LLONG			i64MyMoney;			//背包金币
        GameGuessResult() {
            gameState = GS_GUESS;
            guessNumber = 1;
            canContinueGuess = false;
            i64Reward = 0;
            i64MyMoney = 0;
        }
    };
    
    //划分数据同步
    struct GameSwapScoreReseponse
    {
        LLONG			i64Reward;			//当局奖金
        LLONG			i64MyMoney;			//背包金币
        unsigned int	iSwapCode;			//0-下分成功，结束；1-上分成功；2-金币不足上分失败；3-超过单次上分限额
        GameSwapScoreReseponse() {
            i64Reward = 0;
            i64MyMoney = 0;
            iSwapCode = 0;
        }
    };
    
    //通知进入lucky状态
    struct GameNotifyLucky
    {
        int		gameState;
        bool	isLucky;
        GameNotifyLucky() {
            gameState = GS_LUCKY;
            isLucky = false;
        }
    };
    
    //发送开中lucky后的开奖消息
    struct GameLuckyOpen
    {
        int				gameState;		//游戏状态
        bool			isBigLucky;		//是4-6倍的大lucky还是小lucky
        int				openTimes;		//此lucky状态额外开几个其他的水果
        int				iOpenLucky[6];	//单次开lucky最多只能开出6个，最少一个，取决于开中的是那种lucky
       // LLONG			i64Reward;		//当局奖金
        LLONG			i64MyMoney;		//背包金币
        GameLuckyOpen() {
            gameState = GS_LUCKY;
            isBigLucky = false;
            openTimes = 0;
            memset(iOpenLucky, -1, sizeof(iOpenLucky));
           // i64Reward = 0;
            i64MyMoney = 0;
        }
    };
    
    //同步奖池数据
    struct GameRewardsPool
    {
        LLONG		i64RewardsPool;
        GameRewardsPool()
        {
            i64RewardsPool = 0;
        }
    };
    
    //发送结算结果消息
    struct GameSettlementMsg	
    {
        int				gameState;
        
        LLONG			i64Reward;		//当局实际输赢
        LLONG			i64MyMoney;		//背包金币
        
        GameSettlementMsg()
        {
            gameState = GS_SETTLEMENT;
            i64Reward = 0;
            i64MyMoney = 0;
        }
    };
    
    //没钱退出
    struct tag_s2c_tip_exit
    {
        int     gameState;
        BYTE	bIsExit;
        LLONG	i64Money;
        tag_s2c_tip_exit()
        {
            gameState = GS_FREE_NOTE;
            bIsExit = -1;
            i64Money = 0;
        }
    };
    
    
    //五花牛全服通知
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
    
    
    //////////////////////////////////////////////////////////////////////////
    /////////////////////C2S
    //////////////////////////////////////////////////////////////////////////
    //下注
    struct GameXiaZhuRequest
    {
        BYTE			bDeskStation;
        unsigned int	bBetType;
        GameXiaZhuRequest()
        {
            bDeskStation = BYTE(-1);
            bBetType = BetA_Apple;
        }
    };
    
    //押大小
    struct GameGuessRequest
    {
        BYTE	bDeskStation;
        bool	isSmallNum;		//true 压小
        GameGuessRequest() {
            bDeskStation = BYTE(-1);
            isSmallNum = false;
        }
    };
    
    //猜大小期间上下分
    struct GameSwapScoreRequest
    {
        BYTE		bDeskStation;
        bool		isScoreUp;		//true 上分，代表从背包取金币到中奖区域进行猜大小
        GameSwapScoreRequest() {
            bDeskStation = BYTE(-1);
            isScoreUp = false;
        }
    };
    
    //游戏开始
    struct GameBeginRequest
    {
        BYTE		bDeskStation;
        bool		isStartGame;
        GameBeginRequest() {
            bDeskStation = BYTE(-1);
            isStartGame = false;
        }
    };
    
    //游戏请求自动托管
    struct GameAutoRequest
    {
        BYTE		bDeskStation;
        bool		isAutoGame;		//自动按照本局的续压，开奖中奖后直接开始下一局，不进行猜大小
        GameAutoRequest() {
            bDeskStation = BYTE(-1);
            isAutoGame = false;
        }
    };
    
    
    //////////////////////////////////////////////////////////////////////////
    /////////////////////重连包
    //////////////////////////////////////////////////////////////////////////
    


	
}
#pragma pack(pop)
#endif
