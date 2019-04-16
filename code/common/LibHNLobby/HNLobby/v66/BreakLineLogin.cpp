//
//  BreakLineLogin.cpp
//  LibHNLobby
//
//  Created by mac on 2018/10/31.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "BreakLineLogin.hpp"
#include "../pattern/CommandManager.h"
BreakLineLogin::BreakLineLogin()
{
    
}
BreakLineLogin::~BreakLineLogin()
{
    
}

bool BreakLineLogin::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){

    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    addChild(colorLayer);
    
    CommandManager::getInstance()->addLoaderCommand("重新连接服务器...");
    return true;
}
void BreakLineLogin::open(Node* parent, Vec2 vec, int zorder, int tag)
{
    
    
    
    
    
}
void BreakLineLogin::close()
{
    this->removeFromParent();
    
}
