#include "NNGameTableLogic.h"
#include "HNLobby/v66/ExtraReward.h"

//#define SYS_BANKER_STATION				190		//系统坐庄座位号

namespace NN
{
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount)
		: HNGameLogicBase(bDeskIndex, iPlayerCount, bAutoCreate, uiCallback)
		, _gameUICallback(uiCallback)
	{
		_gameStatus = GS_FREE;
		m_iPlayerCount = iPlayerCount;
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

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		_gameUICallback->userSitDown(userSit, user);
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
            stop();
			_gameUICallback->leaveTheGame();
			return;
		}
		_gameUICallback->userUp(userSit, user);
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{

	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		//效验数据
		switch (_gameStatus)
		{
		case GS_FREE:
		{
						CCAssert(sizeof(tag_s2c_config) == objectSize, "sizeof(tag_s2c_config) != objectSize");
						if (sizeof(tag_s2c_config) != objectSize)
						{
							return;
						}

						tag_s2c_config* msg = (tag_s2c_config*)object;
						_gameUICallback->onGameConfig(msg);

						break;
		}
		case GS_CATCH_BANKER:
		{
								CCAssert(sizeof(tag_s2c_station_catch_banker) == objectSize, "sizeof(tag_s2c_station_catch_banker) != objectSize");
								if (sizeof(tag_s2c_station_catch_banker) != objectSize)
								{
									return;
								}

								tag_s2c_station_catch_banker* msg = (tag_s2c_station_catch_banker*)object;
								_gameUICallback->resumeTheCatchBaker(msg, _gameStatus);
								break;
		}

		case  GS_BET:
		{
						CCAssert(sizeof(tag_s2c_station_bet) == objectSize, "sizeof(tag_s2c_station_bet) != objectSize");
						if (sizeof(tag_s2c_station_bet) != objectSize)
						{
							return;
						}

						tag_s2c_station_bet* msg = (tag_s2c_station_bet*)object;
						_gameUICallback->resumeTheGameBet(msg, _gameStatus);
						break;
		}
		case  GS_SEND_CARD:
		{
							  CCAssert(sizeof(tag_s2c_station_send) == objectSize, "sizeof(tag_s2c_station_send) != objectSize");
							  if (sizeof(tag_s2c_station_send) != objectSize)
							  {
								  return;
							  }

							  tag_s2c_station_send* msg = (tag_s2c_station_send*)object;
							  _gameUICallback->resumeTheGameSendCard(msg, _gameStatus);

							  break;
		}
		default:
			return;
			break;
		}
		//_gameUICallback->resumeTheGame(pcene, _gameStatus);
	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) // 180
		{
			if (S2C_PREPARE_MSG == messageHead->bAssistantID)//准备消息
			{
				CCAssert(sizeof (tag_s2c_prepare) == objectSize, "sizeof(tag_s2c_prepare) != objectSize");
				if (objectSize != sizeof(tag_s2c_prepare)) return;

				tag_s2c_prepare* msg = (tag_s2c_prepare*)object;
				_gameUICallback->onGamePrepare(msg);
			}
			else if (S2C_CATCH_BANK_MSG == messageHead->bAssistantID)//抢庄
			{
				CCAssert(sizeof(tag_s2c_catch_banker) == objectSize, "sizeof(tag_s2c_catch_banker) != objectSize");
				if (sizeof(tag_s2c_catch_banker) != objectSize)	return;

				tag_s2c_catch_banker* msg = (tag_s2c_catch_banker*)object;
				_gameUICallback->onGameCatchBanker(msg);
			}
			else if (S2C_CONFIRM_BANKER == messageHead->bAssistantID)//确定庄家
			{

				CCAssert(sizeof(tag_s2c_confirm_banker_two) == objectSize, "sizeof(tag_s2c_confirm_banker_two) != objectSize");
				if (sizeof(tag_s2c_confirm_banker_two) != objectSize)	return;

				tag_s2c_confirm_banker_two* msg = (tag_s2c_confirm_banker_two*)object;
				_gameUICallback->onGameConfirmBanker(msg);

			}
			else if (S2C_BET_MSG == messageHead->bAssistantID)//下注
			{
				CCAssert(sizeof(tag_s2c_bet_two) == objectSize, "sizeof(tag_s2c_bet_two) != objectSize");
				if (sizeof(tag_s2c_bet_two) != objectSize)	return;

				tag_s2c_bet_two* msg = (tag_s2c_bet_two*)object;
				_gameUICallback->onGameBet(msg);
			}
			else if (S2C_SEND_CARD_MSG == messageHead->bAssistantID)//发牌
			{
				CCAssert(sizeof(tag_s2c_send_card) == objectSize, "sizeof(tag_s2c_send_card) != objectSize");
				if (sizeof(tag_s2c_send_card) != objectSize)	return;

				tag_s2c_send_card* msg = (tag_s2c_send_card*)object;
				_gameUICallback->onGameSendCard(msg);
			}
			else if (S2C_OPEN_OK_MSG == messageHead->bAssistantID)
			{
				CCAssert(sizeof(tag_s2c_open_card_ok) == objectSize, "sizeof(tag_s2c_open_card_ok) != objectSize");
				if (sizeof(tag_s2c_open_card_ok) != objectSize)	return;

				tag_s2c_open_card_ok* msg = (tag_s2c_open_card_ok*)object;
				_gameUICallback->onGameOpenOk(msg);
			}
			else if (S2C_OPEN_CARD_MSG == messageHead->bAssistantID)//开牌
			{
				CCAssert(sizeof(tag_s2c_open_card) == objectSize, "sizeof(tag_s2c_open_card) != objectSize");
				if (sizeof(tag_s2c_open_card) != objectSize)	return;

				tag_s2c_open_card* msg = (tag_s2c_open_card*)object;
				_gameUICallback->onGameOpenCard(msg);
			}
			else if (S2C_RESULT_MSG == messageHead->bAssistantID)//结算
			{
				CCAssert(sizeof(tag_s2c_result) == objectSize, "sizeof(tag_s2c_result) != objectSize");
				if (sizeof(tag_s2c_result) != objectSize)	return;

				tag_s2c_result* msg = (tag_s2c_result*)object;
				_gameUICallback->onGameResult(msg);
			}
			else if (S2C_TIP_EXIT_MSG == messageHead->bAssistantID)//没钱退出
			{
				CCAssert(sizeof(tag_s2c_tip_exit) == objectSize, "sizeof(tag_s2c_tip_exit) != objectSize");
				if (sizeof(tag_s2c_tip_exit) != objectSize)	return;

				tag_s2c_tip_exit* msg = (tag_s2c_tip_exit*)object;
				_gameUICallback->onGameTipExit(msg);
			}
			else if (S2C_NOTIFY_MSG == messageHead->bAssistantID)
			{
				CCAssert(sizeof(tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
				if (sizeof(tag_s2c_notify) != objectSize)	return;

				tag_s2c_notify* msg = (tag_s2c_notify*)object;
				_gameUICallback->onGameNotify(msg);
			}
			else if (S2C_REWARD_POOL == messageHead->bAssistantID)//奖池金额
			{
				CCAssert(sizeof(tag_s2c_reward_pool) == objectSize, "sizeof(tag_s2c_reward_pool) != objectSize");
				if (sizeof(tag_s2c_reward_pool) != objectSize)	return;

				tag_s2c_reward_pool* msg = (tag_s2c_reward_pool*)object;
				_gameUICallback->onGamePoolMoney(msg);
			}
			else if (S2C_REWARD_GET_RECORD == messageHead->bAssistantID)//获奖记录
			{
				CCAssert(sizeof(tag_s2c_reward_get_record) == objectSize, "sizeof(tag_s2c_reward_get_record) != objectSize");
				if (sizeof(tag_s2c_reward_get_record) != objectSize)	return;

				tag_s2c_reward_get_record* msg = (tag_s2c_reward_get_record*)object;
				_gameUICallback->onGameRewardGetRecord(msg);
			}
			else if (S2C_PERSONAL_CONTRIBUTION == messageHead->bAssistantID)//个人贡献
			{
				CCAssert(sizeof(tag_s2c_personal_contribution) == objectSize, "sizeof(tag_s2c_personal_contribution) != objectSize");
				if (sizeof(tag_s2c_personal_contribution) != objectSize)	return;

				tag_s2c_personal_contribution* msg = (tag_s2c_personal_contribution*)object;
				_gameUICallback->onGamePersonalContribution(msg);
			}
			else if (S2C_PALYER_STATE == messageHead->bAssistantID)//自己状态
			{
				CCAssert(sizeof(tag_s2c_player_state) == objectSize, "sizeof(tag_s2c_player_state) != objectSize");
				if (sizeof(tag_s2c_player_state) != objectSize)	return;

				tag_s2c_player_state* msg = (tag_s2c_player_state*)object;
				_gameUICallback->onGameUpdatPlayerState(msg);
			}
            else if (ASS_GR_DISSOLVE_VIPROOM == messageHead->bAssistantID) {
//                CCAssert(sizeof(MSG_GR_S_DissolveVipRoom_Req) == objectSize, "sizeof(MSG_GR_S_DissolveVipRoom_Req) != objectSize");
//                if (sizeof(MSG_GR_S_DissolveVipRoom_Req) != objectSize)	return;
//                
//                MSG_GR_S_DissolveVipRoom_Req* msg = (MSG_GR_S_DissolveVipRoom_Req*)object;
//                if (msg) {
//                    
//                }
			}
			else if (ASS_NOTIFYVIPUSER_WINLOSE == messageHead->bAssistantID)
			{
				CCAssert(sizeof(VipUserWinLoseRlt) == objectSize, "sizeof(VipUserWinLoseRlt) != objectSize");
				if (sizeof(VipUserWinLoseRlt) != objectSize)	return;

				VipUserWinLoseRlt* msg = (VipUserWinLoseRlt*)object;
				_gameUICallback->onGameResultTag(msg->nWinLose);
			}
			else if (ASS_NOTIFYVIPUSER_GAMELOG == messageHead->bAssistantID)
			{
				CCAssert(sizeof(VipRoomPlayerGameLogMsg) == objectSize, "sizeof(VipRoomPlayerGameLogMsg) != objectSize");
				if (sizeof(VipRoomPlayerGameLogMsg) != objectSize)	return;

				VipRoomPlayerGameLogMsg* msg = (VipRoomPlayerGameLogMsg*)object;
				_gameUICallback->onVipGameResultLog(msg->mUserLog);
			}
            else if (messageHead->bAssistantID == ASS_SHOW_LUCKYMONEY)
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
            else if (messageHead->bAssistantID == ASS_LUCKYMONEY_RESULT)
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
		}
		else
		{

		}
	}

	//void GameTableLogic::dealSystemMessage(MSG_SystemTalk* pData)
	//{
	//	_gameUICallback->showSystemWords(pData->message);
	//}

	void GameTableLogic::initTableLogicData()
	{

	}

	UserInfoStruct* GameTableLogic::getUserInfoBySeatNo(BYTE seatNo)
	{
		UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(seatNo);
		return userInfo;
	}

	void GameTableLogic::enterGame()
	{
		if ((-1 == _mySeatNo) && !_autoCreate)
		{
			for (int i = 0; i < m_iPlayerCount; i++)
			{
				sendUserSit(logicToViewSeatNo(i));
				break;
			}
		}
		else
		{
			_seatOffset = -_mySeatNo;
			loadUsers();

			if ((INVALID_DESKSTATION != _mySeatNo) && _autoCreate)
			{
				sendGameInfo();
			}
		}
	}

	void GameTableLogic::loadUsers()
	{
		for (int i = 0; i < m_iPlayerCount; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (pUser != nullptr)
			{
				_gameUICallback->addUser(i, i == _mySeatNo);
			}
		}
	}

	void GameTableLogic::sendPrepare(BYTE bChairID)
	{
		tag_c2s_prepare msg;
		msg.bChairID = bChairID;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_PREPARE, &msg, sizeof(msg));
	}

	void GameTableLogic::sendCatchData(BYTE bChairID, BYTE bIsCatch)
	{
		tag_c2s_catch_banker msg;
		msg.bIsCatch = bIsCatch;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_CATCH_BANK, &msg, sizeof(msg));
	}

	void GameTableLogic::sendBet(BYTE bChairID, long long i64Money)
	{
		tag_c2s_bet_two msg;
		msg.i64BetMoney = i64Money;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_BET, &msg, sizeof(msg));
	}

	void GameTableLogic::sendOpenCard(BYTE bChairID)
	{
		tag_c2s_open_card msg;
		msg.bChairID = bChairID;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_CARD, &msg, sizeof(msg));
	}
    
}
