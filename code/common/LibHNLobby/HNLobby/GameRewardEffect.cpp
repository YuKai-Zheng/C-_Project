//
//  GameRewardEffect.cpp
//  LibHNLobby
//
//  Created by mac on 2018/5/7.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "GameRewardEffect.hpp"
GameRewardEffect::GameRewardEffect()
:tipImg(nullptr),effectNode(nullptr)
{

}

GameRewardEffect::~GameRewardEffect(){
    
}
void GameRewardEffect::createEffect(Node * parentNode)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    auto node = GameRewardEffect::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPosition(Vec2(winSize.width/2,winSize.height - 130));
    parentNode->addChild(node,1234567);
}

bool GameRewardEffect::init()
{
    if (!Node::init()) {
        return false;
    }
    
    tipImg = Sprite::create("hallimg/main/gxzj.png");
    if (tipImg) {
        tipImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        tipImg->setPosition(Vec2::ZERO);
        addChild(tipImg);
        tipImg->setScale(0.10f, 0.10f);
        tipImg->runAction(Sequence::create(ScaleTo::create(0.5,1.0f),DelayTime::create(0.2),CallFunc::create([this](){
            effectNode = CSLoader::createNode("csb/v66/haveReward.csb");
            if (effectNode) {
                effectNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                effectNode->setPosition(Vec2::ZERO);
                addChild(effectNode);
                auto action = CSLoader::createTimeline("csb/v66/haveReward.csb");
                if (action) {
                    effectNode->runAction(action);
                    action->gotoFrameAndPlay(0,false);
                }
            }
            
        }), NULL));
    }
    
   
    
    runAction(Sequence::create(DelayTime::create(1.60f),RemoveSelf::create(),NULL));
    
    
    
    return true;
}
