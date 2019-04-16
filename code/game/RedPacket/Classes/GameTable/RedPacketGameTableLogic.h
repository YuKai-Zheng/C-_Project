#ifndef __RedPacket_GameTableLogic_h__
#define __RedPacket_GameTableLogic_h__

#include "RedPacketGameTableUICallback.h"

namespace RedPacket
{
    class GameTableUICallback;
    
    class GameTableLogic : public HN::HNGameLogicBase
    {
    public:
        GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount);
        virtual ~GameTableLogic();
        virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
        virtual void dealGameStartResp(BYTE bDeskNO);
        virtual void dealGameEndResp(BYTE bDeskNO);
        virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
        virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
        virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
        virtual void dealGameStationResp(void* object, INT objectSize);
        virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);
        
    public:
        //获取user信息 通过座位号
        UserInfoStruct* getUserInfoBySeatNo(BYTE seatNo);
        
        //获取本桌所有用户
        HNGameTableUsersData* getDeskUserList(){return _deskUserList;}
        
        void initTableLogicData();
        
        //获取房间基础信息
        tag_s2c_BaseConfigMsgReq getRoomBaseData();
        
    public:
        //请求红包列表
        void sendRequestPacketList();
        //发送红包
        void sendRedPacketMessage(LLONG packetMoney, int packetPart, int thunderNumber);
        //拆红包请求
        void sendOpenRedPacketMessage(unsigned int packetID);
        //红包被抢信息记录请求
        void sendPacketOpenDetailsMessage(unsigned int packetID);
        //请求个人红包历史信息
        void sendMyPacketHistoryMessage();
        //请求玩家列表
        void sendRequestAllPlayerList();
        //请求房间基础信息
        void sendRoomBaseData();
        //请求玩家身上的金币
        void sendRequestMyInfo();
        
    private:
        //接收到红包列表回包消息
        void dealGameGetPacketListNotify();
        //有新红包通知
        void dealGameNewPacketNotify(tag_s2c_RedPacketListReq* object);
        //个人红包历史消息处理
        void dealMyPacketHistory();
        //所有玩家列表
        void dealAllPlayerList();
        //所有领取红包玩家列表
        void dealOpenPacketPlayerList();
        
    private:
        GameTableUICallback* _gameUICalback;
        INT _NTSeatNo;
        
		long long _i64MyMoney;
        ////////////////////new
    public:
        
    private:
        int m_iPlayerCount;
        std::vector<tag_s2c_RedPacketListReq>       m_getPacketList;
        std::vector<tag_s2c_MyHistoryRobLogReq>     m_myPacketHistory;
        std::vector<tag_s2c_PlayerListMsgReq>       m_allPlayerList;
        std::vector<RobRedPacketUserInfo>           m_openPacketPlayerList;
        tag_s2c_BaseConfigMsgReq                    m_roomBaseData;
        
    public:
        void enterGame();
        void loadUsers();
        
		long long getMyMoney();
        
    };
    
}


#endif 
