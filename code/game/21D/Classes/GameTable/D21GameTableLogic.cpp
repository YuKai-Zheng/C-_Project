#include "D21GameTableLogic.h"
#include "D21GameTableUICallback.h"
#include "HNCommon\HNLog.h"

using namespace HN;

namespace D21
{
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate)
		: _uiCallback(uiCallback)
		, HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, uiCallback)
	{

		initParams();
	}

	GameTableLogic::~GameTableLogic()
	{

	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if(MDM_GM_GAME_NOTIFY != messageHead->bMainID)
		{
			HNLOG("onGameMessage MainID is error.");
			return;
		}

		switch (messageHead->bAssistantID)
		{
		case ASS_AGREE://同意游戏消息
			HNLOG("dealAgreeGameResp");
			dealAgreeGameResp(object, objectSize);
			break;
		case ASS_CATCH:
		{
			HNLOG("dealCatchBanker");//抢庄

			if (objectSize != sizeof(GameMessageCatch))	return;

			_gameStatus = GS_CATCH;

			GameMessageCatch* sData = (GameMessageCatch*)object;
			_uiCallback->dealCatchBankerMsg(sData);
			break;
		}
		case ASS_PLAYER_STATE:{
			dealPlayerState(object, objectSize);
			break;
		}			
		case ASS_NT://确定庄家
		{
			HNLOG("dealNtResp");

			if (objectSize != sizeof(GameMessageNt))	return;

			GameMessageNt* data = (GameMessageNt*)object;

			_NTSeatNo = data->bNtStation;

			_uiCallback->dealConfirmBankerMsg(data);

			break;
		}
		case ASS_BET://开始下注
		{
			HNLOG("dealBetResp");

			if (objectSize != sizeof(GameMessageBetResult)) return;

			_gameStatus = GS_BET;

			GameMessageBetResult* data = (GameMessageBetResult*)object;
			_uiCallback->dealBetMsg(data);

			break;
		}
		case ASS_BETRESULT://下注结果消息
		{
			HNLOG("dealBetResultResp");

			if (objectSize != sizeof(GameMessageBetResult))	return;
			
			GameMessageBetResult* data = (GameMessageBetResult*)object;
			_uiCallback->dealBetMsg(data);

			//累计当局玩家下注
			_allPlayerNoteMoeny += data->i64Note;
			//_uiCallback->upDataRecordBoard(_allPlayerNoteMoeny);
			
			break;
		}
		case ASS_SEND://开始发牌
		{
			HNLOG("dealSendCardResp");

			if (objectSize != sizeof(GameMessageSendCard))	return;
			GameMessageSendCard* data = (GameMessageSendCard*)object;

			_gameStatus = GS_SEND;

			_uiCallback->dealStartSendCardMsg(data);
			
			break;
		}
		case ASS_INSURANCE://保险
		{
			HNLOG("deaBuylnsurance");

			if (objectSize != sizeof(GameMessageInsurance))	return;
			GameMessageInsurance* data = (GameMessageInsurance*)object;

			_uiCallback->dealBuyInsuranceMsg(data);

			break;
		}
		case ASS_OPERATE://操作通知
		{
			if (objectSize != sizeof(GameMessageOperateNotify)) return;
			GameMessageOperateNotify* data = (GameMessageOperateNotify*)object;

			_gameStatus = GS_PLAY_GAME;

			_uiCallback->dealOperateNotifyMsg(data);

			break;
		}
		case ASS_DIVIDE://分牌结果
		{
			if (objectSize != sizeof(GameMessageDivideResult)) return;
			GameMessageDivideResult* data = (GameMessageDivideResult*)object;

			_uiCallback->dealDivideResultMsg(data);

			break;
		}
		case ASS_SAFEEND:								//游戏安全结束
			HNLOG("dealSafeFinishResp");
			dealSafeFinishResp(object, objectSize);
			break;
		case ASS_CALL:										//叫牌消息
			HNLOG("dealCallResp");
			dealCallResp(object, objectSize);
			break;
		case ASS_RESTART:									//重新开始消息
			HNLOG("dealRestartResp");
			dealRestartResp(object, objectSize);
			break;
		case ASS_NORMALFINISH:								//游戏正常结束
			HNLOG("dealNormalFinishResp");
			dealNormalFinishResp(object, objectSize);
			break;
		case ASS_SUPER_USER:								//超端客户端
			HNLOG("dealSuperMessageResp");
			dealSuperMessageResp(object, objectSize);
			break;
		default:
			break;
		}
	}
	//**********框架消息*****************************************

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		HNLOG("=========dealGameStationResp");
		if (_gameStatus != GS_WAIT_ARGEE && _gameStatus != GS_WAIT_SETGAME && _gameStatus != GS_WAIT_NEXT)
		{
			_uiCallback->setMsg(GBKToUtf8("亲，请耐心等待下一局！"));
		}
		switch(_gameStatus)
		{
		case GS_WAIT_SETGAME:						//游戏没有开始状态
		case GS_WAIT_ARGEE:							//等待玩家开始状态
			{
														_gameStatus = GS_WAIT_ARGEE;
				if (objectSize != sizeof(GameStation_1))	return;
				GameStation_1 *gameStation_1=(GameStation_1 *)object;
				
				_BeginWaitTime = gameStation_1->bGameBeginWaitTime;							//游戏开始等待时间
				_BetTime	   = gameStation_1->bGameBetTime;											//游戏下注时间
				_CallCardTime = gameStation_1->bCallCardTime;										//游戏叫牌时间
				_CatchTime = gameStation_1->bGameCatchTime;										//游戏抢庄时间
				_insuranceTime = gameStation_1->bGameInsuranceTime;							//游戏保险
				_GameState	   = gameStation_1->bGameStation;										//游戏状态
				memcpy(_i64BetUnit, gameStation_1->i64BetUnit, sizeof(_i64BetUnit));

				_uiCallback->createBetButton(_i64BetUnit);
				for(int i=0; i<PLAY_COUNT; i++)
				{
					BYTE viewSeatNo = logicToViewSeatNo(i);
					_userReady[i] = gameStation_1->bUserReady[i];
					_uiCallback->showUserReady(viewSeatNo, _userReady[i]);
					if (i == _mySeatNo && !_userReady[i])
					{
						//启动等待时钟
						_uiCallback->starTimer(TIME_WAIT, _BeginWaitTime, logicToViewSeatNo(_mySeatNo));
						//显示开始按钮
						_uiCallback->showBtnStart(logicToViewSeatNo(_mySeatNo));
					}
				}
				break; 
			}
		case GS_BET:								//下注状态
			{
														_gameStatus = GS_BET;

				if (objectSize != sizeof(GameStation_2))	return;
				GameStation_2 *gameStation_2 = (GameStation_2 *)object;

				_BeginWaitTime = gameStation_2->bGameBeginWaitTime;							//游戏开始等待时间
				_BetTime	   = gameStation_2->bGameBetTime;											//游戏下注时间
				_CallCardTime  = gameStation_2->bCallCardTime;										//游戏叫牌时间
				_CatchTime = gameStation_2->bGameCatchTime;										//游戏抢庄时间
				_insuranceTime = gameStation_2->bGameInsuranceTime;							//游戏保险
				_GameState	   = gameStation_2->bGameStation;										//游戏状态

				_NTSeatNo = gameStation_2->iNtPeople;
				_uiCallback->setNt(logicToViewSeatNo(_NTSeatNo),_NTSeatNo==_mySeatNo);
				break;
			}
		case GS_SEND:								//发牌状态
			{
														_gameStatus = GS_SEND;

				if (objectSize != sizeof(GameStation_5))	return;
				GameStation_5 *gameStation_5 = (GameStation_5 *)object;

				_BeginWaitTime = gameStation_5->bGameBeginWaitTime;							//游戏开始等待时间
				_BetTime	   = gameStation_5->bGameBetTime;											//游戏下注时间
				_CallCardTime  = gameStation_5->bCallCardTime;										//游戏叫牌时间
				_CatchTime = gameStation_5->bGameCatchTime;										//游戏抢庄时间
				_insuranceTime = gameStation_5->bGameInsuranceTime;							//游戏保险
				_GameState	   = gameStation_5->bGameStation;										//游戏状态

				_NTSeatNo = gameStation_5->iNtPeople;
				_uiCallback->setNt(logicToViewSeatNo(_NTSeatNo),_NTSeatNo==_mySeatNo);
				break;
			}
		case GS_PLAY_GAME:					//游戏进行中
			{
				_gameStatus = GS_PLAY_GAME;
				//_uiCallback->createBetButton(_i64BetUnit);
				if (objectSize != sizeof(GameStation_3))	return;
				GameStation_3 *gameStation_3 = (GameStation_3 *)object;

				_BeginWaitTime = gameStation_3->bGameBeginWaitTime;							//游戏开始等待时间
				_BetTime	   = gameStation_3->bGameBetTime;											//游戏下注时间
				_CallCardTime = gameStation_3->bCallCardTime;										//游戏叫牌时间
				_CatchTime = gameStation_3->bGameCatchTime;										//游戏抢庄时间
				_insuranceTime = gameStation_3->bGameInsuranceTime;							//游戏保险
				_GameState	   = gameStation_3->bGameStation;										//游戏状态

				_NTSeatNo = gameStation_3->iNtPeople;
				_uiCallback->setNt(logicToViewSeatNo(_NTSeatNo),_NTSeatNo==_mySeatNo);

				break;
			}
		case GS_WAIT_NEXT:						//等待下一盘开始
			{
													_gameStatus = GS_WAIT_NEXT;

				if (objectSize != sizeof(GameStation_4))	return;
				GameStation_4 *gameStation_4 = (GameStation_4 *)object;

				_BeginWaitTime = gameStation_4->bGameBeginWaitTime;							//游戏开始等待时间
				_BetTime	   = gameStation_4->bGameBetTime;											//游戏下注时间
				_CallCardTime  = gameStation_4->bCallCardTime;										//游戏叫牌时间
				_CatchTime = gameStation_4->bGameCatchTime;										//游戏抢庄时间
				_insuranceTime = gameStation_4->bGameInsuranceTime;							//游戏保险
				_GameState	   = gameStation_4->bGameStation;										//游戏状态
				memcpy(_i64BetUnit, gameStation_4->i64BetUnit, sizeof(_i64BetUnit));

				_uiCallback->createBetButton(_i64BetUnit);
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(gameStation_4->bUserReady[i])
					{
						_uiCallback->showUserReady(logicToViewSeatNo(i), true);
					}
				}
				//启动等待时钟
				_uiCallback->starTimer(TIME_WAIT, _BeginWaitTime, logicToViewSeatNo(_mySeatNo));
				//显示开始按钮
				_uiCallback->showBtnStart(logicToViewSeatNo(_mySeatNo));
				break;
			}
		}
		//重连进来计分板清0
		//_uiCallback->upDataRecordBoard(0, 0, 0);
	}
	//**********框架消息*****************************************
	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{
		//HNLOG("===OutGame=====dealUserAgreeResp");
		_userReady[agree->bDeskStation] = (agree->bAgreeGame == 1);	
		//玩家存在并且点了开始按钮才显示准备图片
		if (_userReady[agree->bDeskStation] && _existPlayer[agree->bDeskStation])
		{
			if(agree->bDeskStation == _mySeatNo)
			{
				_uiCallback->stopTimer();
			}	
			_uiCallback->showUserReady(logicToViewSeatNo(agree->bDeskStation), true);
		}	
	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{

	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{

	}

	void GameTableLogic::dealGamePointResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		//HNLOG("=========dealUserSitResp");
		HNGameLogicBase::dealUserSitResp(userSit, user);
		bool isMe = (user->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID);
		if(isMe)
		{
			loadDeskUsers();
			loadUsers();
		}
		else
		{
			if (_playerSitted[userSit->bDeskStation])	return;
			_playerSitted[userSit->bDeskStation] = true;

			BYTE seatNo = logicToViewSeatNo(userSit->bDeskStation);
			_uiCallback->addUser(seatNo, userSit->bDeskStation == _mySeatNo, userSit->dwUserID);
		}	
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user)
	{
		//HNLOG("=========dealUserUpResp");
		HNGameLogicBase::dealUserUpResp(userSit, user);

		_playerSitted[userSit->bDeskStation] = false;
		_userReady[userSit->bDeskStation] = false;

		//玩家离开后清理
		BYTE seatNo = logicToViewSeatNo(userSit->bDeskStation);
		_uiCallback->removeUser(seatNo, userSit->bDeskStation == _mySeatNo);
		if(_mySeatNo == INVALID_DESKSTATION || _mySeatNo == userSit->bDeskStation)
		{
			_uiCallback->leaveDesk();
		}
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{

	}

	/***********************************************************************************/
	
	/**********游戏内消息**************************************************************/

	//处理服务器同意游戏消息
	void GameTableLogic::dealAgreeGameResp(void* object, INT objectSize)
	{
		;
	}

	void GameTableLogic::dealPlayerState(void* object, INT objectSize)
	{
		if (objectSize != sizeof(GamePlayerStation)) return;
		GamePlayerStation* sData = (GamePlayerStation*)object;

		memcpy(_bPlayerStation, sData->bPlayerStation, sizeof(_bPlayerStation));
	}

	//处理服务器发过来的开始游戏消息
	void GameTableLogic::dealPlayResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(GameMessageStart))	return;
		GameMessageStart* data = (GameMessageStart*)object;
		
		//校验第一个叫分人的座位号
		if (data->bFirstScoreStation < 0 || data->bFirstScoreStation >= PLAY_COUNT)	return;

		//游戏开始操作
		_GameState = GS_PLAY_GAME;
// 		_uiCallback->starTimer(_CallCardTime, logicToViewSeatNo(data->bFirstScoreStation));
// 
// 		//操作面板
// 		if (data->flag == true && data->bFirstScoreStation == _mySeatNo)
// 		{
// 			_uiCallback->setCallingbtnStatus(_mySeatNo == _NTSeatNo);
// 		}
	}

	//处理服务器发过来的游戏安全结束消息
	void GameTableLogic::dealSafeFinishResp(void* object, INT objectSize)
	{
		_gameStatus = GS_WAIT_NEXT;
		if(objectSize != sizeof(GameMessageEnd))	return;
		GameMessageEnd* data = (GameMessageEnd*)object;
		
		//HNLOG("=========dealSafeFinishResp");
// 		for (int i=0; i<PLAY_COUNT; i++)
// 		{
// 			if (_existPlayer[i])
// 			{
// 				_uiCallback->stopTimer(INVALID_BYTE);
// 			}
// 		}
		//启动等待时钟
		_uiCallback->starTimer(TIME_WAIT_NEXT, _BeginWaitTime, INVALID_BYTE);
		//显示开始按钮
		_uiCallback->showBtnStart(logicToViewSeatNo(_mySeatNo));		

		//重置数据
		refreshParams();
		_uiCallback->refreshData();
	}

	//处理服务器发过来的叫牌消息
	void GameTableLogic::dealCallResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(GameMessageNeedCard))	return;
		GameMessageNeedCard* data = (GameMessageNeedCard*)object;
		
		//检验座位号
		if (data->bDeskStation<0 || data->bDeskStation>=PLAY_COUNT)		return;

		_uiCallback->dealOperateResp(data);
	}
	//处理服务器发过来的重新开始消息
	void GameTableLogic::dealRestartResp(void* object, INT objectSize)
	{

	} 
	//处理服务器发过来的游戏正常结束消息
	void GameTableLogic::dealNormalFinishResp(void* object, INT objectSize)
	{
		//CCLOG("===objectSize=%d=====sizeof(GameMessageNormalFinish)=%d", objectSize, sizeof(GameMessageNormalFinish));
		if(objectSize != sizeof(GameMessageNormalFinish))	return;
		GameMessageNormalFinish* data = (GameMessageNormalFinish*)object;
		_gameStatus = GS_WAIT_NEXT;
		_uiCallback->openResultView(true);

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (data->bIsRuning[i] > 0)
			{
				BYTE seateNo = logicToViewSeatNo(i);
				
				if (_mySeatNo == i)
				{
					_uiCallback->starTimer(TIME_WAIT_NEXT, _BeginWaitTime, INVALID_BYTE);
				}
			}

			if (data->bIsRuning[i] == 1)
			{
				//找到该玩家 发送昵称
				UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
				if (pUser)
				{
					_uiCallback->upDataResult(pUser->nickName, data->i64UserScore[i], data->iUserPoint[i][0], data->i64UserScore[i] < 0,i);
				}
			}

			//更新计分面板
			if (i == _mySeatNo)
			{
				_lastMoney = data->i64UserScore[i];
				_historyMoney += data->i64UserScore[i];
				//_uiCallback->upDataRecordBoard(_historyMoney, _lastMoney, _allPlayerNoteMoeny); delete by qiuyx
			}
		}
		//显示开始按钮
		_uiCallback->showBtnStart(logicToViewSeatNo(_mySeatNo));
		//重置数据
		refreshParams();
		_uiCallback->refreshData();
	}

	//处理服务器发过来的超端用户信息
	void GameTableLogic::dealSuperMessageResp(void* object, INT objectSize)
	{
		
	}

	//游戏内消息-结束
	/************************************************************************/
	void GameTableLogic::initParams()
	{
		memset(_bPlayerStation, -1, sizeof(_bPlayerStation));
		_NTSeatNo			= INVALID_DESKSTATION;
		_SumAllBet			= 0;
		_maxPoint			= 0;
		_allPlayerNoteMoeny = 0;
		_lastMoney = 0;
		_historyMoney = 0;

		memset(_userReady , false , sizeof(_userReady));
		memset(_UserBet , 0 , sizeof(_UserBet));
		memset(_playerSitted , false , sizeof(_playerSitted));
	}

	void GameTableLogic::refreshParams()
	{
		_NTSeatNo			= INVALID_DESKSTATION;
		_SumAllBet			= 0;
		_maxPoint			= 0;
		_allPlayerNoteMoeny = 0;
		memset(_userReady , false , sizeof(_userReady));
	}

	void GameTableLogic::enterGame()
	{
		//HNLOG("=========enterGame");
		HNGameLogicBase::loadDeskUsers();
		loadUsers();
		if ((INVALID_DESKSTATION != _mySeatNo) && _autoCreate)
		{
			sendGameInfo();
		}
	}

	void GameTableLogic::loadUsers()
	{
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(_existPlayer[i] && pUser != nullptr)
			{
				_playerSitted[i] = true;
				seatNo = logicToViewSeatNo(i);
				_uiCallback->addUser(seatNo, i == _mySeatNo, pUser->dwUserID);
			}
		}
	}
	void GameTableLogic::GameEnd()
	{
		//for (int i=0; i<PLAY_COUNT; i++)
		//{
		//	_uiCallback->stopTimer();
		//}
		_uiCallback->stopTimer();
	}

	//=====================================================================
	void GameTableLogic::requestReady()
	{
		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_START;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME,(void*)&data, sizeof(data));
	}

	void GameTableLogic::requestSendBet(LLONG money)
	{
		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.i64Note = money;
		data.iBtType = CLICK_BETOPERATE;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON,(void*)&data, sizeof(data));
	}

	void GameTableLogic::requestGiveUpBet()
	{
		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_OPERATE;
		data.iBtParam = 2;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON,(void*)&data, sizeof(data));
	}

	void GameTableLogic::requestGiveUpGame()
	{
		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_PLAYOPERATE;
		data.iBtParam = 2;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON,(void*)&data, sizeof(data));
	}

	/////分牌，加倍，停牌，要牌对应参数0-3
	void GameTableLogic::requestDividedCard()
	{
		if (_gameStatus != GS_PLAY_GAME) return;

		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_PLAYOPERATE;
		data.iBtParam = 0;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON,(void*)&data, sizeof(data));
	}

	void GameTableLogic::requestDoublle()
	{
		if (_gameStatus != GS_PLAY_GAME) return;

		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_PLAYOPERATE;
		data.iBtParam = 1;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON, (void*)&data, sizeof(data));
	}

	void GameTableLogic::requestStopCard()
	{
		if (_gameStatus != GS_PLAY_GAME) return;

		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_PLAYOPERATE;
		data.iBtParam = 2;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON,(void*)&data, sizeof(data));
	}

	void GameTableLogic::requestNeedCard()
	{
		if (_gameStatus != GS_PLAY_GAME) return;

		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_PLAYOPERATE;
		data.iBtParam = 3;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON, (void*)&data, sizeof(data));
	}

	void GameTableLogic::requestCatchBanker(GameStationButton*cData)
	{
		GameStationButton data;
		data.bDeskStation = _mySeatNo;
		data.iBtType = CLICK_CATCH;
		data.iBtParam = cData->iBtParam;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON, (void*)&data, sizeof(data));
	}
	void	GameTableLogic::requestInsurance(GameStationButton*cdata)
	{
		GameStationButton data;
		data.bDeskStation = cdata->bDeskStation;
		data.iBtType = CLICK_INSURANCE;
		data.iBtParam = cdata->iBtParam;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, GAME_BUTTON, (void*)&data, sizeof(data));
	}
}
