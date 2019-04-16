#include "BJLGameTableLogic.h"
#include "HNLobby/v66/ExtraReward.h"

namespace BJL
{

	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool autoCreate)
		: HNGameLogicBase(deskNo, PLAY_COUNT, autoCreate, uiCallback)
		, _NTSeatNo(INVALID_DESKSTATION)
		, _NTMoney(0)
		, _NTPlayCount(0)
		, _NTWinMoney(0)
		, _maxBet(0)
		, _gameStatus(0)
	{
		_gameUICallback = uiCallback;

		_userMoney = PlatformLogic()->loginResult.i64Money;
		_gameUICallback->IUpdateUserMoney(_userMoney);

		memset(_NTList, INVALID_DESKSTATION, sizeof(_NTList));
		memset(_areaMaxBet, 0, sizeof(_areaMaxBet));
	}

	GameTableLogic::~GameTableLogic()
	{

	}

	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{

	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{

	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{

	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, const UserInfoStruct* user)
	{

	}

	//起立
	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		HNLOG("onUserUpMessage");
// 		if(_mySeatNo == INVALID_DESKSTATION)
// 		{
			if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
                stop();
				_gameUICallback->leaveTheGame();
				return;
			}
//		}
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
		_gameStatus = pGameInfo->bGameStation;
	}

	//场景消息
	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		GameStationBase* pData = (GameStationBase*)object;

		_NTSeatNo      = pData->iNtStation;
		_NTPlayCount   = pData->iNtPlayCount;
		_NTMoney       = pData->i64NtMoney;
		_NTWinMoney    = pData->i64NtWinMoney;
		_userWin       = pData->i64UserWin;
		_userMoney     = pData->i64MyMoney;
		_maxBet        = pData->i64UserMaxNote;
		_NTMinLimit    = pData->i64ShangZhuangLimit;
		_betTime       = pData->iXiaZhuTime;
		_openTime      = pData->iKaiPaiTime;
		_freeTime      = pData->iFreeTime;
		_showWinTime   = pData->iShowWinTime;

		memcpy(_NTList, pData->byZhuangList, sizeof(_NTList));
		memcpy(_records, pData->TLuziData, sizeof(_records));

		//更新记录
		int recordCount = getRecords().size();
		int page = recordCount / 14;
		if (recordCount % 14 == 0 && page > 0)
		{
			page--;
		}
		_gameUICallback->refreshRecord(page);

		//更新点数
		_gameUICallback->refreshPoint();

		// 更新庄家列表
		_gameUICallback->IUpdateNTList();

		// 更新庄家信息
		if(_NTSeatNo != INVALID_DESKSTATION)
		{
			auto user = _deskUserList->getUserByDeskStation(_NTSeatNo);
			if(user != nullptr)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				_gameUICallback->IUpdateNTInfo(user->dwUserID, ForceGBKToUtf8(user->nickName), _NTMoney, user->bLogoID, _NTPlayCount);
#else
				_gameUICallback->IUpdateNTInfo(user->dwUserID, GBKToUtf8(user->nickName), _NTMoney, user->bLogoID, _NTPlayCount);
#endif
			}
		}

		// 更新玩家的钱
		_gameUICallback->IUpdateUserMoney(_userMoney);

		// 下注提示
		_gameUICallback->IShowBetHint(_gameStatus == GS_NOTE_STATE, true);

		// 等待提示
		_gameUICallback->IShowWaitHint(_gameStatus == GS_WAIT_SETGAME || _gameStatus == GS_WAIT_AGREE || _gameStatus == GS_WAIT_NEXT);

		switch (_gameStatus)
		{
			case GS_WAIT_SETGAME:       //游戏没有开始状态
				{
					CCAssert(sizeof(GameStationBase) == objectSize, "size error.");

					S_C_NoNtStation data;
					data.i64NtMoney = pData->i64NtMoney;
					data.i64NtWinMoney = pData->i64NtWinMoney;
					data.iNowNtStation = pData->iNtStation;
					data.iNtPlayCount = pData->iNtPlayCount;

					dealNoNTWaitResp(&data, sizeof(data));
				}				
				break;
			case GS_WAIT_AGREE:			//等待玩家开始状态
			case GS_WAIT_NEXT:			//等待下一盘游戏开始
				{
					int n = sizeof(GameStationBase);

					CCAssert(sizeof(GameStationBase) == objectSize, "size error.");
					_gameUICallback->IStartTimer(_freeTime);
				}				
				break;
			case GS_NOTE_STATE:			//下注状态				
				{
					CCAssert(sizeof(GameStation_Bet) == objectSize, "size error.");
					GameStation_Bet* pData = (GameStation_Bet*)object;
					memcpy(_areaBet, pData->i64QuYuZhu, sizeof(_areaBet));
					memcpy(_areaMaxBet, pData->i64AreaMaxZhu, sizeof(_areaMaxBet));

					for (BYTE i = 0 ; i < BET_ARES ; i++)
					{
						if (0 != pData->i64QuYuZhu[i])
						{
							_gameUICallback->IAreaBet(i, pData->i64QuYuZhu[i]);
						}
					}

					//刷新可下值
					_gameUICallback->refreshCanBetNum(_areaMaxBet);

					// 倒计时
					_gameUICallback->IStartTimer(_freeTime);

					// 下注按钮
					int count = 5;
					bool enabledFlag[5];
					for(int i = 0; i < 5; i++)
					{
						enabledFlag[i] = (_userMoney >= G_i64ChouMaValues[i]);
					}
					_gameUICallback->ISetBetEnable(enabledFlag, count);

					for (int i = 0; i < 8; i++)
					{
						_gameUICallback->IDisplayOtherBetMoney(i, pData->i64QuYuZhu[i]);
					}
				}
				break;
			case GS_SEND_CARD:			//下注状态
				{
					CCAssert(sizeof(GameStation_SendCard) == objectSize, "size error.");

					GameStation_SendCard * pData = (GameStation_SendCard *)object;
					memcpy(_areaMaxBet, pData->i64AreaMaxZhu, sizeof(_areaMaxBet));

					//刷新可下值
					_gameUICallback->refreshCanBetNum(_areaMaxBet);

					// 倒计时
					_gameUICallback->IStartTimer(_openTime);

					// 下注按钮
                    const int COUNT = 5;
					bool enabled[COUNT];
					memset(enabled, 0, sizeof(enabled));
					_gameUICallback->ISetBetEnable(enabled, COUNT);

					// 初始化牌
					_gameUICallback->IInitCard(pData->byUserCard, pData->byUserCard[0][2] ? 3 : 2, pData->byUserCard[1][2] ? 3 : 2, pData->iXPaiXing, pData->iZPaiXing);					

					// 发牌
					_gameUICallback->ISendCard(false);

					for (int i = 0; i < 8; i++)
					{
						_gameUICallback->IDisplayOtherBetMoney(i, pData->i64QuYuZhu[i]);
					}
				}
				
				break;
			case GS_SHOW_WIN:			//显示中奖区域
				{
					CCAssert(sizeof(GameStation_ShowWin) == objectSize, "size error.");
					GameStation_ShowWin* pData = (GameStation_ShowWin*)object;

					_roundTotaBetMoney = pData->i64ZhongZhu;
					memcpy(_roundAreaBet, pData->i64QuYuZhu, sizeof(_roundAreaBet));
					memcpy(_userAreaBet, pData->i64UserXiaZhuData, sizeof(_userAreaBet));
					memcpy(_areaMaxBet, pData->i64AreaMaxZhu, sizeof(_areaMaxBet));
					memcpy(_winArea, pData->iWinQuYu, sizeof(_winArea));

					//刷新可下值
					_gameUICallback->refreshCanBetNum(_areaMaxBet);

					// 显示倒计时
					_gameUICallback->IStartTimer(_showWinTime);
					_gameUICallback->IShowWinArea(true);

					for (int i = 0; i < 8; i++)
					{
						_gameUICallback->IDisplayOtherBetMoney(i, pData->i64QuYuZhu[i]);
					}
				}
				break;
		}
	}


	void GameTableLogic::dealApplyNTResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (S_C_ApplyZhuangResult) == objectSize, "size error.");
		
		S_C_ApplyZhuangResult* pData = (S_C_ApplyZhuangResult*)object;
		_NTSeatNo     = pData->iNowNtStation;
		_NTMoney      = pData->i64NtMoney;
		_NTWinMoney   = pData->i64NtWin;
		_NTPlayCount  = pData->iZhuangBaShu;
		memcpy(_NTList, pData->byZhuangList, sizeof(_NTList));

		// 更改上下庄显示
		_gameUICallback->IApplyNT(isInNTList(_mySeatNo));
		
		// 下庄成功
		if(pData->bXiaZhuang && _mySeatNo == pData->byDeskStation)
		{		
			_gameUICallback->IShowNoticeMessage(GBKToUtf8("将在下一局游戏退出庄家位置"));
		}

		// 更新庄家列表
		_gameUICallback->IUpdateNTList();
	}


	void GameTableLogic::dealNoNTWaitResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (S_C_NoNtStation) == objectSize, "size error.");
		S_C_NoNtStation* pData = (S_C_NoNtStation*)object;

		_NTSeatNo    = pData->iNowNtStation;
		_NTMoney     = pData->i64NtMoney;
		_NTWinMoney  = pData->i64NtWinMoney;
		_NTPlayCount = pData->iNtPlayCount;

		_gameUICallback->IApplyNT(isInNTList(_mySeatNo));
		_gameUICallback->IUpdateNTInfo(0, GetText("BJL_Without_Banker"), 0, -1, _NTPlayCount);

		// 隐藏庄家
		_gameUICallback->IStartTimer(0);
		_gameUICallback->onGameStateTip(GS_WAIT_SETGAME);
	}

	void GameTableLogic::dealNoticeBetResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (S_C_GameBegin) == objectSize, "size error.");
		S_C_GameBegin* pData = (S_C_GameBegin*)object;

		// 更新数据
		_gameStatus  = GS_NOTE_STATE;
		_userMoney   = pData->i64UserMoney;
		_NTSeatNo    = pData->iNowNtStation;
		_NTMoney     = pData->i64NtMoney;
		_NTWinMoney  = pData->i64NtWinMoney;
		_NTPlayCount = pData->iNtPlayCount;
		_maxBet      = pData->i64MaxXiaZhu;
		memcpy(_NTList, pData->byZhuangList, sizeof(_NTList));
		memcpy(_areaMaxBet, pData->i64AreaMaxZhu, sizeof(_areaMaxBet));		

		// 显示庄家信息
		std::string name = GBKToUtf8("无庄家");
		int iLogoID = 0;
        int userID = 0;
		if(_NTSeatNo != INVALID_DESKSTATION)
		{
			auto user = _deskUserList->getUserByDeskStation(_NTSeatNo);
			if(user != nullptr)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                name = ForceGBKToUtf8(user->nickName);
#else
				name = GBKToUtf8(user->nickName);
#endif
                userID = user->dwUserID;
                iLogoID = user->bLogoID;
			}
		}		
		_gameUICallback->IUpdateNTInfo(userID, name, _NTMoney, iLogoID, _NTPlayCount);

		//更新累计
		_gameUICallback->refreshBankerTotalWinNum(_NTWinMoney);

		//更新区域可押注值
		_gameUICallback->refreshCanBetNum(_areaMaxBet);

		// 显示申请上、下庄
		_gameUICallback->IApplyNT(isInNTList(_mySeatNo));

		// 更新庄家列表
		_gameUICallback->IUpdateNTList();

		// 倒计时
		_gameUICallback->IStartTimer(_betTime);

		// 下注提示
		_gameUICallback->IShowBetHint(true, false);

		// 等待提示
		_gameUICallback->IShowWaitHint(false);

		// 显示下注按钮
		const int COUNT = 5;
		bool enabled[COUNT];
		memset(enabled, 1, sizeof(enabled));
		_gameUICallback->ISetBetEnable(enabled, COUNT);

		// 下注上限提示
		char str[50] = {0};
		if(_maxBet > 0)
		{
			sprintf(str, "本局下注上限：%lld", _maxBet);
			_gameUICallback->IShowNoticeMessage(GBKToUtf8(str));
		}

		// 游戏开始(顺便重复上局下注)
		_gameUICallback->IGameStart();

		_gameUICallback->onGameStateTip(GS_NOTE_STATE);

		// 待清理
		_gameUICallback->OnHandleBegin(pData);
	}

	void GameTableLogic::dealBetResultResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (C_S_UserNoteResult) == objectSize, "size error.");
		C_S_UserNoteResult* pData = (C_S_UserNoteResult*)object;

		_betSeatNo           = pData->byDeskStation;
		_betArea             = pData->iArea;
		_betMoney            = pData->i64Money;
		_roundTotaBetMoney   = pData->i64ZhongZhu;
		memcpy(_areaBet, pData->i64QuYuZhuTrue, sizeof(_areaBet));
		memcpy(_roundAreaBet, pData->i64QuYuZhu, sizeof(_roundAreaBet));
		memcpy(_userAreaBet, pData->i64UserXiaZhuData, sizeof(_userAreaBet));
		memcpy(_areaMaxBet, pData->i64AreaMaxZhu, sizeof(_areaMaxBet));

		//更新区域可押注值
		_gameUICallback->refreshCanBetNum(_areaMaxBet);

		// 下注动画
		int betType[] = {1000, 10000, 100000, 1000000, 5000000, 10000000, 0 };
		for(int i = 0; i < 7; i++)
		{
			if(pData->i64Money == betType[i])
			{
				_gameUICallback->IPlayNoteAnimation(i, pData->iArea, pData->byDeskStation);
				break;
			}
		}

		// 显示自己的下注额
		if (pData->byDeskStation == _mySeatNo)
		{
			_gameUICallback->IDisplayMyBetMoney(pData->iArea, pData->i64Money);
		}
		for (int i = 0; i < 8; i++)
		{
			_gameUICallback->IDisplayOtherBetMoney(i, pData->i64QuYuZhu[i]);
		}

		// 更新我的金币
		if(pData->byDeskStation == _mySeatNo)
		{
			_userMoney -= pData->i64Money;
			_gameUICallback->IUpdateUserMoney(_userMoney);
            
            // 下注按钮
            int count = 5;
            bool enabledFlag[5];
            for(int i = 0; i < 5; i++)
            {
                enabledFlag[i] = (_userMoney >= G_i64ChouMaValues[i]);
            }
            _gameUICallback->ISetBetEnable(enabledFlag, count);
		}
	}


	void GameTableLogic::dealSendCardResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (S_C_SendCard) == objectSize, "size error.");
		S_C_SendCard* pData = (S_C_SendCard*)object;

		_gameStatus = GS_SEND_CARD;

		memcpy(_userCard, pData->byUserCard, sizeof(_userCard));
		memcpy(_NTCardType, pData->iZPaiXing, sizeof(_NTCardType));
		memcpy(_playerCardType, pData->iXPaiXing, sizeof(_playerCardType));
		memcpy(_winArea, pData->iWinQuYu, sizeof(_winArea));		

		//保存下注
		_gameUICallback->saveBetProcess();

		// 传输牌值
		_gameUICallback->IInitCard(pData->byUserCard, pData->byUserCard[0][2] == 0 ? 2 : 3, pData->byUserCard[1][2] == 0 ? 2 : 3, pData->iXPaiXing, pData->iZPaiXing);

		// 隐藏下注按钮
		const int COUNT = 5;
		bool enabled[COUNT] = {0};
		memset(enabled, 0, sizeof(enabled));
		_gameUICallback->ISetBetEnable(enabled, COUNT);

		// 发牌
		_gameUICallback->ISendCard(true);

		// 下注提示
		_gameUICallback->IShowBetHint(false, false);

		// 等待提示
		_gameUICallback->IShowWaitHint(false);

		// 显示倒计时
		_gameUICallback->IStartTimer(_openTime);

		//移动到新位置
		//_uiCallback->SetImageTimeMoveNewPostion();

		_gameUICallback->onGameStateTip(GS_SEND_CARD);
	}

	void GameTableLogic::dealShowWinAreaResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (S_C_ShowWinAreas) == objectSize, "size error.");
		S_C_ShowWinAreas* pData = (S_C_ShowWinAreas*)object;
		
		_gameStatus = GS_SHOW_WIN;
		memcpy(_winArea, pData->iWinQuYu, sizeof(_winArea));

		// 倒计时
		_gameUICallback->IStartTimer(_showWinTime);
		
		// 开奖面板
		_gameUICallback->IShowOpenBoard(false);

		// 中奖区域
		_gameUICallback->IShowWinArea(true);

		_gameUICallback->onGameStateTip(GS_SHOW_WIN);
		//倒计时移动到原来位置
		//_uiCallback->SetImageTimeMoveOldPostion();

	}

	void GameTableLogic::dealGameEndResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (S_C_GameResult) == objectSize, "size error.");
		S_C_GameResult* pData = (S_C_GameResult*)object;

		_gameStatus     = GS_WAIT_NEXT;
		_NTSeatNo       = INVALID_DESKSTATION;

		_NTToatalScore  = pData->i64NtScoreSum;
		_NTMoney        = pData->i64NtMoney;
		_NTWinMoney     = pData->i64NtWin;
		_userPercent    = pData->fUserPercent;
		_userTotalScore = pData->i64UserScoreSum;
		_userWin        = pData->i64UserWin;
		_userMoney      = pData->i64UserMoney;

		memcpy(_userAreaScore, pData->i64UserAreaScore, sizeof(_userAreaScore));
		memcpy(_NTList, pData->byZhuangList, sizeof(_NTList));
		memcpy(_records, pData->TLuziData, sizeof(_records));

		//更新记录
		int recordCount = getRecords().size();
		int page = recordCount / 14;
		if (recordCount % 14 == 0 && page > 0)
		{
			page--;
		}
		_gameUICallback->refreshRecord(page);

		//更新点数
		_gameUICallback->refreshPoint();

		//更新累计
		_gameUICallback->refreshBankerTotalWinNum(_NTWinMoney);
		_gameUICallback->refreshMyTotalWinNum(_userWin);

		// 结算
		_gameUICallback->showSettlement(pData);

		// 中间区域
		_gameUICallback->IShowWinArea(false);

		// 庄家列表
		_gameUICallback->IUpdateNTList();

		// 倒计时
		_gameUICallback->IStartTimer(_freeTime);

		// 更新我的金币
		if(_NTSeatNo != INVALID_DESKSTATION)
		{
			auto user = _deskUserList->getUserByDeskStation(_NTSeatNo);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			_gameUICallback->IUpdateNTInfo(user->dwUserID, ForceGBKToUtf8(user->nickName), _NTMoney, user->bLogoID, _NTPlayCount);
#else
			_gameUICallback->IUpdateNTInfo(user->dwUserID, GBKToUtf8(user->nickName), _NTMoney, user->bLogoID, _NTPlayCount);
#endif
        }

		// 更新庄家金币
		_gameUICallback->IUpdateUserMoney(_userMoney);

		// 等待提示
		_gameUICallback->IShowWaitHint(true);

	}

	//钱少提醒退出
	void GameTableLogic::dealGameTipExit(void* object, INT objectSize)
	{
		CCAssert(sizeof (tag_s2c_tip_exit) == objectSize, "size error.");
		tag_s2c_tip_exit* pData = (tag_s2c_tip_exit*)object;

		if (pData->bIsExit == 1)
		{
			_gameUICallback->IShowTipExit(pData->i64Money);
		}		
	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch(messageHead->bAssistantID)
		{
		case S_C_APPLY_ZHUANG_RESULT:	// 申请上庄结果
			HNLOG("dealApplyNTResp");
			dealApplyNTResp(object, objectSize);
			break;
		case S_C_NO_NT_WAITE:	        // 无庄等待
			HNLOG("dealNoNTWaitResp");
			dealNoNTWaitResp(object, objectSize);
			break;
		case S_C_NOTICE_XIA_ZHU:        // 游戏开始开始下注
			HNLOG("dealNoticeBetResp");
			dealNoticeBetResp(object, objectSize);
			break;
		case S_C_XIA_ZHU_RESULT:	    // 下注结果
			HNLOG("dealBetResultResp");
			dealBetResultResp(object, objectSize);
			break;
		case S_C_SEND_CARD:		        // 开始发牌
			HNLOG("dealSendCardResp");
			dealSendCardResp(object, objectSize);
			break;
		case S_C_SHOW_WINAREA:	        // 显示中奖区域
			HNLOG("dealShowWinAreaResp");
			dealShowWinAreaResp(object, objectSize);
			break;
		case S_C_GAME_END:		        // 游戏结束结算消息
			HNLOG("dealGameEndResp");
			dealGameEndResp(object, objectSize);
			break;
		case S_C_TIP_EXIT_MSG:
		{
			dealGameTipExit(object, objectSize);
			break;
		}
		case NN::S2C_NOTIFY_MSG:
		{
			 CCAssert(sizeof(NN::tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
			 if (sizeof(NN::tag_s2c_notify) != objectSize)	return;

			 NN::tag_s2c_notify* msg = (NN::tag_s2c_notify*)object;
			 _gameUICallback->onGameNotify(msg);
			 break;
		}
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
//                //年兽
//            case NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG :
//            {
//                //年兽出现 150
//                _uiCallback->onGameShowNianShou();
//            }
//                break;
//            case NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT:
//            {
//                //赶年兽 151
//                CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//                if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)    return;
//                
//                NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//                _uiCallback->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//            }
//                break;
//            case NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG:
//            {
//                //对局获得鞭炮 152
//                CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//                if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)   return;
//                
//                NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//                _uiCallback->onGameMatchGainFirecraker(msg->nUserID);
//            }
//                break;
                
		default:
			HNLOG("AssistID:%u", messageHead->bAssistantID);
		}		
	}

	// 请求下注
	void GameTableLogic::requestBet(int betType, int betArea)
	{
		if (_mySeatNo != INVALID_DESKSTATION && _mySeatNo == _NTSeatNo)
		{
			_gameUICallback->IShowNoticeMessage(GBKToUtf8("庄家不能下注,闲家才能下注!"));
			return ;
		}

		if(_gameStatus != GS_NOTE_STATE)
		{
			_gameUICallback->IShowNoticeMessage(GBKToUtf8("未到下注时间！"));
			return ;
		}
		else if (betType < 0 || betType > 4)//筹码5个
		{
			_gameUICallback->IShowNoticeMessage(GBKToUtf8("请选择下注类型！"));
			return ;
		}

		LLONG betValue[] = {1000, 10000, 100000, 1000000, 5000000, 10000000, 0};
		if (_userMoney < betValue[betType])
		{
			_gameUICallback->IShowNoticeMessage(GBKToUtf8("金币不足！"));
			return ;
		}

		if (betValue[betType] > _areaMaxBet[betArea])
		{
			_gameUICallback->IShowNoticeMessage(GBKToUtf8("本次下注超过当前区域最大下注!"));
			return ;
		}

		sendBet(_deskNo, betType, betArea);
	}

	void GameTableLogic::sendBet(BYTE deskNo, int betType, int betArea)
	{
		C_S_UserNote data;
		data.byDeskStation = deskNo;
		data.iChouMaType   = betType;
		data.iNoteArea     = betArea;

		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_XIA_ZHU, &data, sizeof(C_S_UserNote));
	}

	void GameTableLogic::requestBeNT(bool isNT)
	{
		char str[128] = {0};

		if (_userMoney < _NTMinLimit)
		{
			sprintf(str, GetText("BJL_Request_Banker_Failure"), _NTMinLimit);
			_gameUICallback->IShowNoticeMessage(str);
			return ;
		}		

		// 申请上庄
		if (isNT)
		{
			if(_mySeatNo != INVALID_DESKSTATION && _mySeatNo == _NTSeatNo)
			{
				_gameUICallback->IShowNoticeMessage(GBKToUtf8("您已经是庄家"));
				return;
			}

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				if(_mySeatNo == _NTList[i])
				{
					_gameUICallback->IShowNoticeMessage(GBKToUtf8("您已在庄家列表中，请耐心等待"));
					return ; 
				}
			}

			sendBeNT(true);
		}

		// 申请下庄
		else	
		{
			sendBeNT(false);
		}
	}

	//申请断线重连，或者刚刚进入房间时申请恢复桌子数据包
	void GameTableLogic::requestGameData()
	{
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_APPLY_GAME_DATA);
	}

	vector<string> GameTableLogic::requestNTList()
	{
		std::vector<std::string> ntList;

		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_NTList[i] == INVALID_DESKSTATION)
			{
				break;
			}

			auto user = _deskUserList->getUserByDeskStation(_NTList[i]);
			if(user == nullptr)
			{
				continue;
			}

			ntList.push_back(ForceGBKToUtf8(user->nickName));
		}

		return ntList;		
	}


	std::vector<LuziData> GameTableLogic::getRecords()
	{
		std::vector<LuziData> records;

		for(int i = 0; i < MAXCOUNT; i++)
		{
			if(_records[i].byXpoint != 255 && _records[i].byZPoint != 255)
			{
				records.push_back(_records[i]);
			}
// 			if(records.size() > 15)//只显示最近15条
// 			{
// 				records.erase(records.begin());
// 			}
		}

		return records;
	}

	int* GameTableLogic::getWinArea()
	{
		return _winArea;
	}

	int GameTableLogic::getFreeTime()
	{
		return _freeTime;
	}

	BYTE GameTableLogic::getGameStatus()
	{
		return _gameStatus;
	}

	bool GameTableLogic::isInNTList(BYTE seatNo)
	{
		bool isNT = (seatNo != INVALID_DESKSTATION && seatNo == _NTSeatNo);

		if(!isNT)
		{
			for(int i = 0; i < PLAY_COUNT; i++)
			{
				if(_NTList[i] == INVALID_DESKSTATION)
				{
					break;
				}

				if(_NTList[i] == seatNo)
				{
					isNT = true;
					break;
				}
			}
		}

		return isNT;
	}

	BYTE GameTableLogic::getBankerDeskStation()
	{
		return _NTSeatNo;
	}

	void GameTableLogic::sendBeNT(bool isNT)
	{
		C_S_ApplyShangZhuang data;
		data.bShangZhuang = isNT;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_APPLY_ZHUANG, &data, sizeof(data));
	}


	void GameTableLogic::requestStandUp()
	{
		do 
		{
			// 网络连接断了  直接退出房间列表
			if (!RoomLogic()->connected())
			{
                stop();
				_gameUICallback->leaveTheGame();
				break;
			}
			sendUserUp();
            stop();
			_gameUICallback->leaveTheGame();
		} while (0);
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
    
    
}
