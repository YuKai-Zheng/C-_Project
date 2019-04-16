//
//  RedPacketMyRecordLayer.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketMyRecordLayer_h
#define RedPacketMyRecordLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"
#include "RedPacketMyRecordCellLayer.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketMyRecordLayer : public Layer {
    public:
        RedPacketMyRecordLayer();
        ~RedPacketMyRecordLayer();
        CREATE_FUNC(RedPacketMyRecordLayer);
        virtual bool init() override;
        
        void setSenderHead(UserInfoStruct* user);
        
        void openAddToParent(HNLayer* parent, Vec2 pos);
        void addHistoryCell(RedPacketMyRecordCellLayer* cell);
        void addRobMoney(LLONG money);
        
    private:
        void resetData();
        void createUI();
        
        void closeLayer();
        
        void backButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        
    private:
        ImageView*      m_bg;
        UserHead*       m_userHead;
        Text*           m_userName;
        Button*         m_closeButton;
        Text*           m_title;
        Text*           m_packetMoney;
        ListView*       m_detailsList;
        
        LLONG           m_robMoney;     //自己抢红包的总金额
        
    };
}

#endif /* RedPacketMyRecordLayer_h */
