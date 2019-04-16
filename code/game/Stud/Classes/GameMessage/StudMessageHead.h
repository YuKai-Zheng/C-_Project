#ifndef __STUD_AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE_H__
#define __STUD_AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE_H__

#include "HNNetExport.h"

namespace STUD
{
	#ifdef MAX_PATH
	#undef MAX_PATH
	#endif // MAX_PATH
	
	// audio effect
	const static char* STUDAUDIO_ALLIN									= "allin.mp3";									//梭哈
	const static char* STUDAUDIO_BET										= "bet.mp3";									//下注
	const static char* STUDAUDIO_CALL									= "call.mp3";										
	const static char* STUDAUDIO_CHECK								= "check.mp3";
	const static char* STUDAUDIO_FOLD									= "giveUp.mp3";
	const static char* STUDAUDIO_RAISE									= "raise.mp3";                                 //加注


	const static char* STUDAUDIO_CHIP									= "stud/audio/table/betchip.wav";
	const static char* STUDAUDIO_PRIZE									= "stud/audio/table/prize.wav";
	const static char* STUDAUDIO_OVERTIME							= "stud/audio/table/betovertime.wav";
	const static char* STUDAUDIO_BOARD_CARD						= "stud/audio/table/boardcard.wav";
	const static char* STUDAUDIO_FIGHT_CARD						= "stud/audio/table/fightcard.wav";

	const static char* STUDAUDIO_SELF_ACTION						= "stud/audio/table/selfaction.wav";
	const static char* STUDAUDIO_SIT										= "stud/audio/table/sitdown.mp3";

	const static char* STUDAUDIO_LOSE									= "stud/audio/table/snglose.mp3";
	const static char* STUDAUDIO_WIN									= "stud/audio/table/sngwin.mp3";
	const static char* STUDAUDIO_WIN_BOARD						= "stud/audio/table/winboard.wav";
	const static char* STUDAUDIO_ACTIVITY								= "stud/audio/table/activityprize.wav";

	enum STUD_COMMAND
	{
		MAX_PATH								= 260,
		GAME_KIND_ID							= 1,
		GAME_NAME_ID						= 12100202,				// 名字 ID

		// total cards
		TOTAL_CARDS							= 52,

		//版本定义
		GAME_MAX_VER = 1,												//现在最高版本
		GAME_LESS_VER = 1,													//现在最低版本
		GAME_CHANGE_VER					= 0,							//修改版本


		//游戏信息
		PLAY_COUNT							= 2,							// 游戏人数
		HANDCARD_COUNT					= 5,							// 手牌数
	

		MAX_NAME_INFO						= 256,

		//按注类型
		TYPE_GIVE_UP							= 0x00,						//放弃
		TYPE_NOTE								= 0x01,						//下注
		TYPE_ADD									= 0x02,						//加注
		TYPE_FOLLOW								= 0x03,					//跟注
		TYPE_SHUTTLE							= 0x04,						//梭哈


		SHOWHA_TIME							= 2,							//第三张牌便可以梭
		//#define FIRST_CAN_GIVEUP					true				//第一把可放弃
		NOTE_LIMIT								= 200,						//下注上限

		//游戏进行状态	
		GAME_FREE								= 220,						//空闲
		GAME_PLAYING							= 221,						//游戏中	
	
	};

	enum GAME_STATUS
	{
		GS_WAIT_SETGAME					= 0,							//等待东家设置状态
		GS_WAIT_ARGEE						= 1,							//等待同意设置
		GS_SEND_CARD						= 20,							//发牌状态
		GS_WAIT_BACK							= 21,							//等待扣压底牌
		GS_PLAY_GAME							= 22,							//游戏中状态
		GS_WAIT_NEXT							= 23							//等待下一盘开始 
	};


	#ifdef  SSYJ //谁是赢家
	#define CLIENT_DLL_NAME					TEXT("10409400.ico")
	#define SERVER_DLL_NAME					TEXT("10409400.DLL")
	#define NAME_ID							10400400							//名字 ID
	#define PLAY_COUNT						4							//游戏人数
	#define SKIN_FOLDER						TEXT("sh")					//图片文件夹
	#define SKIN							TEXT("cardskin1")					//两套图片中用一套
	#endif

