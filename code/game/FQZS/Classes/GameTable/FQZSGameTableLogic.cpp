#include "FQZSGameTableLogic.h"
#include "HNLobby/v66/ExtraReward.h"

#define SYS_BANKER_STATION				190		//系统坐庄座位号

namespace FQZS
{
    GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount)
    : HNGameLogicBase(bDeskIndex, iPlayerCount, bAutoCreate, uiCallback)
    , _gameUICallback(uiCallback)
	, _i64MyMoney(0)
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
                CCAssert(sizeof(GameStation_Base) == objectSize, "sizeof(GameStation_Base) != objectSize");
				if (sizeof(GameStation_Base) != objectSize)
                {
                    return;
                }
                
				GameStation_Base* msg = (GameStation_Base*)object;

				_i64MyMoney = msg->i64MyMoney;
				_gameUICallback->resumeTheGameFree(msg, GS_FREE);
                
                break;
            }
			case  GS_XIAZHU:
			{
				CCAssert(sizeof(GameStation_Bet) == objectSize, "sizeof(GameStation_Bet) != objectSize");
				if (sizeof(GameStation_Bet) != objectSize)
				{
					return;
				}

				GameStation_Bet* msg = (GameStation_Bet*)object;

				_i64MyMoney = msg->i64MyMoney;
				_gameUICallback->resumeTheGameBet(msg, GS_XIAZHU);

				break;
			}
			case  GS_XIAZHU_FINISH:
			case  GS_KAIJIANG:
			{
								 CCAssert(sizeof(GameStation_Kaijiang) == objectSize, "sizeof(GameStation_Kaijiang) != objectSize");
								 if (sizeof(GameStation_Kaijiang) != objectSize)
								 {
									 return;
								 }

								 GameStation_Kaijiang* msg = (GameStation_Kaijiang*)object;

								 _i64MyMoney = msg->i64MyMoney;
								 _gameUICallback->resumeTheGameKaiJiang(msg, GS_KAIJIANG);

				break;
			}
			case  GS_GAME_FINISH:
			{
									CCAssert(sizeof(GameStation_Base) == objectSize, "sizeof(GameStation_Base) != objectSize");
									if (sizeof(GameStation_Base) != objectSize)
									{
										return;
									}

									GameStation_Base* msg = (GameStation_Base*)object;

									_i64MyMoney = msg->i64MyMoney;
									_gameUICallback->resumeTheGameOver(msg, GS_GAME_FINISH);
									break;
			}
            default:
                return;
                break;
        }
    }
    
    void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
    {
        if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) // 180
        {
            if (ASS_BEGIN == messageHead->bAssistantID)//开始
            {
				CCAssert(sizeof (GameBeginStruct) == objectSize, "sizeof(GameBeginStruct) != objectSize");
				if (objectSize != sizeof(GameBeginStruct)) return;

				GameBeginStruct* msg = (GameBeginStruct*)object;

				_i64MyMoney = msg->i64MyMoney;
				_gameUICallback->onGameBegin(msg);
            }
            else if (ASS_XIAZHU == messageHead->bAssistantID)//通知下注
            {
                CCAssert(sizeof(GameNote) == objectSize, "sizeof(GameNote) != objectSize");
				if (sizeof(GameNote) != objectSize)	return;
                
				GameNote* msg = (GameNote*)object;
                _gameUICallback->onGameBeginBet(msg);
            }
			else if (ASS_XIAZHU_RESULT == messageHead->bAssistantID)//下注结果
            {
				CCAssert(sizeof(GameXiaZhuResponse) == objectSize, "sizeof(GameXiaZhuResponse) != objectSize");
				if (sizeof(GameXiaZhuResponse) != objectSize)	return;

				GameXiaZhuResponse* msg = (GameXiaZhuResponse*)object;

				_i64MyMoney = msg->iUserMoney;
				_gameUICallback->onGameBetResult(msg);
            }
			else if (ASS_PLAY_GAME == messageHead->bAssistantID)//转圈开奖
            {
				CCAssert(sizeof(GamePlay) == objectSize, "sizeof(GamePlay) != objectSize");
				if (sizeof(GamePlay) != objectSize)	return;
                
				GamePlay* msg = (GamePlay*)object;
				//
				_gameUICallback->onGameStartCircle(msg);
            }
			else if (ASS_SHOW_RESULT == messageHead->bAssistantID)//显示开奖结果
			{
				CCAssert(sizeof(GamePlayResult) == objectSize, "sizeof(GamePlayResult) != objectSize");
				if (sizeof(GamePlayResult) != objectSize) return;
				
				GamePlayResult* msg = (GamePlayResult*)object;
				_gameUICallback->onGameShowResult(msg);
			}
            else if (ASS_CONTINUE_END == messageHead->bAssistantID)//结算
            {
                 CCAssert(sizeof(GameEndMsg) == objectSize, "sizeof(GameEndMsg) != objectSize");
				 if (sizeof(GameEndMsg) != objectSize)	return;
                 
				 GameEndMsg* msg = (GameEndMsg*)object;

				 _i64MyMoney = msg->i64UserMoney;
 				_gameUICallback->onGameCalcuResult(msg);
            }
			else if (S2C_TIP_EXIT_MSG == messageHead->bAssistantID)//没钱退出
			{
				CCAssert(sizeof(le_s2c_tip_exit) == objectSize, "sizeof(le_s2c_tip_exit) != objectSize");
				if (sizeof(le_s2c_tip_exit) != objectSize)	return;

				le_s2c_tip_exit* msg = (le_s2c_tip_exit*)object;
				_gameUICallback->onGameTipExit(msg);
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
// 			else if (S2C_NOTIFY_MSG == messageHead->bAssistantID)
// 			{
// 				CCAssert(sizeof(tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
// 				if (sizeof(tag_s2c_notify) != objectSize)	return;
// 
// 				tag_s2c_notify* msg = (tag_s2c_notify*)object;
// 				_gameUICallback->onGameNotify(msg);
// 			}
//            //年兽
//            else if (NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG == messageHead->bAssistantID)//年兽出现 150
//            {
//                _gameUICallback->onGameShowNianShou();
//            }
//            else if (NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT == messageHead->bAssistantID)//赶年兽 151
//            {
//                CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//                if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)    return;
//                
//                NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//                _gameUICallback->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//            }
//            else if (NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG == messageHead->bAssistantID)//对局获得鞭炮 152
//            {
//                CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//                if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)   return;
//                
//                NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//                _gameUICallback->onGameMatchGainFirecraker(msg->nUserID);
//            }
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
        for (int i = 0; i < m_iPlayerCount; i++)
        {
            UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
            if (pUser != nullptr)
            {
                _gameUICallback->addUser(i, i == _mySeatNo);
            }
        }
    }
    
	long long GameTableLogic::getMyMoney()
	{
		return _i64MyMoney;
	}

	void GameTableLogic::sendBet(int iType, long long i64Money)
	{
		GameXiaZhuRequest msg;
		msg.bBetType = iType;
		msg.i64BetMoney = i64Money;

		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_XIAZHU_RESULT, &msg, sizeof(msg));
	}

    void GameTableLogic::sendPrepare(BYTE bChairID)
    {
// 		le_c2s_prepare msg;
//         msg.bChairID = bChairID;
//         
//         HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_PREPARE, &msg, sizeof(msg));
    }
    
    void GameTableLogic::sendCatchData(BYTE bChairID, BYTE bIsCatch)
    {
//         le_c2s_catch_banker msg;
//         msg.bIsCatch = bIsCatch;
//         
//         HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_CATCH_BANK, &msg, sizeof(msg));
    }
    
	//设置红包金额
    void GameTableLogic::sendLEMoney(BYTE bChairID, long long i64Money)
    {
//         le_c2s_envelope_money msg;
//         msg.i64LEMoney = i64Money;
//         
// 		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_MONEY_VALUE, &msg, sizeof(le_c2s_envelope_money));
    }

	//设定雷数字
	void GameTableLogic::sendLEBomb(BYTE bChairID, int iBombValue)
	{
// 		le_c2s_envelope_bomb_value msg;
// 		msg.iLEBombValue = iBombValue;
// 
// 		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_BOMB_VALUE, &msg, sizeof(le_c2s_envelope_bomb_value));
	}

	//闲家拆红包
	void GameTableLogic::sendHasOpenLE(BYTE bChairID)
	{
// 		le_c2s_envelope_has_open msg;
// 		msg.bChairID = bChairID;
// 		if (msg.bChairID != INVALID_BYTE)
// 		{
// 			HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_LE, &msg, sizeof(le_c2s_envelope_has_open));
// 		}
	}
    
	//void GameTableLogic::sendOpenCard(BYTE bChairID)
	//{
	//	tag_c2s_open_card msg;
	//	msg.bChairID = bChairID;

	//	HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_CARD, &msg, sizeof(msg));
	//}
    
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
