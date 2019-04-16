//
//  PopularizeSignSecret.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "PopularizeSignSecret.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/globel.h"

PopularizeSignSecret::PopularizeSignSecret() {
    
}

PopularizeSignSecret::~PopularizeSignSecret() {
    
}

bool PopularizeSignSecret::init() {
    if (!Layer::init()) {
        return false;
    }
    
    resetData();
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    addChild(colorLayer);
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        return true;
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        closeLayer();
    };

    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
    
    this->setContentSize(WIN_SIZE);
    this->setIgnoreAnchorPointForPosition(false);
    
    createUI();
    
    return true;
}

void PopularizeSignSecret::openAddToParent(Layer *parent, cocos2d::Vec2 pos) {
    if (parent) {
        this->setPosition(pos);
        parent->addChild(this);
        //
        if (m_myID) {
            m_myID->setString(to_string(PlatformLogic()->loginResult.dwUserID));
        }
    }
}

void PopularizeSignSecret::resetData() {
    m_bg = nullptr;
    touchListener = nullptr;
    m_myID = nullptr;
}

void PopularizeSignSecret::createUI() {
    m_bg = ImageView::create("v66/module/tuiguangfuli/tuiguangmiji.png");
    if (m_bg) {
        m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_bg->setPosition(WIN_SIZE/2);
        m_bg->setTouchEnabled(true);
        m_bg->setSwallowTouches(true);
        addChild(m_bg);
        //
        auto closeButton = Button::create("v66/niuniu/bairenniuniu/3_1.png", "v66/niuniu/bairenniuniu/3_1.png");
        if (closeButton) {
            closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            closeButton->setPosition(Vec2(m_bg->getContentSize().width-3, m_bg->getContentSize().height-3));
            closeButton->addTouchEventListener(CC_CALLBACK_2(PopularizeSignSecret::closeButtonCallBack, this));
            m_bg->addChild(closeButton);
        }
        //
        m_myID = Text::create("", FONT_TTF_FZCY, 24);
        if (m_myID) {
            m_myID->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_myID->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_myID->setTextColor(Color4B(255, 238, 92, 255));
            m_myID->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_myID->setPosition(Vec2(m_bg->getContentSize().width*0.27f, m_bg->getContentSize().height*0.635f));
            m_bg->addChild(m_myID);
        }

    }
}

void PopularizeSignSecret::closeLayer() {
    this->removeFromParent();
}

void PopularizeSignSecret::closeButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    closeLayer();
}

