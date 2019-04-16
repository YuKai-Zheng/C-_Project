//
//  PopularizeSignResult.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "PopularizeSignResult.h"
#include "HNLobby/v88/PlatformResV88.h"
#include "HNCommon/HNConverCode.h"

#define Popularize_Article_Count 3

PopularizeSignResult::PopularizeSignResult() {
    
}

PopularizeSignResult::~PopularizeSignResult() {
    
}

bool PopularizeSignResult::init() {
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

void PopularizeSignResult::openAddToParent(Layer *parent, cocos2d::Vec2 pos) {
    if (parent) {
        this->setPosition(pos);
        parent->addChild(this);
    }
}

void PopularizeSignResult::setRefuseInfo(std::string reason) {
    if (m_refuseReason) {
        m_refuseReason->setString(ForceGBKToUtf8(reason.c_str()));
    }
}

void PopularizeSignResult::resetData() {
    m_bg = nullptr;
    m_okButton = nullptr;
    touchListener = nullptr;
    m_articles.clear();
    m_refuseReason = nullptr;
}

void PopularizeSignResult::createUI() {
    m_bg = ImageView::create("v66/module/tuiguangfuli/shenqingshibai.png");
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
            m_okButton->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.08f));
            m_okButton->addTouchEventListener(CC_CALLBACK_2(PopularizeSignResult::okayButtonCallBack, this));
            m_bg->addChild(m_okButton);
        }
        //
        for (int i = 0; i < Popularize_Article_Count; i++) {
            ImageView* article = ImageView::create("v66/module/tuiguangfuli/2.png");
            if (article) {
                article->setTag(i);
                article->setPosition(Vec2(m_bg->getContentSize().width*0.85f, m_bg->getContentSize().height*0.45f+m_bg->getContentSize().height*0.12f*i));
                article->setVisible(false);
                m_articles.push_back(article);
                m_bg->addChild(article);
            }
        }
        //
        m_refuseReason = Text::create("", FONT_TTF_FZCY, 34);
        if (m_refuseReason) {
            m_refuseReason->ignoreContentAdaptWithSize(false);
            m_refuseReason->setContentSize(Size(m_bg->getContentSize().width*0.55f, m_bg->getContentSize().height*0.45f));
            m_refuseReason->setTextVerticalAlignment(TextVAlignment::TOP);
            m_refuseReason->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_refuseReason->setTextColor(Color4B::WHITE);
            m_refuseReason->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_refuseReason->setPosition(Vec2(m_bg->getContentSize().width*0.65f, m_bg->getContentSize().height*0.55f));
            m_bg->addChild(m_refuseReason);
        }

    }
}

void PopularizeSignResult::closeLayer() {
//    this->removeFromParent();
    if (this->getParent()) {
        this->getParent()->removeFromParent();
    }
}

void PopularizeSignResult::okayButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    closeLayer();
}

void PopularizeSignResult::setArticleAchievementAtIndex(unsigned int index, bool hasAchieved) {
    if (index < m_articles.size()) {
        auto article = m_articles.at(index);
        if (article) {
            article->loadTexture(hasAchieved == true ? "v66/module/tuiguangfuli/2.png" : "v66/module/tuiguangfuli/2_1.png");
        }
    }
}

