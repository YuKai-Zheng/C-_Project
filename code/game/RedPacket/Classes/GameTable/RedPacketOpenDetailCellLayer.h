﻿//
//  RedPacketOpenDetailCellLayer.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketOpenDetailCellLayer_h
#define RedPacketOpenDetailCellLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"

#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketOpenDetailCellLayer : public Layout {
    public:
        RedPacketOpenDetailCellLayer();
        ~RedPacketOpenDetailCellLayer();
        CREATE_FUNC(RedPacketOpenDetailCellLayer);
        virtual bool init() override;
        
        void setHead(UserInfoStruct* user);
        void setOpenPacketPlayerInfo(RobRedPacketUserInfo* info);
        
        Size getContentSize();
        
    private:
        void resetData();
        void createUI();
        
    private:
        UserHead*       m_userHead;
        Text*           m_userName;
        Text*           m_time;
        Text*           m_packetMoney;
        ImageView*      m_luckyTip;
        ImageView*      m_bombTip;
        ImageView*      m_line;
        Size            m_contentSize;
        
        RobRedPacketUserInfo    m_openPlayerInfo;
    };
}

#endif /* RedPacketOpenDetailCellLayer_h */