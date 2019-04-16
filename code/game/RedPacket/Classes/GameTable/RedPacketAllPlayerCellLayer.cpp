//
//  RedPacketAllPlayerCellLayer.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 03/08/2017.
//
//

#include "RedPacketAllPlayerCellLayer.h"
#include "HNLobby/v88/PlatformResV88.h"
#include "HNCommon/HNConverCode.h"

namespace RedPacket {
    RedPacketAllPlayerCellLayer::RedPacketAllPlayerCellLayer() {
        
    }
    
    RedPacketAllPlayerCellLayer::~RedPacketAllPlayerCellLayer() {
        
    }
    
    bool RedPacketAllPlayerCellLayer::init() {
        if (!Button::init()) {
            return false;
        }
                
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketAllPlayerCellLayer::resetData() {
        m_userHead = nullptr;
        m_userName = nullptr;
        m_playerID = 0;
        m_contentSize = Size::ZERO;
    }
    
    void RedPacketAllPlayerCellLayer::createUI() {
        m_userHead = UserHead::create("v88/module/gerenxinxi/4.png");
        if (m_userHead) {
            m_userHead->setScale(0.75f);
            m_userHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_userHead->setPosition(Vec2(0, 0));
            m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "v88/module/gerenxinxi/5.png");
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
    
    void RedPacketAllPlayerCellLayer::setHead(UserInfoStruct* user) {
        if (!user) {
            if (m_userHead)
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88);
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
    
    int RedPacketAllPlayerCellLayer::getPlayerID() {
        return m_playerID;
    }
    
    Size RedPacketAllPlayerCellLayer::getContentSize() {
        return m_contentSize;
    }
    
}
