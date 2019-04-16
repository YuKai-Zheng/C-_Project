//
//  GameLogOnCompositeV66.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 21/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "GameLogOnCompositeV66.h"
#include "PlatformResV66.h"
#include "Sound/HNAudioEngine.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "../command/VertifyCodeCom.h"
#include "../pattern/CommandManager.h"
#include "Tool/Tools.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

#include "base/WeChatAndroid.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/WechatMgr.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

static const char* LOGON_NODE_CSB = "csb/newplatform/login/node_mainlogin.csb";
//static const char* LOGON_NODE_CSB = "login/login.csb";

GameLogOnCompositeV66::GameLogOnCompositeV66() {
    memset(&_logon_node, 0x0, sizeof(_logon_node));
}

GameLogOnCompositeV66::~GameLogOnCompositeV66() {
    
}

void GameLogOnCompositeV66::onExitTransitionDidStart() {
    HNLayer::onExitTransitionDidStart();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameLogOnCompositeV66::onEnter() {
    HNLayer::onEnter();

    if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
        HNAudioEngine::getInstance()->stopBackgroundMusic();
    }
    
//    HNAudioEngine::getInstance()->playBackgroundMusic("NewYear/music/landMusic.mp3", true);
    HNAudioEngine::getInstance()->playBackgroundMusic(LAND_BACKGROUND_MUSIC_V66, true);
}

void GameLogOnCompositeV66::setVisible(bool b) {
	IGameLogOn::setVisible(b);
	auto user = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_accountlogin");
	auto wechat = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_wechatlogin");
	auto localLogin = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_touristlogin");
	user->setVisible(b);
	wechat->setVisible(b);
	localLogin->setVisible(b);

	auto offset = 0;// (user->getPosition().x - wechat->getPosition().x) / 2;
	//iOS如果微信没有安装需要隐藏按钮
	bool isWechatInstalled = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	isWechatInstalled = WechatMgr::isAppInstalled();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	isWechatInstalled = WeChatAndroid::getInstance()->isAppInstalled();
#endif
	if (wechat) {
		wechat->setVisible(isWechatInstalled);

		if (!isWechatInstalled) {
			user->setPositionX(user->getPositionX() - offset);
			localLogin->setPositionX(localLogin->getPositionX() + offset);

		}
	}
}

