//
//  RedPacketOpenPacketLayerV77.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketOpenPacketLayerV77_h
#define RedPacketOpenPacketLayerV77_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"
#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketOpenPacketLayerV77 : public Layer {
    public:
        RedPacketOpenPacketLayerV77();
        ~RedPacketOpenPacketLayerV77();
        CREATE_FUNC(RedPacketOpenPacketLayerV77);
        virtual bool init() override;
        
        void setSenderHead(UserInfoStruct* user);
        void openAddToParent(HNLayer* parent, Vec2 pos, bool isMe);
        
        void setOpenPacketCallBack(std::function<void()> callBack);
        void setshowPacketDetailCallBack(std::function<void()> callBack);
        
        void setHasRobPacket(bool hasRob);
        void setMyPacketInfo(tag_c2s_RobRedPacketReq* packetInfo);
        
//        void setShouldPlayAnimation(bool playAnimation);
        
    private:
        void resetData();
        void createUI();
        
        void closeLayer();
        
        void backButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void openButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void showDetailButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        
    private:
        EventListenerTouchOneByOne*		touchListener;
        
    private:
        ImageView*      m_bg;
        UserHead*       m_userHead;
        Text*           m_userName;
        Text*           m_packetInfo;
        Button*         m_closeButton;
        Button*         m_openButton;
        Button*         m_showDetailButton;
        Layer*          m_parentLayer;
        std::function<void()>   m_openPacketCallBack;
        std::function<void()>   m_showPacketDetailCallBack;
        bool            m_isMe;
//        bool            m_canPlayAnimation;
        
        tag_c2s_RobRedPacketReq m_myPacketInfo;
    };
}

#endif /* RedPacketOpenPacketLayerV77_h */
