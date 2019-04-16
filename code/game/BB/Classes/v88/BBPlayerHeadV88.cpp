//
//  BBPlayerHeadV88.cpp
//  MixProject
//
//  Created by 黄百超 on 12/03/2017.
//
//

#include "BBPlayerHeadV88.h"
#include "cocostudio/CocoStudio.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"
#include "HNLobby/v66/PlatformResV66.h"

namespace BB {
    
    static const char* Player_Head_CSB = "csb/Games/BB_V66/PlayerHead_V66.csb";
//    static const char* Icon_Res_Path = "v88/dayday_toux/%d.png";
    
    BBPlayerHeadV88::BBPlayerHeadV88() {
        
    }
    
    BBPlayerHeadV88::~BBPlayerHeadV88() {
        
    }
    
    bool BBPlayerHeadV88::init() {
        if (!Layer::init()) {
            return false;
        }
        initDefaultUI();
        return true;
    }
    
    void BBPlayerHeadV88::setPlayerIcon(UserInfoStruct* user) {
        m_HeadImage = UserHead::create(HEAD_FRAME_IN_GAME_V66);
		m_HeadImage->setClipUserHeadOffset(Vec2(0, 15));
        if (m_HeadImage) {
            m_HeadImage->setScale(0.85f);
			if (_head.bg) {
				m_HeadImage->setAnchorPoint(_head.bg->getAnchorPoint());
				m_HeadImage->setPosition(_head.bg->getPosition());
				m_HeadImage->setLocalZOrder(_head.money->getLocalZOrder() - 1);
            }
            m_HeadImage->setHeadWithInfo(user);
            _head.node->addChild(m_HeadImage);
        }
    }
    
    void BBPlayerHeadV88::setPlayerName(char playerName[61]) {
        if (_head.name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            _head.name->setString(ForceGBKToUtf8(playerName));
#else
            _head.name->setString(GBKToUtf8(playerName));
#endif
        }
    }
    
    void BBPlayerHeadV88::setPlayerMoney(long long money) {
        if (_head.money) {
            char coin[128] = {0};
            //sprintf(coin, "%lld", money);
			string str = Tools::addComma(money);
			_head.money->setString(str);
        }
    }
    
    void BBPlayerHeadV88::setHeadPosition(cocos2d::Vec2 pos) {
        if (_head.node) {
            _head.node->setPosition(pos);
        }
    }
    
    void BBPlayerHeadV88::initDefaultUI() {
        auto root = CSLoader::createNode(Player_Head_CSB);
        if (root) {
            _head.node = root;
			_head.node->setScale(0.85f);
            
            auto bg = dynamic_cast<ImageView*>(root->getChildByName("Image_HeadFrame"));
            if (bg) {
                _head.bg = bg;
				_head.bg->setVisible(false);
                   
            }

			auto icon = dynamic_cast<Sprite*>(root->getChildByName("Image_Head"));
			if (icon) {
				_head.icon = icon;
                _head.icon->setVisible(false);
			}
			auto name = dynamic_cast<Text*>(root->getChildByName("Text_NickName"));
			if (name) {
				_head.name = name;
			}
			auto money = dynamic_cast<Text*>(root->getChildByName("AtlasLabel_Gold"));
			if (money) {
				_head.money = money;
			}
            
            addChild(root);
        }
    }
    
}
