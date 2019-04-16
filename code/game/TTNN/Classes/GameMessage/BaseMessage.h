#ifndef __BASE_MESSAGE_H__
#define __BASE_MESSAGE_H__

#include <memory>
#pragma pack(push,1)

#ifndef __HN_BaseType_H__
typedef         long long LLONG;
typedef         unsigned char BYTE;
#endif

enum {
	MAX_NAME_INFO =                  256,

	// 下注区个数【本游戏 天 、地 、玄 、黄 四个下注区】.
	MAX_NOTE_COUNT = 4,
	// 保存路子把数.
	GAME_SAVE_COUNT = 72,
	// 系统做庄，庄家号
	SYSTEM_ZHANG_STATION = 190,
	// 游戏人数
	PLAY_COUNT = 180,
	//每组联合上庄最大数
	MAX_UION_COUNT = 10,
	//联合上庄区域数
	MAX_UION_AREA_COUNT = 10,

	// 版本定义
	GAME_MAX_VER = 1,                                                       // 现在最高版本
	GAME_LESS_VER = 1,                                              // 现在最低版本
	GAME_CHANGE_VER = 0,                                                    // 修改版本
	//游戏内部开发版本号
	DEV_HIGH_VERSION = 2,                                   // 内部开发高版本号(每修正一次LOW_VERSIONG+1)
	DEV_LOW_VERSION = 1,                                            // 内部开发低版本号

	// 游戏状态定义
	GS_WAIT_SETGAME = 0,                    // 等待东家设置状态
	GS_WAIT_AGREE = 1,              // 等待同意设置
	GS_FREE = 20,           // 空闲状态
	GS_BET = 21,            // 下注状态
	GS_SEND_CARD = 22,      // 发牌状态
	GS_PLAY_GAME = 23,      // 结算状态
	GS_WAIT_NEXT = 24,      // 等待下一盘开始

	//百变牛牛下注区域定义定义
	GF_TIANG = 0,              // 天
	GF_DI = 1, // 地
	GF_XUAN = 2,       // 玄
	GF_HUANG = 3,      // 黄

	// 有座位的玩家数.
	NUMBER_OF_PLAYER_SEAT = 12,

	/////////
	ASS_SUPER_SET_CARD = 79, /// 提牌器消息id，平台决定的游戏不能修改

	// 消息ID
	ASS_GAME_BEGIN = 111,           /// 游戏开始	DWJ
	ASS_KAI_PAI = 112,      // 开牌消息	DWJ
	ASS_RESULT = 113, ///结算		DWJ
	ASS_FREE_TIME = 114,    /// 空闲消息		DWJ
	ASS_WU_ZHUANG = 115,    /// 游戏无庄闲置消息消息	DWJ

	ASS_GET_NIU_TOU = 118,  //获取牛头消息

	ASS_IS_SUPER_USER = 120, //通知玩家是超端	DWJ
	ASS_SUPER_SET = 121,    //超端设置

	ASS_XIA_ZHU = 130,  /// 下注消息
	ASS_SHANG_ZHUANG = 131, /// 上庄消息		DWJ

	//增添联合上庄 LCL：2015-04-27
	ASS_SHANG_ZHUANG_UION = 132, //联合上庄消息
	ASS_SHANG_ZHUANG_UION_LIST = 133, //联合上庄显示列表

	ASS_NOT_XIAZHU = 153,                                                           //不能下注
	MDM_GR_PROP = 160,                                 //房间和游戏中道具相关的消息
	ASS_PROP_GIVE = 4,                                          //赠送道具

	//*---------------------------------------------------------------------------------------
	//客户端向服务端发送的消息
	ASS_USER_CHANG_CHAIR = 140, //玩家坐上椅子消息
	ASS_USER_STAND = 142, //玩家从座位上站起消息

	//*---------------------------------------------------------------------------------------
	//服务端向客户端发送的消息
	ASS_USER_CHANG_CHAIR_RESULT = 141, //玩家换椅子消息
	ASS_USER_STAND_RESULT = 143, //玩家从座位站起来消息

	//*---------------------------------------------------------------------------------------
    //与年兽活动冲突，修改
    ASS_NOTICE_UPDATE_SEAT = 145,//151,  //玩家离开通知更新座位上头像
    S_C_REACH_MAX_BET_LIMIT_SIG = 146,//152, //提前开牌
 
//  ASS_NOTICE_UPDATE_SEAT = 151,  //玩家离开通知更新座位上头像
//	S_C_REACH_MAX_BET_LIMIT_SIG = 152, //提前开牌
    
