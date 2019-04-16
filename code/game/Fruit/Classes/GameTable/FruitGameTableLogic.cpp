#include "FruitGameTableLogic.h"
#include "HNLobby/v66/ExtraReward.h"
//#define SYS_BANKER_STATION				190		//系统坐庄座位号

namespace Fruit
{
    GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount)
    : HNGameLogicBase(bDeskIndex, iPlayerCount, bAutoCreate, uiCallback)
    , _gameUICallback(uiCallback)
	, _i64MyMoney(0)
    {
        _gameStatus = GS_FREE_NOTE;
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
            case GS_FREE_NOTE:
            {
                
                break;
            }
            case GS_KAIJIANG:
            {
                
                break;
            }
            case GS_GUESS:
            {
                
                break;
            }
            case GS_LUCKY:
            {
                
                break;
            }
            case GS_GAME_MACHINE:
            {
                
                break;
            }
            case GS_SETTLEMENT:
            {
                
                break;
            }
            default:
                break;
        }

    }
    
    void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
    {
        if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) // 180
        {
            if (ASS_USER_SIT == messageHead->bAssistantID) {
                //
            }
            else if (ASS_SYNC_DATA == messageHead->bAssistantID) {
                CCASSERT(sizeof(SyncDataStruct) == objectSize, "sizeof(SyncDataStruct) != objectSize");
                if (objectSize != sizeof(SyncDataStruct)) {
                    return;
                }
                SyncDataStruct* msg = (SyncDataStruct*)object;
                _gameStatus = msg->gameState;

				_i64MyMoney = msg->i64MyMoney;
				_gameUICallback->syncBasicDataFirstTime(msg);
                
            }
            else if (ASS_NOTE_RESPONSE == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameNoteResponse) == objectSize, "sizeof(GameNoteResponse) != objectSize");
                if (objectSize != sizeof(GameNoteResponse)) {
                    return;
                }
                GameNoteResponse* msg = (GameNoteResponse*)object;
				_gameStatus = msg->gameState;
				_gameUICallback->onGameNoteResponse(msg);
                
            }
            else if (ASS_NOTE_FAIL == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameNoteFailedMsg) == objectSize, "sizeof(GameNoteFailedMsg) != objectSize");
                if (objectSize != sizeof(GameNoteFailedMsg)) {
                    return;
                }
                GameNoteFailedMsg* msg = (GameNoteFailedMsg*)object;

				_gameUICallback->onGameNoteFail(msg);
                
            }
            else if (ASS_START_FAIL == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameBeginFailLackCoin) == objectSize, "sizeof(GameBeginFailLackCoin) != objectSize");
                if (objectSize != sizeof(GameBeginFailLackCoin)) {
                    return;
                }
                GameBeginFailLackCoin* msg = (GameBeginFailLackCoin*)object;

				_gameUICallback->onGameBeginFailLackCoin(msg);
				
                
            }
            else if (ASS_KAIJIANG_SINGLE == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameLoopResult) == objectSize, "sizeof(GameLoopResult) != objectSize");
                if (objectSize != sizeof(GameLoopResult)) {
                    return;
                }
                GameLoopResult* msg = (GameLoopResult*)object;
                _gameStatus = msg->gameState;
				_gameUICallback->onGameOpenLogo(msg);
                
            }
            else if (ASS_GUESS_NUM == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameNotifyGuessNumber) == objectSize, "sizeof(GameNotifyGuessNumber) != objectSize");
                if (objectSize != sizeof(GameNotifyGuessNumber)) {
                    return;
                }
                GameNotifyGuessNumber* msg = (GameNotifyGuessNumber*)object;
                _gameStatus = msg->gameState;
				_gameUICallback->onGameNotifyGuess(msg);	
                
            }
            else if (ASS_GUESS_NUM_RESPONSE == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameGuessResult) == objectSize, "sizeof(GameGuessResult) != objectSize");
                if (objectSize != sizeof(GameGuessResult)) {
                    return;
                }
                GameGuessResult* msg = (GameGuessResult*)object;
                _gameStatus = msg->gameState;
				_gameUICallback->onGameGuessResult(msg);
				        
            }
            else if (ASS_SWAP_RESPONSE == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameSwapScoreReseponse) == objectSize, "sizeof(GameSwapScoreReseponse) != objectSize");
                if (objectSize != sizeof(GameSwapScoreReseponse)) {
                    return;
                }
                GameSwapScoreReseponse* msg = (GameSwapScoreReseponse*)object;
				_gameUICallback->onGameSwapScoreResponse(msg);
                
            }
            else if (ASS_LUCKY_NOTIFY == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameNotifyLucky) == objectSize, "sizeof(GameNotifyLucky) != objectSize");
                if (objectSize != sizeof(GameNotifyLucky)) {
                    return;
                }
                GameNotifyLucky* msg = (GameNotifyLucky*)object;
                _gameStatus = msg->gameState;
                
            }
            else if (ASS_LUCKY_OPEN == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameLuckyOpen) == objectSize, "sizeof(GameLuckyOpen) != objectSize");
                if (objectSize != sizeof(GameLuckyOpen)) {
                    return;
                }
                GameLuckyOpen* msg = (GameLuckyOpen*)object;
                _gameStatus = msg->gameState;

				_gameUICallback->onGameOpenLuckyLogos(msg);
            }
            else if (ASS_UPDATE_REWARDS == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameRewardsPool) == objectSize, "sizeof(GameRewardsPool) != objectSize");
                if (objectSize != sizeof(GameRewardsPool)) {
                    return;
                }
                GameRewardsPool* msg = (GameRewardsPool*)object;
				_gameUICallback->onGameRewardsPool(msg);

            }
            else if (ASS_GAME_SETTLEMENT == messageHead->bAssistantID) {
                CCASSERT(sizeof(GameSettlementMsg) == objectSize, "sizeof(GameSettlementMsg) != objectSize");
                if (objectSize != sizeof(GameSettlementMsg)) {
                    return;
                }
                GameSettlementMsg* msg = (GameSettlementMsg*)object;
                _gameStatus = msg->gameState;
				_gameUICallback->onGameSettlementMsg(msg);
                
            }
			else if (ASS_LACK_MONEY_OUT == messageHead->bAssistantID)//没钱退出
			{
				CCAssert(sizeof(tag_s2c_tip_exit) == objectSize, "sizeof(tag_s2c_tip_exit) != objectSize");
				if (sizeof(tag_s2c_tip_exit) != objectSize)	return;

				tag_s2c_tip_exit* msg = (tag_s2c_tip_exit*)object;
                _gameStatus = msg->gameState;
				_gameUICallback->onGameTipExit(msg);
			}
			else if (S2C_NOTIFY_MSG == messageHead->bAssistantID)
			{
				CCAssert(sizeof(tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
				if (sizeof(tag_s2c_notify) != objectSize)	return;

				tag_s2c_notify* msg = (tag_s2c_notify*)object;
				_gameUICallback->onGameNotify(msg);
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
    

	void GameTableLogic::sendNote(unsigned int iType)
	{
		if (!(_gameStatus == GS_FREE_NOTE||_gameStatus == GS_XIAZHU))
            return;
        GameXiaZhuRequest msg;
        msg.bDeskStation = _mySeatNo;
        msg.bBetType = iType;
		HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, GS_C_NOTE, &msg, sizeof(GameXiaZhuRequest));
	}

    void GameTableLogic::sendStart()
    {
		if (_gameStatus == GS_FREE_NOTE || _gameStatus == GS_XIAZHU) {
            //开始
            GameBeginRequest msg;
            msg.bDeskStation = _mySeatNo;
            msg.isStartGame = true;
            HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, GS_C_START, &msg, sizeof(GameBeginRequest));
        }
        else if (_gameStatus == GS_GUESS) {
            //下分
            GameSwapScoreRequest msg;
            msg.bDeskStation = _mySeatNo;
            msg.isScoreUp = false;
            HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, GS_C_SWAP_SCORE, &msg, sizeof(GameSwapScoreRequest));
        }
    }
    
    void GameTableLogic::sendAutoGame()
    {
        if (_gameStatus != GS_FREE_NOTE)
            return;
        GameAutoRequest msg;
        msg.bDeskStation = _mySeatNo;
        msg.isAutoGame = true;
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, GS_C_AUTO, &msg, sizeof(GameAutoRequest));

    }
    
    void GameTableLogic::sendGuessNumber(bool isSmall)
    {
        if (_gameStatus != GS_GUESS)
            return;
        GameGuessRequest msg;
        msg.bDeskStation = _mySeatNo;
        msg.isSmallNum = isSmall;
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, GS_C_GUESS_NUM, &msg, sizeof(GameGuessRequest));
    }
    
    void GameTableLogic::sendSwapScore(bool isLeft)
    {
        if (_gameStatus != GS_GUESS)
            return;
        GameSwapScoreRequest msg;
        msg.bDeskStation = _mySeatNo;
        msg.isScoreUp = isLeft;
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, GS_C_SWAP_SCORE, &msg, sizeof(GameSwapScoreRequest));
    }
    
}
