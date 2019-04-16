#include "OnePieceSlotGameTableLogic.h"
#include "base/ccMacros.h"
#include "HNLobby/v66/ExtraReward.h"

namespace OnePieceSlot
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
			else if (S2C_GAME_START == messageHead->bAssistantID)//开始游戏
			{
				CCAssert(sizeof(struct_s2c_startGame) == objectSize, "sizeof(struct_s2c_startGame) != objectSize");
				if (sizeof(struct_s2c_startGame) != objectSize) return;
				 
				struct_s2c_startGame* msg = (struct_s2c_startGame*)object;
				_gameUICallback->onGameStart(msg);
			}
			else if (S2C_DO_CAI_DA_XIAO == messageHead->bAssistantID)//是否猜大小
			{
				CCAssert(sizeof(struct_s2c_doCaiDaXiao) == objectSize, "sizeof(struct_s2c_doCaiDaXiao) != objectSize");
				if (sizeof(struct_s2c_doCaiDaXiao) != objectSize) return;

				struct_s2c_doCaiDaXiao* msg = (struct_s2c_doCaiDaXiao*)object;
				_gameUICallback->onGameDoCaiDaXiao(msg);
			}
			else if (S2C_CAI_DA_XIAO == messageHead->bAssistantID)//猜大小
			{
				CCAssert(sizeof(struct_s2c_caiDaXiao) == objectSize, "sizeof(struct_s2c_caiDaXiao) != objectSize");
				if (sizeof(struct_s2c_caiDaXiao) != objectSize) return;

				struct_s2c_caiDaXiao* msg = (struct_s2c_caiDaXiao*)object;
				_gameUICallback->onGameCaiDaXiao(msg);
			}
			else if (S2C_CAI_RESULT == messageHead->bAssistantID)//本轮猜结果
			{
				CCAssert(sizeof(struct_s2c_caiResult) == objectSize, "sizeof(struct_s2c_caiResult) != objectSize");
				if (sizeof(struct_s2c_caiResult) != objectSize) return;

				struct_s2c_caiResult* msg = (struct_s2c_caiResult*)object;
				_gameUICallback->onGameCaiResult(msg);
			}
			else if (S2C_CAI_FINIISH == messageHead->bAssistantID)//猜大小结束
			{
				CCAssert(sizeof(struct_s2c_caiFinish) == objectSize, "sizeof(struct_s2c_caiFinish) != objectSize");
				if (sizeof(struct_s2c_caiFinish) != objectSize) return;

				struct_s2c_caiFinish* msg = (struct_s2c_caiFinish*)object;
				_gameUICallback->onGameFinish(msg);
			}
			else if (S2C_TIP_EXIT_MSG == messageHead->bAssistantID)//没钱离开
			{
				CCAssert(sizeof(tag_s2c_tip_exit) == objectSize, "sizeof(tag_s2c_tip_exit) != objectSize");
				if (sizeof(tag_s2c_tip_exit) != objectSize) return;

			}
			else if (S2C_UPDATE_POOL_MONEY == messageHead->bAssistantID)//奖池
			{
				CCAssert(sizeof(struct_s2c_pool) == objectSize, "sizeof(struct_s2c_pool) != objectSize");
				if (sizeof(struct_s2c_pool) != objectSize) return;

				struct_s2c_pool* msg = (struct_s2c_pool*)object;
				_gameUICallback->onGamePoolMoney(msg);
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
    
}
