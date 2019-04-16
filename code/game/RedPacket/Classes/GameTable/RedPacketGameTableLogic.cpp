#include "RedPacketGameTableLogic.h"
#include "RedPacketCommonDef.h"
//#define SYS_BANKER_STATION				190		//系统坐庄座位号

namespace RedPacket
{
    GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount)
    : HNGameLogicBase(bDeskIndex, iPlayerCount, bAutoCreate, uiCallback)
    , _gameUICalback(uiCallback)
	, _i64MyMoney(0)
    {
//        _gameStatus = GS_FREE;
        m_iPlayerCount = iPlayerCount;
        m_getPacketList.clear();
        m_myPacketHistory.clear();
        m_allPlayerList.clear();
        m_openPacketPlayerList.clear();
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
        _gameUICalback->userSitDown(userSit, user);
    }
    
    void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
    {
		if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
            stop();
			_gameUICalback->leaveTheGame();
			return;
		}
        _gameUICalback->userUp(userSit, user);
    }
    
    void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
    {
        
    }
    
    void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
    {

        //效验数据
       // switch (_gameStatus)
       // {
//            case GS_FREE:
//            {
//                CCAssert(sizeof(GameStation_Base) == objectSize, "sizeof(GameStation_Base) != objectSize");
//                if (sizeof(GameStation_Base) != objectSize)
//                {
//                    return;
//                }
//                
//                GameStation_Base* msg = (GameStation_Base*)object;
//                
//                _i64MyMoney = msg->i64MyMoney;
//                _gameUICalback->resumeTheGameFree(msg, GS_FREE);
//                
//                break;
        //}


    }
    
    void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
    {
        if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) // 180
        {
            if (C2S_RedEnvelope_RedPacketListMsg == messageHead->bAssistantID) {
                //请求红包列表回复及新红包通知
                tag_s2c_RedPacketListReq *nameInfo = (tag_s2c_RedPacketListReq *)object;
                INT Count = objectSize / sizeof(tag_s2c_RedPacketListReq);
                Count = Count < 0 ? 0 : Count;
                
                int i = 0;
                while (i < Count) {
                    m_getPacketList.push_back(*nameInfo);
                    i++;
                    nameInfo++;
                }
                if (messageHead->bHandleCode == ERR_GP_LIST_FINISH) {
                    //分包结束，开始处理UI逻辑
                    dealGameGetPacketListNotify();
                }
            }
            else if (C2S_RedEnvelope_NewRedPacketMsg == messageHead->bAssistantID) {
                //新红包通知消息
                CCAssert(sizeof(tag_s2c_RedPacketListReq) == objectSize, "sizeof(tag_s2c_RedPacketListReq) != objectSize");
                if (sizeof(tag_s2c_RedPacketListReq) != objectSize)	return;
                
                tag_s2c_RedPacketListReq* packetRes = (tag_s2c_RedPacketListReq*)object;
                dealGameNewPacketNotify(packetRes);
            }
            else if (C2S_RedEnvelope_EmitRedPacketMsg == messageHead->bAssistantID) {
                //发送红包回复包
                CCAssert(sizeof(tag_c2s_EmitRedPacketReq) == objectSize, "sizeof(tag_c2s_EmitRedPacketReq) != objectSize");
                if (sizeof(tag_c2s_EmitRedPacketReq) != objectSize)	return;
                
                tag_c2s_EmitRedPacketReq* packetRes = (tag_c2s_EmitRedPacketReq* )object;
                
                _gameUICalback->onSendPacketResponse(packetRes);
            }
            else if (C2S_RedEnvelope_RobRedPacketMsg == messageHead->bAssistantID) {
                //抢红包回复
                CCAssert(sizeof(tag_c2s_RobRedPacketReq) == objectSize, "sizeof(tag_c2s_RobRedPacketReq) != objectSize");
                if (sizeof(tag_c2s_RobRedPacketReq) != objectSize)	return;
                
                tag_c2s_RobRedPacketReq* packetRes = (tag_c2s_RobRedPacketReq* )object;
                _gameUICalback->onOpenPacketResponse(packetRes);
            }
            else if (C2S_RedEnvelope_RobRedPacketLogMsg == messageHead->bAssistantID) {
                //红包已抢基础信息通知
                CCAssert(sizeof(tag_c2s_RedPacketRobLogReq) == objectSize, "sizeof(tag_c2s_RedPacketRobLogReq) != objectSize");
                if (sizeof(tag_c2s_RedPacketRobLogReq) != objectSize)	return;
                
                tag_c2s_RedPacketRobLogReq* packetRes = (tag_c2s_RedPacketRobLogReq* )object;
                _gameUICalback->onNotifyPacketOpenDetailsResponse(packetRes);
            }
            else if (C2S_RedEnvelope_RobRedPacketLogMsgEx == messageHead->bAssistantID) {
                //红包领取详情单个玩家领取信息
                RobRedPacketUserInfo *nameInfo = (RobRedPacketUserInfo *)object;
                INT Count = objectSize / sizeof(RobRedPacketUserInfo);
                Count = Count < 0 ? 0 : Count;
                
                int i = 0;
                while (i < Count) {
                    m_openPacketPlayerList.push_back(*nameInfo);
                    i++;
                    nameInfo++;
                }
                if (messageHead->bHandleCode == ERR_GP_LIST_FINISH) {
                    //分包结束，开始处理UI逻辑
                    dealOpenPacketPlayerList();
                }
            }
            else if (C2S_RedEnvelope_MyHistoryRobLogMsg == messageHead->bAssistantID) {
                //个人红包历史信息
                tag_s2c_MyHistoryRobLogReq *nameInfo = (tag_s2c_MyHistoryRobLogReq *)object;
                INT Count = objectSize / sizeof(tag_s2c_MyHistoryRobLogReq);
                Count = Count < 0 ? 0 : Count;
                
                int i = 0;
                while (i < Count) {
                    m_myPacketHistory.push_back(*nameInfo);
                    i++;
                    nameInfo++;
                }
                if (messageHead->bHandleCode == ERR_GP_LIST_FINISH) {
                    //分包结束，开始处理UI逻辑
                    dealMyPacketHistory();
                }
            }
            else if (C2S_RedEnvelope_PlayerListMsg == messageHead->bAssistantID) {
                //请求玩家列表
                tag_s2c_PlayerListMsgReq *nameInfo = (tag_s2c_PlayerListMsgReq *)object;
                INT Count = objectSize / sizeof(tag_s2c_PlayerListMsgReq);
                Count = Count < 0 ? 0 : Count;
                
                int i = 0;
                while (i < Count) {
                    m_allPlayerList.push_back(*nameInfo);
                    i++;
                    nameInfo++;
                }
                if (messageHead->bHandleCode == ERR_GP_LIST_FINISH) {
                    //分包结束，开始处理UI逻辑
                    dealAllPlayerList();
                }
            }
            else if (C2S_RedEnvelope_BaseConfigMsg == messageHead->bAssistantID) {
                //房间基础信息
                CCAssert(sizeof(tag_s2c_BaseConfigMsgReq) == objectSize, "sizeof(tag_s2c_BaseConfigMsgReq) != objectSize");
                if (sizeof(tag_s2c_BaseConfigMsgReq) != objectSize)	return;
                
                tag_s2c_BaseConfigMsgReq* packetRes = (tag_s2c_BaseConfigMsgReq* )object;
                m_roomBaseData = *packetRes;
                if (_gameUICalback) {
                    _gameUICalback->onUpdateRoomBaseInfo();
                }
            }
            else if (C2S_RedEnvelope_RobRedPacketOverMsg == messageHead->bAssistantID) {
                //红包过期或者领完
                CCAssert(sizeof(tag_s2c_RobRedPacketOverReq) == objectSize, "sizeof(tag_s2c_RobRedPacketOverReq) != objectSize");
                if (sizeof(tag_s2c_RobRedPacketOverReq) != objectSize)	return;
                
                tag_s2c_RobRedPacketOverReq* packetRes = (tag_s2c_RobRedPacketOverReq* )object;
                if (_gameUICalback) {
                    _gameUICalback->onNotityPacketExpiredMessage(packetRes);
                }
            }
            else if (C2S_RedEnvelope_PlayerWalletMoneyMsg == messageHead->bAssistantID) {
                //请求玩家身上的金币
                CCAssert(sizeof(tag_s2c_PlayerWalletMoneyMsgReq) == objectSize, "sizeof(tag_s2c_PlayerWalletMoneyMsgReq) != objectSize");
                if (sizeof(tag_s2c_PlayerWalletMoneyMsgReq) != objectSize)	return;
                
                tag_s2c_PlayerWalletMoneyMsgReq* packetRes = (tag_s2c_PlayerWalletMoneyMsgReq* )object;
                if (packetRes) {
                    if (_gameUICalback) {
                        _gameUICalback->onUpdateMyPacketMoney(packetRes->i64WalletMoney);
                    }
                }
            }
//            else if (S2C_NOTIFY_MSG == messageHead->bAssistantID)//没钱退出
//			{
//				CCAssert(sizeof(tag_s2c_tip_exit) == objectSize, "sizeof(tag_s2c_tip_exit) != objectSize");
//				if (sizeof(tag_s2c_tip_exit) != objectSize)	return;
//
////				tag_s2c_tip_exit* msg = (tag_s2c_tip_exit*)object;
////				_gameUICalback->onGameTipExit(msg);
//			}
//			
//
        }
    }
    
    void GameTableLogic::initTableLogicData()
    {
        
    }
    
    tag_s2c_BaseConfigMsgReq GameTableLogic::getRoomBaseData() {
        return m_roomBaseData;
    }
    
    UserInfoStruct* GameTableLogic::getUserInfoBySeatNo(BYTE seatNo)
    {
        UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(seatNo);
        return userInfo;
    }
    
    void GameTableLogic::enterGame()
    {
        if (_mySeatNo < 0 && !_autoCreate)
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
                _gameUICalback->addUser(i, i == _mySeatNo);
            }
        }
    }
    
	long long GameTableLogic::getMyMoney()
	{
		return _i64MyMoney;
	}
    
    void GameTableLogic::sendRequestPacketList() {
        
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_RedPacketListMsg);
    }
    
    void GameTableLogic::sendRedPacketMessage(LLONG packetMoney, int packetPart, int thunderNumber) {
        tag_c2s_EmitRedPacketRes packet;
        packet.i64RedPacketMoney = packetMoney;
        packet.nRedPacketPart = packetPart;
        packet.nThunderNumber = thunderNumber;

        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_EmitRedPacketMsg, &packet, sizeof(tag_c2s_EmitRedPacketRes));
    }
    
    void GameTableLogic::sendOpenRedPacketMessage(unsigned int packetID) {
        tag_c2s_RobRedPacketRes packet;
        packet.nRedPacketID = packetID;
        
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_RobRedPacketMsg, &packet, sizeof(tag_c2s_RobRedPacketRes));
    }
    
    void GameTableLogic::sendPacketOpenDetailsMessage(unsigned int packetID) {
        tag_c2s_RedPacketRobLogRes packet;
        packet.nRedPacketID = packetID;
        
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_RobRedPacketLogMsg, &packet, sizeof(tag_c2s_RedPacketRobLogRes));
    }
    
    //抢红包历史信息
    void GameTableLogic::sendMyPacketHistoryMessage() {
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_MyHistoryRobLogMsg);
    }
    
    //请求玩家列表
    void GameTableLogic::sendRequestAllPlayerList() {
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_PlayerListMsg);
    }
    
    //请求房间基础信息
    void GameTableLogic::sendRoomBaseData() {
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_BaseConfigMsg);
    }
    
    //请求玩家身上的金币
    void GameTableLogic::sendRequestMyInfo() {
        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, C2S_RedEnvelope_PlayerWalletMoneyMsg);
    }
    
    void GameTableLogic::dealGameGetPacketListNotify() {
        if (m_getPacketList.size() <= 0) {
            return;
        }
        for (int i = 0; i < m_getPacketList.size(); i++) {
            tag_s2c_RedPacketListReq* packet = &m_getPacketList.at(i);
            if (packet) {
                _gameUICalback->onNotifyNewPacketResponse(packet);
            }
        }
        m_getPacketList.clear();
    }
    
    void GameTableLogic::dealGameNewPacketNotify(tag_s2c_RedPacketListReq* object) {
        if (object == nullptr) {
            return;
        }
        _gameUICalback->onNotifyNewPacketResponse(object);
    }
    
    void GameTableLogic::dealMyPacketHistory() {
        if (m_myPacketHistory.size() <= 0) {
            return;
        }
        for (int i = 0; i < m_myPacketHistory.size(); i++) {
            tag_s2c_MyHistoryRobLogReq* packet = &m_myPacketHistory.at(i);
            if (packet) {
                _gameUICalback->onGetMyPacketHistiryResponse(packet);
            }
        }
        m_myPacketHistory.clear();
    }
    
    void GameTableLogic::dealAllPlayerList() {
        if (m_allPlayerList.size() <= 0) {
            return;
        }
        if (_gameUICalback) {
            _gameUICalback->onUpdateAllPlayerListResponse(m_allPlayerList);
        }
        m_allPlayerList.clear();
    }
    
    void GameTableLogic::dealOpenPacketPlayerList() {
        if (m_openPacketPlayerList.size() <= 0) {
            return;
        }
        if (_gameUICalback) {
            for (int i = 0; i < m_openPacketPlayerList.size(); i++) {
                RobRedPacketUserInfo* packetRes = &m_openPacketPlayerList.at(i);
                if (packetRes) {
                    _gameUICalback->onNotifyPacketOpenPlayerDetailResponse(packetRes);
                }
            }
        }
        m_openPacketPlayerList.clear();
    }
    
}
