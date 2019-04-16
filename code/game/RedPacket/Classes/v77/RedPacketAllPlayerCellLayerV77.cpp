//
//  RedPacketAllPlayerCellLayerV77.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 03/08/2017.
//
//

#include "RedPacketAllPlayerCellLayerV77.h"
#include "HNLobby/vip/PlatformResVIP.h"
#include "HNCommon/HNConverCode.h"

namespace RedPacket {
    RedPacketAllPlayerCellLayerV77::RedPacketAllPlayerCellLayerV77() {
        
    }
    
    RedPacketAllPlayerCellLayerV77::~RedPacketAllPlayerCellLayerV77() {
        
    }
    
    bool RedPacketAllPlayerCellLayerV77::init() {
        if (!Button::init()) {
            return false;
        }
                
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketAllPlayerCellLayerV77::resetData() {
        m_userHead = nullptr;
        m_userName = nullptr;
        m_playerID = 0;
        m_contentSize = Size::ZERO;
    }
    
    void RedPacketAllPlayerCellLayerV77::createUI() {
        m_userHead = UserHead::create("v88/module/gerenxinxi/4.png");
        if (m_userHead) {
            m_userHead->setScale(0.75f);
            m_userHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_userHead->setPosition(Vec2(0, 0));
            m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD, "v88/module/gerenxinxi/5.png");
            addChild(m_userHead);
            //
            m_userName = Text::create("我的名字是什么", FONT_TTF_MSYH, 20);
            if (m_userName) {
                m_userName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                m_userName->setPosition(Vec2(m_userHead->getPositionX(), m_userHead->getPositionY() - m_userHead->getContentSize().height * 0.75f * 0.6f));
                m_userName->setColor(Color3B::GRAY);
                addChild(m_userName);
                
                m_contentSize = Size(m_userHead->getContentSize().width*0.8f, m_userHead->getContentSize().height*0.75f*1.5f);
                
                this->setContentSize(m_contentSize);
            }
        }
        
    }
    
    void RedPacketAllPlayerCellLayerV77::setHead(UserInfoStruct* user) {
        if (!user) {
            if (m_userHead)
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD);
            if (m_userName)
                m_userName->setString("");
            return;
        }
        m_playerID = user->dwUserID;
        if (m_userHead)
            m_userHead->setHeadWithInfo(user);
        if (m_userName)
            m_userName->setString(ForceGBKToUtf8(user->nickName));
    }
    
    int RedPacketAllPlayerCellLayerV77::getPlayerID() {
        return m_playerID;
    }
    
    Size RedPacketAllPlayerCellLayerV77::getContentSize() {
        return m_contentSize;
    }
    
}
