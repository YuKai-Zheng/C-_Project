#ifndef __BB_COMMON_DEFINE_h__
#define __BB_COMMON_DEFINE_h__

#include <memory.h>
#include "HNBaseType.h"
#include <vector>



#pragma pack(push,1)

namespace BB
{

#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)

    //服务端向客户端发送消息ID
#define ASS_SYNC_BASIC_DATA		51
#define ASS_USER_SIT 			55
#define ASS_GAME_FREE 			56
#define ASS_CONFIRM_BANKER		57
#define ASS_BEGAN_XIAZHU		58
#define ASS_XIAZHU_FINISH		59
#define ASS_XIAZHU_RESPONSE		60
#define ASS_BANKER_RESPONSE		61
#define ASS_REBET_RESPONSE		62
#define ASS_CLEAR_RESPONSE		63
#define ASS_KAIJIANG			64
#define ASS_SETTLEMENT			65
#define ASS_LACK_MONEY			68		//判断金币是否低于房间限额
#define ASS_XIAZHU_FAIL			69
#define ASS_XIAZHUANG_RESPONSE	67
#define ASS_ROBOT_XIAZHU		66
    
    //客户端请求消息ID
#define GS_REQUEST_BANKER		70
#define GS_XIAZHU_REQUEST		71
#define GS_CLEAR_REQUEST		72
#define GS_REBET_REQUEST		73
    
    //断线重连包
#define RESUME_XIAZHU_STATION	80
#define RESUME_KAIJIANG_STATION 81
    
//#define S2C_NOTIFY_MSG          149
	enum
	{
		S2C_NOTIFY_MSG = 149
	};
    
    //////////////////////////////////////////////////////////////////////////
    //游戏信息
    enum
    {
        GAME_NAME_ID	=	31000501,
        GAME_KIND_ID	=	1,
        PLAYER_COUNT	=   2,
    };
    
    
    //游戏状态定义
    enum Game_State
    {
        GS_FREE				= 0,		//等待东家设置状态
        GS_CONFIRM_BANKER	= 10,		//确定庄家
        GS_XIAZHU	        = 20,		//下注状态
        GS_XIAZHU_FINISH	= 21,
        GS_KAIJIANG			= 22,
        GS_GAME_FINISH		= 23,
        GS_WAIT_NEXT		= 24,		//等待下一盘开始
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
    
    
    //游戏数据设定，转盘loop上32种
    //下注区域设定，顺时针共8种
    enum {
        BetA_Lam_Max = 0,
        BetA_BMW_Max = 1,
        BetA_Ben_Max = 2,
        BetA_VM_Max = 3,
        BetA_VM_Min = 4,
        BetA_Ben_Min = 5,
        BetA_BMW_Min = 6,
        BetA_Lam_Min = 7,
        LoopK_A_BetA_Count = 8,         //转盘上共有32种，8个组合一重复
        Open_History_Logo_Count = 9,    //最多纪录十局游戏开奖记录
        Loop_Sprite_Count = 32          //结果展示区域有32个
    };
    
    
    //下注金额，共五种
    enum {
        Bet_Money_1k = 0,
        Bet_Money_1w = 1,
        Bet_Money_10w = 2,
        Bet_Money_100w = 3,
        Bet_Money_500w = 4,
        Bet_Coin_Count      //下注筹码为5个
    };
    
    //每种筹码对应金额
    static const long long BetMoneyValues[Bet_Coin_Count] = {
        1000,
        10000,
        100000,
        1000000,
        5000000
    };
    
    
    //转盘上32种车标的排列，由左上角开始，图标顺序与下注区域车标由左至右顺序一致
    const int g_Logos[Loop_Sprite_Count] = {
        BetA_Lam_Max, BetA_Lam_Min, BetA_BMW_Max, BetA_BMW_Min, BetA_Ben_Max, BetA_Ben_Min, BetA_VM_Max, BetA_VM_Min,
        BetA_Lam_Max, BetA_Lam_Min, BetA_BMW_Max, BetA_BMW_Min, BetA_Ben_Max, BetA_Ben_Min, BetA_VM_Max, BetA_VM_Min,
        BetA_Lam_Max, BetA_Lam_Min, BetA_BMW_Max, BetA_BMW_Min, BetA_Ben_Max, BetA_Ben_Min, BetA_VM_Max, BetA_VM_Min,
        BetA_Lam_Max, BetA_Lam_Min, BetA_BMW_Max, BetA_BMW_Min, BetA_Ben_Max, BetA_Ben_Min, BetA_VM_Max, BetA_VM_Min,
    };
    