	#ifdef FOURSH
	#define CLIENT_DLL_NAME					TEXT("10409400.ico")
	#define SERVER_DLL_NAME					TEXT("10409400.DLL")
	#define NAME_ID							10409400							//名字 ID
	#define PLAY_COUNT						4							//游戏人数
	#define SKIN_FOLDER						TEXT("sh")					//图片文件夹
	#define SKIN							TEXT("cardskin1")					//两套图片中用一套
	#endif

	#ifdef TWOSH
	#define SERVER_DLL_NAME					TEXT("10409200.DLL")
	#define CLIENT_DLL_NAME					TEXT("10409200.ico")
	#define NAME_ID							12100202									//名字 ID
	#define PLAY_COUNT						2											//游戏人数

	#define SKIN_FOLDER						TEXT("sh")									//图片文件夹
	#define SKIN							TEXT("cardskin1")							//两套图片中用一套
	#endif


	#ifdef VIDEO
	#define CLIENT_DLL_NAME					TEXT("10409405.ico")
	#define SERVER_DLL_NAME					TEXT("10409405.DLL")
	#define NAME_ID							10409405									//名字 ID
	#define SKIN_FOLDER						TEXT("sh")									//图片文件夹
	#define SKIN							TEXT("cardskin1")							//两套图片中用一套
	#endif

	//static CHAR szTempStr[MAX_PATH] = {0};//暂存字符串，用来获取 dll_name 等的时候使用
	//#define GET_CLIENT_DLL_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".ico")) //获取客户端 
	//#define GET_SERVER_DLL_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".dll")) //获取服务器端 
	//#define GET_SKIN_FOLDER(A)              (sprintf(A,"%d",NAME_ID),strcat(A,""))     //获取图片文件夹
	/********************************************************************************/
	//#define SUPER_VERSION					//超级客户端
	//#define SET_JUDGE						//设置胜率败率

	/********************************************************************************/

	#pragma pack(1)

	//游戏开始
struct GameBeginStruct
{
	BYTE				iBeenPlayGame;					//游戏回合
	LLONG				iBaseNote;						//底注	
	LLONG				iLimitNote;						//最大注数
	bool				bUserState[PLAY_COUNT];			//玩家状态
	bool				bIsSuperUser;					//判断自己是否超级客户端：由服务器传来数据，每局都校验
	bool				bChangeRobotCard;				//是否改变了机器人的牌 true 改变了 false 没有改变
};

//收集筹码动画
struct CollectJetonStruct
{
	LLONG				JetonTotal;
	BYTE				bDeskStation;
};

//发牌数据包
struct SendCardStruct
{
	BYTE				iCard;							//牌标号
	BYTE				bDeskStatin;					//新一轮出牌者
};
//发牌数据包
struct SendOneTurnCardStruct
{
	BYTE    iStartPos;                    ///发牌起始位置
	BYTE    iCardList[PLAY_COUNT][5];     ///发牌
	BYTE    iCardCount[PLAY_COUNT];       /// 当前每个玩家发牌的张数
	int		iFirstSendCardCount;		  ///第一次发的牌张数
	SendOneTurnCardStruct()
	{
		memset(this,0,sizeof(SendOneTurnCardStruct));
		iStartPos = 255; 
	}

};
//发牌一轮扑克数据包
struct SendOneTurnCard
{
	BYTE                iTurnCard[PLAY_COUNT];     //一轮扑克数据
	BYTE                bStartPos;                 //起始位置
	SendOneTurnCard()
	{
		memset(iTurnCard,0,sizeof(iTurnCard));
		bStartPos = 255 ;
	}
};

//限注
struct LimitStruct
{
	BYTE				iNoteTime;						//下注次数
	LLONG				iLimit;							//限注
};

//发牌結束数据包
struct SendCardFinishStruct
{
	BYTE				bDeskStatin;					//发牌完成
};

//游戏开始数据包
struct BeginPlayStruct
{
	BYTE				bDeskStaton;					//出牌的位置
};

//发送用户结果
struct tagUserResult
{
	bool				bNewTurn;						//新一轮
	bool				bBet[PLAY_COUNT];				
	BYTE				bAddTimes;						//加倍标记
	BYTE				bNoteTimes;						//押注次数
	BYTE				iNowBigNoteStyle;				//下注类型
	BYTE				iOutPeople;						//下注者
	BYTE				m_byUserAddNum[PLAY_COUNT];		//本轮各个玩家加注次数 2011-12-13
	LLONG				iNowBigNote;					//当前下注
	LLONG				iTotalNote;						//总下注

