//
//  FloatCoin.cpp
//  LibHNUI
//
//  Created by 黄百超 on 25/10/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "FloatCoin.h"
#include "Tool/Tools.h"

namespace HN {
    
    FloatCoin::FloatCoin() {
    }
    
    FloatCoin::~FloatCoin() {
    }
    
    bool FloatCoin::init() {
        if (!Layer::init()) {
            return false;
        }
        
        resetData();
        createUI();
        
        return true;
    }
    
    void FloatCoin::openParent(cocos2d::Node *parent, cocos2d::Vec2 vec, int zOrder) {
        if (parent) {
            this->setPosition(vec);
            parent->addChild(this, zOrder);
        }
    }
    
    void FloatCoin::updateCoin(long long money) {
        if (m_bg) {
            char moneyValue[32] = {0};
            if (money > 0) {
                m_bg->loadTexture("v66/fontpng/jiesuan_1.png");
                m_moneyText = TextAtlas::create("", "v66/fontpng/shuzi2.png", 72, 96, "0");
                std::string num = StringUtils::format("%lld",money);
                sprintf(moneyValue, "+%s", num.c_str());
                

            }
//            else if (money < 0)
//            {
//                m_bg->loadTexture("v66/fontpng/jiesuan1_1.png");
//                m_moneyText = TextAtlas::create("", "v66/fontpng/jiesuan1.png", 24, 32, "+");
//                sprintf(moneyValue, "%s", Tools::addComma(money).c_str());
//            }
            else
            {
                // 不显示0
                m_bg->removeFromParentAndCleanup(true);
                return;
            }
            //
            if (m_moneyText) {
                m_moneyText->setPosition(m_bg->getContentSize()/2);
                m_bg->addChild(m_moneyText);
                m_moneyText->setString(moneyValue);
                
                auto size = m_moneyText->getContentSize();
                
                auto imgJia = ImageView::create("v66/fontpng/jia.png");
                if (imgJia) {
                    imgJia->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    imgJia->setPosition(Vec2(-10,size.height/2));
                    m_moneyText->addChild(imgJia);
                }
                
                // play action
                runAction(Sequence::create(CallFunc::create([&]() {
                    if (m_bg) {
                        m_bg->runAction(MoveBy::create(1.5f, Vec2(0, 50)));
                    }
                }), DelayTime::create(1.8f), RemoveSelf::create(), nullptr));
            }
        }
    }
    
    void FloatCoin::resetData() {
        m_bg = nullptr;
        m_moneyText = nullptr;
    }
    
    void FloatCoin::createUI() {
        m_bg = ImageView::create("v66/fontpng/jiesuan_1.png");
        if (m_bg) {
            addChild(m_bg);
        }
    }
    
}
