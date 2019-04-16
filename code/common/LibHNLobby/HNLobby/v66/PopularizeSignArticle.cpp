//
//  PopularizeSignArticle.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "PopularizeSignArticle.h"

PopularizeSignArticle::PopularizeSignArticle() {
    
}

PopularizeSignArticle::~PopularizeSignArticle() {
    
}

bool PopularizeSignArticle::init() {
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

void PopularizeSignArticle::openAddToParent(Layer *parent, cocos2d::Vec2 pos) {
    if (parent) {
        this->setPosition(pos);
        parent->addChild(this);
    }
}

void PopularizeSignArticle::resetData() {
    m_bg = nullptr;
    m_okButton = nullptr;
    touchListener = nullptr;
}

void PopularizeSignArticle::createUI() {
    m_bg = ImageView::create("v66/module/tuiguangfuli/shenqingtiaokuang.png");
    if (m_bg) {
        m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_bg->setPosition(WIN_SIZE/2);
        m_bg->setTouchEnabled(true);
        m_bg->setSwallowTouches(true);
        addChild(m_bg);
        //
        m_okButton = AUIButton::create("v66/button/yellow1/BG.png", "v66/button/yellow1/BG_1.png");
        if (m_okButton) {
            m_okButton->setTitle("v66/button/yellow1/11.png");
            m_okButton->setTitleOffset(Vec2(-4, 4));
            m_okButton->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.1f));
            m_okButton->addTouchEventListener(CC_CALLBACK_2(PopularizeSignArticle::okayButtonCallBack, this));
            m_bg->addChild(m_okButton);
        }
        //
        auto closeButton = Button::create("hallimg/niuniu/bairenniuniu/3_1.png", "hallimg/niuniu/bairenniuniu/3_1.png");
        if (closeButton) {
            closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            closeButton->setPosition(Vec2(m_bg->getContentSize().width-3, m_bg->getContentSize().height-3));
            closeButton->addTouchEventListener(CC_CALLBACK_2(PopularizeSignArticle::closeButtonCallBack, this));
            m_bg->addChild(closeButton);
        }
    }
}

void PopularizeSignArticle::closeLayer() {
    this->removeFromParent();
}

void PopularizeSignArticle::okayButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    closeLayer();
}

void PopularizeSignArticle::closeButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    closeLayer();
}
