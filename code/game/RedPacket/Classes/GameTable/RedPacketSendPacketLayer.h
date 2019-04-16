//
//  RedPacketSendPacketLayer.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketSendPacketLayer_h
#define RedPacketSendPacketLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"
#include "UI/HNEditBox.h"
#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketSendPacketLayer : public Layer, public ui::EditBoxDelegate {
    public:
        RedPacketSendPacketLayer();
        ~RedPacketSendPacketLayer();
        CREATE_FUNC(RedPacketSendPacketLayer);
        virtual bool init() override;
        virtual void editBoxReturn(ui::EditBox* editBox) override;
        
        void openAddToParent(HNLayer* parent, Vec2 pos);
        void setBombNumber(int number);
        
        void setSendPacketCallBack(std::function<void(LLONG packetMoney, int packetPart, int thunderNumber)> callBack);
        void updateRoomBaseData(tag_s2c_BaseConfigMsgReq baseData);
        
    private:
        void resetData();
        void createUI();
        
        void closeLayer();
        
        void backButtonCallBack(Ref* pSender, Widget::TouchEventType type);
//        void helpButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void sendButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void setBombCallBack(Ref* pSender, Widget::TouchEventType type);
        
    private:
        ImageView*      m_bg;
        Button*         m_closeButton;
//        Button*         m_helpButton;
        Text*           m_title;
        Button*         m_sendButton;
        ImageView*      m_packetMoneyBG;
        HNEditBox*      m_packetMoneyEditBox;
        ImageView*      m_packetBombBG;
        Button*         m_bombButton;
        Text*           m_rateText;
        Text*           m_packetMoneyText;
        Text*           m_multiText;
        
        std::function<void(LLONG packetMoney, int packetPart, int thunderNumber)>       m_sendPacketCallBack;
        
    private:
        int             m_setBombNumber;
        LLONG           m_setPacketMoney;
        int             m_setPacketPart;
        
        LLONG           m_minMoney;
        LLONG           m_maxMoney;
        
    };
}

#endif /* RedPacketSendPacketLayer_h */
