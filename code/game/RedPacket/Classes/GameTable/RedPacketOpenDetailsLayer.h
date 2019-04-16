//
//  RedPacketOpenDetailsLayer.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketOpenDetailsLayer_h
#define RedPacketOpenDetailsLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"
#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketOpenDetailsLayer : public Layer {
    public:
        RedPacketOpenDetailsLayer();
        ~RedPacketOpenDetailsLayer();
        CREATE_FUNC(RedPacketOpenDetailsLayer);
        virtual bool init() override;
        
        void setSenderHead(UserInfoStruct* user);
        void openAddToParent(Layer* parent, Vec2 pos);
        void setPacketInfo(tag_c2s_RobRedPacketReq* info);
        void setOpenPacketDetailInfo(tag_c2s_RedPacketRobLogReq* info);
        void setOpenPacketPlayerDetail(RobRedPacketUserInfo* userInfo);
        
        void setMyHistoryBtnCallBack(std::function<void()> callBack);
        
    private:
        void resetData();
        void createUI();
        
        void closeLayer();
        
        void backButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void myHistoryButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        
    private:
        ImageView*      m_bg;
        UserHead*       m_userHead;
        Text*           m_userName;
        Button*         m_closeButton;
        Text*           m_title;
        Text*           m_tips;
        Text*           m_packetResult;
        Text*           m_packetMoney;
        Text*           m_bombNumber;
        ImageView*      m_bombImage;
        ListView*       m_detailsList;
        Button*         m_myHistoryButton;
        
        tag_c2s_RobRedPacketReq         m_packetInfo;
        tag_c2s_RedPacketRobLogReq      m_openPacketDetailInfo;
        
        
        std::function<void()>   m_myHistoryCallBack;
        
    };
}

#endif /* RedPacketOpenDetailsLayer_h */