	S_C_XIAZUANG_NOT_ENOUPH_MONEY_SIG = 153, //下庄提示：金钱不足
	S_C_REACH_NT_NUM_LIMIT_SIG = 154, //达到上庄最大次数
	S_C_MORETHAN_HALF_LIMIT_SIG = 155, //超过上庄限制的一半
	S_C_MORETHAN_OTHERS_SIG = 156, //超过其他上庄的人的钱
	S_C_BEEN_ZUANG_SUCCESS_SIG = 157, //成为庄家成功
	S_C_UPDATE_ZUANG_NUM_SIG = 158, //更新庄把数
	S_C_UPDATE_BET_AREA_SIG = 159, //更新投注区域
	S_C_TIP_EXIT_MSG = 160 //提示退出游戏
};

/////////

//断线重连基础数据包
struct  TagGameState_Base {
	BYTE bySeatPosition[NUMBER_OF_PLAYER_SEAT];      //12个座位坐的情况
	BYTE gameState;                 //游戏状态
	BYTE bMaxBase;                          /// 最大倍率即牛牛的倍率

	int iXiaZhuTime;                ///下注时间
	int iKaiPaiTime;                ///开牌时间
	int iFreeTime;                          ///空闲时间
	int iRemaindTime;               ///剩余时间
	int iXiaZhuMax;                         ///总下注上限

	int curBankerStation;                   ///当前庄家的位置
	int iGameBeen;                          ///当前局数
	int iZhuangBaShu;               ///庄家进行了几把
	int iGameCount;                         ///当前已经进行了几把
	LLONG i64NtWin;                         ///庄家输赢成绩

	int iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];               ///最近X局的信息
	short int bankerList[PLAY_COUNT + 1][2];                        //上庄列表【第一列表示抢庄号,第二列表示是否抢庄】

	LLONG i64ShangZhuangLimit;                              ///上庄需要的最少金币
	LLONG i64MaxNote;                                                       ///个人游戏最大下注数
	LLONG i64RectNoteMax[MAX_NOTE_COUNT];           ///区域下注限制
	LLONG i64ChoumaValue[6];                                        // 六个筹码的值

	TagGameState_Base() {
		memset(this, 0, sizeof(TagGameState_Base));
	}
};
/*------------------------------------------------------------------------*/
//断线重连空闲阶段数据包
struct  TagGameState_WaiteAgee : public TagGameState_Base {

	TagGameState_WaiteAgee() {
		memset(this, 0, sizeof(TagGameState_WaiteAgee));
	}
};
/*------------------------------------------------------------------------*/
//断线重连下注阶段数据包
struct  TagGameState_Bet : public TagGameState_Base {
	LLONG totalBet;                                         ///本把当前总注额
	LLONG zoneBet[MAX_NOTE_COUNT];          ///本把每个区域下的注额
	LLONG zoneBetTrue[MAX_NOTE_COUNT]; ///本把真实每个区域下的注额
	LLONG myBet[MAX_NOTE_COUNT];            ///本把自已下注
	TagGameState_Bet() {
		memset(this, 0, sizeof(TagGameState_Bet));
	}
};
/*------------------------------------------------------------------------*/
//断线重连发牌阶段数据包
struct  TagGameState_OpenCard : public TagGameState_Bet {
	int iComputePoint[MAX_NOTE_COUNT];
	int cowValue[MAX_NOTE_COUNT + 1];

	BYTE byCard[5][5];
	BYTE byCardCount[5];

	//LLONG  iZhongZhu;						///本把当前总注额
	//LLONG  zoneBet[MAX_NOTE_COUNT];		///本把每个区域的下注额
	//LLONG  iQuYuZhuTrue[MAX_NOTE_COUNT];	///本把真实每个区域下的注额
	//LLONG  myBet[MAX_NOTE_COUNT];		///本把自已的下注额
	TagGameState_OpenCard() {
		memset(this, 0, sizeof(TagGameState_OpenCard));
	}
};

//断线重连结算阶段数据包
struct  TagGameState_ShowResult : public TagGameState_Bet {
	//LLONG  iZhongZhu;						///本把当前总注额
	//LLONG  iQuYuZhu[MAX_NOTE_COUNT];		///本把每个区域下的注额
	//LLONG  iQuYuZhuTrue[MAX_NOTE_COUNT];	///本把真实每个区域下的注额
	//LLONG  iMyZhu[MAX_NOTE_COUNT];		///本把自已下注