    //各个区域赔率，共有8个下注区域
    static int g_iBetRate[LoopK_A_BetA_Count] = {
        40, 30, 20, 10,
        5, 5, 5, 5
    };
    
    //各个区域压中概率
    static int g_iBetAreaProbability[LoopK_A_BetA_Count] = {
        8000,4000,2000,2000,
        1000,8000,1000,2000
    };
    
    //压得钱区域概率
    static  int g_iBetMoneyTypeProbability[Bet_Coin_Count] = {
        8000, 8000, 1000, 10, 1
    };
    
    
    //游戏状态消息定义
    enum {
        //文字
        GSTATE_XIAZHU_FAIL_BANKER_LACK_MONEY = 0,   //庄家不够赔，下注失败
        GSTATE_XIAZHU_FAILE_LACK_MONEY,             //金币不足，下注失败
        GSTATE_XIAZHU_FAILE_OVER,                   //超过个人下注限额，下注失败
        GSTATE_XIAZHU_SUCCEED,                      //
        GSTATE_SHANGZHUANG_FAIL_LACK_MONEY,
        GSTATE_SHANGZHUANG_SUCCEED,
        GSTATE_BE_BANKER_SUCCEED,
        //图片
        GSTATE_BE_BANKER_TWO,
        GSTATE_BE_BANKER_THREE,
        GSTATE_BE_BANKER_FOUR,
        GSTATE_BE_BANKER_FIVE,
        GSTATE_XIAZHUANG_OVER_FIVE,         //任庄满5局下庄
        GSTATE_XIAZHUANG_LACK_MONEY,        //金币不足，被下庄
    };


    //庄家状态代码定义
    enum {
        BANKER_NONE = 0,			//非庄
        BANKER_BE_BANKER_SUCCEED,	//上庄成功
        BANKER_TWO,					//连庄两局
        BANKER_THREE,				//连庄三局
        BANKER_FOUR,				//连庄四局
        BANKER_FIVE,				//连庄五局
        BANKER_XIAZHUANG_OVER_FIVE,	//连庄满五局，下庄
        BANKER_XIAZHUANG_LACK_COIN,	//金币不足下庄
    };
    
    
    /*******游戏数据包******/
    //////////////s2c/////////////////
    //玩家进入游戏同步消息
    struct SyncDataStruct
    {
        BYTE		bDeskStation;
        int			gameState;
        LLONG		i64MyMoney;
        LLONG		i64SingleNoteLimit;
        int			imultiplying[LoopK_A_BetA_Count];   //每个区域压中后庄家的赔率
        int			iHistory[Open_History_Logo_Count];	//路子，最近9局开出的记录
        int			bCurrentStateLeftTimes;
        int         bCurrentBankerID;
        LLONG		iCurrentBankerMoney;
        char		bCurrentBankerNickName[61];
        int			requestBankerListCount;				//请求上庄的人数
        LLONG		requestBankerNeedMoney;				//上庄需要的最少金币
        SyncDataStruct()
        {
            bDeskStation = BYTE(-1);
            gameState = GS_FREE;
            i64MyMoney = 0;
            i64SingleNoteLimit = 0;
            memset(imultiplying, 0, sizeof(imultiplying));
            memset(iHistory, -1, sizeof(iHistory));
            bCurrentBankerID = 0;
            iCurrentBankerMoney = 0;
            memset(bCurrentBankerNickName, 0, sizeof(bCurrentBankerNickName));
            bCurrentStateLeftTimes = 0;
            requestBankerListCount = 0;
            requestBankerNeedMoney = 0;
        }
    };
    //玩家坐下消息结构
    struct PlayerSitStruct
    {
        BYTE	bDeskStation;
        int		gameState;
        LLONG	i64MyMoney;
        LLONG i64SingleNoteLimit;
        int		imultiplying[LoopK_A_BetA_Count];   //每个区域压中后庄家的赔率
        int		iHistory[Open_History_Logo_Count];		//路子，最近9局开出的记录
        int		bCurrentStateLeftTimes;
        int     bCurrentBankerID;
        LLONG iCurrentBankerMoney;
        char	bCurrentBankerNickName[61];
        PlayerSitStruct()
        {
            bDeskStation = BYTE(-1);
            gameState = GS_FREE;
            i64MyMoney = 0;
            i64SingleNoteLimit = 0;
            memset(imultiplying, 0, sizeof(imultiplying));
            memset(iHistory, -1, sizeof(iHistory));
            bCurrentBankerID = 0;
            iCurrentBankerMoney = 0;
            memset(bCurrentBankerNickName, 0, sizeof(bCurrentBankerNickName));
            bCurrentStateLeftTimes = 0;
        }
    };
    
