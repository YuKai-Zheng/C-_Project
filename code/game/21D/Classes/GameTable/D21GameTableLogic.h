#ifndef __D21_Game_Table_Logic_H__
#define __D21_Game_Table_Logic_H__

#include "HNNetExport.h"
#include "D21MessageHead.h"
#include "HNLogicExport.h"

namespace D21
{
	class GameTableUICallback;

	class GameTableLogic: public HN::HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();	
	public:
		//**********�����Ϣ*****************************************
		// 处理服务器游戏消息	
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		//处理服务器状态消息	
		virtual void dealGameStationResp(void* object, INT objectSize);

		//用户同意
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);

		// 
		virtual void dealGameStartResp(BYTE bDeskNO);

		// 
		virtual void dealGameEndResp(BYTE bDeskNO);

		// 
		virtual void dealGamePointResp(void* object, INT objectSize);

		//处理服务器坐下消息	
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);

		// 处理服务器离开游戏消息	
		virtual void dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user);

		// 游戏信息
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);

		//**********框架******************************************
		
		//处理服务器同意游戏消息	
		void dealAgreeGameResp(void* object, INT objectSize);

		//更新状态
		void dealPlayerState(void* object, INT objectSize);

		//处理服务器发过来的开始游戏消息
		void dealPlayResp(void* object, INT objectSize);

		//处理服务器发过来的游戏安全结束消息
		void dealSafeFinishResp(void* object, INT objectSize);

		//处理服务器发过来的叫牌消息
		void dealCallResp(void* object, INT objectSize);

		//处理服务器发过来的重新开始消息
		void dealRestartResp(void* object, INT objectSize);

		//处理服务器发过来的游戏正常结束消息
		void dealNormalFinishResp(void* object, INT objectSize);

		//处理服务器发过来的超端用户信息
		void dealSuperMessageResp(void* object, INT objectSize);

	public:
		//游戏内消息-结束
		virtual void initParams();

		//初始化值
		virtual void refreshParams();

		//游戏结束
		void GameEnd();
		
	public:
		void enterGame();															//进入游戏
		void loadUsers();															//加载用户
		void requestReady();														//请求准备
		void requestSendBet(LLONG money);										//请求下注
		
		void requestGiveUpBet();												//请求放弃
		void requestGiveUpGame();											//请求放弃游戏
		
		void requestCatchBanker(GameStationButton*data);		//抢庄
		void requestInsurance(GameStationButton*data);			//保险
		
		void requestDividedCard();												//分牌
		void requestDoublle();													//请求加倍
		void requestStopCard();													//停牌
		void requestNeedCard();													//要牌
	protected:
		GameTableUICallback*	_uiCallback;
		
	public:
		BYTE	_NTSeatNo;								//庄家号
		BYTE	_GameState;								//游戏状态
		BYTE	_CallCardTime;							//发牌时间
		BYTE	_BetTime;									//下注时间
		BYTE	_BeginWaitTime;						//开始等待时间
		BYTE	_CatchTime;								//抢庄时间
		BYTE	_insuranceTime;							//保险时间

		LLONG	_SumAllBet;							//总注
		LLONG	_UserBet[PLAY_COUNT];			//用户下注值

		LLONG	_allPlayerNoteMoeny;			// 所有玩家当局总下注
		LLONG	_lastMoney;							// 记录进入游戏开启的 前一句输赢 第一次 0
		LLONG	_historyMoney;						// 进入游戏起总输赢

		BYTE    _bPlayerStation[PLAY_COUNT];	//用户状态
		bool	_bePlaying[PLAY_COUNT];			//����Ƿ������״̬������;������
		bool	_playerSitted[PLAY_COUNT];		//已经坐下玩家
		bool	_userReady[PLAY_COUNT];			//用户准备
		int		_maxPoint;									//最大位置
		
		////////
		long long _i64BetUnit[4];				//筹码单元
	};
	
}


#endif