	tagUserResult()
	{
		memset(this,0,sizeof(tagUserResult));
		memset(m_byUserAddNum,0,sizeof(m_byUserAddNum));
		bNewTurn = false;
	}
};

//游戏结束统计数据包
struct GameEndStruct
{	
	BYTE				iCardShape[PLAY_COUNT];					//牌形
	INT				bShowCardShape[PLAY_COUNT];			//显示牌形
	INT					iUserState[PLAY_COUNT];					//四家状态(提前放弃,还是梭)
	LLONG				iTurePoint[PLAY_COUNT];					//玩家得分
	LLONG				iChangeMoney[PLAY_COUNT];
};

//游戏提前结束包
struct GameAheadStruct
{
	BYTE				bDeskStation;					//胜者
	LLONG				iTurePoint[PLAY_COUNT];					//庄家得分
	LLONG				iChangeMoney[PLAY_COUNT];				//金币
};
//游戏结束统计数据包
struct GameCutStruct
{
	INT					bDeskStation;					//退出位置
	LLONG				iTurePoint[PLAY_COUNT];			//庄家得分
	LLONG				iChangeMoney[PLAY_COUNT];
};

struct HaveThingStruct
{
	BYTE				pos;
	char				szMessage[61];
};

struct LeaveResultStruct
{
	BYTE				bDeskStation;
	int					bArgeeLeave;
};

struct MessageStruct							
{
	char				 Message[200];
};

struct StopResultStruct
{
	bool				bArgee;
};


//用户处理信息
struct tagUserProcess
{
	bool                bBet;						//是否下注
	BYTE				iVrebType;					//所处理的按钮
	LLONG				iNote;						//下注数
	tagUserProcess()
	{
		memset(this,0,sizeof(tagUserProcess));
	}
};

//代替断线用户处理信息
struct ReplaceUserNoteStruct
{
	BYTE				bDeskStation;				//代替的位置
	BYTE				iVrebType;					//所处理的按钮
	LLONG				iNote;						//下注数
};

//机器人托管
struct UseAIStation
{
	BYTE				bDeskStation;				//坐号
	INT				bState;						//状态
};

//发送用户押注
struct NoteResult
{
	bool				bBet[PLAY_COUNT];						//是否下注
	BYTE				bNoteStyle;					//下注类型
	BYTE				iOutPeople;					//下注者
	LLONG				iCurNote;					//当前下注数
	LLONG				iPlayerTotalNote[PLAY_COUNT];	//本局游戏中各玩家下注的总数  xyh 20110818
	
};

//结算
struct GameFinishNotify
{
	char				name[PLAY_COUNT][61];
	BYTE				iCardShape[PLAY_COUNT];		//牌型
	LLONG			    iBasePoint;
	BYTE				iStyle;			
	LLONG				iWardPoint[PLAY_COUNT];
	LLONG				iMoney[PLAY_COUNT];

	GameFinishNotify()
	{
		memset(this,0,sizeof(GameFinishNotify));
	}
};

//用户提醒 金币不足退出数据
struct tag_s2c_tip_exit
{
    BYTE    bChairID;
	BYTE	bIsExit;
	long long	i64Money;
	tag_s2c_tip_exit()
	{
        bChairID = 255;
		bIsExit = 255;
		i64Money = 0;
	}
};

//colin90511牌信息
struct CardInfo
{
    BYTE                bDeskStation;              //座位号
	BYTE                iUserCard[5];               //五张牌值                 
};

struct GameStation_Base
{
	//游戏版本
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	INT					iRoomBasePoint;					//房间倍数
	INT				    iRunPublish;					//逃跑扣分