    //游戏开始
    struct GameFreeStruct
    {
        BYTE				bDeskStation;
        int					gameState;							//游戏当前的状态
        LLONG				i64MyMoney;							/** 自己金币 */
        int					imultiplying[LoopK_A_BetA_Count];   //每个区域压中后庄家的赔率
        int					iHistory[Open_History_Logo_Count];		//路子，最近9局开出的记录
        int					bCurrentStateLeftTimes;
        GameFreeStruct()
        {
            bDeskStation = BYTE(-1);
            gameState = GS_FREE;
            i64MyMoney = 0;
            memset(imultiplying, 0, sizeof(imultiplying));
            memset(iHistory, -1, sizeof(iHistory));
            bCurrentStateLeftTimes = 0;
        }
    };
    
    //游戏开始消息结构体，开始下注通知前服务端会确定出庄家，之后才允许下注
    struct GameBeganAndConfirmBanker
    {
        BYTE				bDeskStation;
        int					gameState;
        int					bCurrentStateLeftTimes;
        LLONG				i64SingleNoteLimit;					//个人下注限额，单局可下注最大金额
        //banker
        int					bCurrentBankerID;					//当前庄家位置
        LLONG				iCurrentBankerMoney;				//当前庄家的金币
        char				bCurrentBankerNickName[61];			//当前庄家名字
        int					iBankerListWaitNumber;
        //only for real player
        bool				isBanker;							//是否是庄
        bool                isWaitingBanker;                    //是否正在等待上庄
        int					bankerCode;                         //
        
        GameBeganAndConfirmBanker() {
            bDeskStation = BYTE(-1);
            gameState = GS_CONFIRM_BANKER;
            bCurrentStateLeftTimes = 0;
            i64SingleNoteLimit = 0;
            bCurrentBankerID = 0;
            iCurrentBankerMoney = 0;
            memset(bCurrentBankerNickName, 0, sizeof(bCurrentBankerNickName));
            iBankerListWaitNumber = 0;
            isBanker = false;
            isWaitingBanker = false;
            bankerCode = BANKER_NONE;
        }
    };
    
    ////服务端确定庄家
    //struct GameConfirmBanker
    //{
    //	int gameState;
    //	int bankerIndex;
    //	GameConfirmBanker() {
    //		gameState = GS_CONFIRM_BANKER;
    //		bankerIndex = BYTE(-1);
    //	}
    //};
    
    //游戏下注，任意玩家下注，通知给客户端
    struct GameBeganBet
    {
        int gameState;
        int iPrevHasNote;
        int bCurrentStateLeftTimes;
        bool isBanker;
        GameBeganBet()
        {
            gameState = GS_XIAZHU;
            iPrevHasNote= 0;
            bCurrentStateLeftTimes = 0;
            isBanker = false;
        }
    };
    
    //下注结束
    struct GameXiaZhuFinish
    {
        int gameState;
        int bCurrentStateLeftTime;
        GameXiaZhuFinish()
        {
            gameState = GS_XIAZHU_FINISH;
            bCurrentStateLeftTime = 0;
        }
    };
    
