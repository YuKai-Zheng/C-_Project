#include "PubgGameTableLogic.h"
#include "Pubg/Classes/GameMessage/PubgCommonDef.h"
#include "HNLobby/v66/ExtraReward.h"

namespace Pubg
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
            else if (messageHead->bAssistantID == S2C_FINAL_MODE_RESULT) { //大逃杀
                CCASSERT(sizeof(struct_s2c_finalmode_result) == objectSize, "sizeof(struct_s2c_finalmode_result) != objectSize");
                if (objectSize != sizeof(struct_s2c_finalmode_result)) {
                    return;
                }
                struct_s2c_finalmode_result* msg = (struct_s2c_finalmode_result*)object;
                _gameUICallback->dealGameFinalMode(msg);
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
            else if (messageHead->bAssistantID == S2C_FREE_COUNT)
            {
                CCASSERT(sizeof(struct_s2c_free_count) == objectSize, "sizeof(struct_s2c_free_count) != objectSize");
                if (objectSize != sizeof(struct_s2c_free_count)) {
                    return;
                }
                struct_s2c_free_count* msg = (struct_s2c_free_count*)object;
                _gameUICallback->dealGameFreeCount(msg);
                
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
    void GameTableLogic::sendFinalStart()
    {
        if (_gameStatus == GS_FINAL_MODE) {
            HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_START_FINAL_MODE,nullptr, 0);
        }
        
    }
    
}
