#include "TigerGameTableLogic.h"
#include "TigerCommonDef.h"
#include "HNLobby/v66/ExtraReward.h"

namespace Tiger
{
    GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount)
    : HNGameLogicBase(bDeskIndex, iPlayerCount, bAutoCreate, uiCallback)
    , _gameUICallback(uiCallback)
//    , _i64MyMoney(0)
    {
		_gameStatus = GS_FREE;
        m_iPlayerCount = iPlayerCount;
    }
    

	GameTableLogic::~GameTableLogic() {
	}
    
    void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
        _gameUICallback->userSitDown(userSit, user);
    }
    
    void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID) {
			stop();
			_gameUICallback->leaveTheGame();
			return;
		}
        _gameUICallback->userUp(userSit, user);
    }
    
    void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) {
    }
    
	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree) {
	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO) {
	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO) {
	}
    
	void GameTableLogic::dealGameStationResp(void* object, INT objectSize) {
	}
    
    void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
        if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) {
            if (messageHead->bAssistantID == S2C_GAME_STATE){
                CCASSERT(sizeof(struct_s2c_game_state) == objectSize, "sizeof(struct_s2c_game_state) != objectSize");
                if (objectSize != sizeof(struct_s2c_game_state)) {
                    return;
                }
                struct_s2c_game_state* msg = (struct_s2c_game_state*)object;
                _gameStatus = msg->bGameState;
                _gameUICallback->dealGameState(msg);
            }
            else if (messageHead->bAssistantID == S2C_BASE_INFO) {
                CCASSERT(sizeof(struct_s2c_base_info) == objectSize, "sizeof(struct_s2c_base_info) != objectSize");
                if (objectSize != sizeof(struct_s2c_base_info)) {
                    return;
                }
                struct_s2c_base_info* msg = (struct_s2c_base_info*)object;
                _gameUICallback->dealGameBaseInfo(msg);
            }
            else if (messageHead->bAssistantID == S2C_GAME_BEGIN) {
                CCASSERT(sizeof(struct_s2c_game_begin) == objectSize, "sizeof(struct_s2c_game_begin) != objectSize");
                if (objectSize != sizeof(struct_s2c_game_begin)) {
                    return;
                }
                struct_s2c_game_begin* msg = (struct_s2c_game_begin*)object;
                _gameUICallback->dealGameBegin(msg);
            }
            else if (messageHead->bAssistantID == S2C_GAME_FINISH) {
                CCASSERT(sizeof(struct_s2c_game_finish) == objectSize, "sizeof(struct_s2c_game_finish) != objectSize");
                if (objectSize != sizeof(struct_s2c_game_finish)) {
                    return;
                }
                struct_s2c_game_finish* msg = (struct_s2c_game_finish*)object;
                _gameUICallback->dealGameFinish(msg);
            }
            else if (messageHead->bAssistantID == S2C_TIP_EXIT_MSG) {
                CCASSERT(sizeof(tag_s2c_tip_exit) == objectSize, "sizeof(tag_s2c_tip_exit) != objectSize");
                if (objectSize != sizeof(tag_s2c_tip_exit)) {
                    return;
                }
                tag_s2c_tip_exit* msg = (tag_s2c_tip_exit*)object;
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
        }
    }
    
    void GameTableLogic::initTableLogicData() {
        
    }


	void GameTableLogic::loadUsers() {
//        for (int i = 0; i < m_iPlayerCount; i++)
//        {
//            UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
//            if (pUser != nullptr)
//            {
//                _gameUICallback->addUser(i, i == _mySeatNo);
//            }
//        }
	}
    
	void GameTableLogic::enterGame() {
		if ((-1 == _mySeatNo) && !_autoCreate) {
			for (int i = 0; i < m_iPlayerCount; i++)
			{
				sendUserSit(logicToViewSeatNo(i));
				break;
			}
		}
		else {
			_seatOffset = -_mySeatNo;
			loadUsers();

			if ((INVALID_DESKSTATION != _mySeatNo) && _autoCreate)
			{
				sendGameInfo();
			}
		}
	}

//    long long GameTableLogic::getMyMoney() {
//        return _i64MyMoney;
//    }
    
    BYTE GameTableLogic::getGameStatus() {
        return _gameStatus;
    }
    
	//////////////////////////////////////////////////////////////////////////////
    void GameTableLogic::sendStart(struct_c2s_start* msg) {
		if (_gameStatus == GS_FREE && msg != nullptr) {
			HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_START_GAME, msg, sizeof(struct_c2s_start));
        }
    }
    
}