	BYTE byCard[5][5];
	BYTE byCardCount[5];

	LLONG iZhuangFen;       ///庄家的得分
	LLONG iXianFen;                 ///闲家的得分
	LLONG iUserFen;                 ///当前玩家的得分
	TagGameState_ShowResult() {
		memset(this, 0, sizeof(TagGameState_ShowResult));
	}
};

///游戏开始信息
typedef         struct tagBeginData {
	BYTE bySeatPosition[NUMBER_OF_PLAYER_SEAT];  //12个座位坐的情况
	int bUionBankerInfo[MAX_UION_COUNT][MAX_UION_COUNT];  //联合上庄列表，上庄区域，该上庄区域的玩家座位号
	LLONG iNTdata[4];       //庄家的位置,总分（当前金币数），成绩(赢的总金币)，局数（坐庄的局数）
	LLONG minMoneyOfApplyBanker;            ///申请庄家的最低金钱
	LLONG iQiangZhuangMoney; //20121116dwj 抢庄花费的金币
	LLONG maxBetOfZone;       ///区域能下的最大赌注
	LLONG i64ChoumaValue[6];        // 六个筹码的值

	short int iNTlist[PLAY_COUNT + 1][2];           //庄家列表位置 第一列表示庄家列表，第二列表示是否抢庄了

	int iGameBeen;    ///当前局数
	int iZhuangBaShu;  ///庄家进行了几把
	int iBankerSeatNo; ///当前庄家的座位号
	int iGameCount;   ///游戏次数
	int iXiaZhuTime; ///下注时间
	int iKaiPaiTime; ///开牌时间
	int iFreeTime;  ///空闲时间
	int iXiaZhuMax; ///本局总下注上限

	BYTE maxRate; //最大倍率即牛牛的倍率

	tagBeginData() {
		memset(this, 0, sizeof(tagBeginData));
		iBankerSeatNo = -1;
		memset(i64ChoumaValue, 0, sizeof(i64ChoumaValue));
		memset(bUionBankerInfo, -1, sizeof(bUionBankerInfo));
	}
} BEGIN_DATA;

///下注数据包
struct  TagXiaZhu {
	bool bIsAi;                     /// 是否机器人下注
	BYTE byStation;                 ///位置
	int iBetArea;                   ///下注区域1-4
	int iChouMaType;            //筹码类型

	int iMoney;                     //下注值
	LLONG iMaxZhu;          ///还可下最大注
	LLONG totalBet; ///本把当前总注额
	LLONG iXiaZhuMax;       ///下注最大值
	LLONG iUserXiaZhu[MAX_NOTE_COUNT + 1];                  //玩家区域下注
	LLONG zoneBet[MAX_NOTE_COUNT];                  ///本把每个区域下的注额
	LLONG zoneBetTrue[MAX_NOTE_COUNT];                      /// 真实区域注

	TagXiaZhu() {
		memset(this, 0, sizeof(TagXiaZhu));
	}
};

///开牌数据包
typedef         struct tagResult {
	LLONG iZhuangFen;              ///庄家的得分
	LLONG iXianFen;                         ///闲家的得分
	LLONG iUserFen;                         ///当前玩家的得分
	LLONG iNtWin;                           ///当前庄家赢的金币
	int iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];               //路子信息
	tagResult() {
		memset(this, 0, sizeof(tagResult));
	}
} RESULT;

///开牌数据包
typedef         struct tagKaiPai {
	BYTE pai[5][5];                         //牌数据
	int iCardCount[5];              //牌张数
	int iComputePoint[MAX_NOTE_COUNT];              ///区域倍率
	int cowValue[MAX_NOTE_COUNT + 1];               ///牛点数

	tagKaiPai() {
		memset(this, 0, sizeof(tagKaiPai));
		memset(pai, 255, sizeof(pai));
	}
} KAI_PAI;

///上庄数据包
typedef         struct tagShangZhuang {
	///申请的位置
	int station;
	bool shang;
	bool qiang;
	LLONG iQiangZhuangMoney;
	//20121119dwj 设置一个二维数组，第一列表示抢庄号,第二列表示是否抢庄;
	short int zhuangState[PLAY_COUNT + 1][2];
	///是否成功
	bool success;
	/// 是否机器人
	bool bIsAi;
	tagShangZhuang() {
		memset(this, 0, sizeof(tagShangZhuang));
		memset(zhuangState, -1, sizeof(zhuangState));
	}
} SHANG_ZHUANG;


