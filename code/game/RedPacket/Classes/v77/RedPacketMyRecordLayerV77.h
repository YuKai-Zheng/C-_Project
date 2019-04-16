//
//  RedPacketMyRecordLayerV77.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketMyRecordLayerV77_h
#define RedPacketMyRecordLayerV77_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"
#include "RedPacketMyRecordCellLayerV77.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketMyRecordLayerV77 : public Layer {
    public:
        RedPacketMyRecordLayerV77();
        ~RedPacketMyRecordLayerV77();
        CREATE_FUNC(RedPacketMyRecordLayerV77);
        virtual bool init() override;
        
        void setSenderHead(UserInfoStruct* user);
        
        void openAddToParent(HNLayer* parent, Vec2 pos);
        void addHistoryCell(RedPacketMyRecordCellLayerV77* cell);
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

#endif /* RedPacketMyRecordLayerV77_h */
