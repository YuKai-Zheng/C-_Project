//
//  HelpLayer.cpp
//  LibHNUI
//
//  Created by 黄百超 on 26/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "HelpLayer.h"
#include "Sound/HNAudioEngine.h"
#include "HNLobby/v66/PlatformResV66.h"

namespace HN {
    HelpLayer::HelpLayer() {
    }
    
    HelpLayer::~HelpLayer() {
    }
    
    bool HelpLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        this->setIgnoreAnchorPointForPosition(false);
        
        EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(HelpLayer::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(HelpLayer::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        helpBg = ImageView::create("v66/common/help.png");
        helpBg->setTouchEnabled(true);
        helpBg->setPosition(Vec2(helpBg->getContentSize().width / 2, helpBg->getContentSize().height / 2));
        addChild(helpBg);
        
        //this->setBgImage("v66/common/help.png");
        this->setContentSize(helpBg->getContentSize());
        
        return true;
    }
    
    void HelpLayer::closeHelpCallBack(Ref *pSender)
    {
        this->moveSelfOutWithAction();
    }
    
    bool HelpLayer::onTouchBegan(Touch *touch, Event *unused_event) {
        Point point = touch->getLocation();
        if (!this->getBoundingBox().containsPoint(point)) {
            this->moveSelfOutWithAction();
        }
        return false;
    }
    
    void HelpLayer::onTouchEnded(Touch *touch, Event *unused_event) {
    }
    
    
    //传入单张图片到容器里
    void HelpLayer::setContentOnView(std::string contentPath) {
        auto helpContent = Sprite::create(contentPath);
        if (helpContent) {
            float scaleValue = 550 / helpContent->getContentSize().width;
            if (630 >= helpContent->getContentSize().height * scaleValue) {
                setContentSprite(helpContent);
            }
            else {
                auto view = ui::ScrollView::create();
                view->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                view->setDirection(ui::ScrollView::Direction::VERTICAL);
                view->setContentSize(Size(550,630));
                view->addChild(helpContent);
                helpContent->setScale(scaleValue);
                view->setInnerContainerSize(helpContent->getContentSize() * scaleValue);
                addChild(view);
                view->setPosition(Vec2(24,35));
                helpContent->setAnchorPoint(Vec2(0,0));
                helpContent->setPosition(Vec2(0, 0));
                view->scrollToTop(0.15, true);
            }
        }
    }
    
    void HelpLayer::setContentOnViewEx(std::string contentPath) {
        auto helpContent = Sprite::create(contentPath);
        if (helpContent) {
            
            auto view = ui::ScrollView::create();
            view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            view->setDirection(ui::ScrollView::Direction::VERTICAL);
            view->setContentSize(Size(900,540));
            
            auto size = helpContent->getContentSize();
            float scale = 900/size.width;
            view->setInnerContainerSize(helpContent->getContentSize()*scale);
            helpBg->addChild(view);
            view->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2 - 30));
            helpContent->setAnchorPoint(Vec2(0.5,1));
            helpContent->setPosition(Vec2(view->getContentSize().width/2, size.height));
            helpContent->setScale(scale);
            view->addChild(helpContent);
            view->scrollToTop(0.15, true);
            
        }
    }
    
    //传入单张图片作为帮助界面内容
    void HelpLayer::setContentSprite(Sprite* sprite) {
        if (sprite) {
            float scaleValue = (getContentSize().width - 70) / sprite->getContentSize().width;
            sprite->setScale(scaleValue);
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprite->setPosition(Vec2(10, this->getContentSize().height / 2));
            addChild(sprite);
        }
    }

    void HelpLayer::setBgImage(std::string bgPath) {
        if (helpBg) {
            helpBg->loadTexture(bgPath);
            this->setContentSize(helpBg->getContentSize());
        }
    }
    
    void HelpLayer::setOrignalPosition() {
        this->setPosition(Vec2(-getContentSize().width, WIN_SIZE.height / 2));
    }
    
    void HelpLayer::moveSelfOutWithAction() {
        this->runAction(Sequence::create(MoveBy::create(0.3f, Point(-(getContentSize().width + 10), 0.0f)),
                                         RemoveSelf::create(),
                                         nullptr));
    }
    
    void HelpLayer::moveSelfInWithAction() {
        this->runAction(Sequence::create(MoveBy::create(0.3f, Point((getContentSize().width + 10), 0.0f)),
                                         nullptr));
    }
    
    Vec2 HelpLayer::convertGL(float x, float y) {
        return Director::getInstance()->convertToGL(Vec2(x, y));
    }
}
