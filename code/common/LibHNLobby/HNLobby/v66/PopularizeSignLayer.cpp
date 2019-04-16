//
//  PopularizeSignLayer.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "PopularizeSignLayer.h"
#include "PopularizeSignArticle.h"
#include "HNLobby/GamePrompt.h"
#include "HNPlatform/HNPlatformLogic.h"

PopularizeSignLayer::PopularizeSignLayer() {
    
}

PopularizeSignLayer::~PopularizeSignLayer() {
    
}

bool PopularizeSignLayer::init() {
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

void PopularizeSignLayer::openAddToParent(Layer *parent, cocos2d::Vec2 pos) {
    if (parent) {
        this->setPosition(pos);
        parent->addChild(this);
    }
}

void PopularizeSignLayer::setIsSigning(bool isSign) {
    if (m_signButton) {
        m_signButton->setVisible(!isSign);
    }
    if (m_agreeCheckBox) {
        m_agreeCheckBox->setVisible(!isSign);
    }
    if (m_signState) {
        m_signState->setVisible(isSign);
    }
}

void PopularizeSignLayer::resetData() {
    m_bg = nullptr;
    touchListener = nullptr;
    m_signButton = nullptr;
    m_articleButton = nullptr;
    m_agreeCheckBox = nullptr;
    m_signState = nullptr;
}

void PopularizeSignLayer::createUI() {
    m_bg = ImageView::create("v66/module/tuiguangfuli/bg.png");
    if (m_bg) {
        m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_bg->setPosition(WIN_SIZE/2);
        m_bg->setTouchEnabled(true);
        m_bg->setSwallowTouches(true);
        addChild(m_bg);
        //
        m_signButton = AUIButton::create("v66/module/tuiguangfuli/changanniu.png", "v66/module/tuiguangfuli/changanniu1.png");
        if (m_signButton) {
            m_signButton->setTitle("v66/module/tuiguangfuli/yijianzhuanqian.png");
            m_signButton->setTitleOffset(Vec2(-4, 4));
            m_signButton->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.065f));
            m_signButton->addTouchEventListener(CC_CALLBACK_2(PopularizeSignLayer::signButtonCallBack, this));
            m_bg->addChild(m_signButton);
            //
            m_articleButton = Button::create("v66/module/tuiguangfuli/shenqingtiaokuan/1.png", "v66/module/tuiguangfuli/shenqingtiaokuan/1.png");
            if (m_articleButton) {
                m_articleButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                m_articleButton->setPosition(Vec2(m_bg->getContentSize().width*0.97f, m_signButton->getPositionY()));
                m_articleButton->addTouchEventListener(CC_CALLBACK_2(PopularizeSignLayer::articleButtonCallBack, this));
                m_bg->addChild(m_articleButton);
                //
                m_agreeCheckBox = CheckBox::create("v66/module/tuiguangfuli/shenqingtiaokuan/2.png", "v66/module/tuiguangfuli/shenqingtiaokuan/2.png", "v66/module/tuiguangfuli/shenqingtiaokuan/3.png", "v66/module/tuiguangfuli/shenqingtiaokuan/2.png", "v66/module/tuiguangfuli/shenqingtiaokuan/3.png");
                if (m_agreeCheckBox) {
                    m_agreeCheckBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    m_agreeCheckBox->setPosition(Vec2(m_articleButton->getPositionX()-m_articleButton->getContentSize().width-10, m_articleButton->getPositionY()));
                    m_agreeCheckBox->setSelected(true);
                    m_agreeCheckBox->addEventListener(CC_CALLBACK_2(PopularizeSignLayer::checkBoxCallBack, this));
                    m_bg->addChild(m_agreeCheckBox);
                }
            }
        }
        //
        m_signState = ImageView::create("v66/module/tuiguangfuli/shenhezhong.png");
        if (m_signState) {
            m_signState->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_signState->setPosition(Vec2(m_bg->getContentSize().width*0.8f, m_bg->getContentSize().height*0.3f));
            m_signState->setVisible(false);
            m_bg->addChild(m_signState);
        }
    }
}

void PopularizeSignLayer::closeLayer() {
//    this->removeFromParent();
    if (this->getParent()) {
        this->getParent()->removeFromParent();
    }
}

void PopularizeSignLayer::signButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    if (m_agreeCheckBox == nullptr) {
        closeLayer();
    }
    else if (m_agreeCheckBox->isSelected()) {
        //发消息请求申请
        MSG_GP_TuiGuang_ShenQing shenqing;
        shenqing.iUserID = PlatformLogic()->loginResult.dwUserID;
        shenqing.iType = 1;     //请求申请
        PlatformLogic()->sendData(MDM_GP_TUIGUANG_SHENQING, ASS_GP_TUIGUANG_SHENQING, &shenqing, sizeof(MSG_GP_TuiGuang_ShenQing), HN_SOCKET_CALLBACK(PopularizeSignLayer::signPopularizeSelector, this));
        
//        closeLayer();
    }
    else {
        auto prompt = GamePromptLayer::create();
        if (prompt) {
            prompt->showPrompt("请先勾选同意条款");
        }
    }
}

void PopularizeSignLayer::articleButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    auto articleLayer = PopularizeSignArticle::create();
    if (articleLayer) {
        articleLayer->openAddToParent(this, WIN_SIZE/2);
    }
}

void PopularizeSignLayer::checkBoxCallBack(cocos2d::Ref *sender, CheckBox::EventType type) {
    switch (type) {
        case CheckBox::EventType::UNSELECTED:
            
            break;
        case CheckBox::EventType::SELECTED:
            
            break;
    }
}

bool PopularizeSignLayer::signPopularizeSelector(HN::HNSocketMessage *socketMessage) {
    CCAssert(sizeof(MSG_GP_TuiGuang_ShenQing) == socketMessage->objectSize, "sizeof(MSG_GP_TuiGuang_ShenQing) != objectSize");
    if (sizeof(MSG_GP_TuiGuang_ShenQing) != socketMessage->objectSize)    return false;
    MSG_GP_TuiGuang_ShenQing* msg = (MSG_GP_TuiGuang_ShenQing*)socketMessage->object;
    if (msg == nullptr) {
        return true;
    }
    if (socketMessage) {
        if (socketMessage->messageHead.bHandleCode == 1) {
            setIsSigning(true);
        }
    }
    return true;
}

