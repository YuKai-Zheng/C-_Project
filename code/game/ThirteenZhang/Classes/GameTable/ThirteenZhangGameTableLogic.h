#ifndef __ThirteenZhang_Game_Table_Logic_H__
#define __ThirteenZhang_Game_Table_Logic_H__

//#include "HNLogicExport.h"
//#include "HNRoomLogic/HNGameLogicBase.h"
//#include "HNNetExport.h"
//#include "HNBaseType.h"
#include "ThirteenZhangGameLogic.h"
#include "ThirteenZhangMessageHead.h"
#include <functional>
//#include "NewYearInGameMessageHead.h"
#include "ThirteenZhangGameTableUICallback.h"

namespace ThirteenZhang
{
    class IGameTableUICallback;
    class GameLogic;
    
    typedef std::function<void(bool force)> ccExitCallback;
    
    class GameTableLogic: public HN::HNGameLogicBase
    {
        IGameTableUICallback* _gameUICallback;
        ccExitCallback _exitCB;
        
        bool  _bSuper;
        BYTE _superStation;
        
    public:
        TGameBaseData TableData;
        
    public:
        GameTableLogic(IGameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
        virtual ~GameTableLogic();
        
    public:
        // 是超端
        bool isSuper() const { return _bSuper; }
        const char* getCardTypeName(BYTE type);
        
    public:
//        // 抢庄
//        void sendUserRobNt(bool rob);
        // 开牌
        void sendOpenCard(INT heapCount[3], BYTE heapCard[3][5]);
		//	请求一键摊牌
		void requestManualOpenCard();
        //  通知是否参与比牌
        void requestOpenWithSpecialShape(int specialShape, bool useShape);
        
        /*
         * @func: framework message function.
         * @info: they will be called when frame message come from base class.
         */
    protected:
        virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree) override;
        virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) override;
        virtual void dealGameStationResp(void* object, INT objectSize) override;
        virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
        virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        
        ////////////////////////////////////////////////////////////////////
        ////聊天接口
    protected:
        virtual void dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk) override;
        
    public:
        virtual void sendUserUp(const ccExitCallback& callback);
        virtual void sendForceQuit(const ccExitCallback& callback);
//        void leaveTheGame();
        //VIP房主解散房间
        void sendDissolveRoom(BYTE bDeskIndex);
        
    public:
        virtual void loadDeskUsersUI();
        virtual void clearDesk() override;
        
    public:
        //赶年兽
        void sendDriveNianShou(int nUserID);
        
    };
}


#endif
