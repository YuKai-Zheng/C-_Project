#include "EightSlotGameTableLogic.h"
#include "HNLobby/v66/ExtraReward.h"
//#define SYS_BANKER_STATION                190        //系统坐庄座位号

namespace EightSlot
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

		}
	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) // 180
		{
			if (S2C_GAME_STATE == messageHead->bAssistantID)//状态包
			{
				CCAssert(sizeof(struct_game_state) == objectSize, "sizeof(struct_game_state) != objectSize");
				if (sizeof(struct_game_state) != objectSize) return;

				struct_game_state* msg = (struct_game_state*)object;
				_gameStatus = msg->bGameState;

				_gameUICallback->onGameState();
			}
			else if (S2C_BASE_INFO == messageHead->bAssistantID)//基础消息包
			{
				CCAssert(sizeof(struct_s2c_base) == objectSize, "sizeof(struct_s2c_base) != objectSize");
				if (sizeof(struct_s2c_base) != objectSize) return;

				struct_s2c_base* msg = (struct_s2c_base*)object;
				_gameUICallback->onGameBaseInfo(msg);
			}
			else if (S2C_GAME_RESULT == messageHead->bAssistantID)//开始游戏
			{
				CCAssert(sizeof(struct_s2c_startGame) == objectSize, "sizeof(struct_s2c_startGame) != objectSize");
				if (sizeof(struct_s2c_startGame) != objectSize) return;

				struct_s2c_startGame* msg = (struct_s2c_startGame*)object;
				_gameUICallback->onGameStart(msg);
			}
			else if (S2C_TIP_EXIT_MSG == messageHead->bAssistantID)//没钱离开
			{
				CCAssert(sizeof(tag_s2c_tip_exit) == objectSize, "sizeof(tag_s2c_tip_exit) != objectSize");
				if (sizeof(tag_s2c_tip_exit) != objectSize) return;

			}
            else if(S2C_UPDATE_MONEY == messageHead->bAssistantID) //更新金钱和分数(下分后)
            {
                CCAssert(sizeof(struct_s2c_update_money) == objectSize, "sizeof(struct_s2c_update_money) != objectSize");
                if (sizeof(struct_s2c_update_money) != objectSize) return;
                
                struct_s2c_update_money* msg = (struct_s2c_update_money*)object;
                _gameUICallback->onGameUpdateMoney(msg);
                
                
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
    
}
