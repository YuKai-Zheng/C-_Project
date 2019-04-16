//
//  BBPlayerHeadV99.cpp
//  MixProject
//
//  Created by 黄百超 on 12/03/2017.
//
//

#include "BBPlayerHeadV99.h"
#include "cocostudio/CocoStudio.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"
#include "HNLobby/globel.h"

namespace BB {
    
    static const char* Player_Head_CSB = "Games/31000501/PlayerHead.csb";
    static const char* Icon_Res_Path = "hallimg/dayday_toux/%d.png";
    
    BBPlayerHeadV99::BBPlayerHeadV99() {
        
    }
    
    BBPlayerHeadV99::~BBPlayerHeadV99() {
        
    }
    
    bool BBPlayerHeadV99::init() {
        if (!Layer::init()) {
            return false;
        }
        initDefaultUI();
        return true;
    }
    
    void BBPlayerHeadV99::setPlayerIcon(int iconIndex) {
        if (_head.icon) {
            _head.icon->setVisible(false);
            char icon[128] = {0};
            if (iconIndex < 0 || iconIndex > 39) {
                iconIndex = 1;
            }
            sprintf(icon, Icon_Res_Path, iconIndex);
            _head.icon->setTexture(icon);
            _head.icon->setScale(0.7f);
            if (Tools::needLoadWechatImage()) {
                Texture2D* texture = new Texture2D();
                Tools::loadWechatImageFromLocal(texture);
                _head.icon->setTexture(texture);
            }
            _head.icon->setVisible(true);
        }
    }
    
    void BBPlayerHeadV99::setPlayerName(char playerName[61]) {
        if (_head.name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            _head.name->setString(ForceGBKToUtf8(playerName));
#else
            _head.name->setString(GBKToUtf8(playerName));
#endif
        }
    }
    
    void BBPlayerHeadV99::setPlayerMoney(long long money) {
        if (_head.money) {
            _head.money->setString(Tools::addComma(money));
        }
    }
    
    void BBPlayerHeadV99::setHeadPosition(cocos2d::Vec2 pos) {
        if (_head.node) {
            _head.node->setPosition(pos);
        }
    }
    
    void BBPlayerHeadV99::initDefaultUI() {
        auto root = CSLoader::createNode(Player_Head_CSB);
        if (root) {
            _head.node = root;
            
            auto bg = dynamic_cast<ImageView*>(root->getChildByName("Image_HeadFrame"));
            if (bg) {
                _head.bg = bg;
                
                auto icon = dynamic_cast<Sprite*>(bg->getChildByName("Image_Head"));
                if (icon) {
                    _head.icon = icon;
                }
                auto name = dynamic_cast<Text*>(bg->getChildByName("Text_NickName"));
                if (name) {
                    _head.name = name;
                }
                auto money = dynamic_cast<Text*>(bg->getChildByName("AtlasLabel_Gold"));
                if (money) {
                    _head.money = money;
                }
                
            }
            
            addChild(root);
        }
    }
    
}
