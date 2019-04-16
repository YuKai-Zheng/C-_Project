#include "HNLobby/v66/ExtraReward.h"
#include "GoldenFlowerGameTableLogic.h"
#include "GoldenFlowerGameTableUICallback.h"
#include "GoldenFlowerMessageHead.h"
#include "HNNetExport.h"


using namespace HN;

namespace goldenflower
{
	
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate):
		_gameUICallback(uiCallback), HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, uiCallback) 
	{
		//_tableInfo.byDeskNo = deskNo;
		//_tableInfo.bAutoCreate = bAutoCreate;
        //initParams();
	}

	GameTableLogic::~GameTableLogic()
	{
	
	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CCAssert(MDM_GM_GAME_NOTIFY == messageHead->bMainID, "mainId error.");

		switch(messageHead->bAssistantID)
		{
		// 游戏开始--
		case S_C_GAME_BEGIN:
			HNLOG("S_C_GAME_BEGIN");
			dealGameBeginResp(object, objectSize);
			break;

		// 开始发牌--
		case S_C_SEND_CARD:
			HNLOG("S_C_SEND_CARD");
			dealSendCardResp(object, objectSize);
			break;

		// 开始游戏--
		case S_C_GAME_PLAY:
			HNLOG("S_C_GAME_PLAY");
			dealGamePlayResp(object, objectSize);
			break;

		// 通知玩家动作--
		case S_C_CALL_ACTION:
			HNLOG("S_C_CALL_ACTION");
			dealCallActionResp(object, objectSize);
			break;

		// 游戏结束--
		case S_C_GAME_END:
			HNLOG("S_C_GAME_END");
			dealGameEndResp(object, objectSize);
			break;

		// 超端校验
		case S_C_SUPER_PROOF:
			HNLOG("S_C_SUPER_PROOF");
			dealSuperProofResp(object, objectSize);
			break;

		// 超端设置结果
		case S_C_SUPER_SET_RESULT:
			HNLOG("S_C_SUPER_SET_RESULT");
			dealSuperSetResultResp(object, objectSize);
			break;

		// 玩家看牌结果--
		case S_C_LOOK_CARD_RESULT:
			HNLOG("S_C_LOOK_CARD_RESULT");
			dealLookCardResultResp(object, objectSize);
			break;

		// 玩家跟注结果--
		case S_C_GEN_ZHU_RESULT:
			HNLOG("S_C_GEN_ZHU_RESULT");
			dealGenZhuResultResp(object, objectSize);
			break;

		// 加注结果--
		case S_C_JIA_ZHU_RESULT:
			HNLOG("S_C_JIA_ZHU_RESULT");
			dealJiaZhuResultResp(object, objectSize);
			break;

		// 玩家比牌结果--
		case S_C_BI_PAI_RESULT:
			HNLOG("S_C_BI_PAI_RESULT");
			dealBiPaiResultResp(object, objectSize);
			break;
			
		// 玩家弃牌结果--
		case S_C_GIVE_UP_RESULT:
			HNLOG("S_C_GIVE_UP_RESULT");
			dealGiveUpResultResp(object, objectSize);
			break;
                
        case S_C_LACK_MONEY_TIP:
            HNLOG("S_C_LACK_MONEY_TIP");
            dealUserLackMoneyTipResp(object, objectSize);
            break;
		
		//下注金额
		case S_C_GAME_NOTE_NTF:
			dealBetValueResultResp(object, objectSize);
			break;

        //金币不足退出
        case S2C_TIP_EXIT_MSG:
            dealGameTipExit(object, objectSize);
            break;
               
        case S2C_NOTIFY_MSG:
        {
            CCAssert(sizeof(tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
            if (sizeof(tag_s2c_notify) != objectSize)	return;
            
            tag_s2c_notify* msg = (tag_s2c_notify*)object;
            _gameUICallback->onGameNotify(msg);
            break;
        }
		case ASS_NOTIFYVIPUSER_WINLOSE:
		{
			CCAssert(sizeof(VipUserWinLoseRlt) == objectSize, "sizeof(VipUserWinLoseRlt) != objectSize");
			if (sizeof(VipUserWinLoseRlt) != objectSize)	return;

			VipUserWinLoseRlt* msg = (VipUserWinLoseRlt*)object;
			_gameUICallback->onGameResultTag(msg->nWinLose);
			break;
		}
		case ASS_NOTIFYVIPUSER_GAMELOG:
		{
			CCAssert(sizeof(VipRoomPlayerGameLogMsg) == objectSize, "sizeof(VipRoomPlayerGameLogMsg) != objectSize");
			if (sizeof(VipRoomPlayerGameLogMsg) != objectSize)	return;

			VipRoomPlayerGameLogMsg* msg = (VipRoomPlayerGameLogMsg*)object;
			_gameUICallback->onVipGameResultLog(msg->mUserLog);
		}
		break;
        case ASS_SHOW_LUCKYMONEY:
            {
                // 出现红包
                if (objectSize != sizeof(LuckyMoneyNotify))
                {
                    CCASSERT(false, "sizeof(LuckyMoneyNotify) != objectSize");
                    return;
                }
                
                auto msg = (LuckyMoneyNotify*)object;
                _gameUICallback->onLuckyMoneyNotify(msg);
            }
                break;
        case ASS_LUCKYMONEY_RESULT:
            {
                // 打开红包结果
                if (objectSize != sizeof(LuckyMoneyResult))
                {
                    CCASSERT(false, "sizeof(LuckyMoneyResult) != objectSize");
                    return;
                }
                
                auto msg = (LuckyMoneyResult*)object;
                _gameUICallback->onLuckyMoneyResult(msg);
            }
                break;
//            //年兽
//        case NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG :
//        {
//            //年兽出现 150
//            _uiCallback->onGameShowNianShou();
//        }
//            break;
//        case NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT:
//        {
//            //赶年兽 151
//            CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//            if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)    return;
//            
//            NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//            _uiCallback->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//        }
//            break;
//        case NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG:
//        {
//            //对局获得鞭炮 152
//            CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//            if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)   return;
//            
//            NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//            _uiCallback->onGameMatchGainFirecraker(msg->nUserID);
//        }
//            break;
                
		default:
			break;
		}
	}


	void GameTableLogic::dealGameBeginResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_GameBegin), "size error.");
		S_C_GameBegin* pData = (S_C_GameBegin*)object;

		// 游戏数据重置
		refreshParams();

		_NTSeatNo   = pData->byNtStation;
		_GuoDi      = pData->iGuoDi;
		_DiZhu      = pData->iDiZhu;
		//_ShangXian  = pData->i64ShangXian;
		_DingZhu    = pData->i64DingZhu;
		_ZongXiaZhu = pData->i64ZongXiaZhu;

		memcpy(_UserState, pData->iUserState, sizeof(_UserState));
		memcpy(_XiaZhuData, pData->i64XiaZhuData, sizeof(_XiaZhuData));

		// 更新玩家信息
		showUserInfo();

		//隐藏准备标志
		_gameUICallback->showReadySign(INVALID_SEAT_NO, false);
		_gameUICallback->showGiveUpCard(INVALID_SEAT_NO, false);
		_gameUICallback->showTotalNoteFirst(pData->i64ZongXiaZhu);

		// 锅底信息
		//_uiCallback->showLimitNote(_ShangXian);
		_gameUICallback->showBaseNote(_DiZhu);
