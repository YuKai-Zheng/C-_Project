#include "SHZGameTableLogic.h"
#include "SHZCommonDef.h"
#include "HNLobby/v66/ExtraReward.h"

//#define SYS_BANKER_STATION				190		//系统坐庄座位号

namespace SHZ
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
	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{

	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{

	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{

	}
	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
	}
   
    
    void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
    {
        if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) // 180
        {
            if (messageHead->bAssistantID == S2C_BASE_INFO){
                CCASSERT(sizeof(struct_s2c_base) == objectSize, "sizeof(SyncDataStruct) != objectSize");
                if (objectSize != sizeof(struct_s2c_base)) {
                    return;
                }
                struct_s2c_base* msg = (struct_s2c_base*)object;
                _gameUICallback->syncBasicDataFirstTime(msg);
            }
            else if (messageHead->bAssistantID == S2C_HAS_MARY) //有小玛丽
            {
                CCASSERT(sizeof(struct_s2c_hasMary) == objectSize, "sizeof(struct_s2c_hasMary) != objectSize");
                if (objectSize != sizeof(struct_s2c_hasMary)) {
                    return;
                }
                struct_s2c_hasMary* msg = (struct_s2c_hasMary*)object;
                if (msg->maryTimes > 0) {
                    _gameUICallback->openHasMaryTip(msg);
                }
            }
            else if (messageHead->bAssistantID == S2C_DO_MARY) //小玛丽数据更新
            {
                CCASSERT(sizeof(struct_s2c_maryData) == objectSize, "sizeof(struct_s2c_maryData) != objectSize");
                if (objectSize != sizeof(struct_s2c_maryData)) {
                    return;
                }
                struct_s2c_maryData* msg = (struct_s2c_maryData*)object;
                _gameUICallback->updateMaryData(msg);
            }
            else if (messageHead->bAssistantID == S2C_MARY_FINISH) //小玛丽结束
            {
                CCASSERT(sizeof(struct_s2c_maryFinish) == objectSize, "sizeof(struct_s2c_maryFinish) != objectSize");
                if (objectSize != sizeof(struct_s2c_maryFinish)) {
                    return;
                }
                struct_s2c_maryFinish* msg = (struct_s2c_maryFinish*)object;
                _gameUICallback->maryFinishTip(msg);
            }
            else if (messageHead->bAssistantID == S2C_GAME_START)//开始游戏开奖结果
            {
                CCASSERT(sizeof(struct_s2c_startGame) == objectSize, "sizeof(struct_s2c_startGame) != objectSize");
                if (objectSize != sizeof(struct_s2c_startGame)) {
                    return;
                }
                struct_s2c_startGame* msg1 = (struct_s2c_startGame*)object;

                _gameUICallback->openLineResultData(msg1);
            }
            else if (messageHead->bAssistantID == S2C_DO_CAI_DA_XIAO)//是否可以猜大小
            {
                CCASSERT(sizeof(struct_s2c_caiDaXiao) == objectSize, "sizeof(struct_s2c_caiDaXiao) != objectSize");
                if (objectSize != sizeof(struct_s2c_caiDaXiao)) {
                    return;
                }
                struct_s2c_caiDaXiao* msg2 = (struct_s2c_caiDaXiao*)object;
                _gameUICallback->openLineResultIsCanGuess(msg2);
            }
            else if (messageHead->bAssistantID == S2C_CAI_DA_XIAO)//开始猜大小
            {
                CCASSERT(sizeof(struct_s2c_doCaiDaXiao) == objectSize, "sizeof(struct_s2c_doCaiDaXiao) != objectSize");
                if (objectSize != sizeof(struct_s2c_doCaiDaXiao)) {
                    return;
                }
                struct_s2c_doCaiDaXiao* msg7 = (struct_s2c_doCaiDaXiao*)object;
                _gameUICallback->gueessSizeStar(msg7);
            }
            else if (messageHead->bAssistantID == S2C_CAI_RESULT)
            {
                CCASSERT(sizeof(struct_s2c_caiResult) == objectSize, "sizeof(struct_s2c_caiResult) != objectSize");
                if (objectSize != sizeof(struct_s2c_caiResult)) {
                    return;
                }
                struct_s2c_caiResult* msg3 = (struct_s2c_caiResult*)object;
                _gameUICallback->guessSizeResult(msg3);
            }
            else if (messageHead->bAssistantID == S2C_CAI_FINIISH)
            {
                CCASSERT(sizeof(struct_s2c_caiFinish) == objectSize, "sizeof(struct_s2c_caiFinish) != objectSize");
                if (objectSize != sizeof(struct_s2c_caiFinish)) {
                    return;
                }
                struct_s2c_caiFinish* msg4 = (struct_s2c_caiFinish*)object;
                _gameUICallback->guessSizeOver(msg4);

            }
            else if (messageHead->bAssistantID == S2C_TIP_EXIT_MSG)
            {
                CCASSERT(sizeof(tag_s2c_tip_exit) == objectSize, "sizeof(tag_s2c_tip_exit) != objectSize");
                if (objectSize != sizeof(tag_s2c_tip_exit)) {
                    return;
                }
                tag_s2c_tip_exit* msg5 = (tag_s2c_tip_exit*)object;
                _gameUICallback->onGameTipExit(msg5);

            }
            else if (messageHead->bAssistantID == S2C_GAME_STATE) // 更新游戏状态
            {
                CCASSERT(sizeof(struct_game_state) == objectSize, "sizeof(struct_game_state) != objectSize");
                if (objectSize != sizeof(struct_game_state)) {
                    return;
                }
            
                struct_game_state* msg6 = (struct_game_state*)object;
                _gameStatus = msg6->bGameState;
                _gameUICallback->updateGameState(msg6);
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

   
    

    
	long long GameTableLogic::getMyMoney()
	{
		return _i64MyMoney;
	}
	//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    void GameTableLogic::sendStart(long long betMoney)
    {
		if (_gameStatus == GS_FREE) {
            //开始
			struct_c2s_start msg;
			msg.i64NoteMoney = betMoney;
			HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_GAME_START, &msg, sizeof(struct_c2s_start));
        }

    }
    
	//是否参与比倍
	void GameTableLogic::sendIsGuessSize(int isGuessSize)
	{
		if (_gameStatus == GS_GUESS_MODEL) {
			//开始
			struct_c2s_doCaiDaXiao msg;
			msg.nDo = isGuessSize;
			HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_DO_CAI, &msg, sizeof(struct_c2s_doCaiDaXiao));
		}

	}

	//猜大小
	void GameTableLogic::sendGuessNumber(int guessNum,int isGuess)
	{
		if (_gameStatus == GS_GUESS_MODEL) {
			//开始
			struct_c2s_caiDaXiao msg;
			msg.nCaiType = guessNum;
			msg.nDo = isGuess;
			HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_CAI, &msg, sizeof(struct_c2s_caiDaXiao));
		}

	}
    
	
    
}
