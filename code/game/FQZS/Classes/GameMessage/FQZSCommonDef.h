#ifndef __FQZS_COMMON_DEFINE_h__
#define __FQZS_COMMON_DEFINE_h__

#include <memory.h>
#include "HNNetExport.h"
#include <vector>
#pragma pack(push,1)

namespace FQZS
{
#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)
 

#define GAME_NOTE_COUNT 11
#define Rate_Count 14
#define Bet_Count 5
	//////////////////////////////////////////////////////////////////////////
    //游戏信息
    enum
    {
        GAME_NAME_ID	=	31000500,
        GAME_KIND_ID	=	1,
		PLAYER_COUNT	= 180,
		Open_Area		= 28,	//开区域
	};
   
	enum
	{
		GS_FREE = 0,	//等待东家设置状态
		GS_XIAZHU = 20,			//下注状态
		GS_XIAZHU_FINISH = 21,	//下注结束
		GS_KAIJIANG = 22,		//开奖阶段
		GS_GAME_FINISH = 23,	//游戏结束
	};
    
	//s2c
	enum
	{
		ASS_BEGIN = 55,
		ASS_XIAZHU = 56,		 //XIAZHU   
		ASS_PLAY_GAME = 57,		 //	开始转盘
		ASS_SHOW_RESULT = 58,	 // 显示转盘结果
		ASS_NEWTURN_START = 59,	 //开始新一局       
		ASS_CONTINUE_END = 60,
		ASS_XIAZHU_RESULT = 61,
		S2C_TIP_EXIT_MSG = 72,
		//S2C_NOTIFY_MSG=149,
	};

	enum
	{
		CLIENT_TIMER_FREE,							//空闲时间
		CLIENT_TIMER_BET,							//下注时间
		CLIENT_TIMER_KAIJIANG,						//开奖
		CLIENT_TIMER_KAIJIANG_RESULT,				//开奖结果展示
		CLIENT_TIMER_OVER,
	};

	enum {
		WAIT_LE_SET = 0,
		DONE_LE_MONEY = 1,
		DONE_LE_BOMB = 2
	};
	enum
	{
		LEState_Clear,	//红包安全
		LEState_Bomb,	//红包有炸弹
		LEState_UnOpen,	//红包未拆
		LEState_Open,	//红包已拆
	};

	enum Kind
	{
		//飞禽
		K_FQ = 0,	//飞禽
		K_YZ = 1,	//燕子
		K_GZ = 2,	//鸽子
		K_KQ = 3,	//孔雀
		K_LY = 4,	//老鹰
		//走兽
		K_ZS = 5,	//走兽
		K_SZ = 6,	//狮子
		K_XM = 7,	//熊猫
		K_HZ = 8,	//猴子
		K_TZ = 9,	//兔子
		//
		K_SY = 10,	//鲨鱼，银鲨
		K_JS = 11,	//金鲨
		K_TS = 12,	//通杀
		K_TP = 13,	//通赔
		k_XT = 14,  //续投
		k_QK = 15,	//清空
	};

	static BYTE Area_Kind[Open_Area] = {
		K_JS,				//金鲨
		K_YZ, K_YZ, K_YZ,	//燕子
		K_GZ, K_GZ, K_GZ,	//鸽子
		K_TS,				//通杀
		K_KQ, K_KQ, K_KQ,	//孔雀
		K_LY, K_LY, K_LY,	//老鹰
		K_SY,				//鲨鱼
		K_SZ, K_SZ, K_SZ,	//狮子
		K_XM, K_XM, K_XM,	//熊猫
		K_TP,				//通赔
		K_HZ, K_HZ, K_HZ,	//猴子
		K_TZ, K_TZ, K_TZ,	//兔子
	};

	//仅用于记录已开过的动物类型
	static BYTE Open_Kind[12] = {
		K_YZ,	//燕子
		K_GZ,	//鸽子
		K_KQ,	//孔雀
		K_LY,	//老鹰
		K_SZ,	//狮子
		K_XM,	//熊猫
		K_HZ,	//猴子
		K_TZ,	//兔子
		K_SY,	//鲨鱼，银鲨
		K_JS,	//金鲨
		K_TS,	//通杀
		K_TP,	//通赔
	};

	enum Em_TableBtnTag {
		em_toolbarBtn = 1000,
		em_toolbarChangeTableBtn,
		em_toolbarSetBtn,
		em_toolbarHelpBtn,
		em_toolbarStandupBtn,
		em_toolbarLeaveBtn,
		em_toolbarNextOutBtn,
		em_toolbarNextOutLight,
		em_shopBtn,
		em_emotionBtn,
		em_noSeatBtn,
		em_openBank,
		em_chatBtn,
		em_chatOpenBtn,
		em_makerBtn,
		em_uionmakerBtn,
		em_repeatBetBtn,
		em_cancelRepeatBetBtn
	};

	enum {
		Bet_Area_Num = 11,	//下注界面按钮数量（不包含取消与续投）
		Open_Animal_Num = 10,	//服务端开出的动物总数量为12种
	};
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////S2C消息结构体
	//////////////////////////////////////////////////////////////////////////