bool GameLogOnCompositeV66::init() 
{
    if (!HNLayer::init()) 
	{
        return false;
    }

    _logon_node.node = CSLoader::createNode(LOGON_NODE_CSB);
    auto node = _logon_node.node;
    addChild(_logon_node.node);
	_logon_node.node->setLocalZOrder(2);


	auto antAction = CSLoader::createTimeline(LOGON_NODE_CSB);
	runAction(antAction);
	antAction->gotoFrameAndPlay(0);

	auto animation_position = static_cast<Node*>(Tools::getNodeByName(_logon_node.node, "Node_animation_position"));
	if (animation_position != nullptr)
	{
		Tools::PlaySkeletonAnimation(animation_position, "csb/newplatform/login/denglu/person_atlas_spien/", "dl_chaifen_renwu",-1,Vec2(-70.f,-20.0f));
	}

	auto logo_effect_light = static_cast<Node*>(Tools::getNodeByName(_logon_node.node, "Node_logo_effect_light"));
	if (logo_effect_light != nullptr)
	{
		Tools::PlayFrameAnimation(logo_effect_light,"csb/newplatform/animationnode/Node_logo_effect_light.csb");
	}

	auto particle_effect_petal1 = static_cast<Node*>(Tools::getNodeByName(_logon_node.node, "Node_particle_effect_petal1"));
	if (particle_effect_petal1 != nullptr)
	{
		Tools::PlayFrameAnimation(particle_effect_petal1,"csb/newplatform/animationnode/Node_particle_effect_petal.csb");
	}

	auto particle_effect_petal2 = static_cast<Node*>(Tools::getNodeByName(_logon_node.node, "Node_particle_effect_petal2"));
	if (particle_effect_petal2 != nullptr)
	{
		Tools::PlayFrameAnimation(particle_effect_petal2,"csb/newplatform/animationnode/Node_particle_effect_petal.csb");
	}

	auto person_effect_heart = static_cast<Node*>(Tools::getNodeByName(_logon_node.node, "Node_person_effect_heart"));
	if (person_effect_heart != nullptr)
	{
		Tools::PlayFrameAnimation(person_effect_heart,"csb/newplatform/animationnode/Node_person_effect_heart.csb");
	}

	auto particle_effect_petal = static_cast<Node*>(Tools::getNodeByName(_logon_node.node, "Node_particle_effect_petal"));
	if (particle_effect_petal != nullptr)
	{
		Tools::PlayFrameAnimation(particle_effect_petal,"csb/newplatform/animationnode/Node_particle_effect_petal.csb");
	}
	
    auto user = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_accountlogin");
    auto wechat = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_wechatlogin");
    auto localLogin = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_touristlogin");
	user->setVisible(true);
	wechat->setVisible(true);
	localLogin->setVisible(true);
    
   createBtnEffect(user);
   createBtnEffect(wechat);
   createBtnEffect(localLogin);
    
    user->addTouchEventListener(CC_CALLBACK_2(GameLogOnCompositeV66::userLogonCallback, this));
    wechat->addTouchEventListener(CC_CALLBACK_2(GameLogOnCompositeV66::wechatLogonCallBack, this));
    localLogin->addTouchEventListener(CC_CALLBACK_2(GameLogOnCompositeV66::localLogonCallBack, this));
    
    auto offset = (user->getPosition().x - wechat->getPosition().x)/2;
    //iOS如果微信没有安装需要隐藏按钮
    bool isWechatInstalled = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    isWechatInstalled = WechatMgr::isAppInstalled();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    isWechatInstalled = WeChatAndroid::getInstance()->isAppInstalled();
#endif
    if (wechat) {
        wechat->setVisible(isWechatInstalled);
       
        if (!isWechatInstalled) {
            user->setPositionX(user->getPositionX() - offset);
            localLogin->setPositionX(localLogin->getPositionX() + offset);
          
        }
    }
    

// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//     const static std::string PACKAGE_PATH = "com/jlx/MixProject";
//     if (!JniHelper::callStaticBooleanMethod(PACKAGE_PATH + "/AppActivity", "checkSystemCondition"))
//     {
//         GamePromptLayer::create()->showPrompt("您的手机内存过小，此游戏运行可能不流畅！");
//     }
// #endif
    
    return true;
}
void GameLogOnCompositeV66::createBtnEffect(Ref* pSender)
{
    if (pSender) {
        auto btn = (Button*)pSender;
        if (btn == nullptr) {
            return;
        }
        btn->setPressedActionEnabled(false);
        btn->setZoomScale(0.0f);
        auto name = btn->getName();
        std::string str;
        if (name.compare("btn_accountlogin") == 0) {
			str = "csb/newplatform/animationnode/node_login_yellow.csb";
        }
        else if (name.compare("btn_wechatlogin") == 0) {
            str = "csb/newplatform/animationnode/node_login_green.csb";
        }
        else if (name.compare("btn_touristlogin") == 0) {
			str = "csb/newplatform/animationnode/node_login_blue.csb";
        }
        if (!str.empty()) {
            auto actionNode = CSLoader::createNode(str);
            if (actionNode)
			{
                //actionNode->setPosition(Vec2(btn->getContentSize().width/2-5,btn->getContentSize().height/2+12));
				actionNode->setPosition(Vec2(btn->getContentSize().width / 2 , btn->getContentSize().height / 2));
                btn->addChild(actionNode);
                auto action = CSLoader::createTimeline(str);
                if (action)
				{
					action->setTimeSpeed(action->getTimeSpeed() * 3.5f);
                    actionNode->runAction(action);
                    action->gotoFrameAndPlay(0);
                }
            }
        }
    }
    
}
void GameLogOnCompositeV66::userLogonCallback(Ref* pSender, Widget::TouchEventType type) {
	auto btn = (Button*)pSender;
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
			btn->setScale(0.8f);
			break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
			btn->setScale(1.0f);
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
			btn->setScale(1.0f);
			auto user = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_accountlogin");
			auto wechat = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_wechatlogin");
			auto localLogin = (Button*)_logon_node.node->getChildByName("bg")->getChildByName("btn_touristlogin");
			user->setVisible(false);
			wechat->setVisible(false);
			localLogin->setVisible(false);
            this->onLogOnCallBack();
        }
            break;
    }
}

void GameLogOnCompositeV66::wechatLogonCallBack(Ref* pSender, Widget::TouchEventType type) {
    //演示版本屏蔽微信实际功能
	auto btn = (Button*)pSender;
    switch (type) {
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		btn->setScale(0.8f);
		break;
		//case cocos2d::ui::Widget::TouchEventType::MOVED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		btn->setScale(1.0f);
		break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
			btn->setScale(1.0f);
            this->wechatLogOnCallBack(pSender, type);

        }
            break;
    }
}

void GameLogOnCompositeV66::localLogonCallBack(Ref* pSender, Widget::TouchEventType type) {
	auto btn = (Button*)pSender;
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
			btn->setScale(0.8f);
			break;
        //case cocos2d::ui::Widget::TouchEventType::MOVED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
			btn->setScale(1.0f);
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
			btn->setScale(1.0f);
            this->IGameLogOn::localLogonCallBack
			();
        }
            break;
    }
}
