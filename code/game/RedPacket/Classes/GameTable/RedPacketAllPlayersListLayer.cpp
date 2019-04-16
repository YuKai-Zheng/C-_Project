//
//  RedPacketAllPlayersListLayer.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 03/08/2017.
//
//

#include "RedPacketAllPlayersListLayer.h"
#include "HNLobby/v88/PlatformResV88.h"

//#define RED_WIN_SIZE Size(WIN_SIZE.height, WIN_SIZE.width)
#define Player_Count_Phone  5
#define Player_Count_Pad    7

namespace RedPacket {
    RedPacketAllPlayersListLayer::RedPacketAllPlayersListLayer() {
        
    }
    
    RedPacketAllPlayersListLayer::~RedPacketAllPlayersListLayer() {
        
    }
    
    bool RedPacketAllPlayersListLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        
        this->setContentSize(WIN_SIZE);
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketAllPlayersListLayer::openAddToParent(HNLayer *parent, cocos2d::Vec2 pos) {
        if (parent) {
            this->setPosition(pos);
            parent->addChild(this);
        }
    }
    
    void RedPacketAllPlayersListLayer::addCellIntoList(RedPacket::RedPacketAllPlayerCellLayer *cell) {
        if (cell) {
            m_allPlayers.push_back(cell);
        }
    }
    
    void RedPacketAllPlayersListLayer::updateAllPlayers() {
        if (m_allPlayers.size() > 0 && m_packetCells) {
            unsigned long hCount = 0;
            unsigned long vCount = 0;
            if(Application::getInstance()->getTargetPlatform() == cocos2d::Application::Platform::OS_IPAD) {
                hCount = Player_Count_Pad;
            }
            else {
                hCount = Player_Count_Phone;
            }
            vCount = (m_allPlayers.size() / hCount) + 1;
            m_packetCells->setInnerContainerSize(Size(m_packetCells->getInnerContainerSize().width, m_cellSize.height * (vCount + 0.5f)));
            for (int i = 0; i < m_allPlayers.size(); i++) {
				auto cell = m_allPlayers.at(i);
				if (m_packetCells->getChildByTag(cell->getPlayerID())) continue;
                if (cell) {
					cell->setTag(cell->getPlayerID());
                    cell->setPosition(Vec2(15 + m_cellSize.width * (i % hCount + 0.5f) + m_offsetH * (i % hCount), m_packetCells->getInnerContainerSize().height - m_cellSize.height * (i / hCount + 0.5f)));
                    m_packetCells->addChild(cell);
                }
            }
        }
    }
    
    void RedPacketAllPlayersListLayer::resetData() {
        m_topBar = nullptr;
        m_toolBtn = nullptr;
        m_title = nullptr;
        m_packetCells = nullptr;
        m_offsetH = 0.0f;
        m_allPlayers.clear();
        m_cellSize = Size::ZERO;
    }
    
    void RedPacketAllPlayersListLayer::createUI() {
        auto bg = ImageView::create("v88/fahongbao/qunchengyuan/bg.png");
        if (bg) {
			bg->setContentSize(WIN_SIZE);
			bg->setScale9Enabled(true);
            bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            bg->setPosition(WIN_SIZE/2);
            bg->setTouchEnabled(true);
            addChild(bg);
        }
        m_topBar = ImageView::create("v88/fahongbao/2.png");
        if (m_topBar) {
            m_topBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            m_topBar->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height));
            addChild(m_topBar);
        }
        m_toolBtn = Button::create("v88/fahongbao/3.png", "v88/fahongbao/4.png");
        if (m_toolBtn) {
            m_toolBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            m_toolBtn->setPosition(Vec2(0, WIN_SIZE.height));
            m_toolBtn->addTouchEventListener(CC_CALLBACK_2(RedPacketAllPlayersListLayer::backButtonCallBack, this));
            addChild(m_toolBtn);
        }
      
        m_packetCells = ScrollView::create();
        if (m_packetCells) {
            m_packetCells->setDirection(ListView::Direction::VERTICAL);
            m_packetCells->setContentSize(Size(WIN_SIZE.width, WIN_SIZE.height - 210));
            m_packetCells->setInnerContainerSize(m_packetCells->getContentSize());
            m_packetCells->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_packetCells->setPosition(WIN_SIZE/2);
            m_packetCells->setScrollBarAutoHideEnabled(false);
            m_packetCells->setBounceEnabled(true);
            addChild(m_packetCells);
            
            auto cell = RedPacketAllPlayerCellLayer::create();
            if (cell) {
                m_cellSize = cell->getContentSize();
                if(Application::getInstance()->getTargetPlatform() == cocos2d::Application::Platform::OS_IPAD) {
                    m_offsetH = (m_packetCells->getContentSize().width - 30 - m_cellSize.width * Player_Count_Pad) / (Player_Count_Pad - 1);
                }
                else {
                    m_offsetH = (m_packetCells->getContentSize().width - 30 - m_cellSize.width * Player_Count_Phone) / (Player_Count_Phone - 1);
                }
            }
        }
        m_title = Text::create("玩家列表", FONT_TTF_MSYH, 35);
        if (m_title) {
            m_title->setTextAreaSize(Size(WIN_SIZE.width - 200, 100));
            m_title->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_title->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            if (m_toolBtn) {
                m_title->setPosition(Vec2(m_toolBtn->getPositionX()+m_toolBtn->getContentSize().width, m_toolBtn->getPositionY()-m_toolBtn->getContentSize().height/2));
            }
            addChild(m_title);
        }
    }
    
    void RedPacketAllPlayersListLayer::closeLayer() {
        this->removeFromParent();
    }
    
    void RedPacketAllPlayersListLayer::backButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        closeLayer();
    }
    
}
