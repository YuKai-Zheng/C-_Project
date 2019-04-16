//
//  RedPacketMyRecordLayer.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#include "RedPacketMyRecordLayer.h"
#include "HNLobby/v88/PlatformResV88.h"
#include "HNCommon/HNConverCode.h"
#include "RedPacketMyRecordCellLayer.h"
#include "Tool/Tools.h"

//#define RED_WIN_SIZE Size(WIN_SIZE.height, WIN_SIZE.width)

namespace RedPacket {
    
    RedPacketMyRecordLayer::RedPacketMyRecordLayer() {
        
    }
    
    RedPacketMyRecordLayer::~RedPacketMyRecordLayer() {
        
    }
    
    bool RedPacketMyRecordLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        
        this->setContentSize(WIN_SIZE);
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketMyRecordLayer::setSenderHead(UserInfoStruct *user) {
        if (!user) {
            if (m_userHead)
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88);
            if (m_userName)
                m_userName->setString("");
            return;
        }
        if (m_userHead)
            m_userHead->setHeadWithInfo(user);
        if (m_userName)
            m_userName->setString(ForceGBKToUtf8(user->nickName));
    }
    
    void RedPacketMyRecordLayer::openAddToParent(HNLayer *parent, cocos2d::Vec2 pos) {
        if (parent) {
            this->setPosition(pos);
            parent->addChild(this);
        }
    }
    
    void RedPacketMyRecordLayer::addHistoryCell(RedPacket::RedPacketMyRecordCellLayer *cell) {
        if (m_detailsList) {
            m_detailsList->pushBackCustomItem(cell);
        }
    }
    
    void RedPacketMyRecordLayer::addRobMoney(LLONG money) {
        m_robMoney += money;
        if (m_packetMoney) {
            m_packetMoney->setString(Tools::addComma(m_robMoney));
        }
    }
    
    void RedPacketMyRecordLayer::resetData() {
        m_bg = nullptr;
        m_userHead = nullptr;
        m_userName = nullptr;
        m_closeButton = nullptr;
        m_title = nullptr;
        m_packetMoney = nullptr;
        m_detailsList = nullptr;
        m_robMoney = 0;
    }
    
    void RedPacketMyRecordLayer::createUI() {
        m_bg = ImageView::create("v88/fahongbao/jilu/bg.png");
        if (m_bg) {
			m_bg->setContentSize(WIN_SIZE);
			m_bg->setScale9Enabled(true);
            m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_bg->setPosition(WIN_SIZE/2);
            m_bg->setTouchEnabled(true);
            addChild(m_bg);
            //
            m_userHead = UserHead::create("v88/module/gerenxinxi/4.png");
            if (m_userHead) {
                m_userHead->setScale(0.85f);
                m_userHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_userHead->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.78f));
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "v88/module/gerenxinxi/5.png");
                m_bg->addChild(m_userHead);
                //
                m_userName = Text::create("", FONT_TTF_MSYH, 30);
                if (m_userName) {
                    m_userName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                    m_userName->setPosition(Vec2(m_userHead->getPositionX(), m_userHead->getPositionY() - m_userHead->getContentSize().height * 0.85f * 0.6f));
                    m_userName->setColor(Color3B::BLACK);
                    m_bg->addChild(m_userName);
                }
            }
            //
            m_closeButton = Button::create("v88/fahongbao/1.png", "v88/fahongbao/0.png");
            if (m_closeButton) {
                m_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                m_closeButton->setPosition(Vec2(0, m_bg->getContentSize().height));
                m_closeButton->addTouchEventListener(CC_CALLBACK_2(RedPacketMyRecordLayer::backButtonCallBack, this));
                addChild(m_closeButton);
                //
                m_title = Text::create("红包记录", FONT_TTF_MSYH, 32);
                if (m_title) {
                    m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    m_title->setPosition(Vec2(m_closeButton->getPositionX()+m_closeButton->getContentSize().width, m_closeButton->getPositionY()-m_closeButton->getContentSize().height/2));
                    m_title->setColor(Color3B(255, 225, 176));
                    addChild(m_title);
                }
            }
            m_packetMoney = Text::create("", FONT_TTF_MSYH, 110);
            if (m_packetMoney) {
                m_packetMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_packetMoney->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_packetMoney->setTextHorizontalAlignment(TextHAlignment::CENTER);
                m_packetMoney->setPosition(Vec2(WIN_SIZE.width/2, m_bg->getContentSize().height*0.6f));
                m_packetMoney->setColor(Color3B::BLACK);
                addChild(m_packetMoney);
            }
            m_detailsList = ListView::create();
            if (m_detailsList) {
                m_detailsList->setGravity(cocos2d::ui::ListView::Gravity::TOP);
                m_detailsList->setDirection(ListView::Direction::VERTICAL);
                m_detailsList->setContentSize(Size(WIN_SIZE.width, WIN_SIZE.height*0.46f));
                m_detailsList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                m_detailsList->setPosition(Vec2(WIN_SIZE.width/2, 10));
                m_detailsList->setScrollBarAutoHideEnabled(false);
                m_detailsList->setBounceEnabled(true);
                addChild(m_detailsList);
            }
        }
    }
    
    void RedPacketMyRecordLayer::closeLayer() {
        this->removeFromParent();
    }
    
    void RedPacketMyRecordLayer::backButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        closeLayer();
    }
    
}
