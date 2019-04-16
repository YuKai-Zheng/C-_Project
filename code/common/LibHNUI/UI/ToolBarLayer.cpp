//
//  ToolBarLayer.cpp
//  LibHNUI
//
//  Created by 黄百超 on 01/07/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "ToolBarLayer.h"

#include "Sound/HNAudioEngine.h"
#include "HNLobby/v66/PlatformResV66.h"

namespace HN {
    
    const static float ICON_Y_POS[3] = {0.2f, 0.5f, 0.8f};
    
    ToolBarLayer::ToolBarLayer() {
        m_bg = nullptr;
        m_funcImages.clear();
        m_canClose = false;
    }
    
    ToolBarLayer::~ToolBarLayer() {
    }
    
    bool ToolBarLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        this->setIgnoreAnchorPointForPosition(false);
        
        EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(ToolBarLayer::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(ToolBarLayer::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        m_bg = Sprite::create(TOOL_BAR_BG);
        if (m_bg) {
            m_bg->setPosition(Vec2(m_bg->getContentSize().width/2,m_bg->getContentSize().height/2));
            addChild(m_bg);
            this->setContentSize(m_bg->getContentSize());
        }
        
        m_hideBtn = Button::create(TOOL_BAR_SHOW_HIDE, TOOL_BAR_SHOW_HIDE);
        if (m_hideBtn) {
            m_hideBtn->addTouchEventListener(CC_CALLBACK_2(ToolBarLayer::showHideButtonCallBack, this));
            m_hideBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            m_hideBtn->setPosition(Vec2(10, -10));
            addChild(m_hideBtn);
        }
        
        return true;
    }
    
    bool ToolBarLayer::onTouchBegan(Touch *touch, Event *unused_event) {
        Point point = touch->getLocation();
        if (!this->getBoundingBox().containsPoint(point)) {
            close();
        }
        return false;
    }
    
    void ToolBarLayer::onTouchEnded(Touch *touch, Event *unused_event) {
    }
    
    void ToolBarLayer::addImageAndCallBack(std::string imagePath, TapCallback callBack) {
        auto image = ImageView::create(imagePath);
        
        if (image) {
            image->setTouchEnabled(true);
            m_funcImages.push_back(image);
            image->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                if (type == Widget::TouchEventType::ENDED) {
                    if (callBack) {
                        callBack();
                    }
                    close();
                }
            });
            image->setPosition(Vec2(getContentSize().width / 2, 0));
            addChild(image);
        }
    }
    
    //传入单张图片到容器里
    void ToolBarLayer::create(Node* parent, Vec2 vec, int zorder, int tag) {
        if (m_funcImages.size() <= 0) {
            return;
        }
        for (int i = 0; i < m_funcImages.size(); i++) {
            if (m_funcImages.at(i)) {
                m_funcImages.at(i)->setPositionY(getContentSize().height * ICON_Y_POS[i]);
            }
        }
        if (parent) {
            this->setPosition(vec + Vec2(getContentSize().width / 2, getContentSize().height / 2));
            parent->addChild(this, zorder);
        }
    }
    
    //    void ToolBarLayer::changeBgRes(std::string bgRes) {
    //        if (m_bg) {
    //            m_bg->setTexture(bgRes);
    //        }
    //    }
    
    void ToolBarLayer::open() {
        if(m_canClose){
            return;
        }
        
        if (m_hideBtn) {
            m_canClose = true;
            m_hideBtn->setVisible(false);
            this->runAction(MoveBy::create(0.2f, Vec2(0, -getContentSize().height - 10)));
        }
    }
    
    void ToolBarLayer::close() {
        if (!m_canClose) {
            return;
        }
        
        m_canClose = false;
        
        if (m_hideBtn) {
            this->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, getContentSize().height + 10)), DelayTime::create(0.2f), CallFunc::create([=](){
                if (m_hideBtn)
                    m_hideBtn->setVisible(true);
            }), nullptr));
        }
    }
    
    Vec2 ToolBarLayer::convertGL(float x, float y) {
        return Director::getInstance()->convertToGL(Vec2(x, y));
    }
    
    void ToolBarLayer::showHideButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            open();
        }
    }
    
}