    //游戏下注响应
    struct GameXiaZhuResponse
    {
        BYTE	bDeskStation;							//玩家座位号
        LLONG iOwnAllCountNote;					//每个玩家自己下注总数（总押分）
        LLONG iOwnNotes[LoopK_A_BetA_Count];		//自己每个框下注数
        LLONG iUserMoney;
        LLONG iNotePerRegion[LoopK_A_BetA_Count];	//单个区域总的下注，每个下注区域所有玩家的下注值
        GameXiaZhuResponse()
        {
            bDeskStation = BYTE(-1);
            iOwnAllCountNote = 0;
            memset(iOwnNotes,0,sizeof(iOwnNotes));
            iUserMoney = 0;
            memset(iNotePerRegion,0,sizeof(iNotePerRegion));
        }
    };
    
    //机器人下注消息
    struct GameRobotXiaZhuResponse
    {
        int			iNoteArea;			//下注区域
        int			iNoteMoneyIndex;	//金币类型
        LLONG		iNoteMoney;			//单个区域下注金额
        LLONG		iNotePerRegion[LoopK_A_BetA_Count];	//单个区域总的下注，每个下注区域所有玩家的下注值
        GameRobotXiaZhuResponse()
        {
            iNoteArea = -1;
            iNoteMoneyIndex = -1;
            iNoteMoney = 0;
            memset(iNotePerRegion, 0, sizeof(iNotePerRegion));
        }
    };
    
    //游戏下注失败响应
    struct GameXiaZhuFaileMsg
    {
        BYTE	bDeskStation;
        bool	bSucceed;
        int		bFailCode;	//0-自己金币不足	1-庄家不够赔付	2-超过个人下注限额
        GameXiaZhuFaileMsg() {
            bDeskStation = BYTE(-1);
            bSucceed = true;
            bFailCode = 0;
        }
    };
    
    //请求上庄消息
    struct GameRequestBankerResponse
    {
        BYTE bDeskStation;
        int bCurrentBankerID;
        LLONG iCurrentBankerMoney;
        char iBankerNickName[61];
        LLONG iBankerNeedMoney;
        int iBankerListNumber;
        bool requestSucceed;
    };
    
    //下庄消息
    struct GameXiaZhuangResponse
    {
        BYTE bDeskStation;
        bool canXiaZhuang;
    };
    
    //续投消息
    struct GameReBetResponse
    {
        BYTE bDeskStation;
        LLONG iUserMoney;
        bool isSucceed;
        LLONG iOwnNotes[LoopK_A_BetA_Count];
        
        GameReBetResponse()
        {
            bDeskStation = BYTE(-1);
            iUserMoney = 0;
            isSucceed = false;
            memset(iOwnNotes, 0, sizeof(iOwnNotes));
        }
    };
    
    //清空消息
    struct GameClearResponse
    {
        BYTE bDeskStation;
        bool isSucceed;
        LLONG iUserMoney;
        GameClearResponse()
        {
            bDeskStation = BYTE(-1);
            isSucceed = false;
            iUserMoney = 0;
        }
    };
    
    ///发送开始转动消息(开奖)
    struct GameLoopResult
    {
        int			gameState;
        int			iStarPos;       ///开始位置
        int			iEndPos;        ///结束位置
        int			iOpenLogo;		//开奖类型，为8个下注区域中的一个
        int			iTimes;			//圈数，客户端开奖前转动的圈数
        int			bCurrentStateLeftTimes;	//当前阶段剩余时间
        GameLoopResult()
        {
            gameState = GS_KAIJIANG;
            iStarPos = -1;
            iEndPos = -1;
            iOpenLogo = BetA_Lam_Min;
            iTimes = 1;
            bCurrentStateLeftTimes = 0;
        }
    };
    
    
    //发送结算结果消息
    struct GameSettlementMsg	
    {
        BYTE			playerIndex;
        int		        iHistory[Open_History_Logo_Count];		//路子，最近9局开出的记录
        LLONG			i64RealWinMoney;						//输赢
        LLONG			i64UserMoney;							//用户本局结束后身上的钱（赢钱需要扣除手续费）
        BYTE			bankerIndex;							
        LLONG			bankerRealWinMoney;						//庄家本局输赢金币，不计算手续费
        int             openLogoIndexAtLoop;
        GameSettlementMsg()
        {
            playerIndex = BYTE(-1);
            memset(iHistory, -1, sizeof(iHistory));
            i64RealWinMoney = 0;
            i64UserMoney= 0;
            bankerIndex = BYTE(-1);
            bankerRealWinMoney = 0;
            openLogoIndexAtLoop = -1;
        }
    };
    