//		_uiCallback->showLimitPerNote(_DingZhu);
//		_uiCallback->showGuoDi(_GuoDi);
		_gameUICallback->showTotalNote(_ZongXiaZhu);
		//_uiCallback->showMyTotalNote(_DiZhu);

        HNLOG("6 ======");
        //记录当前桌子所有玩家的昵称
        for (int i = 0; i < PLAY_COUNT; i++) {
            UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
            if (nullptr != pUser) {
                if (i < _userName[i].length()) {
                    _userName[i] = pUser->nickName;
                }
                _gameUICallback->showUserTotalBetMoney(i, pData->i64XiaZhuData[i]);
            }
        }
        
        
    }
    
    
	void GameTableLogic::dealSendCardResp(void* object, INT objectSize)
	{
		// 数据大小检测
		CCAssert(objectSize == sizeof(S_C_SendCard), "size error.");

		// 数据备份
		S_C_SendCard* pData = (S_C_SendCard*)object;
		memcpy(_userCard, pData->byCard, sizeof(_userCard));
		memcpy(_userCardCount, pData->byCardCount, sizeof(_userCardCount));

		// 显示手牌
		std::vector<THandCard> cards;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			THandCard card;
			memset(card.byCards, 0x00, sizeof(card.byCards));
			card.bySeatNo = logicToViewSeatNo(i);
			_gameUICallback->showUserNoteMoney(logicToViewSeatNo(i), 0);

			for(int j = 0; j < _userCardCount[i]; j++)
			{
				card.byCards[j] = _userCard[i][j];
			}
			if (_userCardCount[i]>0)
			{
				cards.push_back(card);
			}
		}

		// 显示庄家
		_gameUICallback->showDealer(logicToViewSeatNo(_NTSeatNo));

		_gameUICallback->showHandCard(cards);
	}

	void GameTableLogic::dealGamePlayResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_BeginPlay), "size error.");
		S_C_BeginPlay* pData = (S_C_BeginPlay*)object;

		// 数据备份
		_CurrHandleSeatNo = pData->byCurrHandleStation;
		_CanLook          = pData->bLook;
		_CanFollow        = pData->bFollow;
		_CanOpen          = pData->bOpen;
		_CanGiveUp        = pData->bGiveUp;
		memcpy(_CanAdd, pData->bAdd, sizeof(_CanAdd));

		// 操作提示
		stopAllWait();
		_gameUICallback->showWaitTime(logicToViewSeatNo(_CurrHandleSeatNo), _thinkTime, _thinkTime);
		_gameUICallback->controlShowOrHideTotalNode(true);
		

		// 显示操作按钮
		bool isMe = (_CurrHandleSeatNo == _mySeatNo);
		_gameUICallback->showDashboard(isMe);
		_gameUICallback->setLookVisible(isMe && _CanLook);
		_gameUICallback->setFollowVisible(isMe && _CanFollow);
		_gameUICallback->setAddVisible(isMe ,_CanAdd);
		_gameUICallback->setOpenVisible(isMe && _CanOpen);
		_gameUICallback->setGiveUpVisible(isMe && _CanGiveUp);

		//当前玩家是自己才显示时钟
		if (isMe)
		{
			_gameUICallback->IStartTimer(_thinkTime);
		}

		if (0 == _MyZongXiaZhu)
		{
			_MyZongXiaZhu += _DiZhu;
		}
		//_uiCallback->showMyTotalNote(_MyZongXiaZhu);
	
	}

	void GameTableLogic::dealCallActionResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_NoticeAction), "size error.");
		S_C_NoticeAction* pData = (S_C_NoticeAction*)object;

		_CurrHandleSeatNo = pData->byCurrHandleStation;
		_CanLook          = pData->bCanLook;
		_CanFollow        = pData->bCanFollow;
		_CanOpen          = pData->bCanOpen;
		_CanGiveUp        = pData->bCanGiveUp;
		memcpy(_CanAdd, pData->bCanAdd, sizeof(_CanAdd));

		// 显示倒计时
		stopAllWait();
		_gameUICallback->showWaitTime(logicToViewSeatNo(_CurrHandleSeatNo), _thinkTime, _thinkTime);

		// 显示操作面板
		bool isMe = (_CurrHandleSeatNo == _mySeatNo);
		_gameUICallback->showDashboard(isMe);
		_gameUICallback->setLookVisible(isMe && _CanLook);
		_gameUICallback->setFollowVisible(isMe && _CanFollow);
		_gameUICallback->setAddVisible(isMe,_CanAdd);
		_gameUICallback->setOpenVisible(isMe && _CanOpen);
		_gameUICallback->setGiveUpVisible(isMe && _CanGiveUp);
		//当前玩家是自己才显示时钟
		if (isMe)
		{
			_gameUICallback->IStartTimer(_thinkTime);
		}

	}

	void GameTableLogic::dealGameEndResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_GameEnd), "size error.");
		S_C_GameEnd* pData = (S_C_GameEnd*)object;

		//玩家对应的视图座位号
		BYTE viewSeat[PLAY_COUNT] = {INVALID_SEAT_NO};
		// 玩家输赢
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(pData->i64ChangeMoney[i] != 0)
			{
				_gameUICallback->showUserProfit(logicToViewSeatNo(i), pData->i64ChangeMoney[i]);
				viewSeat[i] = logicToViewSeatNo(i);
			}			
		}
		

		// 显示赢牌玩家
		_gameUICallback->showWin(logicToViewSeatNo(pData->byWiner));
		_gameUICallback->showDashboard(false);
		_gameUICallback->showWatchCard(INVALID_SEAT_NO, false);
