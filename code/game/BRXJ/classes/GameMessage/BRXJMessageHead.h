
#ifndef  __BRXJ_MESSAGEHEAD_H__
#define __BRXJ_MESSAGEHEAD_H__

#include <memory.h>
#include "HNNetExport.h"
#pragma pack(1)
namespace BRXJ
{

	enum COMMOND
	{
		//游戏信息
		GAME_KIND_ID								= 1,
		GAME_NAME_ID							= 11100600,				 // 名字 ID
		PLAY_COUNT								= 180,						// 游戏人数


		//游戏内部开发版本号
		DEV_HIGH_VERSION						= 2,							// 内部开发高版本号(每修正一次LOW_VERSIONG+1)
		DEV_LOW_VERSION						= 0,							// 内部开发低版本号

		//庄家索引
		BANKER_INDEX								= 0,							//庄家索引
		SHUN_MEN_INDEX						= 1,							//顺门索引
		DUI_MEN_INDEX							= 2,							//对门索引
		DAO_MEN_INDEX							= 3,							//倒门索引
		MAX_INDEX									= 3,							//最大索引

		//游戏内
		MAX_NOTE_COUNT						= 3,							//最大下注区域
		USER_HAND_CARD_COUNT			= 2,							//用户手中的牌
		GAME_XIPAI_COUNT						= 72,							//一次洗牌次数
		GAME_SAVE_COUNT						= 72,							//保存路子把数

		//消息ID
		ASS_XIA_ZHU_FINISH						= 32,							//下注结束
		ASS_NO_XIA_ZHU							= 33,							//禁止下注
		ASS_SEND_CARD_FINISH				= 34,							//发牌结束
		ASS_SHOW_CARD							= 35,							//显示发牌结果
		ASS_NEW_TURN							= 36,							//开始新一轮

		S_C_IS_SUPER_USER						= 78,							//超端用户消息
		C_S_SUPER_SET								= 79,							//超端设置
		S_C_SUPER_SET_RESULT					= 80,							//超端设置结果

		C_S_APPLY_ZHUANG						= 90,							//申请上下庄
		S_C_APPLY_ZHUANG_RESULT			= 91,							//申请上下庄结果
		C_S_XIA_ZHU									= 92,							//玩家下注
		S_C_XIA_ZHU_RESULT					= 93,							//玩家下注结果

		S_C_NO_NT_WAITE						= 100,						//通知无庄等待
		S_C_NOTICE_XIA_ZHU					= 101,						//通知下注消息
		S_C_SEND_CARD							= 102,						//发牌消息
		S_C_NO_XIAZHU							= 103,						//禁止下注
		S_C_SHOW_WINAREA					= 104,						//显示中奖区域消息
		S_C_GAME_END							= 105,						//游戏结束
		S2C_TIP_EXIT_MSG						= 106,						//强制退出
		// 游戏状态定义
		GS_WAIT_SETGAME						= 0,							// 无庄等待
		GS_WAIT_AGREE							= 1,							// 等待同意设置
		GS_NOTE_STATE							= 20,							// 下注状态
		GS_SEND_CARD							= 21,							// 发牌状态
		GS_SHOW_WIN								= 22,							// 显示中奖区域
		GS_WAIT_NEXT								= 23							// 等待下一盘开始

	};



	struct	TGameLuzi
	{
		int		iShangMen;
		int		iTianMen;
		int		iXiaMen;
		TGameLuzi()
		{
			memset(this, 0, sizeof(TGameLuzi));
		}
	};
	/*--------------------------------------------------------------------------*/
	//游戏状基础态包
	struct TGameStationBase
	{
		BYTE			byZhuangList[PLAY_COUNT];					 //上庄列表	
		int				iXiaZhuTime;											 // 下注时间					
		int				iKaiPaiTime;     										 // 开牌时间    
		int				iFreeTime;												 // 空闲时间	
		int				iShowWinTime;										 // 显示中奖时间
		int				iRemaindTime;										 // 阶段剩余时间
																					 
		int				iNtStation;												 //庄家位置
		int				iNtPlayCount;											 //庄家坐庄次数
		LLONG		i64NtMoney;											 //庄家金币
		LLONG		i64NtWinMoney;									 //庄家输赢情况
																					 
		LLONG		i64UserWin;											 //个人输赢
		LLONG		i64MyMoney;										 //个人金币数 -从服务端发送过去
																					 
