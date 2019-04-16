//
//  EightSet.cpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/2.
//

#include "EightSet.hpp"
#include "cocostudio/CocoStudio.h"
EightSet::EightSet()
{

    
}
EightSet::~EightSet()
{
    
    imgBg = nullptr;
}
bool EightSet::init()
{
    if (!HNLayer::init()) {
        return false;
    }
    
    
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [this](Touch *touch, Event *unused_event){
        close();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
   
    auto nodeMiddle = CSLoader::createNode("Games/31000512/SetNode.csb");
    if (nodeMiddle) {
        imgBg = dynamic_cast<ImageView*>(nodeMiddle->getChildByName("Image_1"));
        if (imgBg) {
            auto closeBtn = dynamic_cast<Button*>(imgBg->getChildByName("Button_close"));
            if (closeBtn) {
                closeBtn->addTouchEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
                        return;
                    }
                    close();
                });
                
            }
            
            auto ruleBtn = dynamic_cast<Button*>(imgBg->getChildByName("Button_rule"));
            if (ruleBtn) {
                ruleBtn->addTouchEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
                        return;
                    }
                    if (_rCallBack) {
                        _rCallBack(nullptr,Widget::TouchEventType::ENDED);
                    }
                });
            }
            auto exitBtn = dynamic_cast<Button*>(imgBg->getChildByName("Button_exit"));
            if (exitBtn) {
                exitBtn->addTouchEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
                        return;
                    }
                    if (_eCallBack) {
                        _eCallBack(nullptr,Widget::TouchEventType::ENDED);
                    }
                    
                });
            }
            auto musicBtn = dynamic_cast<Button*>(imgBg->getChildByName("Button_music"));
            if (musicBtn) {
                musicBtn->addTouchEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
                        return;
                    }
                    if (_mCallBack) {
                        _mCallBack(nullptr,Widget::TouchEventType::ENDED);
                    }
                    
                });
            }
        }
        
    }
    nodeMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nodeMiddle->setPosition(Vec2(WIN_SIZE.width-130.5,WIN_SIZE.height-220));
    addChild(nodeMiddle);
    return true;
}
void EightSet::open(Node* parent, Vec2 vec, int zorder, int tag)
{
    this->setPosition(vec);
    parent->addChild(this,zorder);
    if (imgBg) {
        imgBg->setPositionX((imgBg->getContentSize().width/2+50));
        imgBg->runAction(MoveTo::create(0.5,Vec2(0,0)));
        
    }
    
    
}
void EightSet::close()
{
    imgBg->runAction(Sequence::create(MoveTo::create(0.3f,Vec2(180,0)),CallFunc::create([this](){
        this->removeFromParent();
    }),NULL));
    
    
}

void EightSet::handleBackBtnEvent(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    
    
}