    //没钱退出
    struct tag_s2c_tip_exit
    {
        BYTE	bIsExit;
        LLONG	i64Money;
        tag_s2c_tip_exit()
        {
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
    //请求上庄
    struct GameRequestBankerRequest
    {
        BYTE bDeskStation;
        bool request;
        GameRequestBankerRequest() {
            bDeskStation = BYTE(-1);
            request = false;
        }
    };
    
    //下注
    struct GameXiaZhuRequest
    {
        BYTE		bDeskStation;
        BYTE		bBetType;
        LLONG		i64BetMoney;
        GameXiaZhuRequest()
        {
            bDeskStation = BYTE(-1);
            bBetType = BYTE(-1);
            i64BetMoney = 0;
        }
    };
    
    //清空
    struct GameClearRequest
    {
        BYTE bDeskStation;
        bool requestClear;
        GameClearRequest() 
        {
            bDeskStation = BYTE(-1);
            requestClear = false;
        }
    };
    
    //续投
    struct GameReBetRequest
    {
        BYTE bDeskStation;
        bool requestReBet;
        GameReBetRequest()
        {
            bDeskStation = BYTE(-1);
            requestReBet = false;
        }
    };
    
    
    //////////////////////////////////////////////////////////////////////////
    /////////////////////重连包
    //////////////////////////////////////////////////////////////////////////
    //公共数据包
    struct GameStation_Base
    {
        BYTE		bDeskStation;
        BYTE		bGameStation;					/**<游戏状态*/
        long long   i64MyMoney;						//钱
        long long	i64SingleNoteLimit;				//单个个人下注限额 
        int         bCurrentBankerID;
        long long	iBankerMoney;
        char		iBankerNickName[61];
        long long   iBankerNeedMoney;
        
        int         imultiplying[LoopK_A_BetA_Count];       //倍率
        int			iCurrentStateLeftTimes;					//时间
        int         iHistory[Open_History_Logo_Count];		//路子
        
        GameStation_Base()
        {
            bDeskStation = BYTE(-1);
            bGameStation = GS_FREE;
            i64MyMoney = 0;
            i64SingleNoteLimit = 0;
            bCurrentBankerID = 0;
            iBankerMoney = 0;
            memset(iBankerNickName, 0, sizeof(iBankerNickName));
            iBankerNeedMoney = 0;
            memset(imultiplying, 0, sizeof(imultiplying));
            iCurrentStateLeftTimes = 0;
            memset(iHistory, -1, sizeof(iHistory));
        }
    };
    
    //下注阶段
    struct GameStation_Bet:GameStation_Base
    {
        long long   i64OwnNotes[LoopK_A_BetA_Count];    //区域下注
        long long	i64AreaNotes[LoopK_A_BetA_Count];	//总得区域下注
        GameStation_Bet()
        {
            GameStation_Base();
            memset(i64OwnNotes, 0, sizeof(i64OwnNotes));
            memset(i64AreaNotes, 0, sizeof(i64AreaNotes));
        }
    };
    
    //开奖阶段
    struct GameStation_Kaijiang:GameStation_Base
    {
        long long   i64OwnNotes[LoopK_A_BetA_Count];		//区域下注
        long long	i64AreaNotes[LoopK_A_BetA_Count];		//总得区域下注
        int         iStarPos;							///开始位置
        int         iEndPos;							///结束位置
        int         iEndLogo;
        GameStation_Kaijiang()
        {
            GameStation_Base();
            memset(i64OwnNotes, 0, sizeof(i64OwnNotes));
            memset(i64AreaNotes, 0, sizeof(i64AreaNotes));
            iStarPos = -1;
            iEndPos = -1;	
            iEndLogo = -1;
        }
    };


	
}
#pragma pack(pop)
#endif