		LLONG		i64UserMaxNote;									 //玩家最大下注数
		LLONG		i64ShangZhuangLimit;							 // 上庄需要的最少金币	
		LLONG		i64ChouMaValues[6];								 //6个筹码值

		LLONG     i64RemainBetNum;									 //剩余最大下注数
		TGameLuzi	tGameLuzi[GAME_SAVE_COUNT];		 //路子信息


		TGameStationBase()
		{
			memset(this, 0, sizeof(TGameStationBase));
			memset(byZhuangList, 255, sizeof(byZhuangList));
		}
	};

	/*--------------------------------------------------------------------------*/
	//游戏下注状态包
	struct TGameStationBase_Note
	{
		BYTE			byZhuangList[PLAY_COUNT];							//上庄列表	
		int				iXiaZhuTime;													// 下注时间					
		int				iKaiPaiTime;     												// 开牌时间    
		int				iFreeTime;														// 空闲时间	
		int				iShowWinTime;												// 显示中奖时间
		int				iRemaindTime;												// 阶段剩余时间

		int				iNtStation;														//庄家位置
		int				iNtPlayCount;													//庄家坐庄次数
		LLONG		i64NtMoney;													//庄家金币
		LLONG		i64NtWinMoney;											//庄家输赢情况

		LLONG		i64UserWin;													//个人输赢
		LLONG		i64MyMoney;												//个人金币数 -从服务端发送过去

		LLONG		i64UserMaxNote;											//玩家最大下注数
		LLONG		i64ShangZhuangLimit;									// 上庄需要的最少金币	
		LLONG		i64ChouMaValues[6];										//6个筹码值

		LLONG		i64QuYuZhuTrue[MAX_NOTE_COUNT];			//真实玩家的下注值
		LLONG		i64QuYuZhu[MAX_NOTE_COUNT];					// 本把每个区域下的注额
		LLONG     i64UserXiaZhu[MAX_NOTE_COUNT];				//玩家下注情况	

		LLONG     i64RemainBetNum;											//剩余最大下注数
		TGameLuzi	tGameLuzi[GAME_SAVE_COUNT];				//路子信息


		TGameStationBase_Note()
		{
			memset(this, 0, sizeof(TGameStationBase_Note));
			memset(byZhuangList, 255, sizeof(byZhuangList));
		}
	};
	/*--------------------------------------------------------------------------*/
	//游戏发牌状态包
	struct TGameStationBase_SendCard
	{
		BYTE		byZhuangList[PLAY_COUNT];								//上庄列表	
		BYTE		byUserCard[4][2];												//牌数据				
		BYTE		byCardCount[4];												//牌张数
		int			iCardShape[4];													//牌型点数
		int			iXiaZhuTime;														// 下注时间					
		int			iKaiPaiTime;     													// 开牌时间    
		int			iFreeTime;															// 空闲时间	
		int			iShowWinTime;													// 显示中奖时间
		int			iRemaindTime;													// 阶段剩余时间

		int				iNtStation;														//庄家位置
		int				iNtPlayCount;													//庄家坐庄次数
		LLONG		i64NtMoney;													//庄家金币
		LLONG		i64NtWinMoney;											//庄家输赢情况

		LLONG		i64UserWin;													//个人输赢
		LLONG		i64MyMoney;												//个人金币数 -从服务端发送过去

		LLONG		i64UserMaxNote;											//玩家最大下注数
		LLONG		i64ShangZhuangLimit;									// 上庄需要的最少金币	
		LLONG		i64ChouMaValues[6];										//6个筹码值

		LLONG		i64QuYuZhuTrue[MAX_NOTE_COUNT];			//真实玩家的下注值
		LLONG		i64QuYuZhu[MAX_NOTE_COUNT];					// 本把每个区域下的注额
		LLONG     i64UserXiaZhu[MAX_NOTE_COUNT];				//玩家下注情况	

		LLONG     i64RemainBetNum;											//剩余最大下注数

		TGameLuzi	tGameLuzi[GAME_SAVE_COUNT];				//路子信息


