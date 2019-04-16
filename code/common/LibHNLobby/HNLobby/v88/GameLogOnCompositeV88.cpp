//
//  GameLogOnCompositeV88.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 21/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "GameLogOnCompositeV88.h"
#include "PlatformResV88.h"
#include "Sound/HNAudioEngine.h"
#include "HNLobby/PlatformDefine.h"
#include "../command/VertifyCodeCom.h"
#include "../pattern/CommandManager.h"
#include "Tool/Tools.h"

#include "base/WeChatAndroid.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/WechatMgr.h"
#endif

static const char* LOGON_NODE_CSB = "csb/v88/loginUI/LogOnNode_v88.csb";

GameLogOnCompositeV88::GameLogOnCompositeV88() {
    memset(&_logon_node, 0x0, sizeof(_logon_node));
}

GameLogOnCompositeV88::~GameLogOnCompositeV88() {
    
}

void GameLogOnCompositeV88::onExitTransitionDidStart() {
    HNLayer::onExitTransitionDidStart();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameLogOnCompositeV88::onEnter() {
    HNLayer::onEnter();

    if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
        HNAudioEngine::getInstance()->stopBackgroundMusic();
    }
    HNAudioEngine::getInstance()->playBackgroundMusic(LAND_BACKGROUND_MUSIC_V88, true);
}

bool GameLogOnCompositeV88::init() {
    if (!HNLayer::init()) {
        return false;
    }
    
    Tools::addAnimationFrame("v88/animation/entry/base/entry_base.csb", this, "entry_base", Vec2(0, 0));
    
    _logon_node.node = CSLoader::createNode(LOGON_NODE_CSB);
    addChild(_logon_node.node);
    
    auto wechat = (Button*)_logon_node.node->getChildByName("Button_wechat");
    auto user = (Button*)_logon_node.node->getChildByName("Button_login");
    if (wechat) {
        wechat->setVisible(false);
    }
    if (user) {
        user->setVisible(false);
    }
    
    Vec2 middlePos = Vec2(0, wechat->getPositionY());
    float offsetX = abs(wechat->getPositionX() - user->getPositionX()) * 0.7f;
    
    _logon_node.userLogonBtn = AUIButton::create("v88/button/yellow4/da.png", "v88/button/yellow4/da.png");
    if (_logon_node.userLogonBtn) {
        _logon_node.userLogonBtn->setName("userLogon");
        _logon_node.userLogonBtn->setTitle("v88/button/yellow4/1.png");
        _logon_node.userLogonBtn->setTitleOffset(Vec2(0, 4));
        _logon_node.userLogonBtn->setPosition(middlePos + Vec2(-offsetX, 0));
        _logon_node.userLogonBtn->addTouchEventListener(CC_CALLBACK_2(GameLogOnCompositeV88::userLogonCallback, this));
        _logon_node.node->addChild(_logon_node.userLogonBtn);
//        Tools::addAnimationFrame("v88/animation/entry/anniu/entry_anniu.csb", _logon_node.userLogonBtn, "entry_anniu", Vec2(0, 0));
    }
    
    _logon_node.wechatLogonBtn = AUIButton::create("v88/button/green3/4.png", "v88/button/green3/4.png");
    if (_logon_node.wechatLogonBtn) {
        _logon_node.wechatLogonBtn->setName("WechatLogon");
        _logon_node.wechatLogonBtn->setTitle("v88/button/green3/5.png");
        _logon_node.wechatLogonBtn->setTitleOffset(Vec2(-4, 4));
        _logon_node.wechatLogonBtn->setPosition(middlePos);
        _logon_node.wechatLogonBtn->addTouchEventListener(CC_CALLBACK_2(GameLogOnCompositeV88::wechatLogonCallBack, this));
        _logon_node.node->addChild(_logon_node.wechatLogonBtn);
        Tools::addAnimationFrame("v88/animation/entry/anniu/entry_anniu.csb", _logon_node.wechatLogonBtn, "entry_anniu", Vec2(0, 0));
    }
    
    _logon_node.localLogonBtn = AUIButton::create("v88/button/blue2/da.png", "v88/button/blue2/da.png");
    if (_logon_node.localLogonBtn) {
        _logon_node.localLogonBtn->setName("LocalLogon");
        _logon_node.localLogonBtn->setTitle("v88/button/blue2/youkedenglu.png");
        _logon_node.localLogonBtn->setTitleOffset(Vec2(-4, 4));
        _logon_node.localLogonBtn->setPosition(middlePos + Vec2(offsetX, 0));
        _logon_node.localLogonBtn->addTouchEventListener(CC_CALLBACK_2(GameLogOnCompositeV88::localLogonCallBack, this));
        _logon_node.node->addChild(_logon_node.localLogonBtn);
//        Tools::addAnimationFrame("v88/animation/entry/anniu/entry_anniu.csb", _logon_node.localLogonBtn, "entry_anniu", Vec2(0, 0));
    }
    
    //iOS如果微信没有安装需要隐藏按钮
     bool isWechatInstalled = false;
 #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
     isWechatInstalled = WechatMgr::isAppInstalled();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	 isWechatInstalled = WeChatAndroid::getInstance()->isAppInstalled();
 #endif
     if (_logon_node.wechatLogonBtn) {
         _logon_node.wechatLogonBtn->setVisible(isWechatInstalled);
         if (!isWechatInstalled) {
             _logon_node.userLogonBtn->setPositionX(-offsetX*0.8f);
             _logon_node.localLogonBtn->setPositionX(offsetX*0.8f);
         }
     }
    
    auto logo = dynamic_cast<Sprite*>(_logon_node.node->getChildByName("Sprite_1"));
    if (logo) {
        logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        logo->setPositionY((WIN_SIZE.height - 10 - _logon_node.userLogonBtn->getPositionY()) / 2 - WIN_SIZE.height / 2);
        logo->setVisible(false);
        
        Tools::addAnimationFrame("v88/animation/entry/logo/entry_logo.csb", _logon_node.node, "entry_logo", Vec2(0, logo->getPositionY()));
    }
    
    return true;
}

void GameLogOnCompositeV88::userLogonCallback(Ref* pSender, Widget::TouchEventType type) {
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        case cocos2d::ui::Widget::TouchEventType::MOVED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            this->onLogOnCallBack();
        }
            break;
    }
}

void GameLogOnCompositeV88::wechatLogonCallBack(Ref* pSender, Widget::TouchEventType type) {
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        case cocos2d::ui::Widget::TouchEventType::MOVED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            this->wechatLogOnCallBack(pSender, type);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            WechatMgr::sendAuthRequestScope();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            WeChatAndroid::getInstance()->goToWeiXin();
#endif
        }
            break;
    }
}

void GameLogOnCompositeV88::localLogonCallBack(Ref* pSender, Widget::TouchEventType type) {
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        case cocos2d::ui::Widget::TouchEventType::MOVED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            this->IGameLogOn::localLogonCallBack();
        }
            break;
    }
}