//联合上庄数据包
typedef struct tagShangZhuangUion {
	bool shang;                  //申请上下庄
	bool qiang;
	bool success;                     ///是否失败
	bool bIsAi;                  //是否是机器人
	BYTE currentStation;                //当前玩家位置
	int uionarea;                //联合上庄区域
	int count;                    //联合上各区域人数
	int iQiangZhuangMoney;
	int station[MAX_UION_COUNT]; //申请联合上庄位置
	int iMoney[MAX_UION_COUNT];   //联合上投入的钱

	tagShangZhuangUion() {
		shang = false;
		success = false;
		bIsAi = false;
		uionarea = -1;
		count = 0;
		memset(this, 0, sizeof(tagShangZhuangUion));
		memset(station, -1, sizeof(station));
		memset(iMoney, -1, sizeof(iMoney));
	}
} SHANG_ZHUANG_UION;

//联合上庄列表庄家
typedef struct tagShangZhuangUionList {
	int bUionBankerInfo[MAX_UION_COUNT][MAX_UION_COUNT];    //联合上庄信息，上庄区域，该上庄区域的玩家座位号
	tagShangZhuangUionList() {
		memset(bUionBankerInfo, -1, sizeof(bUionBankerInfo));
	}
} SHANG_ZHUANG_UION_LIST;

///换庄
typedef struct tagChangeZhuang {
	///上一个庄家
	int bLastStation;
	/// 当前庄家
	int bNowStation;
	// 设置一个二维数组，第一列表示抢庄号,第二列表示是否抢庄;
	short int dzhuangstation[PLAY_COUNT + 1][2];
	tagChangeZhuang() {
		memset(this, 0, sizeof(tagChangeZhuang));
		memset(dzhuangstation, -1, sizeof(dzhuangstation));
	}

} CHANGE_ZHUANG;

///等待消息数据包˛˛
typedef struct tagDengDai {
	int bUionBankerInfo[MAX_UION_AREA_COUNT][10];
	//设置一个二维数组，第一列表示抢庄号,第二列表示是否抢庄;
	short int zhuangState[PLAY_COUNT + 1][2];
	int iZhuangBaShu;//庄家进行了几把
	int iZhuang;//庄家位置
	long long iNtWin; //当前庄家赢的金币
	tagDengDai() {
		memset(zhuangState, -1, sizeof(zhuangState));
		memset(bUionBankerInfo, -1, sizeof(bUionBankerInfo));
		iNtWin = 0;      //当前庄家赢的金币
		iZhuangBaShu = 0; //庄家进行了几把
		iZhuang = -1;
	}
} DENG_DAI;

///测试数据包
typedef struct tagTest {
	///测试类型
	int type;
	tagTest() {
		memset(this, 0, sizeof(tagTest));
	}

} BJL_TEST;

//控制输赢数据包
struct CONTROL_WIN {
	BYTE bGameWinState; ///赢的位置，0-4 代表，庄位，天，地，玄，黄
	BYTE bGameLoseState; ///输的位置，0-4 代表，庄位，天，地，玄，黄
	BYTE bGameWinQueue[MAX_NOTE_COUNT];///天地玄黄名家控制的输赢0-3代表天，地，玄，黄，0代表输。1代表赢 255表示不控制
	CONTROL_WIN() {
		memset((void *)this, 0, sizeof(CONTROL_WIN));
		memset(bGameWinQueue, 255, sizeof(bGameWinQueue));
	}
};

///超端数据包
struct  TagSuperUser {
	BYTE byStation;                                 ///位置
	bool bIsSuper;              /**< 是否开通 */

	TagSuperUser() {
		byStation = -1;
		bIsSuper = false;
	}
};

//超端设置数据包
struct  TagSuperSet {
	BYTE byStation;                                 ///位置
	bool bSuccesse;                                 //设置是否成功
	int iAreaContrl[4];                     ///区域控制天地玄黄  -1表示输 0-表示不控制 1表示赢

	TagSuperSet() {
		memset(this, 0, sizeof(TagSuperSet));
		byStation = 255;
		bSuccesse = false;
	}

	void init() {
		memset(this, 0, sizeof(TagSuperSet));
		byStation = 255;
		bSuccesse = false;
	}
};