		TGameStationBase_SendCard()
		{
			memset(this, 0, sizeof(TGameStationBase_SendCard));
			memset(byZhuangList, 255, sizeof(byZhuangList));
		}
	};
	/*--------------------------------------------------------------------------*/
	//游戏显示中奖区域状态包
	struct TGameStationBase_ShowWin
	{
		BYTE		byZhuangList[PLAY_COUNT];							//上庄列表	
		BYTE		byUserCard[4][2];											//牌数据				
		BYTE		byCardCount[4];											//牌张数
		int			iCardShape[4];												//牌型点数
		int			iCompareResult[4];										//中奖区域
		int			iXiaZhuTime;													// 下注时间					
		int			iKaiPaiTime;     												// 开牌时间    
		int			iFreeTime;														// 空闲时间	
		int			iShowWinTime;												// 显示中奖时间
		int			iRemaindTime;												// 阶段剩余时间

		int				iNtStation;													//庄家位置
		int				iNtPlayCount;												//庄家坐庄次数
		LLONG		i64NtMoney;												//庄家金币
		LLONG		i64NtWinMoney;										//庄家输赢情况

		LLONG		i64UserWin;												//个人输赢
		LLONG		i64MyMoney;											//个人金币数 -从服务端发送过去

		LLONG		i64UserMaxNote;										 //玩家最大下注数
		LLONG		i64ShangZhuangLimit;								 // 上庄需要的最少金币	
		LLONG		i64ChouMaValues[6];									 //6个筹码值

		LLONG		i64QuYuZhuTrue[MAX_NOTE_COUNT];		 //真实玩家的下注值
		LLONG		i64QuYuZhu[MAX_NOTE_COUNT];				 // 本把每个区域下的注额
		LLONG     i64UserXiaZhu[MAX_NOTE_COUNT];			 //玩家下注情况	
																						 
		LLONG     i64RemainBetNum;										 //剩余最大下注数
		TGameLuzi	tGameLuzi[GAME_SAVE_COUNT];			 //路子信息


		TGameStationBase_ShowWin()
		{
			memset(this, 0, sizeof(TGameStationBase_ShowWin));
			memset(byZhuangList, 255, sizeof(byZhuangList));
		}
	};
	/*--------------------------------------------------------------------*/
	struct	C_S_UserNote
	{
		BYTE	byDeskStation;
		int		iChouMaType;												//筹码类型
		int		iNoteArea;														//下注区域
		C_S_UserNote()
		{
			byDeskStation = -1;
			iChouMaType = -1;
			iNoteArea = -1;

		}
	};
	/*--------------------------------------------------------------------*/
	struct	C_S_UserNoteResult
	{
		int				iArea;
		LLONG		i64Money;
		LLONG		i64ZhongZhu;												// 本把当前总注额
		LLONG		i64QuYuZhuTrue[MAX_NOTE_COUNT];			//真实玩家的下注值
		LLONG		i64QuYuZhu[MAX_NOTE_COUNT];					// 本把每个区域下的注额
		LLONG     i64UserXiaZhuData[MAX_NOTE_COUNT];		//玩家下注情况

		LLONG     i64RemainBetNum;        //剩余最大下注数
		C_S_UserNoteResult()
		{
			memset(this, 0, sizeof(C_S_UserNoteResult));
		}
	};
	/*--------------------------------------------------------------------*/
	struct	S_C_GameResult
	{
		BYTE			byZhuangList[PLAY_COUNT];							//上庄列表
		LLONG		i64NtScore;													//庄家得分
		LLONG		i64UserScore;												//闲家得分

		LLONG		i64NtWin;														//庄家输赢成绩
		LLONG		i64NtMoney;													//庄家的金币数量

		LLONG		i64UserWin;													//玩家输赢
		LLONG		i64UserMoney;												//玩家自身的金币

		TGameLuzi	tGameLuzi[GAME_SAVE_COUNT];				//路子信息	
		S_C_GameResult()
		{
			memset(this, 0, sizeof(S_C_GameResult));
		}
	};
	/*--------------------------------------------------------------------*/
	//无庄等待消息
	struct	S_C_NoNtStation
	{
		int			iNowNtStation;										///当前庄家的位置
		int			iNtPlayCount;											//庄家坐庄次数
		LLONG		i64NtMoney;										//庄家金币
		LLONG		i64NtWinMoney;								//庄家输赢情况
		S_C_NoNtStation()
		{
			iNowNtStation = -1;										//当前庄家的位置
			iNtPlayCount = 0;											//庄家坐庄次数
			i64NtMoney = 0;											//庄家金币
			i64NtWinMoney = 0;										//庄家输赢情况
		}
	};
	/*--------------------------------------------------------------------------*/
	///游戏开始信息
	struct S_C_BeginData
	{
		BYTE			byZhuangList[PLAY_COUNT];				//上庄列表