	BYTE				bGameStation;					//游戏状态
	//游戏信息
	BYTE				iThinkTime;						//游戏盘数
	BYTE				iBeginTime;						//游戏盘数
	BYTE				iAllCardCount;					//扑克数目
	LLONG				iLimitNote;						//最高注
	LLONG				iBaseNote;						//底注
	BYTE                byUserFollow;					//第几把必跟
	BYTE                bShowHa;                        //第几轮可以梭哈
	bool				bUserState[PLAY_COUNT];			//玩家状态
	bool				bIsSuperUser;					//判断自己是否超级客户端：由服务器传来数据，每局都校验
	BYTE				m_byAddNum;					/**< 每轮加注次数  0时不做限制 */   //lym 2011-12-13
	BYTE                bPK;                            ///是否PK

	GameStation_Base()
	{
		memset(this,0,sizeof(GameStation_Base));
	}
};

//游戏状态数据包
struct GameStation_1 : GameStation_Base
{
	BYTE				bOtherdata;						//预留数据
	GameStation_1()
	{
		memset(this,0,sizeof(GameStation_1));
	}
};

//游戏状态数据包	（ 等待其他玩家开始 ）
struct GameStation_2 : GameStation_Base
{
	bool				bAgree[PLAY_COUNT];				//玩家是否准备
	BYTE				bOtherdata;						//预留数据
	GameStation_2()
	{
		memset(this,0,sizeof(GameStation_2));
	}
};

//游戏状态数据包	（ 游戏正在发牌状态 ）
struct GameStation_3 : GameStation_Base
{
	LLONG				iTotalNote[PLAY_COUNT];			//用户当前下注
	INT					iUserStation[PLAY_COUNT];		/**< 记录用户状态 */
	//运行状态变量
	INT					iUpGradePeople;					//庄家位置
	BYTE				iBeenPlayGame;					//已经游戏的局数
	//状态信息
	BYTE				iUserCardCount[PLAY_COUNT];		//用户手上的牌数
	BYTE				iUserCard[PLAY_COUNT][5];		//用户手上的牌
	GameStation_3()
	{
		memset(this,0,sizeof(GameStation_3));
	}
};

//游戏状态数据包	（ 游戏中状态 ）
struct GameStation_4 : GameStation_Base	
{
	//运行状态变量
	INT					iUpGradePeople;					//庄家位置
	INT					iOutCardPeople;					//现在出牌用户
	INT					iFirstOutPeople;				//先出牌的用户
	LLONG				iTotalNote[PLAY_COUNT];			//用户当前下注
	LLONG				iThisTurnNote[PLAY_COUNT];		//当前这轮下注
	
	BYTE				iBeenPlayGame;					//已经游戏的局数
	tagUserResult		tag;
	BYTE				iUserCardCount[PLAY_COUNT];		//用户手上的牌数
	BYTE				iUserCard[PLAY_COUNT][5];		//用户手上的牌
	
	INT				ibNetCutPlayer[PLAY_COUNT];	    //断线玩家
	INT					iUserStation[PLAY_COUNT];		/**< 记录用户状态 */
	BYTE				m_byUserAddNum[PLAY_COUNT];		//本轮各个玩家加注次数 2011-12-13
	GameStation_4()
	{
		memset(this,0,sizeof(GameStation_4));
	}
};

//游戏状态数据包	（ 等待下盘开始状态  ）
struct GameStation_5 : GameStation_Base
{
	INT					iUserStation[PLAY_COUNT];		/**< 记录用户状态 */
	bool				bAgree[PLAY_COUNT];				//玩家是否准备
	//运行状态变量
	INT					iUpGradePeople;					//庄家位置

	BYTE				iBeenPlayGame;					//已经游戏的局数
	GameStation_5()
	{
		memset(this,0,sizeof(GameStation_5));
	}
};
	/********************************************************************************/

	#pragma pack()

	// 数据包处理辅助标识

	//#ifdef ASS_UG_USER_SET
	//#undef ASS_UG_USER_SET
	//#endif

	//#ifdef ASS_GAME_BEGIN
	//#undef ASS_GAME_BEGIN
	//#endif

	//#ifdef ASS_SEND_CARD
	//#undef ASS_SEND_CARD
	//#endif