	//没钱退出
	struct le_s2c_tip_exit
	{
		BYTE	bIsExit;
		long long	i64Money;
		le_s2c_tip_exit()
		{
			bIsExit = INVALID_BYTE;
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
	////////////////////////////S2C消息结构体
	//////////////////////////////////////////////////////////////////////////
	//游戏开始
	struct GameBeginStruct
	{
		long long			i64MyMoney;					/** 自己金币 */
		long long			i64SingleNoteLimit;			//单个个人下注限额 
		int					imultiplying[Rate_Count];   //倍率
		int					itime;

		GameBeginStruct()
		{
			i64MyMoney = 0;
			i64SingleNoteLimit = 0;
			memset(&imultiplying, 0, sizeof(imultiplying));
			itime = 0;
		}
	};

	//通知下注
	struct GameNote
	{
		int iTime;
		int iPrevHasNote;
		GameNote()
		{
			iTime = 0;
			iPrevHasNote = 0;
		}
	};

	//下注结果
	struct GameXiaZhuResponse
	{
		int			iType;									//类型 0飞禽 1燕子 2鸽子 3孔雀 4老鹰 5走兽 6狮子 7熊猫 8猴子 9兔子 10鲨鱼，银鲨 14续投 15清空
		long long	iOwnAllCountNote;					// 每个玩家自己下注总数（总押分）
		BYTE		bDeskStation;							// 玩家座位号
		long long	iOwnNotes[GAME_NOTE_COUNT];			///自己每个框下注数
		long long	iNoteNum;							//单次下注值
		long long	iUserMoney;
		long long	iNotebyregion[GAME_NOTE_COUNT];	//单个区域总的下注
		GameXiaZhuResponse()
		{
			iType = -1;
			iOwnAllCountNote = 0;
			bDeskStation = BYTE(-1);
			memset(iOwnNotes, 0, sizeof(iOwnNotes));
			iNoteNum = 0;
			iUserMoney = 0;
			memset(iNotebyregion, 0, sizeof(iNotebyregion));
		}
	};

	//开奖
	struct GamePlay
	{
		int			iStarPos;       ///开始位置
		int			iEndPos;        ///结束位置
		long long	i64Winnings;		//彩金
		int			iEndAnimal;
		bool		iIsShedeng;
		int			iTime;

		GamePlay()
		{
			iStarPos = -1;
			iEndPos = -1;
			i64Winnings = 0;
			iEndAnimal = -1;
			iIsShedeng = -1;
			iTime = 0;
		}
	};

	///发送转动结果消息
	struct GamePlayResult
	{
		int iEndPos;         ///结束位置
		long long i64Winnings;		//彩金
		int iEndAnimal;
		int bIsShark;		//射灯之前是金鲨还是银鲨（0 --都不是，1--金鲨）
		int iTime;

		GamePlayResult()
		{
			iEndPos = -1;
			i64Winnings = 0;
			iEndAnimal = -1;
			bIsShark = -1;
			iTime = 0;
		}
	};


	struct GameEndMsg	///发送结算结果消息
	{
		int		        iHistory[10];	           //路子
		long long       i64RealWinMoney;            //输赢
		long long		i64UserMoney;				//用户钱

		GameEndMsg()
		{
			memset(iHistory, 0, sizeof(iHistory));
			i64RealWinMoney = 0;
			i64UserMoney = 0;
		}
	};
	//C2S/////////////////////////////////////
	struct GameXiaZhuRequest
	{
		BYTE		bBetType;
		long long	i64BetMoney;
		GameXiaZhuRequest()
		{
			bBetType = BYTE(-1);
			i64BetMoney = 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	/////////////////////重连包
	//////////////////////////////////////////////////////////////////////////
	//公共数据包
	struct GameStation_Base
	{
		BYTE		bGameStation;					/**<游戏状态*/
		long long   i64MyMoney;						//钱
		long long	i64SingleNoteLimit;				//单个个人下注限额 
		int         imultiplying[Rate_Count];       //倍率
		int			itime;							//时间
        long long   i64Winning;						//彩金
		int         iHistory[10];					//路子

		GameStation_Base()
		{
			bGameStation = GS_FREE;
			i64MyMoney = 0;
			i64SingleNoteLimit = 0;
			memset(&imultiplying, 0, sizeof(imultiplying));
			itime = 0;
            i64Winning = 0;
			memset(&iHistory, 0, sizeof(iHistory));
		}
	};

	//下注阶段
	struct GameStation_Bet :GameStation_Base
	{
		long long   i64OwnNotes[GAME_NOTE_COUNT];    //区域下注
		long long	i64AreaNotes[GAME_NOTE_COUNT];	//总得区域下注
		GameStation_Bet()
		{
			GameStation_Base();
			memset(i64OwnNotes, 0, sizeof(i64OwnNotes));
			memset(i64AreaNotes, 0, sizeof(i64AreaNotes));
		}
	};

	//开奖阶段
	struct GameStation_Kaijiang :GameStation_Base
	{
		long long   i64OwnNotes[GAME_NOTE_COUNT];		//区域下注
		long long	i64AreaNotes[GAME_NOTE_COUNT];		//总得区域下注
		int         iStarPos;							///开始位置
		int         iEndPos;							///结束位置
		int         iEndAnimal;
//		long long   i64Winning;							//彩金
		GameStation_Kaijiang()
		{
			GameStation_Base();
			memset(i64OwnNotes, 0, sizeof(i64OwnNotes));
			memset(i64AreaNotes, 0, sizeof(i64AreaNotes));
			iStarPos = -1;
			iEndPos = -1;
			iEndAnimal = -1;
//			i64Winning = 0;
		}
	};
}
#pragma pack(pop)
#endif
