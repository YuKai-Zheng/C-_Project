#ifndef __RedPacket_GameTableUIV77_h__
#define __RedPacket_GameTableUIV77_h__

#include <string>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "RedPacketGameTableUICallback.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;
using namespace ui;

namespace RedPacket {
    class GameTableLogic;
    class RedPacketGamePacketsListLayer;
    
    class GameTableUIV77 : public HNGameUIBase, public GameTableUICallback {

    private:
        float						_fScaleX;
        float						_fScaleY;
        Size						_tableSize;

        //桌子
//        Sprite*						_tableBg;
//        Sprite*						_tableBgRight;

        Sprite*						_toolbarBg;

        GameNotice*					m_Notice;                   //广播
        
    private:
        ImageView*                  m_topBar;
        Button*                     m_toolBtn;
        Button*                     m_allPlayersBtn;
        Button*                     m_sendPacketBtn;
//        Button*                     m_myPacketHistoryBtn;
        Text*                       m_title;
        ListView*                   m_packetCells;
//        std::function<void()>       m_backCallBack;
        
        
    private:
        //UI类
        RedPacketGamePacketsListLayer*      m_packetsListLayer;
        
    private:
        Text*	m_TextTip;
        long long    m_i64TipMoney;
//        tag_s2c_tip_exit	tipMsg;

//        typedef std::function<void()> ShowBankerCallback;
//        ShowBankerCallback showBankerCallback;
        unsigned int    m_lastRequestMoneyByPacket;     //自己请求金币时触发的红包ID

    public:
        GameTableLogic*			_GameTableLogic;

        //////////////////////////////////////////////////////////////////////////
    public:
        GameTableUIV77();
        ~GameTableUIV77();
        static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

        bool init(BYTE bDeskIndex, bool bAutoCreate);

        virtual bool onTouchBegan(Touch* touch, Event* event) override;
        virtual void onEnter() override;
        virtual void onExit() override;

        virtual void onExitTransitionDidStart()override;
        
    public:
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //添加用户
        virtual void addUser(BYTE seatNo, bool bMe) override;
        //提醒退出
        virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
        //游戏公告
        virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
        //
        virtual void leaveTheGame() override;
        
        //红包相关逻辑更新UI
        //新红包通知
        virtual void onNotifyNewPacketResponse(tag_s2c_RedPacketListReq* packetRes) override;
        //发红包回复
        virtual void onSendPacketResponse(tag_c2s_EmitRedPacketReq* packetRes) override;
        //抢红包回复
        virtual void onOpenPacketResponse(tag_c2s_RobRedPacketReq* packetRes) override;
//        //当前有玩家抢红包的消息
//        virtual void onNotifyPlayerOpenPacketResponse(RobRedPacketUserInfo* packetRes) override;
        //红包被抢的情况消息回复
        virtual void onNotifyPacketOpenDetailsResponse(tag_c2s_RedPacketRobLogReq* packetRes) override;
        //抢红包玩家消息回包
        virtual void onNotifyPacketOpenPlayerDetailResponse(RobRedPacketUserInfo* packetRes) override;
        //个人抢红包历史信息
        virtual void onGetMyPacketHistiryResponse(tag_s2c_MyHistoryRobLogReq* packetRes) override;
        //更新在线玩家列表
        virtual void onUpdateAllPlayerListResponse(std::vector<tag_s2c_PlayerListMsgReq> allPlayer) override;
        //红包领取完或者过期提醒
        virtual void onNotityPacketExpiredMessage(tag_s2c_RobRedPacketOverReq* packetRes) override;
        //更新身上的金币
        virtual void onUpdateMyPacketMoney(LLONG money) override;
        //更新房间基础信息展示
        virtual void onUpdateRoomBaseInfo() override;
        
    public:
        //侧边栏
        void createToolBarUI();
        //红包UI类
        void createPacketsUI();

    private:
        void onBtnClick(Ref* pSender,Control::EventType event);
        void setBtnTouch(Button* btn, bool bTouch);

        void tipCallback();
        void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

        void resetNewGame();
        void resetUINewGame();
        void resetData();
    
        //按钮回调
    private:
        void toolBarButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void allPlayersButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void sendPacketButtonCallBack(Ref* pSender, Widget::TouchEventType type);
//        void myPacketHistoryButtonCallBack(Ref* pSender, Widget::TouchEventType type);

    };
}

#endif
