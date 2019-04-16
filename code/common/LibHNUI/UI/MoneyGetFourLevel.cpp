//
//  MoneyGetFourLevel.cpp
//  LibHNUI
//
//  Created by mac on 2018/8/8.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "MoneyGetFourLevel.hpp"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "Sound/HNAudioEngine.h"
namespace HN {
    MoneyGetFourLevel::MoneyGetFourLevel()
    {
        
    }
    MoneyGetFourLevel::~MoneyGetFourLevel()
    {
        
        
    }
    bool MoneyGetFourLevel::init()
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
            
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
        
        return true;
    }
    void MoneyGetFourLevel::open(Node* parent, Vec2 vec, int zorder,int uiType)
    {
        if (parent) {
            this->setPosition(vec);
            parent->addChild(this,zorder);
            createUIByType(uiType);
        }
    }
    void MoneyGetFourLevel::close()
    {
        this->removeFromParent();
        
    }
    void MoneyGetFourLevel::createUIByType(int _uiType)
    {
        HNAudioEngine::getInstance()->playEffect("Games/31000508/music/duijinbi.mp3");
        auto jsonPath      = "v66/animation/jinbi/jinbidiaoluo.json";
        auto atlasPath     = "v66/animation/jinbi/jinbidiaoluo.atlas";
        auto animationName = StringUtils::format("jinbi0%d",_uiType);
        auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
        if (skeletonNode) {
            skeletonNode->setScale(1.30f);
            skeletonNode->setPosition(Vec2(WIN_SIZE.width/2 - 560,30));
            skeletonNode->setAnimation(0, animationName,false);
            skeletonNode->setEndListener([=](int trackIndex){
                skeletonNode->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            });
            addChild(skeletonNode,1);
        }
        
        runAction(Sequence::create(DelayTime::create(2.0f),RemoveSelf::create(),NULL));
    }

};