//获取牛头数据包
struct TagGetNiuTou {
	BYTE byStation;          //玩家位置
	BYTE byCount;            //牛头个数
	int iPlayerID;           //玩家ID号
	TagGetNiuTou() {
		byStation = 255;
		byCount = 0;
	}
};

//换座数据包
struct C_S_Chang_Position {
	BYTE byLastPosition;    //原来的位置
	BYTE byPosition;        //要坐下椅子的位置

	C_S_Chang_Position() {
		byLastPosition = 255;
		byPosition = 255;
	}
};

//换座数据包结果
struct S_C_Chang_Position_Result {
	//坐下座位是否成功：TRUE:成功   FALSE:失败
	bool bFlag;
	//玩家本身的桌子号
	BYTE byStation;
	//玩家上次的椅子位置
	BYTE byLastSeatPosition;
	//玩家要坐的椅子位置
	BYTE bySeatPosition;

	S_C_Chang_Position_Result() {
		memset(this, 255, sizeof(S_C_Chang_Position_Result));
		bFlag = false;
	}
};

//坐座位上站起来数据包
struct C_S_Leave_Seat {
	BYTE byPosition;        //玩家站起座位的位置
	C_S_Leave_Seat() {
		byPosition = 255;
	}
};

//坐座位上站起来数据包结果
struct S_C_Leave_Seat_Result {
	bool bFlag;               //站起是否成功  成功：TRUE  失败:FALSE
	BYTE byStation;          //玩家本身位置号
	BYTE bySeatPosition;     //玩家站起的座位位置
	S_C_Leave_Seat_Result() {
		memset(this, 255, sizeof(S_C_Leave_Seat_Result));
		bFlag = false;
	}
};

//成为庄家
struct S_C_REACH_MAX_BET_LIMIT {
	BYTE byDeskStation;
	S_C_REACH_MAX_BET_LIMIT() {
		memset(this, 0, sizeof(S_C_REACH_MAX_BET_LIMIT));
	}
};
//庄家钱少于上庄限制的一半
struct S_C_XIAZUANG_NOT_ENOUPH_MONEY {
	BYTE bDeskStation;
	LLONG i64LimitMoney;
	S_C_XIAZUANG_NOT_ENOUPH_MONEY() {
		memset(this, 0, sizeof(S_C_XIAZUANG_NOT_ENOUPH_MONEY));
	}
};

//庄家坐庄次数达到上庄次数
struct S_C_REACH_NT_NUM_LIMIT {
	BYTE bDeskStation;
	int iLimitNum;
	S_C_REACH_NT_NUM_LIMIT() {
		memset(this, 0, sizeof(S_C_REACH_NT_NUM_LIMIT));
	}
};

//庄家金币超出上庄限制的一半
struct S_C_MORETHAN_HALF_LIMIT {
	BYTE byDeskStation;
	int iExtralCount;
	LLONG i64Limit;
	S_C_MORETHAN_HALF_LIMIT() {
		memset(this, 0, sizeof(S_C_MORETHAN_HALF_LIMIT));
	}
};
//庄家金币超出其他申请上庄的玩家，并且超过上庄限制
struct S_C_MORETHAN_OTHERS {
	BYTE byDeskStation;
	int iExtralCount;
	LLONG i64Limit;
	S_C_MORETHAN_OTHERS() {
		memset(this, 0, sizeof(S_C_MORETHAN_OTHERS));
	}
};

//成为庄家
struct S_C_BEEN_ZUANG_SUCCESS {
	BYTE byDeskStation;
	S_C_BEEN_ZUANG_SUCCESS() {
		memset(this, 0, sizeof(S_C_BEEN_ZUANG_SUCCESS));
	}
};

//更新庄把数
struct S_C_UPDATE_ZUANG_NUM {
	int iZuangBaShu;
	S_C_UPDATE_ZUANG_NUM() {
		memset(this, 0, sizeof(S_C_UPDATE_ZUANG_NUM));
	}
};

struct S_C_UPDATE_BET_AREA {
	BYTE byBetArea;
	S_C_UPDATE_BET_AREA() {
		memset(this, 0, sizeof(S_C_UPDATE_BET_AREA));
	}
};

// 提醒退出游戏【可能因为金钱不足】.
struct S_C_TIP_EXIT {
	// 是否退出.
	bool bIsExit;
	long long i64Money;
	S_C_TIP_EXIT() {
		bIsExit = false;
		i64Money = 0;
	}
};

#pragma pack(pop)

#endif//__BASE_MESSAGE_H__
