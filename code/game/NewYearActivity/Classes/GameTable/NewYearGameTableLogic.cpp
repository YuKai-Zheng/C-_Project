#include "NewYearGameTableLogic.h"
//#define SYS_BANKER_STATION				190		//系统坐庄座位号

namespace NewYearActivity
{
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount)
		: HNGameLogicBase(bDeskIndex, iPlayerCount, bAutoCreate, uiCallback)
		, _gameUICalback(uiCallback)
	{
		_gameStatus = Activity_Null;
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
		
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		// 		if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		// 		{
		// 			_gameUICalback->leaveTheGame();
		// 			return;
		// 		}
		//         _gameUICalback->userUp(userSit, user);
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{

	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		//效验数据
	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) // 180
		{
			if (messageHead->bAssistantID == S2C_SignInfo)//签到信息
			{
			/*	CCAssert(sizeof (tag_s2c_SignInfo) == objectSize, "sizeof(tag_s2c_SignInfo) != objectSize");
				if (objectSize != sizeof(tag_s2c_SignInfo)) return;

				tag_s2c_SignInfo* msg = (tag_s2c_SignInfo*)object;
				_gameUICalback->onGameUpdateSignInfo(msg);*/
				
			}
			else if (messageHead->bAssistantID == S2C_SignResult)//签到结果
			{
			/*	CCAssert(sizeof (tag_s2c_SignResult) == objectSize, "sizeof(tag_s2c_SignResult) != objectSize");
				if (objectSize != sizeof(tag_s2c_SignResult)) return;

				tag_s2c_SignResult* msg = (tag_s2c_SignResult*)object;
				_gameUICalback->onGameUpdateSignResult(msg);*/
			}
			else if (messageHead->bAssistantID == S2C_DriveNianShouInfo)//鞭炮信息
			{
			/*	CCAssert(sizeof (tag_s2c_DriveNianShouInfo) == objectSize, "sizeof(tag_s2c_DriveNianShouInfo) != objectSize");
				if (objectSize != sizeof(tag_s2c_DriveNianShouInfo)) return;

				tag_s2c_DriveNianShouInfo* msg = (tag_s2c_DriveNianShouInfo*)object;
				_gameUICalback->onGameUpdateDriveNianShouInfo(msg);*/
			}
			else if (messageHead->bAssistantID == S2C_DRIVE_NIANSHOU_RESULT)//赶年兽结果151
			{
 			/*	CCAssert(sizeof (GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
 				if (objectSize != sizeof(GanNianShouRlt)) return;
 
 				GanNianShouRlt* msg = (GanNianShouRlt*)object;
 				_gameUICalback->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);*/
			}
			else if (messageHead->bAssistantID == C2S_DRIVE_ISTURNTABLE_TODAY)
			{
				CCAssert(sizeof (GanTurn_Today) == objectSize, "sizeof(GanTurn_Today) != GanTurn_Today");
				if (objectSize != sizeof(GanTurn_Today)) return;

				GanTurn_Today* msg = (GanTurn_Today*)object;
				_gameUICalback->onGameTurnTableToday(msg->nResult);
	
			}
			else if (messageHead->bAssistantID == C2S_DRIVE_TURNTABLE_REQUEST)
			{
				CCAssert(sizeof (GanTurntableRlt) == objectSize, "sizeof(GanTurntableRlt) != objectSize");
				if (objectSize != sizeof(GanTurntableRlt)) return;

				GanTurntableRlt* msg = (GanTurntableRlt*)object;
				_gameUICalback->onGameTurnTableResult(msg->nReward);
			}
		}
	}

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

	}

	void GameTableLogic::sendActivityType(int nType)
	{
		if (nType <= Activity_Null)
		{
			return;
		}

		tag_c2s_Activity msg;
		msg.nActivityType = nType;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_ActivityType, &msg, sizeof(msg));
	}

	void GameTableLogic::sendSign()
	{
		tag_c2s_Sign msg;
		msg.nSign = 1;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_Sign, &msg, sizeof(msg));
	}

	void GameTableLogic::sendDriveNianShou(int nUserID, int nType)
	{
//		GanNianShouRequest msg;
//		msg.nUserID = nUserID;
//		//0活动1游戏
//		msg.bSceneType = 0;
//		msg.bPropType = nType;
//
//		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_DRIVE_NIANSHOUR_REQUEST, &msg, sizeof(msg));
	}

	void GameTableLogic::sendTurnTableResult(int userid)
	{
		GanTurntableRlt msg;
		msg.userId = userid;
		msg.nReward = 0;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_DRIVE_TURNTABLE_REQUEST, &msg, sizeof(msg));
	}

	void GameTableLogic::sendIsPlay(int userid)
	{
		tag_c2s_TurnTableRequest msg;
		msg.nUserID = userid;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_DRIVE_ISTURNTABLE_TODAY, &msg, sizeof(msg));
	}
}