	//#ifdef ASS_SEND_FINISH
	//#undef ASS_SEND_FINISH
	//#endif
	//
	//#ifdef ASS_GAME_PLAY
	//#undef ASS_GAME_PLAY
	//#endif
	//
	//#ifdef ASS_AI_STATION
	//#undef ASS_AI_STATION
	//#endif
	//
	//#ifdef ASS_NEW_TURN
	//#undef ASS_NEW_TURN
	//#endif
	//
	//#ifdef ASS_CONTINUE_END
	//#undef ASS_CONTINUE_END
	//#endif
	//	
	//#ifdef ASS_NO_CONTINUE_END
	//#undef ASS_NO_CONTINUE_END
	//#endif
	//	
	//#ifdef ASS_CUT_END
	//#undef ASS_CUT_END
	//#endif
	//	
	//#ifdef ASS_AHEAD_END
	//#undef ASS_AHEAD_END
	//#endif
	//	
	//#ifdef ASS_HAVE_THING
	//#undef ASS_HAVE_THING
	//#endif
	//	
	//#ifdef ASS_LEFT_RESULT
	//#undef ASS_LEFT_RESULT
	//#endif
	//	
	//#ifdef ASS_MESSAGE
	//#undef ASS_MESSAGE
	//#endif

	enum STUD_MESSAGE
	{
		ASS_UG_USER_SET						= 50,				//用户设置游戏
		ASS_UG_USER_SET_CHANGE		= 51,				//玩家设置更改
		ASS_GAME_BEGIN						= 52,				//游戏开始

		ASS_SEND_CARD						= 53,				//发牌信息
		ASS_SEND_FINISH						= 54,				//发牌完成
		ASS_GAME_PLAY						= 55,				//开始游戏
		ASS_NOTE_LIMIE						= 56,				//本轮限注

		ASS_NOTE									= 60,				//下注
		ASS_VREB_CHECK						= 61,				//用户处理
		ASS_REPLACE_VREB_CHECK		= 62,				//代替(断线)用户处理
		ASS_NOTE_RESULT					= 63,				//下注结果

		ASS_COLLECT_JETON				= 70,				//收集筹码
		ASS_AI_STATION						= 71,				//机器人托管

		ASS_NEW_TURN						= 80,				//新一轮开始
		ASS_SEND_A_CARD					= 81,				//新发一张牌

		ASS_CONTINUE_END					= 90,				//游戏结束
		ASS_NO_CONTINUE_END			= 91,				//游戏结束
		ASS_CUT_END							= 92,				//用户强行离开
		ASS_SALE_END							= 93,				//游戏安全结束
		ASS_NO_WINER							= 94,				//游戏结束无胜出者
		ASS_AHEAD_END						= 95,				//提前结束

		ASS_HAVE_THING						= 100,
		ASS_LEFT_RESULT						= 101,
		ASS_MESSAGE							= 102,
		ASS_STOP_THING						= 103,			//请求提前结束
		ASS_AGREE_STOP						= 104,
		ASS_PLAY_FLASH						= 105,             //colin播放Flash动画
		ASS_BEGIN_FLASH						= 106,
		ASS_MONEY_NOT_ENOUTH		= 107,				//金币不足提示
	};

	enum CARD_TYPE
	{
		SH_ERROR_KIND								= 0,			   /**< 错误 */
		SH_OTHER										= 1,			   /**< 散牌 */
		SH_DOUBLE										= 2,			   /**< 对子 */
		SH_TWO_DOUBLE								= 3,			   /**< 两对 */
		SH_THREE											= 4,			   /**< 三条 */
		SH_SMALL_CONTINUE						= 5,			   /**< 最小顺子 */
		SH_CONTINUE									= 6,			   /**< 顺子 */
		SH_SAME_HUA									= 7,			   /**< 同花 */
		SH_HU_LU											= 8,			   /**< 葫芦 */
		SH_TIE_ZHI										= 9,			   /**< 铁支 */
		SH_SMALL_SAME_HUA_CONTINUE		= 10,			  /**< 同花最小顺子 */
		SH_SAME_HUA_CONTINUE				= 11        	  /**< 同花顺 */
	};	
}

#endif
