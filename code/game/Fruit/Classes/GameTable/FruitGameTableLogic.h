#ifndef __Fruit_GameTableLogic_h__
#define __Fruit_GameTableLogic_h__

#include "FruitGameTableUICallback.h"

namespace Fruit
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
        //virtual void dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk);
        //virtual void dealSystemMessage(MSG_SystemTalk* pData);
        
    public:
        //获取user信息 通过座位号
        UserInfoStruct* getUserInfoBySeatNo(BYTE seatNo);
        
        //获取本桌所有用户
        HNGameTableUsersData* getDeskUserList(){return _deskUserList;}
        
        void initTableLogicData();
    public:
        
    private:
        GameTableUICallback* _gameUICallback;
        INT _NTSeatNo;
        
		long long _i64MyMoney;
        ////////////////////new
    public:
        
    private:
        int m_iPlayerCount;
        
    public:
        void enterGame();
        void loadUsers();
        
		long long getMyMoney();
    public:
        //发送消息
        //下注
		void sendNote(unsigned int iType);
        //开始
        void sendStart();
        //自动
        void sendAutoGame();
        //猜大小
        void sendGuessNumber(bool isSmall);
        //划分
        void sendSwapScore(bool isLeft);
        
    };
    
}


#endif 