		int				iGameCount;										//当前已经进行了几把
		int				iGameBeen;										//当前局数
		int				iNowNtStation;									//当前庄家的位置
		int				iNtPlayCount;										//庄家坐庄次数
		LLONG		i64NtMoney;										//庄家金币
		LLONG		i64NtWinMoney;								//庄家输赢情况
		LLONG		i64UserMoney;									//玩家自身的金币数量

		LLONG		i64PerMaxNotes;								// 本局最大下注总注
		LLONG     i64RemainBetNum;								//剩余最大下注数
		S_C_BeginData()
		{
			memset(this, 0, sizeof(S_C_BeginData));
			iNowNtStation = -1;
			memset(byZhuangList, 255, sizeof(byZhuangList));
		}
	};
	/*---------------------------------------------------------------*/
	//发牌数据包
	struct S_C_SendCard
	{
		int		iKaiPaiTime;					//开牌时间
		BYTE	byUserCard[4][2];
		BYTE	byCardCount[4];
		int		iCardShape[4];				//牌型点数
		S_C_SendCard()
		{
			memset(this, 0, sizeof(S_C_SendCard));
		}
	};
	/*---------------------------------------------------------------*/
	//发牌数据包
	struct S_C_NoXiaZhu
	{
		bool	bCanXiaZhu;					//是否可以下注
		S_C_NoXiaZhu()
		{
			memset(this, 0, sizeof(S_C_NoXiaZhu));
		}
	};
	/*--------------------------------------------------------------------------*/
	//中奖区域数据包
	struct S_C_ShowWinAreas
	{
		int		iCompareResult[4];		//中奖区域
		S_C_ShowWinAreas()
		{
			memset(this, 0, sizeof(S_C_ShowWinAreas));
		}
	};
	/*--------------------------------------------------------------------------*/
	//申请上庄数据包
	struct C_S_ApplyShangZhuang
	{
		bool	bShangZhuang;
		C_S_ApplyShangZhuang()
		{
			memset(this, 0, sizeof(C_S_ApplyShangZhuang));
		}
	};
	/*--------------------------------------------------------------------------*/
	//申请上庄数据包
	struct S_C_ApplyZhuangResult
	{
		bool			bXiaZhuang;										//是否庄家申请下庄
		BYTE			bIsSuccess;										//申请上庄是否成功（0成功，1已在庄家列表，2金币不够)
		BYTE			byDeskStation;									//申请的玩家
		BYTE			byZhuangList[PLAY_COUNT];
		int				iNowNtStation;									//庄家位置
		int				iZhuangBaShu;									//庄家把数
		LLONG		i64NtMoney;										//玩家金币数量
		LLONG		i64NtWin;											//当前庄家赢的金币 

		S_C_ApplyZhuangResult()
		{
			memset(byZhuangList, 255, sizeof(byZhuangList));
			iNowNtStation = -1;
			iZhuangBaShu = 0;
			i64NtMoney = 0;
			i64NtWin = 0;
			bXiaZhuang = false;
			bIsSuccess = 255;
			byDeskStation = 255;
		}
	};
	/*------------------------------------------------------------------------------*/
	/// 通知客户端是否为超级客户端状态消息结构
	struct S_C_IsSuperUser
	{
		BYTE byDeskStation;      /**< 玩家位置 */
		bool bEnable;           /**< 是否开通 */
		S_C_IsSuperUser()
		{
			memset(this, 0, sizeof(S_C_IsSuperUser));
		}
	};
	/*------------------------------------------------------------------------------*/
	struct SuperUserSetData
	{
		bool	bSetSuccese;		//是否设置成功了
		int		iSetResult[4];

		SuperUserSetData()
		{
			memset(this, 0, sizeof(SuperUserSetData));
		}
	};

	struct tag_s2c_tip_exit
	{
		BYTE	bIsExit;
		LLONG	i64Money;
		tag_s2c_tip_exit()
		{
			bIsExit = 255;
			i64Money = 0;
		}
	};
	/*--------------------------------------------------------------------------*/

}
#pragma pack()
#endif