//		showUserInfo();	

		_gameUICallback->showEndBox(pData, viewSeat, logicToViewSeatNo(pData->byWiner));
		
		//显示准备按钮
		sendUserReady();
	}


	void GameTableLogic::dealSuperProofResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_SuperUserProof), "size error.");
		S_C_SuperUserProof* pData = (S_C_SuperUserProof*)object;
	}

	void GameTableLogic::dealSuperSetResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_SuperUserSetResult), "size error.");
		S_C_SuperUserSetResult* pData = (S_C_SuperUserSetResult*)object;
	}

	void GameTableLogic::dealLookCardResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_LookCardResult), "size error.");
		S_C_LookCardResult* pData = (S_C_LookCardResult*)object;

		// 数据备份
		_UserState[pData->byDeskStation] = pData->iUserState;
		
		bool isMe = (pData->byDeskStation == _mySeatNo);
		if(isMe)
		{
			_userCardCount[pData->byDeskStation] = pData->byUserCardCount;
			memcpy(_userCard[pData->byDeskStation], pData->byUserCard, sizeof(_userCard[pData->byDeskStation]));

			// 显示玩家手牌
			std::vector<BYTE> cards(pData->byUserCardCount);
			for (int i = 0; i < pData->byUserCardCount;i++)
			{
				cards[i] = _userCard[pData->byDeskStation][i];
			}
			//memcpy(&cards.front(), _userCard, sizeof(BYTE) * pData->byUserCardCount);
			_gameUICallback->showUserFlipCard(logicToViewSeatNo(_mySeatNo), cards);
			
		}

		// 提示看牌		
		_gameUICallback->showUserLookCard(logicToViewSeatNo(pData->byDeskStation), isMe);	
	}

	void GameTableLogic::dealGenZhuResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_UserGenZhuResult), "size error");
		S_C_UserGenZhuResult* pData = (S_C_UserGenZhuResult*)object;

		// 数据备份
		_ZongXiaZhu                       = pData->i64AllZongZhu;
		_UserState[pData->byDeskStation]  = pData->iUserState;		
		_XiaZhuData[pData->byDeskStation] = pData->i64UserZongZhu[pData->byDeskStation];

		if (pData->byDeskStation == _mySeatNo)
		{
			_MyZongXiaZhu = pData->i64UserZongZhu[pData->byDeskStation];
			// 显示自己总下注
			//_uiCallback->showMyTotalNote(pData->i64UserZongZhu[pData->byDeskStation]);
		}
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (nullptr != pUser)
			{
				_gameUICallback->showUserTotalBetMoney(i, pData->i64UserZongZhu[i]);
			}
		}
		

		// 显示跟注
		_gameUICallback->showUserFollow(logicToViewSeatNo(pData->byDeskStation));
	
		_gameUICallback->showUserNoteMoney(logicToViewSeatNo(pData->byDeskStation), pData->i64FollowNum/*,_DiZhu*/);

		// 显示总下注
		_gameUICallback->showTotalNote(_ZongXiaZhu);

		//	显示当前下注操作轮数
		_gameUICallback->updateOperateCount(pData->iOperateCount[0], pData->iOperateCount[1]);
	}

	void GameTableLogic::dealJiaZhuResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_JiaZhuResult), "size error");
		S_C_JiaZhuResult* pData = (S_C_JiaZhuResult*)object;

		// 备份数据
		_ZongXiaZhu                       = pData->i64AllZongZhu;
		_UserState[pData->byDeskStation]  = pData->iUserState;
		_XiaZhuData[pData->byDeskStation] = pData->i64UserZongZhu[pData->byDeskStation];

		if (pData->byDeskStation == _mySeatNo)
		{
			_MyZongXiaZhu += pData->i64AddNum;
			// 显示自己总下注
			//_uiCallback->showMyTotalNote(_MyZongXiaZhu);
		}
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (nullptr != pUser)
			{
				_gameUICallback->showUserTotalBetMoney(i, pData->i64UserZongZhu[i]);
			}
		}
		

		// 显示加住
		_gameUICallback->showUserNote(logicToViewSeatNo(pData->byDeskStation));
		// 显示加注
		_gameUICallback->showUserNoteMoney(logicToViewSeatNo(pData->byDeskStation), pData->i64AddNum/*,_DiZhu*/);

		// 显示总下注
		_gameUICallback->showTotalNote(pData->i64AllZongZhu);

		//	显示当前下注操作轮数
		_gameUICallback->updateOperateCount(pData->iOperateCount[0], pData->iOperateCount[1]);

	}

	void GameTableLogic::dealBiPaiResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_UserBiPaiResult), "size error");
		S_C_UserBiPaiResult* pData = (S_C_UserBiPaiResult*)object;

		// 数据备份
		_ZongXiaZhu = pData->i64AllZongZhu;
		_XiaZhuData[pData->byDeskStation] = pData->i64UserZongZhu;
		memcpy(_UserState, pData->iUserStation, sizeof(_UserState));

		if (pData->byDeskStation == _mySeatNo)
		{
			_MyZongXiaZhu += pData->i64AddNum;
        }
        //更新自己的筹码
        _gameUICallback->showUserTotalBetMoney(pData->byDeskStation, pData->i64UserZongZhu);
        
		// 显示下注
		_gameUICallback->showUserNoteMoney(logicToViewSeatNo(pData->byDeskStation), pData->i64AddNum/*, _DiZhu*/);

		// 显示总下注
		_gameUICallback->showTotalNote(_ZongXiaZhu);

		// 显示比牌结果
		_gameUICallback->showCompareResult(logicToViewSeatNo(pData->byWinDesk), logicToViewSeatNo(pData->byLostDesk));
	}

	void GameTableLogic::dealGiveUpResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_UserGiveUpResult), "size error");
		S_C_UserGiveUpResult* pData = (S_C_UserGiveUpResult*)object;

		// 备份数据
		_UserState[pData->byDeskStation] = pData->iUserState;

		// 玩家弃牌
		_gameUICallback->showUserGiveUp(logicToViewSeatNo(pData->byDeskStation));
	}
    
    void GameTableLogic::dealUserLackMoneyTipResp(void *object, INT objectSize) {
        CCAssert(objectSize == sizeof(S_C_UserLackMoneyTip), "size error");
        S_C_UserLackMoneyTip* pData = (S_C_UserLackMoneyTip*)object;
        if (pData) {
            _gameUICallback->showUserLackMoneyTip(pData->byDeskStation);
        }
    }

	void GameTableLogic::dealBetValueResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(TMSG_GAME_NOTE_NTF), "size error");
		TMSG_GAME_NOTE_NTF* pData = (TMSG_GAME_NOTE_NTF*)object;

		//// 备份数据
		//_UserState[pData->byDeskStation] = pData->iUserState;

		//底注
		_gameUICallback->showBaseNote(pData->iDiZhu);//底注
		//下注金额
		_gameUICallback->updateBetButtonValues(pData->iNoteCount);
        
        //显示当前下注操作轮数
        _gameUICallback->updateOperateCount(pData->iOperateCount[0], pData->iOperateCount[1]);
	}
    
    //金币不足退出
    void    GameTableLogic::dealGameTipExit(void *object, INT objectSize)
    {
        CCAssert(objectSize == sizeof(tag_s2c_tip_exit), "size error");
        tag_s2c_tip_exit* pData = (tag_s2c_tip_exit*)object;
        
        _gameUICallback->dealGameTipExit(pData);
    }
    
	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
		_gameStation = pGameInfo->bGameStation;

		if(INVALID_DESKNO != _mySeatNo)
		{
			//_uiCallback->showNotice(GBKToUtf8(pGameInfo->szMessage));
			_gameUICallback->showReady(true);
			_gameUICallback->showNextGame(false);
            _gameUICallback->showReadyPrivate();
		}
	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		refreshParams();

		switch (_gameStation)
		{
		case GS_WAIT_SETGAME:
		case GS_WAIT_ARGEE:
		case GS_WAIT_NEXT:
			{
				CCAssert(sizeof(S_C_GameStation_WaiteAgree) == objectSize, "size error.");
				dealWaitAgreeResp((S_C_GameStation_WaiteAgree*)object);
			}			
			break;
		case GS_SEND_CARD:
			{
				CCAssert(sizeof(S_C_GameStation_SendCard) == objectSize, "size error.");
				dealSendCardResp((S_C_GameStation_SendCard*)object);
			}
			
			break;
		case GS_PLAY_GAME:
			{
				CCAssert(sizeof(S_C_GameStation_PlayGame) == objectSize, "size error.");
				dealPlayGameResp((S_C_GameStation_PlayGame*)object);
			}
			break;
		default:
			break;
		}

		return;
	}

	void GameTableLogic::dealWaitAgreeResp(S_C_GameStation_WaiteAgree* pData)
	{
		// 数据缓存
		_thinkTime = pData->byThinkTime;
		_beginTime = pData->byBeginTime;
		_GuoDi     = pData->iGuoDi;
		_DiZhu     = pData->iDiZhu;
		_DingZhu   = pData->i64DingZhu;
		//_ShangXian = pData->i64ShangXian;
		_MyZongXiaZhu = 0;
		memcpy(_agree, pData->bAgree, sizeof(_agree));

		// 界面显示
		// 显示自己总下注
		//_uiCallback->showMyTotalNote(_MyZongXiaZhu);
		_gameUICallback->showBaseNote(_DiZhu);
//		_uiCallback->showGuoDi(_GuoDi);
//		_uiCallback->showLimitPerNote(_DingZhu);
		//_uiCallback->showLimitNote(_ShangXian);
		_gameUICallback->showTotalNote(0);
		_gameUICallback->showWatchCard(INVALID_SEAT_NO, false);
		_gameUICallback->showGiveUpCard(INVALID_SEAT_NO, false);

		//显示准备标志
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (_agree[i])
			{
				_gameUICallback->showReadySign(logicToViewSeatNo(i), true);
			}
			else
			{
				_gameUICallback->showReadySign(logicToViewSeatNo(i), false);
			}
		}

		if(_mySeatNo != INVALID_DESKNO && !_agree[_mySeatNo])
		{
			_gameUICallback->showWaitTime(_mySeatNo, _thinkTime, _thinkTime);
			//当前玩家是自己才显示时钟
			_gameUICallback->IStartTimer(_thinkTime, 1);
		}

	}

	void GameTableLogic::dealSendCardResp(S_C_GameStation_SendCard* pData)
	{
		// 数据缓存
		_thinkTime  = pData->byThinkTime;
		_beginTime  = pData->byBeginTime;
		_NTSeatNo   = pData->byNtStation;
		_GuoDi      = pData->iGuoDi;
		_DiZhu      = pData->iDiZhu;
		//_ShangXian  = pData->i64ShangXian;
		_DingZhu    = pData->i64DingZhu;
		_ZongXiaZhu = pData->i64AllZongZhu;

		memcpy(_userCardCount, pData->byUserCardCount, sizeof(_userCardCount));
		memcpy(_UserState, pData->iUserState, sizeof(_UserState));
		memcpy(_XiaZhuData, pData->i64XiaZhuData, sizeof(_XiaZhuData));

		// 界面显示
		_gameUICallback->showBaseNote(_DiZhu);
//		_uiCallback->showGuoDi(_GuoDi);
//		_uiCallback->showLimitPerNote(_DingZhu);
		//_uiCallback->showLimitNote(_ShangXian);
		_gameUICallback->showTotalNote(_ZongXiaZhu);
		_gameUICallback->showReady(false);

		//隐藏准备标志
		_gameUICallback->showReadySign(INVALID_SEAT_NO, false);
		_gameUICallback->showWatchCard(INVALID_SEAT_NO, false);
		_gameUICallback->showGiveUpCard(INVALID_SEAT_NO, false);

		// 显示玩家手上的牌
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_userCardCount[i] > 0)
			{
				std::vector<BYTE> cards(_userCardCount[i]);
				for (int j = 0; j < _userCardCount[i];j++)
				{
					cards[j] = 0;
				}
				_gameUICallback->showUserHandCard(logicToViewSeatNo(i), cards);
			}
		}

		// 显示玩家下注的钱
		for(int i = 0 ;i < PLAY_COUNT; i++)
		{
			if(_XiaZhuData[i] > 0 )
			{
				_gameUICallback->showUserNoteMoney(logicToViewSeatNo(i), _XiaZhuData[i]/*,_DiZhu*/);
			}

			if (i == _mySeatNo && (STATE_ERR != pData->iUserState[i]))
			{
				//_uiCallback->showMyTotalNote(_MyZongXiaZhu);			
			}
			else if (i == _mySeatNo && (STATE_ERR == pData->iUserState[i]))
			{
				_gameUICallback->showNextGame(true);
			}

			//记录当前桌子所有玩家的昵称
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (nullptr != pUser)
			{
				_userName[i] = pUser->nickName;
			}
			
		}
	}

	void GameTableLogic::dealPlayGameResp(S_C_GameStation_PlayGame* pData)
	{
		// 数据备份
		_thinkTime        = pData->byThinkTime;
		_beginTime        = pData->byBeginTime;
		_NTSeatNo         = pData->byNtStation;
		_CurrHandleSeatNo = pData->byCurrHandleDesk;
		_GuoDi            = pData->iGuoDi;
		
		_DiZhu            = pData->iDiZhu;
		//_ShangXian        = pData->i64ShangXian;
		_DingZhu          = pData->i64DingZhu;
		_ZongXiaZhu       = pData->i64AllZongZhu;
		
		memcpy(_XiaZhuData, pData->i64XiaZhuData, sizeof(_XiaZhuData));
		memcpy(_userCardCount, pData->byUserCardCount, sizeof(_userCardCount));
		memcpy(_userCard, pData->byUserCard, sizeof(_userCard));
		memcpy(_UserState, pData->iUserState, sizeof(_UserState));


		// 界面显示
		_gameUICallback->showBaseNote(_DiZhu);
//		_uiCallback->showGuoDi(_GuoDi);
//		_uiCallback->showLimitPerNote(_DingZhu);
		//_uiCallback->showLimitNote(_ShangXian);
		_gameUICallback->showTotalNote(_ZongXiaZhu);
		_gameUICallback->showReady(false);
		//隐藏准备标志
		_gameUICallback->showReadySign(INVALID_SEAT_NO, false);

		// 显示倒计时
		stopAllWait();
		_gameUICallback->showWaitTime(logicToViewSeatNo(_CurrHandleSeatNo), pData->iRemainderTime, _thinkTime);

		// 显示玩家手上的牌
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_userCardCount[i] > 0)
			{
				std::vector<BYTE> cards(_userCardCount[i]);
				_gameUICallback->showUserHandCard(logicToViewSeatNo(i), cards);
			}

			if (i == _mySeatNo && (STATE_ERR != pData->iUserState[i]))
			{
				//_uiCallback->showMyTotalNote(_MyZongXiaZhu);
				_gameUICallback->IStartTimer(pData->iRemainderTime);
			}
			else if (i == _mySeatNo && (STATE_ERR == pData->iUserState[i]))
			{
				_gameUICallback->showNextGame(true);
			}

			if (STATE_GIVE_UP == _UserState[i])
			{
				_gameUICallback->showGiveUpCard(logicToViewSeatNo(i), true);
			}
			else if (STATE_LOOK == _UserState[i])
			{
				_gameUICallback->showWatchCard(logicToViewSeatNo(i), true);
			}

			//记录当前桌子所有玩家的昵称
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (nullptr != pUser)
			{
				_userName[i] = pUser->nickName;
			}
		}

		// 显示操作按钮
		_CanLook   = pData->bCanLook;
		_CanFollow = pData->bCanFollow;
		_CanOpen   = pData->bCanOpen;
		_CanGiveUp = pData->bCanGiveUp;
		memcpy(_CanAdd, pData->bCanAdd, sizeof(_CanAdd));
		
		_gameUICallback->showDashboard(pData->iUserState[_mySeatNo]);
		_gameUICallback->setLookVisible(_CanLook && (pData->iUserState[_mySeatNo]));
		_gameUICallback->setFollowVisible(_CanFollow && (pData->iUserState[_mySeatNo]));
		_gameUICallback->setAddVisible(pData->iUserState[_mySeatNo], _CanAdd);
		_gameUICallback->setOpenVisible(_CanOpen && (pData->iUserState[_mySeatNo]));
		_gameUICallback->setGiveUpVisible(_CanGiveUp && (pData->iUserState[_mySeatNo]));
	}



	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{
		if(agree->bAgreeGame == 1)
		{
			_gameUICallback->showWaitTime(logicToViewSeatNo(agree->bDeskStation), 0, 0);
			_gameUICallback->showReadySign(logicToViewSeatNo(agree->bDeskStation), true);
            _gameUICallback->storeReadyState(logicToViewSeatNo(agree->bDeskStation), true);
			//当前玩家是自己才显示时钟
			if (agree->bDeskStation == _mySeatNo)
			{
				_gameUICallback->IStartTimer(0);
			}
		}
	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
		_gameUICallback->clearDesk();
	}

	void GameTableLogic::dealGameEndResp(BYTE deskNo)
	{
		_deskUserList->transform([](UserInfoStruct* user, INT index) 
		{
			if (user->bUserState != USER_WATCH_GAME)
			{
				user->bUserState = USER_SITTING;
			}

			if (RoomLogic()->getRoomRule() & GRR_CONTEST)
			{
				user->bDeskNO      = INVALID_DESKNO;
				user->bDeskStation = INVALID_DESKSTATION;
				user->bUserState   = USER_LOOK_STATE;						
			}
		});
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		BYTE vSeatNo = logicToViewSeatNo(userSit->bDeskStation);

		if (_existPlayer[userSit->bDeskStation])
		{		
			_gameUICallback->showUser(vSeatNo, PlatformLogic()->loginResult.dwUserID == userSit->dwUserID, user->bBoy);
		}

        CHAR nickName[61] = {0};
        memcpy(nickName,user->nickName,sizeof(nickName));
		_gameUICallback->showUserName(vSeatNo, nickName);
		_gameUICallback->showUserMoney(vSeatNo, user->i64Money);
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		_gameUICallback->showUserUp(logicToViewSeatNo(userSit->bDeskStation), userSit->bDeskStation == _mySeatNo);
		_gameUICallback->showReadySign(logicToViewSeatNo(userSit->bDeskStation), false);
		_gameUICallback->showWatchCard(logicToViewSeatNo(userSit->bDeskStation), false);
		_gameUICallback->showGiveUpCard(logicToViewSeatNo(userSit->bDeskStation), false);
		if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
            stop();
			_gameUICallback->leaveTheGame();
		}
		else
		{
			_gameUICallback->clearInvalidUser();
		}
	}

	/************************line*********************************/

	void GameTableLogic::sendStandUp()
	{
		do 
		{
			if (!RoomLogic()->connected())
			{
                stop();
				_gameUICallback->leaveTheGame();
				break;
			}

			if (INVALID_DESKNO == _mySeatNo)
			{
                stop();
				_gameUICallback->leaveTheGame();
				break;
			}

			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(_mySeatNo);
			if(myInfo != nullptr && myInfo->bUserState == USER_PLAY_GAME)
			{
				_gameUICallback->showForbidLeaveNotice();
				break;
			}
			HNGameLogicBase::sendUserUp();
		} while (0);
	}

	void GameTableLogic::sendUserSit(BYTE byDeskStation)
	{
		BYTE lSeatNo = viewToLogicSeatNo(byDeskStation);
		MSG_GR_S_UserSit UserSit;
        memset(&UserSit, 0, sizeof(MSG_GR_S_UserSit));
		UserSit.bDeskIndex   = _deskNo;
		UserSit.bDeskStation = lSeatNo;
        UserSit.nClientFlag = 0;
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &UserSit, sizeof(UserSit));
	}

	//自己坐下
	void GameTableLogic::loadUsers()
	{
		// 桌内玩家
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(nullptr != pUser)
			{				
				seatNo = logicToViewSeatNo(i);
				_gameUICallback->showUser(seatNo, i == _mySeatNo, pUser->bBoy);

				_gameUICallback->showUserName(seatNo,pUser->nickName);
				_gameUICallback->showUserMoney(seatNo,pUser->i64Money);
			}
		}


		////玩家在的时候，旋转
		//if(_mySeatNo != INVALID_DESKNO)
		//{
		//	//setTableOffsetStation(_mySeatNo);
		//}
		//else
		//{
		//	_uiCallback->showNotice(GBKToUtf8("买入成功才能进入牌局。"));
		//}

		sendGameInfo();
		
		//显示牌桌信息
		//_uiCallback->showTableInfo(RoomLogic()->getSelectedRoom()->szGameRoomName);		
	}

	void GameTableLogic::clearDesk()
	{
		if(_mySeatNo != INVALID_DESKNO)
		{
			memset(_CanAdd, 0, sizeof(_CanAdd));
			_gameUICallback->showDashboard(false);
			_gameUICallback->setLookVisible(false);
			_gameUICallback->setFollowVisible(false);
			_gameUICallback->setAddVisible(false,_CanAdd);
			_gameUICallback->setOpenVisible(false);
			_gameUICallback->setGiveUpVisible(false);
		}
	}

	LLONG GameTableLogic::getMinBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubMinMoney;
		return 0;
	}

	LLONG GameTableLogic::getMaxBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubMaxMoney;
		return 0;
	}

	LLONG GameTableLogic::getPlanBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubPlanMoney;
		return 0;
	}

	BYTE  GameTableLogic::getMeStation()
	{
		//return _tableInfo.byMeStation;
		return 0;
	}

	void GameTableLogic::sendNote(LLONG note)
	{
		//tagUserProcess process;
		//process.iVrebType = ACTION_ADD;
		//process.iNote = note;
		//process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		//RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, S_C_CALL_ACTION, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendAdd(LLONG note)
	{
		//tagUserProcess process;
		//if(_curNote > 0)
		//{
		//	process.iVrebType = ACTION_ADD;
		//	process.iNote = note;// _curNote;
		//}
		//else
		//{
		//	process.iVrebType = ACTION_ADD;
		//	process.iNote = note;
		//}
		//if(process.iNote > _tableInfo.iLimtePerNote)
		//{
		//	process.iNote = _tableInfo.iLimtePerNote;
		//}
		//process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		//RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, S_C_CALL_ACTION, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendAddBet(int multiple)
	{
		C_S_JiaZhu	TJiaZhu;
		TJiaZhu.byDeskStation = _mySeatNo;
		TJiaZhu.iAddType = multiple;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_JIA_ZHU, &TJiaZhu, sizeof(TJiaZhu));
	}

	void GameTableLogic::sendFollow()
	{
		C_S_UserGenZhu TUserGenZhu;
		TUserGenZhu.byDeskStation = _mySeatNo;

		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_GEN_ZHU, &TUserGenZhu, sizeof(TUserGenZhu));
	}

	void GameTableLogic::sendGiveUp()
	{

		C_S_UserGiveUp TUserGiveUp;
		TUserGiveUp.byDeskStation = _mySeatNo;

		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_GIVE_UP, &TUserGiveUp, sizeof(TUserGiveUp));
	}

	void GameTableLogic::sendLook()
	{
		C_S_LookCard TLookCard;
		TLookCard.byDeskStation = _mySeatNo;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_LOOK_CARD, &TLookCard, sizeof(TLookCard));
	}

	void GameTableLogic::sendCompare(BYTE seatNo)
	{

		C_S_UserBiPai	TUserBiPai;
		TUserBiPai.byDeskStation = _mySeatNo;
		TUserBiPai.byBeDeskStation = seatNo;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_BI_PAI, &TUserBiPai, sizeof(TUserBiPai));
	}



	void GameTableLogic::compareCardReq()
	{
		std::vector<bool> seats(PLAY_COUNT);
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
			if (userInfo != nullptr && _UserState[i] && (_UserState[i] != STATE_GIVE_UP) && playingGame(userInfo->bUserState) && (i != _mySeatNo))
			{
				seats[logicToViewSeatNo(i)] = true;
			}
			else
			{
				seats[logicToViewSeatNo(i)] = false;
			}
		}
		_gameUICallback->showCompareOption(seats);
	}

	void GameTableLogic::sendUserReady()
	{
		if (_mySeatNo != INVALID_DESKNO)
		{
			// 操作提示
			stopAllWait();
			_gameUICallback->showWaitTime(logicToViewSeatNo(_mySeatNo), _beginTime, _beginTime);
			_gameUICallback->IStartTimer(_beginTime, 1);
		}
		//_uiCallback->showNotice(GBKToUtf8("点击开始进入下一局。"));

		_gameUICallback->showNextGame(false);
		//_uiCallback->showReady(true);
		_gameUICallback->delayShowReady(true);
		
	}


	bool GameTableLogic::playingGame(int state)
	{
		return (state > GS_WAIT_ARGEE && state <= GS_PLAY_GAME);
	}
	/************************line*********************************/

	void GameTableLogic::initParams()
	{
		//_mySeatNo = INVALID_DESKNO; 
		_waitTime = 0.f;
		memset(_UserState, 0, sizeof(_UserState));
		memset(_userName, 0, sizeof(_userName));
		_MyZongXiaZhu = 0;
	}

	void GameTableLogic::refreshParams()
	{
		//_thinkTime = 0.f;
		//_beginTime = 0.f;
		_GuoDi = 0;
		_DiZhu = 0;
		_DingZhu = 0;
		//_ShangXian = 0;
		_curNote = 0;
		_MyZongXiaZhu = 0;
		memset(_agree, false, sizeof(_agree));
		memset(_UserState, 0, sizeof(_UserState));
	}

	void GameTableLogic::stopAllWait()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			BYTE vSeatNo = logicToViewSeatNo(i);
			_gameUICallback->showWaitTime(vSeatNo, 0 ,0);
			_gameUICallback->IStartTimer(0);
		}
	}

	void GameTableLogic::initGuoDi()
	{
	}

	void GameTableLogic::showUserInfo()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_existPlayer[i])
			{
				BYTE vSeatNo = logicToViewSeatNo(i);
				UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
				if(pUser != nullptr)
				{
					_gameUICallback->showUserName(vSeatNo, pUser->nickName);
					_gameUICallback->showUserMoney(vSeatNo, pUser->i64Money);
				}
			}
		}
	}

	// 显示带入金额弹窗
	// min:最小带入
	// max:最大带入
	// plan:计划带入
	void GameTableLogic::showBuyMoney(LLONG min, LLONG max, LLONG plan)
	{

	}

	BYTE GameTableLogic::getNextUserStation(BYTE byStation)
	{
		return byStation;
	}

	BYTE GameTableLogic::getLastUserStation(BYTE byStation)
	{

		return byStation;
	}

	BYTE GameTableLogic::getGameStatue()
	{
		return _gameStation;
	}
    
    
    void GameTableLogic::sendDriveNianShou(int nUserID)
    {
//        NewYearActivityInGame::GanNianShouRequest msg;
//        msg.nUserID = nUserID;
//        //0活动1游戏
//        msg.bSceneType = 1;
//        
//        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, NewYearActivityInGame::C2S_DRIVE_NIANSHOUR_REQUEST, &msg, sizeof(msg));
    }
    
    UserInfoStruct* GameTableLogic::getUserInfoBySeatNo(BYTE seatNo)
    {
        UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(seatNo);
        return userInfo;
    }

}


